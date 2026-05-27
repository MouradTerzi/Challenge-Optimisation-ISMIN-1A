from django import forms
from .models import Equipe, Etudiant

"""
    La fonction clean est exécutée automatiquement quand Djano exécute form.is_valid().
"""

class EquipeForm(forms.Form):

    nom_equipe = forms.CharField(max_length=100, label="Nom de l'équipe")
    
    nb_etudiants = forms.ChoiceField(
        choices=[('1', '1 étudiant'), ('2', '2 étudiants')],
        label="Nombre d'étudiants"
    )
    
    # Etudiant 1
    nom1 = forms.CharField(max_length=100, label="Nom")
    prenom1 = forms.CharField(max_length=100, label="Prénom")

    # Etudiant 2 (optionnel)
    nom2 = forms.CharField(max_length=100, label="Nom",    required=False)
    prenom2 = forms.CharField(max_length=100, label="Prénom", required=False)

    def clean(self):
        cleaned_data = super().clean()
        nom_equipe = cleaned_data.get('nom_equipe',' ').strip()
        nb = int(cleaned_data.get('nb_etudiants', 1))
        nom1 = cleaned_data.get('nom1',' ').strip()
        prenom1 = cleaned_data.get('prenom1',' ').strip()
        nom2 = cleaned_data.get('nom2',' ').strip()
        prenom2 = cleaned_data.get('prenom2',' ').strip()
        
        # 1) Vérifier si l'équipe existe déjà
        if nom_equipe and Equipe.objects.filter(nom__iexact=nom_equipe).exists():
            self.add_error('nom_equipe',
                           f"L'équipe « {nom_equipe} » existe déjà !")

        # 2) Vérifier si étudiant 1 existe déjà
        if nom1 and prenom1:
            if Etudiant.objects.filter(
                nom__iexact=nom1,
                prenom__iexact=prenom1
            ).exists():
                self.add_error('nom1',
                               f"L'étudiant « {prenom1} {nom1} » est déjà inscrit dans une équipe !")

        # 3) Si 2 étudiants : vérifier champs obligatoires + existence étudiant 2
        if nb == 2:
            if not nom2:
                self.add_error(
                    'nom2', "Ce champ est obligatoire.")
            if not prenom2:
                self.add_error(
                    'prenom2', "Ce champ est obligatoire.")
        
            if nom2 and prenom2:
                if Etudiant.objects.filter(
                    nom__iexact=nom2,
                    prenom__iexact=prenom2
                ).exists():
                    self.add_error('nom2',
                                   f"L'étudiant « {prenom2} {nom2} » est déjà inscrit dans une équipe !")

        return cleaned_data
    