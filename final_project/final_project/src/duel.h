#pragma once
#include <string>
#include <vector>
#include <map>
#include "ofxJSON.h"

//class that represents a duel between two players
class Duel {

public:
	//class that represents a Player
	struct Player {
		int player_id;
		int attack_level;
		int strength_level;
		int hitpoints_level;
		int defence_level;
		std::string attack_style;
		std::string weapon;
		std::vector<std::string> weapon_styles;
		std::string weapon_id;
		bool alternating_styles;

		//Set the stats of the player.
		void setStats(int player_id, int attack_level, int strength_level,
			int defence_level, int hitpoints_level);

		//Set the combat specifications of the player.
		void setCombatSpecs(std::string attack_style, std::string weapon,
			std::string weapon_id, std::vector<std::string> weapon_styles,
			bool alternating_styles);
	};

	//Run a simulation of two players dueling. Include an analysis accordingly.
	void runSimulation(Player &player_one, Player &player_two, 
		bool should_analyze, int num_runs);

private:
	//Used to parse json.
	ofxJSONElement weapon_json_result;

	//Bonuses (Attack, Strength, Defence) granted by each stance.
	std::map<std::string, std::vector<int>> stance_bonuses =
	{
		std::pair<std::string, std::vector<int>>("accurate", std::vector<int> {3, 0, 0}),
		std::pair<std::string, std::vector<int>>("aggressive", std::vector<int> {0, 3, 0}),
		std::pair<std::string, std::vector<int>>("controlled", std::vector<int> {1, 1, 1}),
		std::pair<std::string, std::vector<int>>("defensive", std::vector<int> {0, 0, 3}),
	};

	int player_one_current_hitpoints;
	std::string player_one_current_style;
	std::map<std::string, int> player_one_weapon_bonuses;
	int player_one_weapon_strength;
	double player_one_attack_speed;

	int player_two_current_hitpoints;
	std::string player_two_current_style;
	std::map<std::string, int> player_two_weapon_bonuses;
	int player_two_weapon_strength;
	double player_two_attack_speed;

	//Set the current hitpoints and current attack style of each player.
	void setCurrentData(Player player_one, Player player_two);

	//Parse weapon data for each player's weapon.
	void parseWeaponData(Player player);
	
	//Add data to represent the players are "unarmed".
	void addUnarmedData(int player_id);

	//Compute the max hit of a player.
	int computeMaxHit(Player player);

	//Compute the accuracy of an attacking player against a defending player.
	int computeAccuracy(Player attacker, Player defender);

	//Return the style which provides the maximum damage per second for a player.
	std::string computeMaxDamagePerSecStyle(Player player);

	//Run a duel between the players.
	void runDuel(Player &player_one, Player &player_two);

	//Run a long-run analysis of two players dueling.
	void runAnalysis(int num_runs);
};