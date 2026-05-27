#!/usr/bin/env bash

set -o errexit

pip install -r requirements.txt

python manage.py collectstatic --no-input

python manage.py migrate

python manage.py loaddata data.json

# Compiler le checker directement sur Render 
g++ -std=c++17 challenge_optimisation/checker/*.cpp -o challenge_optimisation/checker/checker 
