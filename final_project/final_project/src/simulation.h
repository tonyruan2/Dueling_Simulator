#pragma once
#include "ofxDatGui.h"

class Simulation
{
public:
	void setup();
	void saveSimulation(std::vector<int> player_one_simulation_actions,
		std::vector<int> player_two_simulation_actions);
	void draw();

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

	//Vectors are also saved in duel class in order to avoid multiple calls to
	//saveSimulation() for every return statement in runDuelSimulation()
	//Vectors are saved here to loop through the simulation.

	//Represents player one's attacks to player two during the simulation.
	//The difference between each index is a game tick.
	std::vector<int> player_one_simulation_actions;

	//Represents player two's attacks to player one during the simulation.
	//The difference between each index is a game tick.
	std::vector<int> player_two_simulation_actions;

	bool duel_has_started = false;

	ofImage player_one_start_picture;
	ofImage player_one_attack_picture;
	ofImage player_one_defend_picture;
	ofImage player_two_start_picture;
	ofImage player_two_attack_picture;
	ofImage player_two_defend_picture;
	ofxDatGui* results;

	void setStartingPictures();
	void setPlayerOneAttack(int damage);
	void setPlayerTwoAttack(int damage);
	void setBothPlayersAttack(int player_one_damage, 
		int player_two_damage);
	void drawPrevious();
};

