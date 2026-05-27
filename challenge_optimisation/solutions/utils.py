"""
#1. Créer les instances

from ton_app.models import Instance

instances = [
    Instance(
        nom=f"INSTANCE {i+1}",
        numero=i+1,
        description=f"INSTANCE {i+1} du challenge optimisation ISMIN 1A 2026"
    )
    for i in range(15)
]

Instance.objects.bulk_create(instances)

#2. Créer l'équipe "Solution de référence"

equipe = Equipe.objects.create(nom = "Solution de référence")
etudiant = Etudiant.objects.create(nom = "Feillet", prenom = "Dominique", equipe = equipe)

scores = [2515, 1910, 2614, 2717, 2363, 2721, 2114, 2505, 2191, 2301, 2491, 2148, 2511, 2288, 1542]

instances = Instance.objects.all()

solutions = [Solution(equipe = equipe,instance = instance, score = score) for instance, score in zip(instances,scores)]

solutions = [Solution()]
"""