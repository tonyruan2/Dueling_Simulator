#pragma once
#include <string>
#include <vector>

class Duel {

public:
	void runAnalysis(int num_runs);

private:

	struct Player {
		int attack_level;
		int strength_level;
		int hitpoints_level;
		int defence_level;
		
		int current_hitpoints;
		std::string weapon;
		int weapon_strength;
		std::string attack_style;
		std::vector<int> style_level_bonuses = std::vector<int>(3);

		double attack_speed;

		Player();
		void setAttributes(int attack_level, int strength_level,
			int hitpoints_level, int defence_level, std::string weapon);
	};

	Player player_one;
	Player player_two;

	int computeMaxHit(int strength_level, std::string weapon);
	int computeAccuracy(int player_attack_level, std::string weapon, int player_defence_level);

};