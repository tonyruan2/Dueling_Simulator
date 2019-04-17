#pragma once
#include <string>
#include <vector>

class Duel {

public:
	void runAnalysis(int num_runs);

private:

	std::vector<std::vector<int>> style_level_bonuses = { {3, 0, 0}, {0, 3, 0}, {1, 1, 1}, {0, 0, 3} };

	struct Player {
		int attack_level;
		int strength_level;
		int hitpoints_level;
		int defence_level;

		int current_hitpoints;
		std::string weapon;
		int weapon_strength;
		double attack_speed;

		std::string attack_style;

		int max_hit;
		int accuracy;

		Player(int attack_level, int strength_level,
			int hitpoints_level, int defence_level, std::string weapon);
	};

	int computeMaxHit(int strength_level, std::string weapon);
	int computeAccuracy(int player_attack_level, std::string attack_style, std::string weapon, int player_defence_level);

};