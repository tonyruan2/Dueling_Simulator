#pragma once
#include <string>
#include <vector>
#include <map>
#include "ofxJSON.h"

//class that represents a duel between two players
class Duel {

public:
	struct Player {
		int attack_level;
		int strength_level;
		int hitpoints_level;
		int defence_level;
		std::string attack_style;
		std::string weapon;
		int weapon_id;
		bool alternating_styles;

		Player(int attack_level, int strength_level,
			int hitpoints_level, int defence_level,
			std::string attack_style, std::string weapon,
			bool alternating_styles);
	};

	void runSimulation(Player &player_one, Player &player_two);
	void runAnalysis(int num_runs);

private:
	//Used to parse json.
	ofxJSONElement weapon_json_result;

	int player_one_current_hitpoints;
	std::map<std::string, int> player_one_weapon_bonuses;
	int player_one_weapon_strength;
	double player_one_attack_speed;

	int player_two_current_hitpoints;
	std::map<std::string, int> player_two_weapon_bonuses;
	int player_two_weapon_strength;
	double player_two_attack_speed;

	void setCurrentHitpoints(Player &player_one, Player &player_two);
	void parseWeaponData(Player &player, int player_id);

	std::vector<std::vector<int>> style_level_bonuses = { {3, 0, 0}, {0, 3, 0}, {1, 1, 1}, {0, 0, 3} };

	int computeMaxHit(int strength_level, std::string weapon);
	int computeAccuracy(int player_attack_level, std::string attack_style, std::string weapon, int player_defence_level);

};