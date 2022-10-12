import requests

method = 'POST'
url = 'https://sprc.dfilip.xyz/lab1/task1/check/'
headers = {'Content-Type': 'application/x-www-form-urlencoded', 'secret2': 'SPRCisBest'}
params = {'nume': 'Maria', 'grupa': '343C1'}
data = {'secret': 'SPRCisNice'}

r = requests.request(method, url, params=params, data=data, headers=headers)
print(r.text)
