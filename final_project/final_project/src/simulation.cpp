#include "simulation.h"

void Simulation::setup() {
	setupPictures();
	setupHitpointsBars();
	setupPlayerDataDisplay();
}

void Simulation::saveSimulation(int player_one_hitpoints_level, 
	int player_two_hitpoints_level,
	std::vector<int> player_one_simulation_actions,
	std::vector<int> player_two_simulation_actions) {
	current_game_tick_ = 0;
	player_one_hitpoints_bar_->getSlider("Hitpoints:")
		->setMax(player_one_hitpoints_level);
	player_one_hitpoints_bar_->getSlider("Hitpoints:")
		->setValue(player_one_hitpoints_level);
	player_one_hitpoints_bar_->setVisible(true);

	player_two_hitpoints_bar_->getSlider("Hitpoints:")
		->setMax(player_two_hitpoints_level);
	player_two_hitpoints_bar_->getSlider("Hitpoints:")
		->setValue(player_two_hitpoints_level);
	player_two_hitpoints_bar_->setVisible(true);

	this->player_one_simulation_actions_ = player_one_simulation_actions;
	this->player_two_simulation_actions_ = player_two_simulation_actions;
	duel_has_started_ = true;
}

void Simulation::savePlayerOneData(double max_hit, double damage_per_second,
	double win_rate, bool is_winner) {
	player_one_data_->getTextInput("Max hit:")->setText(std::to_string(max_hit));
	player_one_data_->getTextInput("DPS:")->setText(std::to_string(damage_per_second));
	player_one_data_->getTextInput("Long-term WR:")->setText(std::to_string(win_rate));
	if (is_winner) {
		player_one_data_->getTextInput("Winner:")->setText("True");
	}
	else {
		player_one_data_->getTextInput("Winner:")->setText("False");
	}
	player_one_data_->setVisible(true);
}

void Simulation::savePlayerTwoData(double max_hit, double damage_per_second,
	double win_rate, bool is_winner) {
	player_two_data_->getTextInput("Max hit:")->setText(std::to_string(max_hit));
	player_two_data_->getTextInput("DPS:")->setText(std::to_string(damage_per_second));
	player_two_data_->getTextInput("Long-term WR:")->setText(std::to_string(win_rate));
	if (is_winner) {
		player_two_data_->getTextInput("Winner:")->setText("True");
	}
	else {
		player_two_data_->getTextInput("Winner:")->setText("False");
	}
	player_two_data_->setVisible(true);
}

void Simulation::setPlayerDataVisibility(bool visibility) {
	player_one_data_->setVisible(visibility);
	player_two_data_->setVisible(visibility);
}

void Simulation::draw() {
	if (!duel_has_started_) {
		setStartingPictures();
		return;
	}

	if (current_game_tick_ < player_one_simulation_actions_.size()) {
		if (player_one_simulation_actions_.at(current_game_tick_) >= 0
			&& player_two_simulation_actions_.at(current_game_tick_) >= 0) {
			
			previous_image_identifier_ = both_players_attack_identifier_;
			previous_player_one_damage_ 
				= player_one_simulation_actions_.at(current_game_tick_);
			previous_player_two_damage_ 
				= player_two_simulation_actions_.at(current_game_tick_);
			setBothPlayersAttack(
				previous_player_one_damage_, previous_player_two_damage_);
			player_one_hitpoints_bar_->getSlider("Hitpoints:")->setValue(
				player_one_hitpoints_bar_->getSlider("Hitpoints:")
				->getValue() - previous_player_two_damage_);
			player_two_hitpoints_bar_->getSlider("Hitpoints:")->setValue(
				player_two_hitpoints_bar_->getSlider("Hitpoints:")
				->getValue() - previous_player_one_damage_);
		}

		else if (player_one_simulation_actions_.at(current_game_tick_) >= 0) {
			previous_image_identifier_ = player_one_attack_identifier_;
			previous_player_one_damage_
				= player_one_simulation_actions_.at(current_game_tick_);
			setPlayerOneAttack(
				previous_player_one_damage_);
			player_two_hitpoints_bar_->getSlider("Hitpoints:")->setValue(
				player_two_hitpoints_bar_->getSlider("Hitpoints:")
				->getValue() - previous_player_one_damage_);
		}

		else if (player_two_simulation_actions_.at(current_game_tick_) >= 0) {
			previous_image_identifier_ = player_two_attack_identifier_;
			previous_player_two_damage_
				= player_two_simulation_actions_.at(current_game_tick_);
			setPlayerTwoAttack(
				previous_player_two_damage_);
			player_one_hitpoints_bar_->getSlider("Hitpoints:")->setValue(
				player_one_hitpoints_bar_->getSlider("Hitpoints:")
				->getValue() - previous_player_two_damage_);
		}

		else {
			drawPrevious();
		}
	}
	current_game_tick_++;
	ofSleepMillis(game_tick_value_);
	if (current_game_tick_ > player_one_simulation_actions_.size()) {
		current_game_tick_ = 0;
		player_one_hitpoints_bar_->getSlider("Hitpoints:")
			->setValue(max_hitpoints_);
		player_two_hitpoints_bar_->getSlider("Hitpoints:")
			->setValue(max_hitpoints_);
	}
}

