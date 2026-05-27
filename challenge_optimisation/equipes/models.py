from django.db import models


class Equipe(models.Model):
    nom = models.CharField(max_length=100, unique=True)
    date_creation = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.nom


class Etudiant(models.Model):
    equipe = models.ForeignKey(
        Equipe,
        on_delete=models.CASCADE,
        related_name='etudiants'
    )
    nom = models.CharField(max_length=100)
    prenom = models.CharField(max_length=100)

    def __str__(self):
        return f"{self.prenom} {self.nom}"
