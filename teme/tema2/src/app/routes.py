from flask import request, Response
import json
import datetime

from models import *

def bad_post_countries_req(req):
    body = req.json
    if not body:
        return True
    if not "nume" in body:
        return True
    if not "lat" in body:
        return True
    if not "lon" in body:
        return True
    if (not str(body["lat"]).replace('.', '', 1).isdigit()):
        return True
    if (not str(body["lon"]).replace('.', '', 1).isdigit()):
        return True
    return False

def bad_put_countries_req(req, id):
    if bad_post_countries_req(req):
        return True
    body = req.json
    if not "id" in body:
        return True
    if not str(body["id"]).isdigit():
        return True
    if not str(id).isdigit():
        return True
    if int(body["id"]) != id:
        return True
    return False

def bad_post_cities_req(req):
    body = req.json
    if not body:
        return True
    if not "idTara" in body:
        return True
    if not "nume" in body:
        return True
    if not "lat" in body:
        return True
    if not "lon" in body:
        return True
    if (not str(body["idTara"]).isdigit()):
        return True
    if (not str(body["lat"]).replace('.', '', 1).isdigit()):
        return True
    if (not str(body["lon"]).replace('.', '', 1).isdigit()):
        return True
    return False

def bad_put_cities_req(req, id):
    body = req.json
    if bad_post_cities_req(req):
        return True
    if not "id" in body:
        return True
    if not str(body["id"]).isdigit():
        return True
    if not str(id).isdigit():
        return True
    if int(body["id"]) != id:
        return True
    return False

def bad_post_temp_req(req):
    body = req.json
    if not body:
        return True
    if not "idOras" in body:
        return True
    if not "valoare" in body:
        return True
    if not str(body["idOras"]).isdigit():
        return True
    if (not str(body["valoare"]).replace('.', '', 1).isdigit()):
        return True
    return False

def validate_date(date):
    try:
        datetime.datetime.strptime(date, '%Y-%m-%d')
    except:
        return False
    return True

def bad_put_temp_req(req, id):
    body = req.json
    if not body:
        return True
    if not "id" in body:
        return True
    if not "idOras" in body:
        return True
    if not "valoare" in body:
        return True
    if not str(body["id"]).isdigit():
        return True
    if not str(body["idOras"]).isdigit():
        return True
    if not str(body["valoare"]).replace('.', '', 1).isdigit():
        return True
    if not str(id).isdigit():
        return True
    if int(body["id"]) != id:
        return True
    return False

