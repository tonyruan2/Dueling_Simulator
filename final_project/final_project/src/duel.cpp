#include "duel.h"
#include <iostream>
#include <iterator>
#include <cmath>
#include <stdlib.h>
#include <math.h>

void Duel::Player::setStats(int player_id, int attack_level,
	int strength_level, int defence_level, int hitpoints_level) {
	this->player_id = player_id;
	this->attack_level = attack_level;
	this->strength_level = strength_level;
	this->hitpoints_level = hitpoints_level;
	this->defence_level = defence_level;
}

void Duel::Player::setCombatSpecs(std::string selected_weapon_style, std::string weapon,
	std::string weapon_id, std::vector<std::string> weapon_styles, 
	bool alternating_styles) {
	this->selected_weapon_style = selected_weapon_style;
	this->weapon = weapon;
	this->weapon_id = weapon_id;
	this->weapon_styles = weapon_styles;
	this->alternating_styles = alternating_styles;
}

void Duel::setCurrentData(Player player_one, Player player_two) {
	player_one_current_hitpoints = player_one.hitpoints_level;
	player_one_current_style = player_one.selected_weapon_style;

	player_two_current_hitpoints = player_two.hitpoints_level;
	player_two_current_style = player_two.selected_weapon_style;
}

void Duel::addUnarmedData(int player_id) {
	if (player_id == 1) {
		player_one_weapon_bonuses.insert(std::pair<std::string, int>("stab", 0));
		player_one_weapon_bonuses.insert(std::pair<std::string, int>("slash", 0));
		player_one_weapon_bonuses.insert(std::pair<std::string, int>("crush", 0));
		player_one_weapon_strength = 0;
		player_one_attack_speed = 2.4;
	}
	else if (player_id == 2) {
		player_two_weapon_bonuses.insert(std::pair<std::string, int>("stab", 0));
		player_two_weapon_bonuses.insert(std::pair<std::string, int>("slash", 0));
		player_two_weapon_bonuses.insert(std::pair<std::string, int>("crush", 0));
		player_two_weapon_strength = 0;
		player_two_attack_speed = 2.4;
	}
}

void Duel::parseWeaponData(Player player) {
	if (player.weapon_id == "-1") {
		if (player.player_id == 1) {
			addUnarmedData(1);
		}
		else if (player.player_id == 2){
			addUnarmedData(2);
		}
		return;
	}

	std::string url = "https://www.osrsbox.com/osrsbox-db/items-json/"
		+ player.weapon_id + ".json";

	bool parsingSuccessful = weapon_json_result.open(url);

	if (parsingSuccessful)
	{
		if (player.player_id == 1) {
			player_one_weapon_bonuses.insert(
				std::pair<std::string, int> (
					"stab", std::stoi(weapon_json_result
						["equipment"]["attack_stab"].asString())));
			player_one_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"slash", std::stoi(weapon_json_result
						["equipment"]["attack_slash"].asString())));
			player_one_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"crush", std::stoi(weapon_json_result
						["equipment"]["attack_crush"].asString())));
			player_one_weapon_strength = std::stoi(weapon_json_result
				["equipment"]["melee_strength"].asString());
			player_one_attack_speed = game_tick * std::stoi(weapon_json_result
				["weapon"]["attack_speed"].asString());
		}

		if (player.player_id == 2) {
			player_two_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"stab", std::stoi(weapon_json_result
						["equipment"]["attack_stab"].asString())));
			player_two_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"slash", std::stoi(weapon_json_result
						["equipment"]["attack_slash"].asString())));
			player_two_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"crush", std::stoi(weapon_json_result
						["equipment"]["attack_crush"].asString())));
			player_two_weapon_strength = std::stoi(weapon_json_result
				["equipment"]["melee_strength"].asString());
			player_two_attack_speed = game_tick * std::stoi(weapon_json_result
				["weapon"]["attack_speed"].asString());
		}
	}
}

