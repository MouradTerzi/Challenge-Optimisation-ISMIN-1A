from django.urls import path
from . import views

urlpatterns = [
    path('', views.accueil, name='accueil'),
    path('ajouter/', views.ajouter_equipe, name='ajouter_equipe'),
    path('ajoutee/<str:nom>/', views.equipe_ajoutee,
         name='sucess'),  # ← nouveau
]
