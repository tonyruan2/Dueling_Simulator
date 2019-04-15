#pragma once

class Duel {

	struct Player {
		int attack_level;
		int strength_level;
		int hitpoints_level;
		int current_hitpoints;
		int defence_level;
		
		bool using_weapon;
		double attack_speed;
		Player();
		void setAttributes(int attack_level, int strength_level,
			int hitpoints_level, int defence_level,
			bool using_weapon, double attack_speed);
	};
};