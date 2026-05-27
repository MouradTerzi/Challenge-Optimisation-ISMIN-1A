from django.urls import path
from .views import classement_view


urlpatterns = [
    path('', classement_view, name='classement'),
    path('<int:instance_id>/', classement_view, name='classement_instance'),
]