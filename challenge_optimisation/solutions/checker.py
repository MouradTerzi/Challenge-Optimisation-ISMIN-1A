import tempfile
import subprocess
import os
from django.conf import settings
from equipes.models import Equipe
from .models import Instance, Solution

"""
  Vérifier l'entête
"""
def lire_entete(fichier):
    """Lit et retourne les deux premières lignes du fichier."""
    lignes = fichier.read().decode('utf-8').splitlines()
    fichier.seek(0)  # ← rembobiner pour pouvoir relire le fichier après
    if len(lignes) < 2:
        return None, None
    ligne_equipe   = lignes[0].strip()  # ex: "EQUIPE SFL"
    ligne_instance = lignes[1].strip()  # ex: "INSTANCE 1"
    return ligne_equipe, ligne_instance

def extraire_nom_equipe(ligne_equipe):
    """Extrait le nom de l'équipe depuis la ligne EQUIPE xxx."""
    if not ligne_equipe or not ligne_equipe.startswith("EQUIPE "):
        return None
    return ligne_equipe[len("EQUIPE "):].strip()

def extraire_nom_instance(ligne_instance):
    """Extrait le nom de l'instance depuis la ligne INSTANCE xxx."""
    if not ligne_instance or not ligne_instance.startswith("INSTANCE "):
        return None
    numero = ligne_instance[len("INSTANCE "):].strip()
    
    return f"INSTANCE {numero}"

def extraire_numero_instance(ligne_instance):
    """Extrait le nom de l'instance depuis la ligne INSTANCE xxx."""
    if not ligne_instance or not ligne_instance.startswith("INSTANCE "):
        return None, "Erreur INSTANCE"
    
    try:
        numero = int(ligne_instance[len("INSTANCE "):].strip())
        
    except ValueError:
        return None, "Erreur numéro"
    
    return numero,""
    
def verifier_equipe_existe(nom_equipe):
    """Vérifie si l'équipe existe en base de données."""
    if not nom_equipe:
        return None
    return Equipe.objects.filter(nom=nom_equipe).first()

def verifier_instance_existe(numero_instance):
    """Vérifie si l'instance existe en base de données."""
    if not numero_instance:
        return None
    return Instance.objects.filter(numero=numero_instance).first()

def verifier_entete(fichier):
    
    erreurs  = []

    # Lire les deux premières lignes
    ligne_equipe, ligne_instance = lire_entete(fichier)

    if ligne_equipe is None or ligne_instance is None:
        return {
            'statut'  : 'erreur',
            'messages': ["Le fichier doit contenir au moins 2 lignes "
                         "(EQUIPE nom_equipe et INSTANCE numéro)."],
            'equipe'  : None,
            'instance': None
        }
    
    nom_equipe   = extraire_nom_equipe(ligne_equipe)
    num_instance, message_num_instance = extraire_numero_instance(ligne_instance)
    
    # Vérifier le format
    if nom_equipe is None:
        erreurs.append(
            f"Format incorrect ligne 1 : doit commencer par 'EQUIPE '."
            f" Valeur lue : « {ligne_equipe} »"
        )

    if num_instance is None:
        if message_num_instance == "Erreur INSTANCE":
            erreurs.append(
            f"Format incorrect ligne 2 : doit commencer par 'INSTANCE '."
            f" Valeur lue : « {ligne_instance} »"
        )
        
        else:
            erreurs.append(
            f"Format incorrect ligne 2 : le numéro de l'instance doit être un entier '."
            f" Valeur lue : « {ligne_instance} »"
        )

    if erreurs:
        return {
            'statut'  : 'erreur',
            'messages': erreurs,
            'equipe'  : None,
            'instance': None
        }
    
    #Vérifier si l'équipe est : Solution de référence
    if nom_equipe == "Solution de référence":
        return {
            'statut'  : 'erreur',
            'messages': ["La Solution de référence ne peut pas être modifiée."],
            'equipe'  : None,
            'instance': None
        }

    # Vérifier existence en base
    equipe   = verifier_equipe_existe(nom_equipe)
    instance = verifier_instance_existe(num_instance)
   
    if equipe is None:
        erreurs.append(
            f"L'équipe « {nom_equipe} » n'existe pas."
        )

    if instance is None:
        erreurs.append(
            f"L'INSTANCE {num_instance} n'existe pas."
        )

    if erreurs:
        return {
            'statut'  : 'erreur',
            'messages': erreurs,
            'equipe'  : None,
            'instance': None
        }
    
    return {
        'statut'  : 'ok',
        'equipe'  : equipe,
        'instance': instance
    }

"""
    Appeler le checker
""" 
def sauvegarder_fichier_temporaire(fichier):
    """Sauvegarde le fichier uploadé dans un fichier temporaire."""
    with tempfile.NamedTemporaryFile(delete=False, suffix='.txt') as tmp:
        for chunk in fichier.chunks():
            tmp.write(chunk)
        return tmp.name 
    
def appeler_checker(tmp_path,resultat):
    """Appelle l'exécutable checker et retourne son output."""
    verification = subprocess.run(
        [settings.CHECKER_PATH, tmp_path],
        capture_output=True,
        text=True,
        cwd=os.path.dirname(settings.CHECKER_PATH)
    )
    message = verification.stdout.strip()
    print(f"Message du checker :{message}")
    if "_Erreur solution non realisable" in message:
        resultat['statut'] = 'erreur'
        resultat['messages'] = message.splitlines()
    
    else:
        try: #Par protection d'un format inattendu : try
            resultat['score'] = float(message.strip())

        except (ValueError, TypeError):
            resultat['statut'] = 'erreur'
            resultat['messages'] = message.splitlines()

def mise_a_jour_meilleur_sol(fichier,resultat):
    solution = Solution.objects.filter(
        equipe=resultat['equipe'],
        instance=resultat['instance']
    ).first()
    
    if solution:
        resultat['ancien_meilleur_score'] = solution.score
        nouveau_score = resultat['score']
        resultat['nouveau_meilleur_score'] = min(solution.score,nouveau_score)
        if solution.score > nouveau_score:
            resultat['score_amélioré'] = f"Le meilleur score de l'équipe {resultat['equipe'].nom} sur l'instance {resultat['instance'].nom} a été amélioré"
            solution.score = nouveau_score
            nom_fichier = f"EQUIPE_{solution.equipe.nom}_{solution.instance.nom}.txt"

            if fichier:
                if solution.fichier:
                    solution.fichier.delete(save=False)

                solution.fichier.save(
                    nom_fichier,
                    fichier,
                    save=False
                )
            solution.save() #Peut être à mettre à l'interieur de if pour sauvegarder le score et le fichier
        
        else:
            resultat['score_amélioré'] = f"Le meilleur score de l'équipe {resultat['equipe'].nom} sur {resultat['instance'].nom} n'a pas été amélioré"
    else:
        print("Aucune solution trouvée")