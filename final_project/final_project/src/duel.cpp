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

void Duel::runSimulation(Player player_one, Player player_two,
	bool should_analyze, int num_runs) {

	parseWeaponData(player_one);
	parseWeaponData(player_two);
	runDuelSimulation(player_one, player_two);
	simulation_.saveSimulation(player_one.hitpoints_level, player_two.hitpoints_level,
		player_one_simulation_actions_, player_two_simulation_actions_);

	std::cout << "Player one action count: " << player_one_simulation_actions_.size() << std::endl;
	std::cout << "Player two action count: " << player_two_simulation_actions_.size() << std::endl;

	for (int i = 0; i < player_one_simulation_actions_.size(); i++) {
		std::cout << player_one_simulation_actions_.at(i);
	}
	std::cout << std::endl;
	for (int i = 0; i < player_two_simulation_actions_.size(); i++) {
		std::cout << player_two_simulation_actions_.at(i);
	}
	std::cout << std::endl;
	if (should_analyze) {
		runAnalysis(player_one, player_two, num_runs);
	}
}

void Duel::setCurrentData(Player player_one, Player player_two) {
	player_one_current_hitpoints_ = player_one.hitpoints_level;
	player_one_current_style_ = player_one.selected_weapon_style;

	player_two_current_hitpoints_ = player_two.hitpoints_level;
	player_two_current_style_ = player_two.selected_weapon_style;
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

	bool parsingSuccessful = weapon_json_result_.open(url);

	if (parsingSuccessful)
	{
		if (player.player_id == 1) {
			player_one_weapon_bonuses_.insert(
				std::pair<std::string, int> (
					"stab", std::stoi(weapon_json_result_
						["equipment"]["attack_stab"].asString())));
			player_one_weapon_bonuses_.insert(
				std::pair<std::string, int>(
					"slash", std::stoi(weapon_json_result_
						["equipment"]["attack_slash"].asString())));
			player_one_weapon_bonuses_.insert(
				std::pair<std::string, int>(
					"crush", std::stoi(weapon_json_result_
						["equipment"]["attack_crush"].asString())));
			player_one_weapon_strength_ = std::stoi(weapon_json_result_
				["equipment"]["melee_strength"].asString());
			player_one_attack_speed_ = game_tick_ * std::stoi(weapon_json_result_
				["weapon"]["attack_speed"].asString());
		}

		if (player.player_id == 2) {
			player_two_weapon_bonuses_.insert(
				std::pair<std::string, int>(
					"stab", std::stoi(weapon_json_result_
						["equipment"]["attack_stab"].asString())));
			player_two_weapon_bonuses_.insert(
				std::pair<std::string, int>(
					"slash", std::stoi(weapon_json_result_
						["equipment"]["attack_slash"].asString())));
			player_two_weapon_bonuses_.insert(
				std::pair<std::string, int>(
					"crush", std::stoi(weapon_json_result_
						["equipment"]["attack_crush"].asString())));
			player_two_weapon_strength_ = std::stoi(weapon_json_result_
				["equipment"]["melee_strength"].asString());
			player_two_attack_speed_ = game_tick_ * std::stoi(weapon_json_result_
				["weapon"]["attack_speed"].asString());
		}
	}
}

void Duel::addUnarmedData(int player_id) {
	if (player_id == 1) {
		player_one_weapon_bonuses_.insert(std::pair<std::string, int>("stab", 0));
		player_one_weapon_bonuses_.insert(std::pair<std::string, int>("slash", 0));
		player_one_weapon_bonuses_.insert(std::pair<std::string, int>("crush", 0));
		player_one_weapon_strength_ = 0;
		player_one_attack_speed_ = 4 * game_tick_;
	}
	else if (player_id == 2) {
		player_two_weapon_bonuses_.insert(std::pair<std::string, int>("stab", 0));
		player_two_weapon_bonuses_.insert(std::pair<std::string, int>("slash", 0));
		player_two_weapon_bonuses_.insert(std::pair<std::string, int>("crush", 0));
		player_two_weapon_strength_ = 0;
		player_two_attack_speed_ = 4 * game_tick_;
	}
}