void Simulation::setupPictures() {
	player_one_start_picture_.load("player_one_start.png");
	player_one_attack_picture_.load("player_one_attack.png");
	player_one_defend_picture_.load("player_one_defend.png");
	player_two_start_picture_.load("player_two_start.png");
	player_two_attack_picture_.load("player_two_attack.png");
	player_two_defend_picture_.load("player_two_defend.png");

	blue_hit_splat_.load("blue_hit_splat.png");
	red_hit_splat_.load("red_hit_splat.png");
}

void Simulation::setupHitpointsBars() {
	player_one_hitpoints_bar_
		->addSlider("Hitpoints:", 0, max_hitpoints_)->setPrecision(0);
	player_one_hitpoints_bar_->getSlider("Hitpoints:")->setValue(max_hitpoints_);
	player_one_hitpoints_bar_->getSlider("Hitpoints:")->setEnabled(false);
	player_one_hitpoints_bar_->setVisible(false);

	player_two_hitpoints_bar_
		->addSlider("Hitpoints:", 0, max_hitpoints_)->setPrecision(0);
	player_two_hitpoints_bar_->getSlider("Hitpoints:")->setValue(max_hitpoints_);
	player_two_hitpoints_bar_->getSlider("Hitpoints:")->setEnabled(false);
	player_two_hitpoints_bar_->setVisible(false);
}

void Simulation::setupPlayerDataDisplay() {
	player_one_data_->addHeader(":: Player One Simulation Data ::")
		->setBackgroundColor(ofColor(120, 120, 120));
	player_one_data_->addTextInput("Max hit:")->setEnabled(false);
	player_one_data_->addTextInput("DPS:")->setEnabled(false);
	player_one_data_->addTextInput("Long-term WR:")->setEnabled(false);
	player_one_data_->addTextInput("Winner:")->setEnabled(false);
	player_one_data_->setVisible(false);

	player_two_data_->addHeader(":: Player Two Simulation Data ::")
		->setBackgroundColor(ofColor(120, 120, 120));
	player_two_data_->addTextInput("Max hit:")->setEnabled(false);
	player_two_data_->addTextInput("DPS:")->setEnabled(false);
	player_two_data_->addTextInput("Long-term WR:")->setEnabled(false);
	player_two_data_->addTextInput("Winner:")->setEnabled(false);
	player_two_data_->setVisible(false);
}

void Simulation::setStartingPictures() {
	player_one_start_picture_.draw(ofGetWidth() / 3.4, 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_start_picture_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
}

void Simulation::setPlayerOneAttack(int damage) {
	player_one_attack_picture_.draw(ofGetWidth() / 3.4, 0,
		ofGetWidth() / 3, ofGetHeight() / 1.5);
	player_two_defend_picture_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(1, damage);
}

void Simulation::setPlayerTwoAttack(int damage) {
	player_one_defend_picture_.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3,
		ofGetHeight() / 1.5);
	player_two_attack_picture_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(2, damage);
}

void Simulation::setBothPlayersAttack(int player_one_damage,
	int player_two_damage) {
	player_one_attack_picture_.draw(ofGetWidth() / 3.4, 0, ofGetWidth() / 3, 
		ofGetHeight() / 1.5);
	player_two_attack_picture_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 3),
		0, ofGetWidth() / 3, ofGetHeight() / 1.5);
	setPlayerDamage(1, player_one_damage);
	setPlayerDamage(2, player_two_damage);
}

void Simulation::setPlayerDamage(int attacker_id, int damage) {
	if (attacker_id == 1) {
		if (damage == 0) {
			blue_hit_splat_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 2.3),
				ofGetHeight() / 5);
		}
		else if (damage > 0) {
			red_hit_splat_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 2.3),
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
			blue_hit_splat_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 5),
				ofGetHeight() / 5);
		}
		else if (damage > 0) {
			red_hit_splat_.draw((ofGetWidth() / 3.4) + (ofGetWidth() / 5),
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

void Simulation::drawPrevious() {
	if (previous_image_identifier_ == player_one_attack_identifier_) {
		setPlayerOneAttack(previous_player_one_damage_);
	}
	else if (previous_image_identifier_ == player_two_attack_identifier_) {
		setPlayerTwoAttack(previous_player_two_damage_);
	}
	else {
		setBothPlayersAttack(previous_player_one_damage_, previous_player_two_damage_);
	}
}