std::string Duel::findStance(Player player) {
	std::string stance;

	//Learned how to iterate over a map from the website below.
	//https://thispointer.com/how-to-iterate-over-a-map-in-c/
	for (std::pair<std::string, std::vector<int>> element : stance_bonuses) {
		if (player.player_id == 1) {
			if (player_one_current_style.find(element.first) != std::string::npos) {
				stance = element.first;
				break;
			}
		}
		if (player.player_id == 2) {
			if (player_two_current_style.find(element.first) != std::string::npos) {
				stance = element.first;
				break;
			}
		}
	}
	return stance;
}

int Duel::computeMaxHit(Player player) {
	int weapon_strength = 0;
	std::string stance = findStance(player);
	if (player.player_id == 1) {
		weapon_strength = player_one_weapon_strength;
	}
	else if (player.player_id == 2) {
		weapon_strength = player_two_weapon_strength;
	}
	else {
		return -1;
	}

	double effective_strength = player.strength_level 
		+ stance_bonuses.at(stance).at(1) + 8;
	double max_hit = ((effective_strength * (weapon_strength + 64)) / 640) + 0.5;
	return floor(max_hit);
}

std::string Duel::findAttackStyle(Player player) {
	if (player.player_id == 1) {
		for (int style_i = 0; style_i < attack_styles.size(); style_i++) {
			if (player_one_current_style.find(attack_styles.at(style_i))
				!= std::string::npos) {
				return attack_styles.at(style_i);
			}
		}
	}
	else if (player.player_id == 2) {
		for (int style_i = 0; style_i < attack_styles.size(); style_i++) {
			if (player_two_current_style.find(attack_styles.at(style_i))
				!= std::string::npos) {
				return attack_styles.at(style_i);
			}
		}
	}
	else {
		return "stab"; //default to "stab"
	}
}

int Duel::computeMaxAttackRoll(Player player) {
	std::string attacker_stance = findStance(player);
	//Stance bonus for attack located at index 0.
	double effective_attack_level = player.attack_level
		+ stance_bonuses.at(attacker_stance).at(0) + 8;

	double weapon_style_bonus = 0;

	std::string attacker_attack_style = findAttackStyle(player);
	if (player.player_id == 1) {
		weapon_style_bonus = (double) player_one_weapon_bonuses.at(
			attacker_attack_style);
	}
	else if (player.player_id == 2) {
		weapon_style_bonus = (double) player_two_weapon_bonuses.at(
			attacker_attack_style);
	}
	else {
		return -1;
	}

	double max_attack_roll = effective_attack_level * (weapon_style_bonus + 64);
	return floor(max_attack_roll);
}

int Duel::computeMaxDefenceRoll(Player player) {
	std::string defender_stance = findStance(player);
	//Stance bonus for defence located at index 2.
	double effective_defence_level = player.defence_level
		+ stance_bonuses.at(defender_stance).at(2) + 8;
	double max_defence_roll = effective_defence_level * (64);
	return floor(max_defence_roll);
}

double Duel::computeAccuracy(Player attacker, Player defender) {
	double max_attack_roll = computeMaxAttackRoll(attacker);
	double max_defence_roll = computeMaxDefenceRoll(defender);

	double accuracy = 0;
	if (max_attack_roll > max_defence_roll) {
		accuracy = 1 - (max_defence_roll + 2) / (2 * (max_attack_roll + 1));
	}
	else {
		accuracy = max_attack_roll / (2 * max_defence_roll + 1);
	}
	return accuracy;
}

double Duel::computeDamagePerSecond(double accuracy, int max_hit, double attack_speed) {
	double max_hit_double = (double)max_hit;
	return (accuracy *
		((max_hit_double * (max_hit_double + 1) / 2) / (max_hit_double + 1)) //average value
		/ attack_speed); //attack interval
}

