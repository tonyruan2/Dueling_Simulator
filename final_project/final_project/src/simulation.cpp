#include "simulation.h"

void Simulation::setup() {
	player_one_start_picture.load("player_one_start.png");
	player_one_attack_picture.load("player_one_attack.png");
	player_one_defend_picture.load("player_one_defend.png");
	player_two_start_picture.load("player_two_start.png");
	player_two_attack_picture.load("player_two_attack.png");
	player_two_defend_picture.load("player_two_defend.png");
}

void Simulation::saveSimulation(std::vector<int> player_one_simulation_actions,
	std::vector<int> player_two_simulation_actions) {
	this->player_one_simulation_actions = player_one_simulation_actions;
	this->player_two_simulation_actions = player_two_simulation_actions;
	duel_has_started = true;
}

void Simulation::drawPrevious() {
	if (previous_image_identifier == player_one_attack_identifier) {
		setPlayerOneAttack(previous_player_one_damage);
	}
	else if (previous_image_identifier == player_two_attack_identifier) {
		setPlayerTwoAttack(previous_player_two_damage);
	}
	else {
		setBothPlayersAttack(previous_player_one_damage, previous_player_two_damage);
	}
}

void Simulation::draw() {
	if (!duel_has_started) {
		setStartingPictures();
		return;
	}

	if (current_game_tick < player_one_simulation_actions.size()) {
		if (player_one_simulation_actions.at(current_game_tick) >= 0
			&& player_two_simulation_actions.at(current_game_tick) >= 0) {
			
			previous_image_identifier = both_players_attack_identifier;
			previous_player_one_damage 
				= player_one_simulation_actions.at(current_game_tick);
			previous_player_two_damage 
				= player_two_simulation_actions.at(current_game_tick);
			setBothPlayersAttack(
				previous_player_one_damage, previous_player_two_damage);
		}

		else if (player_one_simulation_actions.at(current_game_tick) >= 0) {
			previous_image_identifier = player_one_attack_identifier;
			previous_player_one_damage
				= player_one_simulation_actions.at(current_game_tick);
			setPlayerOneAttack(
				previous_player_one_damage);
		}

		else if (player_two_simulation_actions.at(current_game_tick) >= 0) {
			previous_image_identifier = player_two_attack_identifier;
			previous_player_two_damage
				= player_two_simulation_actions.at(current_game_tick);
			setPlayerTwoAttack(
				previous_player_two_damage);
		}

		else {
			drawPrevious();
		}
	}
	current_game_tick++;
	ofSleepMillis(game_tick_value);
	if (current_game_tick > player_one_simulation_actions.size()) {
		current_game_tick = 0;
	}
}

void Simulation::setStartingPictures() {
	player_one_start_picture.draw(600, 0, 100, 200);
	player_two_start_picture.draw(700, 0, 100, 200);
}

void Simulation::setPlayerOneAttack(int damage) {
	player_one_attack_picture.draw(600, 0, 100, 200);
	player_two_defend_picture.draw(700, 0, 100, 200);
	ofDrawBitmapString(std::to_string(damage), 800, 300);
}

void Simulation::setPlayerTwoAttack(int damage) {
	player_one_defend_picture.draw(600, 0, 100, 200);
	player_two_attack_picture.draw(700, 0, 100, 200);
	ofDrawBitmapString(std::to_string(damage), 600, 300);
}

void Simulation::setBothPlayersAttack(int player_one_damage,
	int player_two_damage) {
	player_one_attack_picture.draw(600, 0, 100, 200);
	player_two_attack_picture.draw(700, 0, 100, 200);
	ofDrawBitmapString(std::to_string(player_one_damage), 800, 300);
	ofDrawBitmapString(std::to_string(player_two_damage), 600, 300);
}