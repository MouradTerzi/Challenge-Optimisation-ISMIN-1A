from django.urls import path
from . import views

urlpatterns = [
    path('soumettre/', views.soumettre_solution, name='soumettre_solution'),
    path('resultat/', views.resultat_solution, name='resultat_solution'),
]