std::string Duel::findStance(Player player) {
	std::string stance;

	//Learned how to iterate over a map from the website below.
	//https://thispointer.com/how-to-iterate-over-a-map-in-c/
	for (std::pair<std::string, std::vector<int>> element : stance_bonuses_) {
		if (player.player_id == 1) {
			if (player_one_current_style_.find(element.first) != std::string::npos) {
				stance = element.first;
				break;
			}
		}
		if (player.player_id == 2) {
			if (player_two_current_style_.find(element.first) != std::string::npos) {
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
		weapon_strength = player_one_weapon_strength_;
	}
	else if (player.player_id == 2) {
		weapon_strength = player_two_weapon_strength_;
	}
	else {
		return -1;
	}

	double effective_strength = player.strength_level 
		+ stance_bonuses_.at(stance).at(1) + 8;
	double max_hit = ((effective_strength * (weapon_strength + 64)) / 640) + 0.5;
	return floor(max_hit);
}

std::string Duel::findAttackStyle(Player player) {
	if (player.player_id == 1) {
		for (int style_i = 0; style_i < attack_styles_.size(); style_i++) {
			if (player_one_current_style_.find(attack_styles_.at(style_i))
				!= std::string::npos) {
				return attack_styles_.at(style_i);
			}
		}
	}
	else if (player.player_id == 2) {
		for (int style_i = 0; style_i < attack_styles_.size(); style_i++) {
			if (player_two_current_style_.find(attack_styles_.at(style_i))
				!= std::string::npos) {
				return attack_styles_.at(style_i);
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
		+ stance_bonuses_.at(attacker_stance).at(0) + 8;

	double weapon_style_bonus = 0;

	std::string attacker_attack_style = findAttackStyle(player);
	if (player.player_id == 1) {
		weapon_style_bonus = (double) player_one_weapon_bonuses_.at(
			attacker_attack_style);
	}
	else if (player.player_id == 2) {
		weapon_style_bonus = (double) player_two_weapon_bonuses_.at(
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
		+ stance_bonuses_.at(defender_stance).at(2) + 8;
	double max_defence_roll = effective_defence_level * (64);
	return floor(max_defence_roll);
}

double Duel::computeAccuracy(Player attacker, Player defender) {
	double max_attack_roll = computeMaxAttackRoll(attacker);
	double max_defence_roll = computeMaxDefenceRoll(defender);

	double accuracy = 0;
	//From https://www.osrsbox.com/blog/2019/01/22/calculating-melee-dps-in-osrs/#1-calculating-max-hit
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
	//From https://www.osrsbox.com/blog/2019/01/22/calculating-melee-dps-in-osrs/#1-calculating-max-hit
	//convert attack interval to seconds
	return (accuracy * (max_hit_double / 2) / (attack_speed / 1000.0));
}

std::string Duel::findStyleWithMaxDamagePerSec(Player attacker, Player defender) {
	std::string start_style;
	if (attacker.player_id == 1) {
		start_style = player_one_current_style_;
	}
	else if (attacker.player_id == 2) {
		start_style = player_two_current_style_;
	}

	std::string max_dps_style = start_style;
	double max_dps = 0;

	for (std::string weapon_style : attacker.weapon_styles) {
		if (attacker.player_id == 1) {
			player_one_current_style_ = weapon_style;
			double calculated_dps = computeDamagePerSecond(computeAccuracy(attacker, defender),
				computeMaxHit(attacker), player_one_attack_speed_);
			if (calculated_dps > max_dps) {
				max_dps = calculated_dps;
				max_dps_style = player_one_current_style_;
			}
		}
		else if (attacker.player_id == 2) {
			player_two_current_style_ = weapon_style;
			double calculated_dps = computeDamagePerSecond(computeAccuracy(attacker, defender),
				computeMaxHit(attacker), player_one_attack_speed_);
			if (calculated_dps > max_dps) {
				max_dps = calculated_dps;
				max_dps_style = player_two_current_style_;
			}
		}
	}

	if (attacker.player_id == 1) {
		player_one_current_style_ = start_style;
	}
	else if (attacker.player_id == 2) {
		player_two_current_style_ = start_style;
	}
	return max_dps_style;
}

std::string Duel::findStyleWithMaxDefence(Player defender) {
	std::string start_style;

	if (defender.player_id == 1) {
		start_style = player_one_current_style_;

	}
	else if (defender.player_id == 2) {
		start_style = player_two_current_style_;
	}

	std::string max_def_style = start_style;
	double max_def_roll = 0;

	for (std::string weapon_style : defender.weapon_styles) {
		if (defender.player_id == 1) {
			player_one_current_style_ = weapon_style;
			int calculated_def_roll = computeMaxDefenceRoll(defender);
			if (calculated_def_roll > max_def_roll) {
				max_def_roll = calculated_def_roll;
				max_def_style = player_one_current_style_;
			}
		}
		else if (defender.player_id == 2) {
			player_two_current_style_ = weapon_style;
			int calculated_def_roll = computeMaxDefenceRoll(defender);
			if (calculated_def_roll > max_def_roll) {
				max_def_roll = calculated_def_roll;
				max_def_style = player_two_current_style_;
			}
		}
	}

	if (defender.player_id == 1) {
		player_one_current_style_ = start_style;
	}
	else if (defender.player_id == 2) {
		player_two_current_style_ = start_style;
	}

	return max_def_style;
}

int Duel::inflictDamageForSimulation(Player attacker, Player defender) {
	if (attacker.player_id == 1) {
		std::cout << "[Attacker] Player 1's weapon style: " << player_one_current_style_ << std::endl;
		std::cout << "[Defender] Player 2's weapon style: " << player_two_current_style_ << std::endl;
	}
	else {
		std::cout << "[Attacker] Player 2's weapon style: " << player_two_current_style_ << std::endl;
		std::cout << "[Defender] Player 1's weapon style: " << player_one_current_style_ << std::endl;
	}

	double attacker_accuracy = computeAccuracy(attacker, defender);
	double roll = (double)(rand() % 10001) / 10000;
	if (roll <= attacker_accuracy) {
		int damage_dealt = rand() % (computeMaxHit(attacker) + 1);
		if (defender.player_id == 1) {
			std::cout << "Player 2 dealt " << damage_dealt << " damage" << std::endl << std::endl;
			player_one_current_hitpoints_ -= damage_dealt;
			return damage_dealt;
		}
		else if (defender.player_id == 2) {
			std::cout << "Player 1 dealt " << damage_dealt << " damage" << std::endl << std::endl;
			player_two_current_hitpoints_ -= damage_dealt;
			return damage_dealt;
		}
	}
	else {
		std::cout << "Player " << attacker.player_id << " dealt 0 damage" << std::endl << std::endl;
		return 0;
	}
}

void Duel::runDuelSimulation(Player player_one, Player player_two) {
	player_one_simulation_actions_.clear();
	player_two_simulation_actions_.clear();

	setCurrentData(player_one, player_two);
	//reset the winner
	winner_id_ = -1;
	//determines who gets the first hit and who has priority when
	//players attack at the same time
	bool player_one_has_priority = rand() % 2;

	//players attack when their attack speed divides their time
	//i.e. if a player has an attack speed of 4, when (4 * gameTick) milliseconds 
	//have passed, then the player will attack
	double player_one_time = 0;
	double player_two_time = 0;

	if (player_one_has_priority) {
		player_one_time = 2.0 * game_tick_;
	}
	else {
		player_two_time = 2.0 * game_tick_;
	}

	std::string player_one_max_dps_style
		= findStyleWithMaxDamagePerSec(player_one, player_two);
	std::string player_one_max_def_style
		= findStyleWithMaxDefence(player_one);

	std::string player_two_max_dps_style
		= findStyleWithMaxDamagePerSec(player_two, player_one);
	std::string player_two_max_def_style
		= findStyleWithMaxDefence(player_two);

	while (player_one_current_hitpoints_ > 0
		&& player_two_current_hitpoints_ > 0) {
		//players are attacking during the same game tick
		if (fmod(player_one_time, player_one_attack_speed_) <= 0.001
			&& fmod(player_two_time, player_two_attack_speed_) <= 0.001) {

			int player_one_damage = 0;
			int player_two_damage = 0;

			if (player_one.alternating_styles) {
				player_one_current_style_
					= player_one_max_dps_style;
			}
			if (player_two.alternating_styles) {
				player_two_current_style_
					= player_two_max_dps_style;
			}

			if (player_one_has_priority) {
				player_one_damage = inflictDamageForSimulation(
					player_one, player_two);
				if (player_two_current_hitpoints_ <= 0) {
					winner_id_ = 1;
				}
				player_two_damage = inflictDamageForSimulation(player_two, player_one);
			}
			else {
				player_two_damage = inflictDamageForSimulation(player_two, player_one);
				if (player_one_current_hitpoints_ <= 0) {
					winner_id_ = 2;
				}
				player_one_damage = inflictDamageForSimulation(
					player_one, player_two);
			}

			player_one_simulation_actions_.push_back(player_one_damage);
			player_two_simulation_actions_.push_back(player_two_damage);

			if (winner_id_ != -1) { //check for a winner after the player with priority dealt damage
				return;
			}
			//assign a winner taking into account the second player's hit
			if (player_one_current_hitpoints_ <= 0) {
				winner_id_ = 2;
				return;
			}
			else if (player_two_current_hitpoints_ <= 0) {
				winner_id_ = 1;
				return;
			}
		}
		else if (player_one_has_priority) {
			if (fmod(player_one_time, player_one_attack_speed_) <= 0.001) {
				if (player_one.alternating_styles) {
					player_one_current_style_
						= player_one_max_dps_style;
					player_one_simulation_actions_.push_back(
						inflictDamageForSimulation(player_one, player_two));
					player_one_current_style_ = player_one_max_def_style;
				}
				else {
					player_one_simulation_actions_.push_back(
						inflictDamageForSimulation(player_one, player_two));
				}
			}
			else {
				player_one_simulation_actions_.push_back(no_attack_action_);
			}

			if (player_two_current_hitpoints_ <= 0) {
				winner_id_ = 1;
				player_two_simulation_actions_.push_back(no_attack_action_);
				return;
			}

			if (fmod(player_two_time, player_two_attack_speed_) <= 0.001) {
				if (player_two.alternating_styles) {
					player_two_current_style_
						= player_two_max_dps_style;
					player_two_simulation_actions_.push_back(
						inflictDamageForSimulation(player_two, player_one));
					player_two_current_style_ = player_two_max_def_style;
				}
				else {
					player_two_simulation_actions_.push_back(
						inflictDamageForSimulation(player_two, player_one));
				}
			}
			else {
				player_two_simulation_actions_.push_back(no_attack_action_);
			}
		}
		//player two is checked first
		else {
			if (fmod(player_two_time, player_two_attack_speed_) <= 0.001) {
				if (player_two.alternating_styles) {
					player_two_current_style_
						= player_two_max_dps_style;
					player_two_simulation_actions_.push_back(
						inflictDamageForSimulation(player_two, player_one));
					player_two_current_style_ = player_two_max_def_style;
				}
				else {
					player_two_simulation_actions_.push_back(
						inflictDamageForSimulation(player_two, player_one));
				}
			}
			else {
				player_two_simulation_actions_.push_back(no_attack_action_);
			}

			if (player_one_current_hitpoints_ <= 0) {
				winner_id_ = 2;
				player_one_simulation_actions_.push_back(no_attack_action_);
				return;
			}

			if (fmod(player_one_time, player_one_attack_speed_) <= 0.001) {
				if (player_one.alternating_styles) {
					player_one_current_style_
						= player_one_max_dps_style;
					player_one_simulation_actions_.push_back(
						inflictDamageForSimulation(player_one, player_two));
					player_one_current_style_ = player_one_max_def_style;
				}
				else {
					player_one_simulation_actions_.push_back(
						inflictDamageForSimulation(player_one, player_two));
				}
			}
			else {
				player_one_simulation_actions_.push_back(no_attack_action_);
			}
		}

		if (player_one_current_hitpoints_ <= 0) {
			winner_id_ = 2;
			return;
		}

		if (player_two_current_hitpoints_ <= 0) {
			winner_id_ = 1;
			return;
		}

		player_one_time += game_tick_;
		player_two_time += game_tick_;
	}
}

void Duel::inflictDamageForLongRun(Player attacker, Player defender) {
	double attacker_accuracy = computeAccuracy(attacker, defender);
	double roll = (double)(rand() % 10001) / 10000;
	if (roll <= attacker_accuracy) {
		int damage_dealt = rand() % (computeMaxHit(attacker) + 1);
		if (defender.player_id == 1) {
			player_one_current_hitpoints_ -= damage_dealt;
		}
		else if (defender.player_id == 2) {
			player_two_current_hitpoints_ -= damage_dealt;
		}
	}
}

int Duel::runDuelForLongRun(Player player_one, Player player_two) {
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

	while (player_one_current_hitpoints_ > 0
		&& player_two_current_hitpoints_ > 0) {
		if (player_one_has_priority) {
			if (fmod(player_one_time, player_one_attack_speed_) <= 0.001) {
				if (player_one.alternating_styles) {
					player_one_current_style_ 
						= player_one_max_dps_style;
					inflictDamageForLongRun(player_one, player_two);
					player_one_current_style_ = player_one_max_def_style;
				}
				else {
					inflictDamageForLongRun(player_one, player_two);
				}
			}
			if (player_two_current_hitpoints_ <= 0) {
				return 1;
			}
			if (fmod(player_two_time, player_two_attack_speed_) <= 0.001) {
				if (player_two.alternating_styles) {
					player_two_current_style_
						= player_two_max_dps_style;
					inflictDamageForLongRun(player_two, player_one);
					player_two_current_style_ = player_two_max_def_style;
				}
				else {
					inflictDamageForLongRun(player_two, player_one);
				}
			}
		}
		else {
			if (fmod(player_two_time, player_two_attack_speed_) <= 0.001) {
				if (player_two.alternating_styles) {
					player_two_current_style_
						= player_two_max_dps_style;
					inflictDamageForLongRun(player_two, player_one);
					player_two_current_style_ = player_two_max_def_style;
				}
				else {
					inflictDamageForLongRun(player_two, player_one);
				}
			}
			if (player_one_current_hitpoints_ <= 0) {
				return 2;
			}
			if (fmod(player_one_time, player_one_attack_speed_) <= 0.001) {
				if (player_one.alternating_styles) {
					player_one_current_style_
						= player_one_max_dps_style;
					inflictDamageForLongRun(player_one, player_two);
					player_one_current_style_ = player_one_max_def_style;
				}
				else {
					inflictDamageForLongRun(player_one, player_two);
				}
			}
		}
		//Run the long-run faster by cycling through the delay interval
		//if the attack speeds are the same.
		if (player_one_attack_speed_ == player_two_attack_speed_) {
			player_one_time += player_one_attack_speed_;
			player_two_time += player_two_attack_speed_;
		}
		else {
			player_one_time += game_tick_;
			player_two_time += game_tick_;
		}
	}

	if (player_one_current_hitpoints_ <= 0) {
		return 2;
	}

	if (player_two_current_hitpoints_ <= 0) {
		return 1;
	}
	return -1;
}

void Duel::runAnalysis(Player player_one, Player player_two, int num_runs) {
	setCurrentData(player_one, player_two);
	double num_player_one_wins = 0;
	for (int run_i = 0; run_i < num_runs; run_i++) {
		int winner = runDuelForLongRun(player_one, player_two);
		if (winner == 1) {
			num_player_one_wins++;
		}
		setCurrentData(player_one, player_two);
	}

	double player_one_win_rate = num_player_one_wins / num_runs * 100;
	double player_two_win_rate = 100 - player_one_win_rate;
	aggregateSimulationData(player_one, player_two, player_one_win_rate, player_two_win_rate);
	std::cout << "Player one has a win rate of " << player_one_win_rate << "% over " << num_runs << " games." << std::endl;
	std::cout << "Player two has a win rate of " << 100 - player_one_win_rate << "% over " << num_runs << " games." << std::endl;
}

void Duel::aggregateSimulationData(Player player_one, Player player_two,
	double player_one_win_rate, double player_two_win_rate) {
	
	double player_one_max_hit = 0;
	double player_one_dps = 0;
	double player_two_max_hit = 0;
	double player_two_dps = 0;

	if (player_one.alternating_styles) {
		if (player_two.alternating_styles) {
			player_two_current_style_ = findStyleWithMaxDefence(player_two);
		}
		else {
			player_two_current_style_ = player_two.selected_weapon_style;
		}
		player_one_current_style_ = findStyleWithMaxDamagePerSec(player_one, player_two);
		player_one_max_hit = computeMaxHit(player_one);
		player_one_dps = computeDamagePerSecond(computeAccuracy(player_one, player_two), 
			player_one_max_hit, player_one_attack_speed_);
	}
	else {
		player_one_current_style_ = player_one.selected_weapon_style;
		if (player_two.alternating_styles) {
			player_two_current_style_ = findStyleWithMaxDefence(player_two);
		}
		else {
			player_two_current_style_ = player_two.selected_weapon_style;
		}
		player_one_max_hit = computeMaxHit(player_one);
		player_one_dps = computeDamagePerSecond(computeAccuracy(player_one, player_two),
			player_one_max_hit, player_one_attack_speed_);
	}

	if (player_two.alternating_styles) {
		if (player_one.alternating_styles) {
			player_one_current_style_ = findStyleWithMaxDefence(player_one);
		}
		else {
			player_one_current_style_ = player_one.selected_weapon_style;
		}
		player_two_current_style_ = findStyleWithMaxDamagePerSec(player_two, player_one);
		player_two_max_hit = computeMaxHit(player_two);
		player_two_dps = computeDamagePerSecond(computeAccuracy(player_two, player_one),
			player_two_max_hit, player_two_attack_speed_);
	}
	else {
		player_two_current_style_ = player_two.selected_weapon_style;
		if (player_one.alternating_styles) {
			player_one_current_style_ = findStyleWithMaxDefence(player_one);
		}
		else {
			player_one_current_style_ = player_one.selected_weapon_style;
		}
		player_two_max_hit = computeMaxHit(player_two);
		player_two_dps = computeDamagePerSecond(computeAccuracy(player_two, player_one),
			player_two_max_hit, player_two_attack_speed_);
	}
	
	bool player_one_win_status = false;
	bool player_two_win_status = false;

	if (winner_id_ == 1) {
		player_one_win_status = true;
	}
	else {
		player_two_win_status = true;
	}

	simulation_.savePlayerOneData(player_one_max_hit, 
		player_one_dps, player_one_win_rate, player_one_win_status);
	simulation_.savePlayerTwoData(player_two_max_hit, 
		player_two_dps, player_two_win_rate, player_two_win_status);
}