std::string Duel::findStyleWithMaxDamagePerSec(Player attacker, Player defender) {
	std::string start_style;
	if (attacker.player_id == 1) {
		start_style = player_one_current_style;
	}
	else if (attacker.player_id == 2) {
		start_style = player_two_current_style;
	}

	std::string max_dps_style = start_style;
	double max_dps = 0;

	for (std::string weapon_style : attacker.weapon_styles) {
		if (attacker.player_id == 1) {
			player_one_current_style = weapon_style;
			double calculated_dps = computeDamagePerSecond(computeAccuracy(attacker, defender),
				computeMaxHit(attacker), player_one_attack_speed);
			if (calculated_dps > max_dps) {
				max_dps = calculated_dps;
				max_dps_style = player_one_current_style;
			}
		}
		else if (attacker.player_id == 2) {
			player_two_current_style = weapon_style;
			double calculated_dps = computeDamagePerSecond(computeAccuracy(attacker, defender),
				computeMaxHit(attacker), player_one_attack_speed);
			if (calculated_dps > max_dps) {
				max_dps = calculated_dps;
				max_dps_style = player_two_current_style;
			}
		}
	}

	if (attacker.player_id == 1) {
		player_one_current_style = start_style;
	}
	else if (attacker.player_id == 2) {
		player_two_current_style = start_style;
	}
	return max_dps_style;
}

std::string Duel::findStyleWithMaxDefence(Player defender) {
	std::string start_style;

	if (defender.player_id == 1) {
		start_style = player_one_current_style;

	}
	else if (defender.player_id == 2) {
		start_style = player_two_current_style;
	}

	std::string max_def_style = start_style;
	double max_def_roll = 0;

	for (std::string weapon_style : defender.weapon_styles) {
		if (defender.player_id == 1) {
			player_one_current_style = weapon_style;
			int calculated_def_roll = computeMaxDefenceRoll(defender);
			if (calculated_def_roll > max_def_roll) {
				max_def_roll = calculated_def_roll;
				max_def_style = player_one_current_style;
			}
		}
		else if (defender.player_id == 2) {
			player_two_current_style = weapon_style;
			int calculated_def_roll = computeMaxDefenceRoll(defender);
			if (calculated_def_roll > max_def_roll) {
				max_def_roll = calculated_def_roll;
				max_def_style = player_two_current_style;
			}
		}
	}

	if (defender.player_id == 1) {
		player_one_current_style = start_style;
	}
	else if (defender.player_id == 2) {
		player_two_current_style = start_style;
	}

	return max_def_style;
}

void Duel::inflictDamage(Player attacker, Player defender) {
	if (attacker.player_id == 1) {
		std::cout << "[Attacker] Player 1's weapon style: " << player_one_current_style << std::endl;
		std::cout << "[Defender] Player 2's weapon style: " << player_two_current_style << std::endl;
	}
	else {
		std::cout << "[Attacker] Player 2's weapon style: " << player_two_current_style << std::endl;
		std::cout << "[Defender] Player 1's weapon style: " << player_one_current_style << std::endl;
	}

	double attacker_accuracy = computeAccuracy(attacker, defender);
	double roll = (double) (rand() % 10001) / 10000;
	if (roll <= attacker_accuracy) {
		int damage_dealt = rand() % computeMaxHit(attacker) + 1;
		if (defender.player_id == 1) {
			std::cout << "Player 2 dealt " << damage_dealt << " damage" << std::endl << std::endl;
			player_one_current_hitpoints -= damage_dealt;
		}
		else if (defender.player_id == 2) {
			std::cout << "Player 1 dealt " << damage_dealt << " damage" << std::endl << std::endl;
			player_two_current_hitpoints -= damage_dealt;
		}
		else {
			return;
		}
	}
	else {
		std::cout << "Player " << attacker.player_id << " dealt 0 damage" << std::endl << std::endl;
	}
}

