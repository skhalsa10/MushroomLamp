from flask import Flask
import requests

app = Flask(__name__)
URL = "http://api.weatherapi.com/v1/current.json"

key="a38c63037d8746d18dd33450211609"
q="Albuquerque"
aqi="no"
PARAMS = {'key': key, 'q':q, 'aqi': aqi}


def getTempF():
    r = requests.get(url = URL, params= PARAMS)
    data = r.json()
    return str(data['current']['temp_f'])

@app.route("/temp-f")
def getTempInF():
    return getTempF()

def create_app():
   return app