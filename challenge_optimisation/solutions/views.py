from django.shortcuts import render, redirect
from pprint import pprint

from .checker import ( 
    verifier_entete,
    sauvegarder_fichier_temporaire,
    appeler_checker,
    mise_a_jour_meilleur_sol)

"""
Utilisateur arrive sur le site
        │
        ▼
Django crée la session
sessionid=abc123
        │
        ▼
Utilisateur navigue sur le site
        │
        ├── request.session['resultats'] = {...}  ← écriture
        ├── request.session.get('resultats')      ← lecture
        └── request.session.pop('resultats')      ← lecture + suppression
        │
        ▼
Fin de session ?
        │
        ├── Expiration (2 semaines par défaut)
        ├── Fermeture navigateur (si configuré)
        ├── logout()
        └── flush()
        │
        ▼
Session supprimée de django_session
Cookie supprimé du navigateur
"""

"""
    Équipe soumet des fichiers
            │
            ▼
    session['resultats'] = {...}     ← stocké
            │
            ▼
    redirect → resultat_solution()
            │
            ▼
    session.pop('resultats')         ← lu et supprimé immédiatement
            │
            ▼
    Page résultat affichée
    Session toujours active mais resultats supprimés
"""
def soumettre_solution(request):
    resultats = {}

    if request.method == 'POST':
        fichiers = request.FILES.getlist('fichiers')
        if fichiers:
            for fichier in fichiers:
                #1. Vérifier l'entête
                res = verifier_entete(fichier)
                if res['statut'] == 'ok':
                    #2. Sauvegarder temporairement le fichier
                    tmp_path = sauvegarder_fichier_temporaire(fichier)
    
                    #3. Appeler le checker
                    appeler_checker(tmp_path,res)
                    pprint(res)
                    if res['statut'] == 'ok':
                        #4. Mise à jour de la meilleure solution
                        mise_a_jour_meilleur_sol(fichier,res)
                        
                #5. Supprimer les objets non sérialisables avant la session
                res['equipe'] = res['equipe'].nom if res.get('equipe') else None
                res['instance'] = res['instance'].nom if res.get('instance') else None
                resultats[fichier.name] = res
            
            request.session['resultats'] = resultats

            return redirect('resultat_solution')
 
        else:
            print("Aucun fichier reçu !")

    return render(request, 'solutions/soumettre.html', {})

def resultat_solution(request):
    """Affiche le résultat du checker sur une page dédiée."""
    resultats = request.session.pop('resultats', {})
    return render(request, 'solutions/resultat.html', {
        'resultats': resultats
    })
