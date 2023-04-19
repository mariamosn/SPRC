from flask import Flask, request, Response
from flask.json import jsonify
import json

app = Flask(__name__)

db = {}
id = 1


@app.route("/movies", methods=["GET"])
def get_movies():
    global db
    res = []
    for key, value in db.items():
        temp = {'id' : key, 'nume' : db[key]}
        res.append(temp)

    return Response(json.dumps(res), status=200)


@app.route("/movies", methods=["POST"])
def post_movies():
    global id
    global db
    payload = request.get_json(silent=True)

    if not payload:
        return Response(status=400)

    title = payload['nume'].strip()
    if not title:
        return Response(status=400)
    
    db[id] = title
    id = id + 1
    return Response(status=201)
        

@app.route("/movie/<int:id>", methods=["PUT"])
def put_movie(id):
    global db

    if not id:
        return Response(status=200)

    payload = request.get_json(silent=True)

    if not payload:
        return Response(status=400)
    
    if not id in db.keys():
        return Response(status=404)

    title = payload['nume'].strip()
    if not title:
        return Response(status=400)
    
    db[id] = payload['nume'].strip()

    return Response(status=200)

@app.route("/movie/<int:id>", methods=["GET"])
def get_movie(id):
    global db

    if not id:
        return jsonify({}), 200
    if not id in db.keys():
        return Response(status=404)

    title = db[id]

    return Response(json.dumps({'id' : id, 'nume' : title}), status=200)

@app.route("/movie/<int:id>", methods=["DELETE"])
def delete_movie(id):
    global db

    if not id:
        return Response(status=200)

    if not id in db.keys():
        return Response(status=404)
    
    del db[id]
    return Response(status=200)

@app.route("/reset", methods=["DELETE"])
def delete_reset():
    global id
    global db
    db = {}
    id = 1
    return Response(status=200)

if __name__ == '__main__':
    app.run('0.0.0.0', port=7020, debug=True)
