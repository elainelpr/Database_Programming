#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "exerciser.h"
#include <string>
using namespace std;
using namespace pqxx;


void exec_sql(connection *c, string sql){
  work W(*c);
  W.exec(sql);
  W.commit();
}

void drop_tables(connection *c){
  //if the table is exist, drop it
  //delete player first and then team, or it will obey Referential Integrity Constraint
  string drop_table = "drop table if exists player, team, color, state";
  exec_sql(c, drop_table);
}

void create_table(string filename, connection *c){
  string line;
  string table_content;
  ifstream initial_file;
  initial_file.open(filename.c_str());
  if(!initial_file){
    perror("Fail to open the file\n");
    exit(EXIT_FAILURE);
  }
  else{
    while(getline(initial_file, line)){
      table_content = table_content + line;
    }

    initial_file.close();
    
    exec_sql(c, table_content);
  }
}

void player_insert(connection *c){
  string temp;
  ifstream player_txt;
  string filename = "player.txt";
  string player_id, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg;
  player_txt.open(filename.c_str());
  if(!player_txt){
    perror("Cannot open the player file\n");
    exit(EXIT_FAILURE);
  }
  else{
    while(getline(player_txt, temp)){
      istringstream line(temp);
      line>>player_id>>team_id>>uniform_num>>first_name>>last_name>>mpg>>ppg>>rpg>>apg>>spg>>bpg;
      add_player(c, stoi(team_id),stoi(uniform_num), first_name, last_name, stoi(mpg), stoi(ppg),
                stoi(rpg), stoi(apg), stod(spg), stod(bpg));
    }
  }

  player_txt.close();
}

void team_insert(connection *c){
  string temp;
  ifstream team_txt;
  string filename = "team.txt";
  string team_id;
  string team_name;
  string team_stateID;
  string team_colorID;
  string team_losses;
  string team_wins;
  team_txt.open(filename.c_str());
  if(!team_txt){
    perror("Cannot open the team txt file\n");
    exit(EXIT_FAILURE);
  }
  else{
    while(getline(team_txt, temp)){
      istringstream line(temp);
      line>>team_id>>team_name>>team_stateID>>team_colorID>>team_wins>>team_losses;
      add_team(c, team_name, stoi(team_stateID), stoi(team_colorID), stoi(team_wins), stoi(team_losses));
    }
  }
  team_txt.close();
}

void state_insert(connection *C){
  string temp;
  ifstream state_txt;
  string filename = "state.txt";
  size_t space;
  string state_name;
  state_txt.open(filename.c_str());
  if(!state_txt){
    perror("Cannot open the state txt file\n");
    exit(EXIT_FAILURE);
  }

  else{
    while(getline(state_txt, temp)){
      space = temp.find(" ");
      state_name = temp.substr(space+1);
      add_state(C, state_name);
    }
  }
  state_txt.close();
}

void color_insert(connection *C){ 
  string temp;
  ifstream color_txt;
  string filename = "color.txt";
  size_t space;
  string color_name;
  color_txt.open(filename.c_str());
  if(!color_txt){
    perror("Cannot open the color txt file\n");
    exit(EXIT_FAILURE);
  }

  else{
    while(getline(color_txt, temp)){
      space = temp.find(" ");
      color_name = temp.substr(space+1);
      add_color(C, color_name);
    }
  }

  color_txt.close();
}

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files
  //if the table is exist, drop the table
  drop_tables(C);
  // create the player table
  create_table("initialize_table.sql", C);
  color_insert(C);
  state_insert(C);
  team_insert(C);
  player_insert(C);
  exercise(C);

  //add data in each text to the table

  //Close database connection
  C->disconnect();

  return 0;
}


