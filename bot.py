import urllib.request
import json
import os
from sys import stderr

api_key = os.getenv("OMDB_API_KEY")
if api_key is None:
    print("API key is not found in your environment", file=stderr)
    print("export OMDB_API_KEY=<your api key>", file=stderr)
    exit(1)

url = f"http://www.omdbapi.com/?t=Red+notice&apikey={api_key}"
with urllib.request.urlopen(url) as response:
    data = response.read().decode("utf-8")

def print_colored(key, value):
    print(f"\033[32m{key}:\033[0m {value}")

for key, value in json.loads(data).items():
    if key == "Ratings":
        print_colored(key, "")
        for rating in value:
            print_colored(f"  - {rating['Source']}", rating['Value'])
    else:
        print_colored(key, value)
