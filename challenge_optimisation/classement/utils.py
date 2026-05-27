from django.shortcuts import get_object_or_404
from solutions.models import Instance, Solution

class Classement:
    """Classe utilitaire pour gérer les classements."""

    @staticmethod
    def by_instance(instance_id):
        """Retourne le classement des équipes pour une instance donnée."""
        instance_active = get_object_or_404(Instance, id=instance_id)
        solutions = (
            Solution.objects
            .filter(instance=instance_active)
            .select_related('equipe')
            .order_by('score')
        )
        
        rank = 0
        for sol in solutions:
            if sol.equipe.nom == "Solution de référence":
                sol.rank = None
                sol.is_reference = True
            else:
                rank += 1
                sol.rank = rank
                sol.is_reference = False

        return instance_active, solutions

    @staticmethod
    def for_all_instances():
        """Retourne le classement global : meilleure équipe par instance."""
        instances = Instance.objects.all().order_by('numero')
        classement_global = []

        for instance in instances:

            # Meilleure équipe hors "Solution de référence"
            meilleure_solution = (
                Solution.objects
                .filter(instance=instance)
                .exclude(equipe__nom="Solution de référence")
                .select_related('equipe')
                .order_by('score')
                .first()
            )

            # Score de référence
            solution_reference = (
                Solution.objects
                .filter(
                    instance=instance,
                    equipe__nom="Solution de référence"
                )
                .first()
            )

            classement_global.append({
                'instance'       : instance.nom,
                'equipe'         : meilleure_solution.equipe.nom
                                   if meilleure_solution else 'Aucune',
                'score'          : meilleure_solution.score
                                   if meilleure_solution else '-',
                'score_reference': solution_reference.score
                                   if solution_reference else '-',
            })

        return classement_global