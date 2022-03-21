from django.db import models

# Create your models here.
class Color(models.Model):
    color_id = models.AutoField(primary_key = True)
    name = models.CharField(max_length = 100)

class State(models.Model):
    state_id = models.AutoField(primary_key = True)
    name = models.CharField(max_length = 100)

class Team(models.Model):
    team_id = models.AutoField(primary_key = True)
    name = models.CharField(max_length = 100)
    state_id = models.ForeignKey(State,related_name='Team_stateID',on_delete=models.CASCADE)
    color_id = models.ForeignKey(Color, related_name='Team_colorID',on_delete=models.CASCADE)
    wins = models.IntegerField(default = 0)
    losses = models.IntegerField(default = 0)

class Player(models.Model):
    player_id = models.AutoField(primary_key = True)
    team_id = models.ForeignKey(Team, on_delete = models.CASCADE)
    uniform_num = models.IntegerField(default = 0)
    first_name = models.CharField(max_length=100)
    last_name = models.CharField(max_length = 100)
    mpg = models.IntegerField(default=0)
    ppg = models.IntegerField(default=0)
    rpg = models.IntegerField(default=0)
    apg = models.IntegerField(default = 0)
    spg = models.FloatField()
    bpg = models.FloatField()
