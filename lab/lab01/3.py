import requests

method = 'POST'
url = 'https://sprc.dfilip.xyz/lab1/task3/login/'
headers = {'Content-Type': 'application/json'}
data = {'username':'sprc', 'password':'admin', 'nume':'maria'}

r = requests.request(method, url, json=data, headers=headers)
print(r.text)

method = 'GET'
url = 'https://sprc.dfilip.xyz/lab1/task3/check/'
cookies = r.cookies

r = requests.request(method, url, cookies=cookies)
print(r.text)