//move run duel and run analysis to top afterwards
int Duel::runDuel(Player player_one, Player player_two) {
	//determines who gets the first hit and who has priority when
	//players attack at the same time
	bool player_one_has_priority = rand() % 2;
	double player_one_time = 0;
	double player_two_time = 0;

	std::string player_one_max_dps_style 
		= findStyleWithMaxDamagePerSec(player_one, player_two);
	std::string player_one_max_def_style
		= findStyleWithMaxDefence(player_one);

	std::string player_two_max_dps_style
		= findStyleWithMaxDamagePerSec(player_two, player_one);
	std::string player_two_max_def_style
		= findStyleWithMaxDefence(player_two);

	while (player_one_current_hitpoints > 0
		&& player_two_current_hitpoints > 0) {
		if (player_one_has_priority) {
			if (fmod(player_one_time, player_one_attack_speed) <= 0.001) {
				if (player_one.alternating_styles) {
					player_one_current_style 
						= player_one_max_dps_style;
					inflictDamage(player_one, player_two);
					player_one_current_style = player_one_max_def_style;
				}
				else {
					inflictDamage(player_one, player_two);
				}
			}
			if (player_two_current_hitpoints <= 0) {
				std::cout << "[The winner is: 1]" << std::endl << std::endl;
				return 1;
			}
			if (fmod(player_two_time, player_two_attack_speed) <= 0.001) {
				if (player_two.alternating_styles) {
					player_two_current_style
						= player_two_max_dps_style;
					inflictDamage(player_two, player_one);
					player_two_current_style = player_two_max_def_style;
				}
				else {
					inflictDamage(player_two, player_one);
				}
			}
		}
		else {
			if (fmod(player_two_time, player_two_attack_speed) <= 0.001) {
				if (player_two.alternating_styles) {
					player_two_current_style
						= player_two_max_dps_style;
					inflictDamage(player_two, player_one);
					player_two_current_style = player_two_max_def_style;
				}
				else {
					inflictDamage(player_two, player_one);
				}
			}
			if (player_one_current_hitpoints <= 0) {
				std::cout << "[The winner is: 2]" << std::endl << std::endl;
				return 2;
			}
			if (fmod(player_one_time, player_one_attack_speed) <= 0.001) {
				if (player_one.alternating_styles) {
					player_one_current_style
						= player_one_max_dps_style;
					inflictDamage(player_one, player_two);
					player_one_current_style = player_one_max_def_style;
				}
				else {
					inflictDamage(player_one, player_two);
				}
			}
		}
		player_one_time += game_tick;
		player_two_time += game_tick;
	}

	if (player_one_current_hitpoints <= 0) {
		std::cout << "[The winner is: 2]" << std::endl << std::endl;
		return 2;
	}

	if (player_two_current_hitpoints <= 0) {
		std::cout << "[The winner is: 1]" << std::endl << std::endl;
		return 1;
	}
	return -1;
}

void Duel::runAnalysis(Player player_one, Player player_two, int num_runs) {
	double num_player_one_wins = 0;
	for (int run_i = 0; run_i < num_runs; run_i++) {
		int winner = runDuel(player_one, player_two);
		if (winner == 1) {
			num_player_one_wins++;
		}
		setCurrentData(player_one, player_two);
	}

	double player_one_win_rate = num_player_one_wins / num_runs * 100;
	double player_two_win_rate = 100 - player_one_win_rate;

	std::cout << "Player one has a winrate of " << player_one_win_rate << "% over " << num_runs << " games." << std::endl;
	std::cout << "Player two has a winrate of " << 100 - player_one_win_rate << "% over " << num_runs << " games." << std::endl;
}

void Duel::runSimulation(Player &player_one, Player &player_two,
	bool should_analyze, int num_runs) {

	setCurrentData(player_one, player_two);
	parseWeaponData(player_one);
	parseWeaponData(player_two);
	runDuel(player_one, player_two);

	if (should_analyze) {
		runAnalysis(player_one, player_two, num_runs);
	}
}