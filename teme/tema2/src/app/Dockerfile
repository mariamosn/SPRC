FROM python:3.6

WORKDIR /app

RUN pip install --upgrade pip
COPY requirements.txt /app
RUN pip install --no-cache-dir -r requirements.txt

COPY . /app

ENV FLASK_APP=app.py
ENV FLASK_DEBUG=true
ENV FLASK_ENV=development

EXPOSE 5000

CMD python app.py
