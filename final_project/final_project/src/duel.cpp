#include "duel.h"
#include <iostream>

Duel::Player::Player(int attack_level, int strength_level,
	int hitpoints_level, int defence_level,
	std::string attack_style, std::string weapon,
	bool alternating_styles) {
	
	this->attack_level = attack_level;
	this->strength_level = strength_level;
	this->hitpoints_level = hitpoints_level;
	this->defence_level = defence_level;
	this->attack_style = attack_style;
	this->weapon = weapon;
	this->alternating_styles = alternating_styles;
}

void Duel::runSimulation(Player &player_one, Player &player_two) {
	setCurrentHitpoints(player_one, player_two);
	parseWeaponData(player_one, 1);
	parseWeaponData(player_two, 2);
}

void Duel::setCurrentHitpoints(Player &player_one, Player &player_two) {
	player_one_current_hitpoints = player_one.hitpoints_level;
	player_two_current_hitpoints = player_two.hitpoints_level;
}

void Duel::parseWeaponData(Player &player, int player_id) {
	std::string url = "https://www.osrsbox.com/osrsbox-db/items-json/"
		+ std::to_string(player.weapon_id) + ".json";

	bool parsingSuccessful = weapon_json_result.open(url);

	if (parsingSuccessful)
	{
		if (player_id == 1) {
			player_one_weapon_bonuses.insert(
				std::pair<std::string, int> (
					"stab", std::stoi(weapon_json_result["equipment"]["attack_stab"].asString())));
			player_one_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"slash", std::stoi(weapon_json_result["equipment"]["attack_slash"].asString())));
			player_one_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"crush", std::stoi(weapon_json_result["equipment"]["attack_crush"].asString())));
			player_one_weapon_strength = std::stoi(weapon_json_result["equipment"]["melee_strength"].asString());
			player_one_attack_speed = std::stoi(weapon_json_result["weapon"]["attack_speed"].asString());
		}

		if (player_id == 2) {
			player_two_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"stab", std::stoi(weapon_json_result["equipment"]["attack_stab"].asString())));
			player_two_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"slash", std::stoi(weapon_json_result["equipment"]["attack_slash"].asString())));
			player_two_weapon_bonuses.insert(
				std::pair<std::string, int>(
					"crush", std::stoi(weapon_json_result["equipment"]["attack_crush"].asString())));
			player_two_weapon_strength = std::stoi(weapon_json_result["equipment"]["melee_strength"].asString());
			player_two_attack_speed = std::stoi(weapon_json_result["weapon"]["attack_speed"].asString());
		}
	}
}

void Duel::runAnalysis(int num_runs) {
	std::cout << "run analysis" << std::endl;
}