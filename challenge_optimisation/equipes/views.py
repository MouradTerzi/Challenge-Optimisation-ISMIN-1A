from django.shortcuts import render, redirect
from .forms import EquipeForm
from .models import Equipe, Etudiant
from solutions.models import Solution, Instance

"""
# Fonctionnement Django des formulaires :
# Lorsque form.is_valid() est appelé :
# 1) Django valide chaque champ individuellement (required, type, max_length, etc.)
# 2) Les méthodes clean_<field>() sont exécutées si elles existent
# 3) Ensuite la méthode clean() du formulaire est appelée (validation globale)
# 4) Si aucune erreur n’est ajoutée, is_valid() retourne True
#    sinon le formulaire est invalide et les erreurs sont accessibles via form.errors
#
# Ici, clean() est utilisé pour appliquer des règles métier :
# - vérifier si l'équipe existe déjà
# - vérifier si un étudiant est déjà inscrit
# - gérer la contrainte selon le nombre d'étudiants (1 ou 2)
#
# Ordre d'exécution complet :
#
# form.is_valid()
#       │
#       ▼
# Validation champ par champ
# (required, type, max_length...)
#       │
#       ▼
# clean_nom_equipe()   ← si elle existe
# clean_nom1()         ← si elle existe
# ...
#       │
#       ▼
# clean()              ← validation globale (règles métier)
#       │
#       ├── erreurs → is_valid() = False → form.errors
#       └── pas d'erreurs → is_valid() = True → form.cleaned_data
"""
def accueil(request):
    return render(request, 'accueil.html')

def ajouter_equipe(request):
    if request.method == 'POST':
        form = EquipeForm(request.POST)
        nb_etudiants = request.POST.get('nb_etudiants', '1')
        if form.is_valid():
            nom_equipe = form.cleaned_data['nom_equipe'].strip()
            nb = int(form.cleaned_data['nb_etudiants'])
            nom1 = form.cleaned_data['nom1'].strip().title()
            prenom1 = form.cleaned_data['prenom1'].strip().title()
            nom2 = form.cleaned_data.get('nom2').strip().title()
            prenom2 = form.cleaned_data.get('prenom2').strip().title()

            equipe = Equipe.objects.create(nom=nom_equipe)
            Etudiant.objects.create(equipe=equipe, nom=nom1, prenom=prenom1)

            if nb == 2:
                Etudiant.objects.create(
                    equipe=equipe, nom=nom2, prenom=prenom2)
            
            solutions = [
                Solution(equipe=equipe, instance=instance, score=1e20)
                for instance in Instance.objects.all()
            ]
            
            Solution.objects.bulk_create(solutions)

            return redirect('sucess', nom=nom_equipe)
    else:
        form = EquipeForm()
        nb_etudiants = '1'

    return render(request, 'equipes/ajouter.html', {
        'form': form,
        'nb_etudiants': nb_etudiants,   # ← on passe la valeur directement
    })

def equipe_ajoutee(request, nom):
    return render(request, 'equipes/succes.html', {'nom': nom})
