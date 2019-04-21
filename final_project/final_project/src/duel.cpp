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

void Duel::Player::setCombatSpecs(std::string attack_style, std::string weapon,
	std::string weapon_id, std::vector<std::string> weapon_styles, 
	bool alternating_styles) {
	this->attack_style = attack_style;
	this->weapon = weapon;
	this->weapon_id = weapon_id;
	this->weapon_styles = weapon_styles;
	this->alternating_styles = alternating_styles;
}

void Duel::setCurrentData(Player player_one, Player player_two) {
	player_one_current_hitpoints = player_one.hitpoints_level;
	player_one_current_style = player_one.attack_style;

	player_two_current_hitpoints = player_two.hitpoints_level;
	player_two_current_style = player_two.attack_style;
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

int Duel::computeMaxHit(Player player) {
	int weapon_strength = 0;
	std::string stance;
	//Learned how to iterate over a map from the website below.
	//https://thispointer.com/how-to-iterate-over-a-map-in-c/
	std::map<std::string, std::vector<int>>::iterator it = stance_bonuses.begin();
	for (std::pair<std::string, std::vector<int>> element : stance_bonuses) {
		if (player.player_id == 1) {
			if (player_one_current_style.find(element.first) != std::string::npos) {
				stance = element.first;
				weapon_strength = player_one_weapon_strength;
				break;
			}
		}
		if (player.player_id == 2) {
			if (player_two_current_style.find(element.first) != std::string::npos) {
				stance = element.first;
				weapon_strength = player_two_weapon_strength;
				break;
			}
		}
	}

	double effective_strength = player.strength_level + stance_bonuses.at(stance).at(1) + 8;
	double max_hit = ((effective_strength * (weapon_strength + 64)) / 640) + 0.5;
	std::cout << "Player " << player.player_id << "'s max hit is " << floor(max_hit) << std::endl;
	return floor(max_hit);
}

int Duel::computeAccuracy(Player attacker, Player defender) {
	return 0;
}

std::string Duel::computeMaxDamagePerSecStyle(Player player) {
	return "TEST";
}

//move run duel and run analysis to top afterwards
void Duel::runDuel(Player &player_one, Player &player_two) {
	std::cout << computeMaxHit(player_one) << std::endl;
	std::cout << computeMaxHit(player_two) << std::endl;
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