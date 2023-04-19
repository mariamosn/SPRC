from flask_sqlalchemy import SQLAlchemy
from sqlalchemy.sql import func

db = SQLAlchemy()

class Tari(db.Model):
    __tablename__ = "Tari"

    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    nume_tara = db.Column(db.String(250), unique=True)
    latitudine = db.Column(db.FLOAT(precision=32, decimal_return_scale=None))
    longitudine = db.Column(db.FLOAT(precision=32, decimal_return_scale=None))

    def __init__(self, nume_tara, latitudine, longitudine):
        self.nume_tara = nume_tara
        self.latitudine = latitudine
        self.longitudine = longitudine

class Orase(db.Model):
    __tablename__ = "Orase"

    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    id_tara = db.Column(db.Integer,
        db.ForeignKey('Tari.id', ondelete='CASCADE'))
    nume_oras = db.Column(db.String(250))
    latitudine = db.Column(db.FLOAT(precision=32, decimal_return_scale=None))
    longitudine = db.Column(db.FLOAT(precision=32, decimal_return_scale=None))
    db.UniqueConstraint('id_tara', 'nume_oras')

    def __init__(self, id_tara, nume_oras, latitudine, longitudine):
        self.id_tara = id_tara
        self.nume_oras = nume_oras
        self.latitudine = latitudine
        self.longitudine = longitudine

class Temperaturi(db.Model):
    __tablename__ = "Temperaturi"

    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    valoare = db.Column(db.FLOAT(precision=32, decimal_return_scale=None))
    timestamp = db.Column(db.DateTime(timezone=True),
        server_default=func.now())
    id_oras = db.Column(db.Integer,
        db.ForeignKey('Orase.id', ondelete='CASCADE'))
    db.UniqueConstraint('id_oras', 'timestamp')

    def __init__(self, valoare, id_oras):
        self.valoare = valoare
        self.id_oras = id_oras