def init_routes(app):
    @app.route("/api/countries", methods=["POST", "GET"])
    def handle_countries():
        if request.method == "POST":
            if bad_post_countries_req(request):
                return Response(status=400)

            name = request.json["nume"]
            lat = float(request.json["lat"])
            lon = float(request.json["lon"])
            new_entry = Tari(name, lat, lon)
            try:
                db.session.add(new_entry)
                db.session.commit()
            except:
                return Response(status=409)

            return Response(json.dumps({'id' : new_entry.id}), status=201)

        elif request.method == "GET":
            response = []
            for country in Tari.query.all():
                response.append({'id': country.id,
                                        'nume': country.nume_tara,
                                        'lat': country.latitudine,
                                        'lon': country.longitudine})
            return Response(json.dumps(response), status=200)

        else:
            return Response(status=400)
    
    @app.route("/api/countries/<id>", methods=["PUT", "DELETE"])
    def handle_countries_id(id):
        if request.method == "PUT":
            if bad_put_countries_req(request, id):
                return Response(status=400)

            name = request.json["nume"]
            lat = float(request.json["lat"])
            lon = float(request.json["lon"])
            prev_record = Tari.query.filter_by(id=id).first()
            if prev_record is None:
                return Response(status=404)

            prev_record.nume_tara = name
            prev_record.latitudine = lat
            prev_record.longitudine = lon
            try:
                db.session.commit()
            except:
                return Response(status=409)

            return Response(status=200)

        elif request.method == "DELETE":
            if not str(id).isdigit():
                return Response(status=400)

            to_be_del = Tari.query.filter_by(id=id).first()
            if to_be_del is None:
                return Response(status=404)

            db.session.delete(to_be_del)
            db.session.commit()
            return Response(status=200)

        else:
            return Response(status=400)
    
    @app.route("/api/cities", methods=["POST", "GET"])
    def handle_cities():
        if request.method == "POST":
            if bad_post_cities_req(request):
                return Response(status=400)

            id_country = int(request.json["idTara"])
            if Tari.query.filter_by(id=id_country).first() is None:
                return Response(status=404)

            name = request.json["nume"]
            lat = float(request.json["lat"])
            lon = float(request.json["lon"])
            new_entry = Orase(id_country, name, lat, lon)
            try:
                db.session.add(new_entry)
                db.session.commit()
            except:
                return Response(status=409)

            return Response(json.dumps({'id' : new_entry.id}), status=201)

        elif request.method == "GET":
            response = []
            for city in Orase.query.all():
                response.append({'id': city.id,
                                        'idTara': city.id_tara,
                                        'nume': city.nume_oras,
                                        'lat': city.latitudine,
                                        'lon': city.longitudine})
            return Response(json.dumps(response), status=200)

        else:
            return Response(status=400)

    @app.route("/api/cities/country/<id_Tara>", methods=["GET"])
    def handle_cities_country(id_Tara):
        if request.method == "GET":
            if Tari.query.filter_by(id=id_Tara).first() is None:
                return Response(status=404)

            response = []
            for city in Orase.query.filter_by(id_tara=id_Tara):
                response.append({'id': city.id,
                                        'idTara': city.id_tara,
                                        'nume': city.nume_oras,
                                        'lat': city.latitudine,
                                        'lon': city.longitudine})
            return Response(json.dumps(response), status=200)

        else:
            return Response(status=400)

    @app.route("/api/cities/<id>", methods=["PUT", "DELETE"])
    def handle_cities_id(id):
        if request.method == "PUT":
            if bad_put_cities_req(request, id):
                return Response(status=400)

            id_country = int(request.json["idTara"])
            if Tari.query.filter_by(id=id_country).first() is None:
                return Response(status=404)

            name = request.json["nume"]
            lat = float(request.json["lat"])
            lon = float(request.json["lon"])
            prev_entry = Orase.filter_by(id=id).first()
            if prev_entry is None:
                return Response(status=404)

            prev_entry.id_tara = id_country
            prev_entry.nume_oras = name
            prev_entry.latitudine = lat
            prev_entry.longitudine = lon
            try:
                db.session.commit()
            except:
                return Response(status=409)

            return Response(status=200)

        elif request.method == "DELETE":
            if not str(id).isdigit():
                return Response(status=400)

            to_be_del = Orase.query.filter_by(id=id).first()
            if to_be_del is None:
                return Response(status=404)

            db.session.delete(to_be_del)
            db.session.commit()
            return Response(status=200)

        else:
            return Response(status=400)

    @app.route("/api/temperatures", methods=["POST", "GET"])
    def handle_temperatures():
        if request.method == "POST":
            if bad_post_temp_req(request):
                return Response(status=400)

            id_city = int(request.json["idOras"])
            if Orase.query.filter_by(id=id_city).first() is None:
                return Response(status=404)

            value = float(request.json["valoare"])
            if not Orase.query.filter_by(id=id_city).first():
                return Response(status=404)

            new_entry = Temperaturi(value, id_city)
            try:
                db.session.add(new_entry)
                db.session.commit()
            except:
                return Response(status=409)

            return Response(json.dumps({'id' : new_entry.id}), status=201)

        elif request.method == "GET":
            lat = request.args.get('lat')
            lon = request.args.get('lon')
            from_date = request.args.get('from')
            until_date = request.args.get('until')

            filters = []
            if from_date and validate_date(from_date):
                parsed_from_date = datetime. \
                    datetime.strptime(from_date, '%Y-%m-%d')
                filters.append(Temperaturi.timestamp >= parsed_from_date)
            if until_date and validate_date(until_date):
                parsed_until_date = datetime. \
                    datetime.strptime(until_date, '%Y-%m-%d')
                filters.append(Temperaturi.timestamp <= parsed_until_date)
            if lat and str(lat).replace('.', '', 1).isdigit():
                lat = float(lat)
                filters.append(Orase.latitudine == lat)
            if lon and str(lon).replace('.', '', 1).isdigit():
                lon = float(lon)
                filters.append(Orase.longitudine == lon)
            
            resp_build = db.session.query(Temperaturi). \
                join(Orase, Temperaturi.id_oras == Orase.id). \
                filter(*filters).all()

            response = []
            for entry in resp_build:
                response.append({'id': entry.id,
                                        'valoare': entry.valoare,
                                        'timestamp': entry.timestamp})
            return Response(json.dumps(response, default=str), status=200)
        
        else:
            return Response(status=400)
    
    @app.route("/api/temperatures/cities/<id>", methods=["GET"])
    def handle_temperatures_cities(id):
        if request.method == "GET":
            from_date = request.args.get('from')
            until_date = request.args.get('until')
            filters = []

            if not str(id).isdigit():
                return Response(status=400)

            if Orase.query.filter_by(id=id).first() is None:
                return Response(status=404)

            filters.append(Temperaturi.id_oras == id)
            if from_date and validate_date(from_date):
                parsed_from_date = datetime. \
                    datetime.strptime(from_date, '%Y-%m-%d')
                filters.append(Temperaturi.timestamp >= parsed_from_date)
            if until_date and validate_date(until_date):
                parsed_until_date = datetime. \
                    datetime.strptime(until_date, '%Y-%m-%d')
                filters.append(Temperaturi.timestamp <= parsed_until_date)
            
            resp_build = db.session.query(Temperaturi). \
                filter(*filters).all()

            response = []
            for entry in resp_build:
                response.append({'id': entry.id,
                                        'valoare': entry.valoare,
                                        'timestamp': entry.timestamp})
            return Response(json.dumps(response, default=str), status=200)
        
        else:
            return Response(status=400)

    @app.route("/api/temperatures/countries/<id_tara>", methods=["GET"])
    def handle_temperatures_countries(id_tara):
        if request.method == "GET":
            from_date = request.args.get('from')
            until_date = request.args.get('until')
            filters = []

            if not str(id_tara).isdigit():
                return Response(status=400)

            if Tari.query.filter_by(id=id_tara).first() is None:
                return Response(status=404)

            filters.append(Orase.id_tara == id_tara)
            if from_date and validate_date(from_date):
                parsed_from_date = datetime. \
                    datetime.strptime(from_date, '%Y-%m-%d')
                filters.append(Temperaturi.timestamp >= parsed_from_date)
            if until_date and validate_date(until_date):
                parsed_until_date = datetime. \
                    datetime.strptime(until_date, '%Y-%m-%d')
                filters.append(Temperaturi.timestamp <= parsed_until_date)
            
            resp_build = db.session.query(Temperaturi). \
                join(Orase, Temperaturi.id_oras == Orase.id). \
                filter(*filters).all()

            response = []
            for entry in resp_build:
                response.append({'id': entry.id,
                                        'valoare': entry.valoare,
                                        'timestamp': entry.timestamp})
            return Response(json.dumps(response, default=str), status=200)
        
        else:
            return Response(status=400)

    @app.route("/api/temperatures/<id>", methods=["PUT", "DELETE"])
    def handle_temp_id(id):
        if request.method == "PUT":
            if bad_put_temp_req(request, id):
                return Response(status=400)

            idOras = int(request.json["idOras"])
            if Orase.query.filter_by(id=idOras).first() is None:
                return Response(status=404)

            value = request.json["valoare"]
            prev_entry = Orase.filter_by(id=id).first()
            if prev_entry is None:
                return Response(status=404)

            prev_entry.id_oras = idOras
            prev_entry.valoare = value
            try:
                db.session.commit()
            except:
                return Response(status=409)

            return Response(status=200)

        elif request.method == "DELETE":
            if not str(id).isdigit():
                return Response(status=400)

            to_be_del = Temperaturi.query.filter_by(id=id).first()
            if to_be_del is None:
                return Response(status=404)

            db.session.delete(to_be_del)
            db.session.commit()
            return Response(status=200)

        else:
            return Response(status=400)
