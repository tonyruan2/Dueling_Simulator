#include "simulation.h"

void Simulation::setup() {
	player_one_start_picture.load("player_one_start.png");
	player_one_attack_picture.load("player_one_attack.png");
	player_one_defend_picture.load("player_one_defend.png");
	player_two_start_picture.load("player_two_start.png");
	player_two_attack_picture.load("player_two_attack.png");
	player_two_defend_picture.load("player_two_defend.png");

	blue_hit_splat.load("blue_hit_splat.png");
	red_hit_splat.load("red_hit_splat.png");

	player_one_health_bar->addSlider("Health:", 0, 99)->setPrecision(0);
	player_one_health_bar->getSlider("Health:")->setValue(99);
	player_one_health_bar->getSlider("Health:")->setEnabled(false);
	player_one_health_bar->getSlider("Health:")->setVisible(false);

	player_two_health_bar->addSlider("Health:", 0, 99)->setPrecision(0);
	player_two_health_bar->getSlider("Health:")->setValue(99);
	player_two_health_bar->getSlider("Health:")->setEnabled(false);
	player_two_health_bar->getSlider("Health:")->setVisible(false);
}

void Simulation::saveSimulation(int player_one_hitpoints_level, 
	int player_two_hitpoints_level,
	std::vector<int> player_one_simulation_actions,
	std::vector<int> player_two_simulation_actions) {
	player_one_health_bar->getSlider("Health:")->setValue(player_one_hitpoints_level);
	player_one_health_bar->getSlider("Health:")->setMax(player_one_hitpoints_level);
	player_one_health_bar->getSlider("Health:")->setVisible(true);

	player_two_health_bar->getSlider("Health:")->setValue(player_two_hitpoints_level);
	player_two_health_bar->getSlider("Health:")->setMax(player_two_hitpoints_level);
	player_two_health_bar->getSlider("Health:")->setVisible(true);

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
	player_one_start_picture.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_start_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3), 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
}

void Simulation::setPlayerOneAttack(int damage) {
	player_one_attack_picture.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_defend_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3), 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(1, damage);
}

void Simulation::setPlayerTwoAttack(int damage) {
	player_one_defend_picture.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_attack_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3), 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(2, damage);
}

void Simulation::setBothPlayersAttack(int player_one_damage,
	int player_two_damage) {
	player_one_attack_picture.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_attack_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3), 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(1, player_one_damage);
	setPlayerDamage(2, player_two_damage);
}

void Simulation::setPlayerDamage(int attacker_id, int damage) {
	if (attacker_id == 1) {
		if (damage == 0) {
			blue_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 2.3), ofGetHeight() / 5);
		}
		else if (damage > 0) {
			red_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 2.3), ofGetHeight() / 5);
		}
		if (damage > 9) {
			ofDrawBitmapString(std::to_string(damage), (ofGetWidth() / 3.4)
				+ (ofGetWidth() / 2.3) + 17, ofGetHeight() / 5 + 27);
		}
		else {
			ofDrawBitmapString(std::to_string(damage), (ofGetWidth() / 3.4)
				+ (ofGetWidth() / 2.3) + 21, ofGetHeight() / 5 + 27);
		}
	}
	else if (attacker_id == 2) {
		if (damage == 0) {
			blue_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 5), ofGetHeight() / 5);
		}
		else if (damage > 0) {
			red_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 5), ofGetHeight() / 5);
		}
		if (damage > 9) {
			ofDrawBitmapString(std::to_string(damage), (ofGetWidth() / 3.4)
				+ (ofGetWidth() / 5) + 17, ofGetHeight() / 5 + 27);
		}
		else {
			ofDrawBitmapString(std::to_string(damage), (ofGetWidth() / 3.4)
				+ (ofGetWidth() / 5) + 21, ofGetHeight() / 5 + 27);
		}
	}
}

void Simulation::savePlayerOneData(double max_hit, double damage_per_second,
	double win_rate, bool is_winner) {

}

void Simulation::savePlayerTwoData(double max_hit, double damage_per_second,
	double win_rate, bool is_winner) {

}