from django.shortcuts import render, get_object_or_404
from django.db.models import Sum
from solutions.models import Instance, Solution
from .utils import Classement
from pprint import pprint

def classement_view(request, instance_id=None):
    instances = Instance.objects.all().order_by('numero')
    instance_active = None
    solutions = []
    classement_global = []

    if instance_id:
        instance_active, solutions = Classement.by_instance(instance_id)
    else:
        
        classement_global = Classement.for_all_instances()

    return render(request, 'classement/classement.html', {
        'instances': instances,
        'instance_active': instance_active,
        'solutions': solutions,
        'classement_global': classement_global,
        'nom_reference'    : 'Solution de référence'
    })