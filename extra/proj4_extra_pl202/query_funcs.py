import os
os.environ.setdefault("DJANGO_SETTINGS_MODULE", "extra_proj4.settings")

import django
django.setup()
    
from myapp.models import State,Color,Team,Player    

def query1(use_mpg, min_mpg, max_mpg,
            use_ppg, min_ppg, max_ppg,
            use_rpg, min_rpg, max_rpg,
            use_apg, min_apg, max_apg,
            use_spg, min_spg, max_spg,
            use_bpg, min_bpg, max_bpg):
            flag = 0
            if(use_mpg == 1):
                res = Player.objects.all().filter(mpg__range=(min_mpg, max_mpg))
                flag = 1
            if(use_ppg == 1):
                if(flag == 1):
                    res.filter(ppg__range=(min_ppg, max_ppg))
                else:
                    res = Player.objects.all().filter(ppg__range=(min_ppg, max_ppg))
            if(use_rpg == 1):
                if(flag == 1):
                    res.filter(rpg__range=(min_rpg, max_rpg))
                else:
                    res = Player.objects.all().filter(rpg__range=(min_rpg, max_rpg))
            if(use_apg == 1):
                if(flag == 1):
                    res.filter(apg__range=(min_apg, max_apg))
                else:
                    res = Player.objects.all().filter(apg__range=(min_apg, max_apg))
            if(use_spg == 1):
                if(flag == 1):
                    res.filter(spg__range=(min_spg, max_spg))
                else:
                    res = Player.objects.all().filter(spg__range=(min_spg, max_spg))
            
            if(use_bpg == 1):
                if(flag == 1):
                    res.filter(bpg__range=(min_bpg, max_bpg))
                else:
                    res = Player.objects.all().filter(bpg__range=(min_bpg, max_bpg))
            print("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
            for player in res:
                print(player.player_id,player.team_id.team_id, player.uniform_num, player.first_name, player.last_name,end=" ")
                print(player.mpg, player.ppg, player.rpg, player.apg, player.spg, player.bpg)
            return 


def query2(team_color):
    res = Team.objects.all().select_related('color_id')
    ans = res.filter(color_id__name = team_color)
    print("Name")
    for teamName in ans:
        print(teamName.name)
    return 

def query3(team_name):
    res = Player.objects.all().select_related('team_id').filter(team_id__name = team_name)
    res.order_by('-ppg')
    print("FIRST_NAME LAST_NAME")
    for player_info in res:
        print(player_info.first_name,end=" ")
        print(player_info.last_name)
    return

def query4(team_state, team_color):
    res = Player.objects.all().select_related('team_id')
    ans = res.filter(team_id__state_id__name = team_state, team_id__color_id__name = team_color)
    print("UNIFORM_NUM FIRST_NAME LAST_NAME")
    for player_info in ans:
        print(player_info.uniform_num, end=" ")
        print(player_info.first_name, end=" ")
        print(player_info.last_name)
    return

def query5(num_wins):
    res = Player.objects.all().select_related('team_id')
    ans = res.filter(team_id__wins__gt=num_wins)
    print("FIRST_NAME LAST_NAME NAME WINS")
    for player_info in ans:
        print(player_info.first_name, player_info.last_name, player_info.team_id.name, player_info.team_id.wins)
    return 

def main():
     query1(0, 35, 40, 0, 0, 0, 1, 5, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0)
     query2("Green")
     return 

if __name__ == "__main__":
    main()

