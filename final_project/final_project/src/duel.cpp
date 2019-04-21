#include "duel.h"
#include <iostream>
#include <iterator>
#include <cmath>

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
		player_one_attack_speed = 4;
	}
	else if (player_id == 2) {
		player_two_weapon_bonuses.insert(std::pair<std::string, int>("stab", 0));
		player_two_weapon_bonuses.insert(std::pair<std::string, int>("slash", 0));
		player_two_weapon_bonuses.insert(std::pair<std::string, int>("crush", 0));
		player_two_weapon_strength = 0;
		player_two_attack_speed = 4;
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
			player_one_attack_speed = std::stoi(weapon_json_result
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
			player_two_attack_speed = std::stoi(weapon_json_result
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
		return 0;
	}

	double effective_strength = player.strength_level 
		+ stance_bonuses.at(stance).at(1) + 8;
	double max_hit = ((effective_strength * (weapon_strength + 64)) / 640) + 0.5;
	return floor(max_hit);
}

std::string Duel::findAttackStyle(Player player) {
	for (int style_i = 0; style_i < attack_styles.size(); style_i++) {
		if (player.selected_weapon_style.find(attack_styles.at(style_i))
			!= std::string::npos) {
			return attack_styles.at(style_i);
		}
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
		return 0;
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

std::string Duel::computeMaxDamagePerSecStyle(Player player) {
	return "TEST";
}

//move run duel and run analysis to top afterwards
void Duel::runDuel(Player &player_one, Player &player_two) {
	std::cout << "Max hit of player 1: " << computeMaxHit(player_one) << std::endl;
	std::cout << "Player 1 has an accuracy of: " << computeAccuracy(player_one, player_two) << std::endl;
	std::cout << "Max hit of player 2: " << computeMaxHit(player_two) << std::endl;
	std::cout << "Player 2 has an accuracy of: " << computeAccuracy(player_two, player_one) << std::endl;
}

void Duel::runAnalysis(int num_runs) {
	std::cout << "run analysis" << std::endl;
}

void Duel::runSimulation(Player &player_one, Player &player_two,
	bool should_analyze, int num_runs) {

	setCurrentData(player_one, player_two);
	parseWeaponData(player_one);
	parseWeaponData(player_two);
	runDuel(player_one, player_two);

	if (should_analyze) {
		runAnalysis(num_runs);
	}
}