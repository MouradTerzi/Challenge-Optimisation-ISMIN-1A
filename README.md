# Challenge-Optimisation-ISMIN-1A
Challenge Optimisation ISMIN 1A

# Structure du projet
challenge_optimisation/          ← dossier racine du projet
│
├── manage.py
├── solution/                    ← application Django
├── equipe/                      ← autre application Django
├── autre_app/
│
└── challenge_optimisation/      ← package de configuration Django
    ├── __init__.py
    ├── settings.py
    ├── urls.py
    ├── asgi.py
    └── wsgi.py


# Déploiement sur Render
# Étape 1 : Modifier le fichier settings.py
Dans :
challenge_optimisation/challenge_optimisation/settings.py

modifier les paramètres suivants :
- DEBUG = False

- ALLOWED_HOSTS = ['.onrender.com']

# Étape 2 : Créer le fichier requirements.txt
Créer un fichier requirements.txt à la racine du projet :
challenge_optimisation/
│
├── manage.py
├── requirements.txt
└── ...

Contenu du fichier :
Django==6.0.4
gunicorn==26.0.0
whitenoise==6.12.0
psycopg2-binary==2.9.12
dj-database-url==3.1.2

Rôle des dépendances
- gunicorn : Serveur WSGI utilisé pour exécuter l’application Django en production.
- whitenoise : Permet à Django de servir les fichiers statiques (CSS, JavaScript, images) en production.
- psycopg2-binary : Pilote Python permettant à Django de se connecter à une base de données PostgreSQL.
- dj-database-url : Permet de configurer automatiquement la base de données PostgreSQL à partir d’une URL d’environnement fournie par Render.

Il est recommandé d'installer les dépendances sur la machine locale pour tester l'application dans les mêmes conditions que sur Render : pip install gunicorn whitenoise psycopg2-binary dj-database-url

Dans :
challenge_optimisation/challenge_optimisation/settings.py

ajouter :
'whitenoise.middleware.WhiteNoiseMiddleware' après la ligne 'django.middleware.security.SecurityMiddleware'
dans MIDDLEWARE

# Étape 3 : Créer le fichier build.sh 
Créer un fichier build.sh à la racine du projet :
challenge_optimisation/
│
├── manage.py
├── build.sh
└── ...

Contenu du fichier : 
- #!/usr/bin/env bash -> Indique que le script doit être exécuté avec l’interpréteur Bash.

- set -o errexit ->Arrête immédiatement l’exécution du script si une commande génère une erreur.
Cela évite de continuer le déploiement avec une configuration incomplète ou incorrecte.

- pip install -r requirements.txt -> Installe automatiquement toutes les dépendances définies dans le fichier requirements.txt.

- python manage.py collectstatic --no-input -> Collecte tous les fichiers statiques de l’application dans un dossier unique. L’option --no-input évite les questions interactives pendant le déploiement automatique.

- python manage.py migrate -> Applique les migrations de la base de données.
Cette commande :
    crée les tables PostgreSQL,
    met à jour la structure de la base,
    applique les modifications des modèles Django.

- python manage.py loaddata data.json -> Charge des données initiales dans la base de données à partir du fichier data.json. Le fichier data.json est généralement un fixture Django exporté avec : 

python manage.py dumpdata --indent 2 > data.json ou 
python manage.py dumpdata equipes solutions classement --indent 2 > data.json (seules les données métiers)

Dans :
challenge_optimisation/challenge_optimisation/settings.py

ajouter:
FIXTURE_DIRS = [
    BASE_DIR / 'fixtures',
]

Rôle de FIXTURE_DIRS
Cette variable indique à Django où chercher les fichiers de données (fixtures), par exemple :
data.json fichiers d’initialisation de base de données

Cela permet d’utiliser : python manage.py loaddata data.json sans avoir à préciser le chemin complet.

g++ -std=c++17 checker/*.cpp -o checker/checker -> Compiler le checker directement sur Render

# Etape 4 : Créer le Web Service sur Render
Sur Render :
Configuration du service
- Name : Challenge-Optimisation-ISMIN-1A
- Source : https://github.com/MouradTerzi/Challenge-Optimisation-ISMIN-1A
- Branch : main
- Root Directory : challenge_optimisation
- Build Command : ./build.sh
- Start Command : gunicorn challenge_optimisation.wsgi:application

Rôle du Start Command
gunicorn : serveur de production pour Django
challenge_optimisation.wsgi:application : point d’entrée WSGI du projet

# Étape 5 : Ajouter la base de données PostgreSQL
1. Créer la base sur Render
Aller sur Render
Créer un service PostgreSQL
Copier l’URL fournie

2. Ajouter la variable d’environnement
Dans le Web Service Render :
DATABASE_URL = ...


3. Modifier settings.py
Dans :
challenge_optimisation/challenge_optimisation/settings.py

ajouter :
import dj_database_url
import os

DATABASES = {
    'default': dj_database_url.config(
        default=os.environ.get('DATABASE_URL')
    )
}

Explication
- os.environ.get('DATABASE_URL') récupère la variable d’environnement Render
- dj_database_url.config() transforme cette URL en configuration Django
- Cela permet de passer facilement de SQLite (local) à PostgreSQL (production)









