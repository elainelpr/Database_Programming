#include "query_funcs.h"
#include <sstream>
#include <iomanip>
using namespace std;
void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    stringstream spg_str, bpg_str;
    spg_str<<spg;
    bpg_str<<bpg;
    work W(*C);
    string insert_playerMssg = "INSERT INTO PLAYER(TEAM_ID, UNIFORM_NUM,FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES("+ to_string(team_id)
                               +","+to_string(jersey_num)+","+W.quote(first_name)+","+W.quote(last_name)+","+to_string(mpg)+","+to_string(ppg)+","
                               +to_string(rpg)+","+to_string(apg)+","+spg_str.str()+","+bpg_str.str()+");";
    W.exec(insert_playerMssg);
    W.commit();

}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    string insert_teamMssg = "INSERT INTO TEAM(NAME, STATE_ID,COLOR_ID,WINS,LOSSES) VALUES("
                            +W.quote(name)+","+to_string(state_id)+","+to_string(color_id)+","+to_string(wins)+","+to_string(losses)+");";
    W.exec(insert_teamMssg);
    W.commit();                       
}


void add_state(connection *C, string name)
{
    work W(*C);
    string insert_name = name;
    insert_name = "INSERT INTO STATE(NAME) VALUES(" + W.quote(name)  + ");";
    W.exec(insert_name);
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    string insert_name = "INSERT INTO COLOR(NAME) VALUES(" +   W.quote(name) + ");";
    W.exec(insert_name);
    W.commit();
}

void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    string attribute;
    int flag = 0;
    if(use_mpg == 1){
        attribute = "SELECT * FROM PLAYER WHERE MPG<="+to_string(max_mpg)+" AND MPG>="+to_string(min_mpg);
        flag = 1;
    }

    if(use_ppg == 1){
        if(flag == 0){
            attribute = "SELECT * FROM PLAYER WHERE PPG<="+to_string(max_ppg)+" AND PPG>="+to_string(min_ppg);
            flag = 1;
        }
        else{
            attribute += " AND PPG<="+to_string(max_ppg)+" AND PPG>="+to_string(min_ppg);
        }
        
    }

    if(use_rpg == 1){
        if(flag == 0){
            attribute = "SELECT * FROM PLAYER WHERE RPG<="+to_string(max_rpg)+" AND RPG>="+to_string(min_rpg);
            flag = 1;
        }
        else{
            attribute += " AND RPG<="+to_string(max_rpg)+" AND RPG>="+to_string(min_rpg);
        }
        
    }

    if(use_apg == 1){
        if(flag == 0){
            attribute = "SELECT * FROM PLAYER WHERE APG<="+to_string(max_apg)+" AND APG>="+to_string(min_apg);
            flag = 1;
        }
        else{
            attribute += " AND APG<="+to_string(max_apg)+" AND APG>="+to_string(min_apg);
        }
        
    }

    if(use_spg == 1){
         if(flag == 0){
            attribute = "SELECT * FROM PLAYER WHERE SPG<="+to_string(max_spg)+" AND SPG>="+to_string(min_spg);
            flag = 1;
        }
        else{
            attribute += " AND SPG<="+to_string(max_spg)+" AND SPG>="+to_string(min_spg);
        }
        
    }

    if(use_bpg == 1){
        if(flag == 0){
            attribute = "SELECT * FROM PLAYER WHERE BPG<="+to_string(max_bpg)+" AND BPG>="+to_string(min_bpg);
            flag = 1;
        }

        else{
             attribute += " AND BPG<="+to_string(max_bpg)+" AND BPG>="+to_string(min_bpg);
        }
       
    }

    attribute += ";";

    nontransaction N(*C);
    result R( N.exec(attribute));
    result::iterator it = R.begin();
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"
       << endl;
    for(; it!=R.end(); it++){
        cout << it[0].as<int>() << " " << it[1].as<int>() << " " << it[2].as<int>()
         << " " << it[3].as<string>() << " " << it[4].as<string>() << " "
         << it[5].as<int>() << " " << it[6].as<int>() << " " << it[7].as<int>()
         << " " << it[8].as<int>() << " " <<fixed<< setprecision(1)
         << it[9].as<double>() << " " <<setprecision(1)<< it[10].as<double>() << endl;
    }
}


void query2(connection *C, string team_color)
{
    string team_name = "SELECT TEAM.NAME FROM TEAM, COLOR WHERE TEAM.COLOR_ID = COLOR.COLOR_ID AND COLOR.NAME='"
                        +team_color+"'"+";";
    nontransaction N(*C);
    result R( N.exec(team_name));
    result::iterator it = R.begin();
    cout<<"NAME"<<endl;
    for(; it!=R.end(); it++){
        cout<<it[0].as<string>()<<endl;
    }
}


void query3(connection *C, string team_name)
{
    string player_name = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.NAME = '"
                        +team_name+"'""ORDER BY PLAYER.PPG DESC";
    nontransaction N(*C);
    result R( N.exec(player_name));
    result::iterator it = R.begin();
    cout<<"FIRST_NAME"<<" "<<"LAST_NAME"<<endl;
    for(; it!=R.end(); it++){
        cout<<it[0].as<string>()<<" "<<it[1].as<string>()<<endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    string player_info = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM FROM PLAYER, COLOR, STATE, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID AND STATE.NAME = '"
                         +team_state+"'"+"AND COLOR.NAME = '"+team_color+"';";
    nontransaction N(*C);
    result R( N.exec(player_info));
    result::iterator it = R.begin();
    cout<<"UNIFORM_NUM FIRST_NAME LAST_NAME"<<endl;
    for(; it!=R.end(); it++){
        cout<<it[2].as<int>()<<" "<<it[0].as<string>()<<" "<<it[1].as<string>()<<endl;
    }
}


void query5(connection *C, int num_wins)
{
    string team_name = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS FROM PLAYER, TEAM WHERE PLAYER.TEAM_ID = TEAM.TEAM_ID AND TEAM.WINS >"
                        +to_string(num_wins);
    nontransaction N(*C);
    result R( N.exec(team_name));
    result::iterator it = R.begin();
    cout<<"FIRST_NAME"<<" "<<"LAST_NAME"<<" NAME"<<" WINS"<<endl;
    for(; it!=R.end(); it++){
        cout<<it[0].as<string>()<<" "<<it[1].as<string>()<<" "<<it[2].as<string>()<<" "<<it[3].as<int>()<<endl;
    }
}
