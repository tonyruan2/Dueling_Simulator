#pragma once
#include "ofxDatGui.h"

//Class that represents a visual simulation of a duel between two players.
class Simulation
{
public:
	//Setup the simulation interface.
	void setup();

	//Save a simulation to be displayed.
	void saveSimulation(int player_one_hitpoints_level, 
		int player_two_hitpoints_level, 
		std::vector<int> player_one_simulation_actions,
		std::vector<int> player_two_simulation_actions);

	//Save player one's data in the graphical interface.
	void savePlayerOneData(double max_hit, double damage_per_second, 
		double win_rate, bool is_winner);

	//Save player two's data in the graphical interface.
	void savePlayerTwoData(double max_hit, double damage_per_second, 
		double win_rate, bool is_winner);

	//Draw the simulation.
	void draw();

	//Change the player data visibility.
	void setPlayerDataVisibility(bool visibility);

private:
	//Each game tick is 600 milliseconds.
	int game_tick_value = 600;

	//Current game_tick in the simulation.
	int current_game_tick = 0;

	//Previous game tick image idenfitier. Used in draw() to hold a stable simulation
	//in between animations.
	std::string previous_image_identifier;

	//Previous damages inflicted by each player. Used in draw() to hold a stable
	//simulation in between animations.
	int previous_player_one_damage = 0;
	int previous_player_two_damage = 0;

	//Simulation image identifiers.
	std::string player_one_attack_identifier = "player_one_attack";
	std::string player_two_attack_identifier = "player_two_attack";
	std::string both_players_attack_identifier = "both_players_attack";

	//The max hitpoints of any player.
	int max_hitpoints = 99;

	//Used to display the hitpoints bar of player one in the simulation.
	ofxDatGui* player_one_hitpoints_bar = new ofxDatGui(
		ofGetWidth() / 3.4, ofGetHeight() / 1.5 + 10);

	//Used to display the hitpoints bar of player two in the simulation.
	ofxDatGui* player_two_hitpoints_bar = new ofxDatGui(
		ofGetWidth() / 3.4 + (ofGetWidth() / 3), ofGetHeight() / 1.5 + 10);

	//Used to display the simulation data of player one.
	ofxDatGui* player_one_data = new ofxDatGui(
		ofGetWidth() / 3.4, ofGetHeight() / 1.5 + 50);

	//Used to display the simulation data of player two.
	ofxDatGui* player_two_data = new ofxDatGui(
		ofGetWidth() / 3.4 + ofGetWidth() / 3, ofGetHeight() / 1.5 + 50);

	//Vectors are also saved in duel class in order to avoid multiple calls to
	//saveSimulation() for every return statement in runDuelSimulation()
	//Vectors are saved here to loop through the simulation.

	//Represents player one's attacks to player two during the simulation.
	//The difference between each index is a game tick.
	std::vector<int> player_one_simulation_actions;

	//Represents player two's attacks to player one during the simulation.
	//The difference between each index is a game tick.
	std::vector<int> player_two_simulation_actions;

	//Variable that determines whether to display the simulation.
	bool duel_has_started = false;

	//Pictures used in the graphical simulation.
	ofImage player_one_start_picture;
	ofImage player_one_attack_picture;
	ofImage player_one_defend_picture;
	ofImage player_two_start_picture;
	ofImage player_two_attack_picture;
	ofImage player_two_defend_picture;
	ofImage blue_hit_splat;
	ofImage red_hit_splat;

	//Load in the pictures used.
	void setupPictures();
	
	//Setup the display of hitpoints bars.
	void setupHitpointsBars();
	
	//Setup the display of player data.
	void setupPlayerDataDisplay();
	
	//Draw the starting pictures in the simulation.
	void setStartingPictures();
	
	//Draw the pictures that represent player one attacking.
	void setPlayerOneAttack(int damage);
	
	//Draw the pictures that represent player two attacking.
	void setPlayerTwoAttack(int damage);
	
	//Draw the pictures that represent both players attacking.
	void setBothPlayersAttack(int player_one_damage, 
		int player_two_damage);

	//Draw player hit splats.
	void setPlayerDamage(int attacker_id, int damage);
	
	//Draw the previous pictures.
	void drawPrevious();
};

