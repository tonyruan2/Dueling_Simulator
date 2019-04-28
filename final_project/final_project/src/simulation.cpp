#include "simulation.h"

void Simulation::setup() {
	setupPictures();
	setupHitpointsBars();
	setupPlayerDataDisplay();
}

void Simulation::setupPictures() {
	player_one_start_picture.load("player_one_start.png");
	player_one_attack_picture.load("player_one_attack.png");
	player_one_defend_picture.load("player_one_defend.png");
	player_two_start_picture.load("player_two_start.png");
	player_two_attack_picture.load("player_two_attack.png");
	player_two_defend_picture.load("player_two_defend.png");

	blue_hit_splat.load("blue_hit_splat.png");
	red_hit_splat.load("red_hit_splat.png");
}

void Simulation::setupHitpointsBars() {
	player_one_hitpoints_bar
		->addSlider("Hitpoints:", 0, max_hitpoints)->setPrecision(0);
	player_one_hitpoints_bar->getSlider("Hitpoints:")->setValue(max_hitpoints);
	player_one_hitpoints_bar->getSlider("Hitpoints:")->setEnabled(false);
	player_one_hitpoints_bar->setVisible(false);

	player_two_hitpoints_bar
		->addSlider("Hitpoints:", 0, max_hitpoints)->setPrecision(0);
	player_two_hitpoints_bar->getSlider("Hitpoints:")->setValue(max_hitpoints);
	player_two_hitpoints_bar->getSlider("Hitpoints:")->setEnabled(false);
	player_two_hitpoints_bar->setVisible(false);
}

void Simulation::setupPlayerDataDisplay() {
	player_one_data->addHeader(":: Player One Simulation Data ::")
		->setBackgroundColor(ofColor(120, 120, 120));
	player_one_data->addTextInput("Max hit:")->setEnabled(false);
	player_one_data->addTextInput("DPS:")->setEnabled(false);
	player_one_data->addTextInput("Long-term WR:")->setEnabled(false);
	player_one_data->addTextInput("Winner:")->setEnabled(false);
	player_one_data->setVisible(false);

	player_two_data->addHeader(":: Player Two Simulation Data ::")
		->setBackgroundColor(ofColor(120, 120, 120));
	player_two_data->addTextInput("Max hit:")->setEnabled(false);
	player_two_data->addTextInput("DPS:")->setEnabled(false);
	player_two_data->addTextInput("Long-term WR:")->setEnabled(false);
	player_two_data->addTextInput("Winner:")->setEnabled(false);
	player_two_data->setVisible(false);
}

void Simulation::saveSimulation(int player_one_hitpoints_level, 
	int player_two_hitpoints_level,
	std::vector<int> player_one_simulation_actions,
	std::vector<int> player_two_simulation_actions) {
	current_game_tick = 0;
	player_one_hitpoints_bar->getSlider("Hitpoints:")
		->setMax(player_one_hitpoints_level);
	player_one_hitpoints_bar->getSlider("Hitpoints:")
		->setValue(player_one_hitpoints_level);
	player_one_hitpoints_bar->setVisible(true);

	player_two_hitpoints_bar->getSlider("Hitpoints:")
		->setMax(player_two_hitpoints_level);
	player_two_hitpoints_bar->getSlider("Hitpoints:")
		->setValue(player_two_hitpoints_level);
	player_two_hitpoints_bar->setVisible(true);

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
			player_one_hitpoints_bar->getSlider("Hitpoints:")->setValue(
				player_one_hitpoints_bar->getSlider("Hitpoints:")
				->getValue() - previous_player_two_damage);
			player_two_hitpoints_bar->getSlider("Hitpoints:")->setValue(
				player_two_hitpoints_bar->getSlider("Hitpoints:")
				->getValue() - previous_player_one_damage);
		}

		else if (player_one_simulation_actions.at(current_game_tick) >= 0) {
			previous_image_identifier = player_one_attack_identifier;
			previous_player_one_damage
				= player_one_simulation_actions.at(current_game_tick);
			setPlayerOneAttack(
				previous_player_one_damage);
			player_two_hitpoints_bar->getSlider("Hitpoints:")->setValue(
				player_two_hitpoints_bar->getSlider("Hitpoints:")
				->getValue() - previous_player_one_damage);
		}

		else if (player_two_simulation_actions.at(current_game_tick) >= 0) {
			previous_image_identifier = player_two_attack_identifier;
			previous_player_two_damage
				= player_two_simulation_actions.at(current_game_tick);
			setPlayerTwoAttack(
				previous_player_two_damage);
			player_one_hitpoints_bar->getSlider("Hitpoints:")->setValue(
				player_one_hitpoints_bar->getSlider("Hitpoints:")
				->getValue() - previous_player_two_damage);
		}

		else {
			drawPrevious();
		}
	}
	current_game_tick++;
	ofSleepMillis(game_tick_value);
	if (current_game_tick > player_one_simulation_actions.size()) {
		current_game_tick = 0;
		player_one_hitpoints_bar->getSlider("Hitpoints:")
			->setValue(max_hitpoints);
		player_two_hitpoints_bar->getSlider("Hitpoints:")
			->setValue(max_hitpoints);
	}
}

void Simulation::setStartingPictures() {
	player_one_start_picture.draw(ofGetWidth() / 3.4, 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_start_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
}

void Simulation::setPlayerOneAttack(int damage) {
	player_one_attack_picture.draw(ofGetWidth() / 3.4, 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_defend_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(1, damage);
}

void Simulation::setPlayerTwoAttack(int damage) {
	player_one_defend_picture.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3,
		ofGetHeight() / 1.5);
	player_two_attack_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(2, damage);
}

void Simulation::setBothPlayersAttack(int player_one_damage,
	int player_two_damage) {
	player_one_attack_picture.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3, 
		ofGetHeight() / 1.5);
	player_two_attack_picture.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(1, player_one_damage);
	setPlayerDamage(2, player_two_damage);
}

void Simulation::setPlayerDamage(int attacker_id, int damage) {
	if (attacker_id == 1) {
		if (damage == 0) {
			blue_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 2.3),
				ofGetHeight() / 5);
		}
		else if (damage > 0) {
			red_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 2.3),
				ofGetHeight() / 5);
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
			blue_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 5),
				ofGetHeight() / 5);
		}
		else if (damage > 0) {
			red_hit_splat.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 5),
				ofGetHeight() / 5);
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
	player_one_data->getTextInput("Max hit:")->setText(std::to_string(max_hit));
	player_one_data->getTextInput("DPS:")->setText(std::to_string(damage_per_second));
	player_one_data->getTextInput("Long-term WR:")->setText(std::to_string(win_rate));
	if (is_winner) {
		player_one_data->getTextInput("Winner:")->setText("True");
	}
	else {
		player_one_data->getTextInput("Winner:")->setText("False");
	}
	player_one_data->setVisible(true);
}

void Simulation::savePlayerTwoData(double max_hit, double damage_per_second,
	double win_rate, bool is_winner) {
	player_two_data->getTextInput("Max hit:")->setText(std::to_string(max_hit));
	player_two_data->getTextInput("DPS:")->setText(std::to_string(damage_per_second));
	player_two_data->getTextInput("Long-term WR:")->setText(std::to_string(win_rate));
	if (is_winner) {
		player_two_data->getTextInput("Winner:")->setText("True");
	}
	else {
		player_two_data->getTextInput("Winner:")->setText("False");
	}
	player_two_data->setVisible(true);
}