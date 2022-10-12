import requests

method = 'POST'
url = 'https://sprc.dfilip.xyz/lab1/task2/'
headers = {'Content-Type': 'application/json'}
data = {'username':'sprc', 'password':'admin', 'nume':'maria'}

r = requests.request(method, url, json=data, headers=headers)
print(r.text)
