#pragma once
#include <string>
#include <vector>
#include <map>
#include "ofxJSON.h"
#include "simulation.h"

//Class that models duels between two players.
class Duel {

public:
	//Class that represents a player.
	struct Player {
		int player_id;
		int attack_level;
		int strength_level;
		int hitpoints_level;
		int defence_level;
		std::string selected_weapon_style;
		std::string weapon;
		std::vector<std::string> weapon_styles;
		std::string weapon_id;
		bool alternating_styles;

		//Set the stats of the player.
		void setStats(int player_id, int attack_level, int strength_level,
			int defence_level, int hitpoints_level);

		//Set the combat specifications of the player.
		void setCombatSpecs(std::string selected_weapon_style, std::string weapon,
			std::string weapon_id, std::vector<std::string> weapon_styles,
			bool alternating_styles);
	};

	//Used for graphical simulation of duel.
	Simulation simulation_;

	//Run a simulation of two players dueling. Include an analysis accordingly.
	void runSimulation(Player player_one, Player player_two, 
		bool should_analyze, int num_runs);

private:
	//Used to parse json.
	ofxJSONElement weapon_json_result_;

	//Bonuses (Attack, Strength, Defence) granted by each stance.
	std::map<std::string, std::vector<int>> stance_bonuses_ =
	{
		std::pair<std::string, std::vector<int>>("accurate", std::vector<int> {3, 0, 0}),
		std::pair<std::string, std::vector<int>>("aggressive", std::vector<int> {0, 3, 0}),
		std::pair<std::string, std::vector<int>>("controlled", std::vector<int> {1, 1, 1}),
		std::pair<std::string, std::vector<int>>("defensive", std::vector<int> {0, 0, 3}),
	};

	//Attack styles. Weapons tend to be superior in one or two and
	//lackluster in the others.
	std::vector<std::string> attack_styles_ = std::vector<std::string>{
		"stab",
		"slash",
		"crush"
	};

	//ID of the winner of the duel simulation.
	int winner_id_ = -1;

	//Each game tick is 600 milliseconds.
	double game_tick_ = 600;

	//Player attributes.
	int player_one_current_hitpoints_;
	std::string player_one_current_style_;
	std::map<std::string, int> player_one_weapon_bonuses_;
	int player_one_weapon_strength_;
	double player_one_attack_speed_;

	int player_two_current_hitpoints_;
	std::string player_two_current_style_;
	std::map<std::string, int> player_two_weapon_bonuses_;
	int player_two_weapon_strength_;
	double player_two_attack_speed_;

	//Int representing a simulation action of not attacking during a game tick.
	int no_attack_action_ = -1;

	//Represents player one's attacks to player two during the simulation.
	//The difference between each index is a game tick.
	std::vector<int> player_one_simulation_actions_;

	//Represents player two's attacks to player one during the simulation.
	//The difference between each index is a game tick.
	std::vector<int> player_two_simulation_actions_;

	//Set the current hitpoints and current attack style of each player.
	void setCurrentData(Player player_one, Player player_two);

	//Parse weapon data for each player's weapon.
	void parseWeaponData(Player player);
	
	//Add data to represent the players are "unarmed".
	void addUnarmedData(int player_id);

	//Find the stance of a player.
	std::string findStance(Player player);

	//Compute the max hit of a player.
	int computeMaxHit(Player player);

	//Find the attack style of a player.
	std::string findAttackStyle(Player player);

	//Compute the Max Attack Roll for a player.
	int computeMaxAttackRoll(Player player);

	//Compute the Max Defence Roll for a player.
	int computeMaxDefenceRoll(Player player);

	//Compute the accuracy of an attacking player against a defending player.
	double computeAccuracy(Player attacker, Player defender);

	//Compute the damage per second using accuracy and max hit.
	double computeDamagePerSecond(double accuracy, int max_hit, double attack_speed);

	//Return the style which provides the maximum damage per second for the attacker.
	std::string findStyleWithMaxDamagePerSec(Player attacker, Player defender);

	//Return the style which provides the most defence.
	std::string findStyleWithMaxDefence(Player defender);

	//Simulates combat between an attacking and a defending player.
	//Changes graphical elements.
	int inflictDamageForSimulation(Player attacker, Player defender);

	//Run a simulation of a duel between two players. 
	//Changes graphical elements.
	void runDuelSimulation(Player player_one, Player player_two);

	//Simulates combat between an attacking player and a defending player.
	//For long-run analysis (i.e. no graphics/printing).
	void inflictDamageForLongRun(Player attacker, Player defender);

	//Run a duel for between two players.
	//For long-run analysis (i.e. no graphics/printing).
	int runDuelForLongRun(Player player_one, Player player_two);

	//Run a long-run analysis of two players dueling.
	void runAnalysis(Player player_one, Player player_two, int num_runs);

	//Aggregates the data in the simulation so that it can be displayed.
	void aggregateSimulationData(Player player_one, Player player_two, 
		double player_one_win_rate, double player_two_win_rate);
};