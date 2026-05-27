from django.db import models
from equipes.models import Equipe


class Instance(models.Model):
    nom = models.CharField(max_length=100, unique=True)
    numero = models.IntegerField(unique=True)
    description = models.TextField(blank=True)
    date_ajout = models.DateTimeField(auto_now_add=True)

    def __str__(self):
        return self.nom

class Solution(models.Model):
    equipe          = models.ForeignKey(Equipe, on_delete=models.CASCADE)
    instance        = models.ForeignKey(Instance, on_delete=models.CASCADE)
    fichier         = models.FileField(upload_to='solutions/',blank=True, null=True)  # Chemin de la meilleure solution
    date_soumission = models.DateTimeField(auto_now_add=True)
    score           = models.FloatField(default=0)
    
    class Meta:
        unique_together = ('equipe', 'instance')  # une seule entrée par équipe/instance

    def __str__(self):
        return f"{self.equipe} - {self.instance} - {self.score}"