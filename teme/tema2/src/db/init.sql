\c weather_db;

CREATE TABLE IF NOT EXISTS Tari (
  id SERIAL NOT NULL PRIMARY KEY,
  nume_tara VARCHAR(250) UNIQUE,
  latitudine NUMERIC(9, 6),
  longitudine NUMERIC(9, 6)
);

CREATE TABLE IF NOT EXISTS Orase (
  id SERIAL NOT NULL PRIMARY KEY,
  id_tara INT,
  nume_oras VARCHAR(250),
  latitudine NUMERIC(9, 6),
  longitudine NUMERIC(9, 6),
  UNIQUE (id_tara, nume_oras),
  CONSTRAINT Fk_Tara
    FOREIGN KEY (id_tara)
    REFERENCES Tari(id)
    ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS Temperaturi (
  id SERIAL NOT NULL PRIMARY KEY,
  valoare NUMERIC(9, 6),
  timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  id_oras INT,
  UNIQUE (id_oras, timestamp),
  CONSTRAINT Fk_Oras
    FOREIGN KEY (id_oras)
    REFERENCES Orase(id)
    ON DELETE CASCADE
);
