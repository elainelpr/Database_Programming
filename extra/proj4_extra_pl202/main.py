import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "extra_proj4.settings")
import django
django.setup()
from myapp.models import State,Color,Team,Player
import query_funcs

def color_insert():
    with open("color.txt") as color_file:
        for line in color_file:
            line_name = line.split()
            color_name = Color(name = line_name[1])
            color_name.save()
    color_file.close()
    return 

def state_insert():
    with open("state.txt") as state_file:
        for state_line in state_file:
            state_name = state_line.split()
            state_name = State(name = state_name[1])
            state_name.save()
    state_file.close()
    return 

def team_insert():
    with open("team.txt") as team_file:
        for team_line in team_file:
            team_info =  team_line.split()
            team = Team(name = team_info[1],state_id = State.objects.get(state_id = team_info[2]), color_id =Color.objects.get(color_id = team_info[3]), wins = team_info[4], losses = team_info[5])
            team.save()
    team_file.close()
    return

def player_insert():
    with open("player.txt") as player_file:
        for player_line in player_file:
            player_info = player_line.split()
            player = Player(team_id = Team.objects.get(team_id = player_info[1]), uniform_num = player_info[2],
                            first_name=player_info[3], last_name=player_info[4], mpg=player_info[5],ppg=player_info[6],
                            rpg=player_info[7], apg=player_info[8],spg=player_info[9],bpg=player_info[10])
            player.save()
    player_file.close()
    return

def main():
    color_insert()
    state_insert()
    team_insert()
    player_insert()

if __name__ == "__main__":
    main()
