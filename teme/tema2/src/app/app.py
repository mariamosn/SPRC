import time

from init import create_app
from models import db

app = create_app()
db.init_app(app)

if __name__ == "__main__":
    time.sleep(1)
    app.app_context().push()
    db.create_all()
    app.run(host="0.0.0.0", debug=True)
