#include "gui.h"
#include <cstdlib>

void Gui::setup() {
	setupPlayer(1);
	setupPlayer(2);
	setupRandomizer();
	setupDuelRunner();
	parseWeaponJson();
}

void Gui::setupPlayer(int player_id) {
	ofxDatGui* gui;
	std::string player_label;
	if (player_id == 1) {
		gui = player_one_gui;
		player_label = "ONE";
	}
	else if (player_id == 2) {
		gui = player_two_gui;
		player_label = "TWO";
	}
	else {
		return;
	}

	gui->addHeader(":: PLAYER " + player_label + " ::")
		->setBackgroundColor(ofColor(120, 120, 120));
	gui->addTextInput("Player name:", "")
		->setInputType(ofxDatGuiInputType::ALPHA_NUMERIC);
	gui->addButton("LOOKUP PLAYER " + player_label)
		->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	gui->getButton("LOOKUP PLAYER " + player_label)
		->setBackgroundColor(ofColor(125, 0, 0));
	gui->getButton("LOOKUP PLAYER " + player_label)
		->setBackgroundColorOnMouseOver(ofColor(155, 0, 0));

	gui->addSlider("Attack:", 1, 99, 1)->setPrecision(0);
	gui->addSlider("Strength:", 1, 99, 1)->setPrecision(0);

	//spelling is Defence in game
	gui->addSlider("Defence:", 1, 99, 1)->setPrecision(0);

	//min hitpoints is 1 for visual comparison to other stats
	//min value in-game is 10
	gui->addSlider("Hitpoints:", 1, 99, 1)->setPrecision(0);

	//to display as an int rather than the default float
	gui->getSlider("Hitpoints:")->setValue(10);

	gui->addSlider("Total value:", 1, 396, 1)
		->setEnabled(false);
	gui->getSlider("Total value:")->setPrecision(0);

	//to display as an int rather than the default float
	gui->getSlider("Total value:")->setValue(computePlayerTotal(player_id));

	gui->addDropdown("Attack style", unarmed_attack_styles)->select(0);
	gui->getDropdown("Attack style")->getChildAt(3)->setVisible(false);

	gui->addDropdown("Weapon", weapons)->select(0);
	gui->addToggle("Alternate attack style?", false);

	gui->onTextInputEvent(this, &Gui::onNameInputEvent);
	gui->onButtonEvent(this, &Gui::onLookupEvent);
	gui->onSliderEvent(this, &Gui::onStatChangeEvent);
	gui->onDropdownEvent(this, &Gui::onWeaponChangeEvent);
}

void Gui::setupRandomizer() {
	randomizer_gui->addHeader("::Randomizer::")
		->setBackgroundColor(ofColor(120, 120, 120));
	randomizer_gui->addButton("Generate random players")
		->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	randomizer_gui->getButton("Generate random players")
		->setBackgroundColor(ofColor(0, 0, 140));
	randomizer_gui->getButton("Generate random players")
		->setBackgroundColorOnMouseOver(ofColor(0, 0, 190));
	randomizer_gui->addToggle("Ensure similar total stats?", false);
	randomizer_gui->addSlider("Max total dif:", 0, 383, 1)->setPrecision(0);

	//to display as an int rather than the default float
	randomizer_gui->getSlider("Max total dif:")->setValue(10);
	randomizer_gui->onButtonEvent(this, &Gui::onRandomizeEvent);
}

void Gui::setupDuelRunner() {
	duel_runner_gui->addHeader("::Duel Runner::")
		->setBackgroundColor(ofColor(120, 120, 120));
	duel_runner_gui->addButton("Run simulation")
		->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	duel_runner_gui->getButton("Run simulation")
		->setBackgroundColor(ofColor(0, 140, 0));
	duel_runner_gui->getButton("Run simulation")
		->setBackgroundColorOnMouseOver(ofColor(0, 190, 0));
	duel_runner_gui->addToggle("Run long-run analysis?");
	duel_runner_gui->addSlider("Total runs:", 0, 10000, 1)->setPrecision(0);

	//to display as an int rather than the default float
	duel_runner_gui->getSlider("Total runs:")->setValue(100);

	duel_runner_gui->onButtonEvent(this, &Gui::onRunEvent);
	duel_runner_gui->onSliderEvent(this, &Gui::onSetRunsEvent);
}

int Gui::computePlayerTotal(int player_id) {
	ofxDatGui* gui;
	if (player_id == 1) {
		gui = player_one_gui;
	}
	else if (player_id == 2) {
		gui = player_two_gui;
	}
	else {
		return -1; //private method should never return -1
	}

	int total_stat_value = gui->getSlider("Attack:")->getValue()
		+ gui->getSlider("Strength:")->getValue()
		+ gui->getSlider("Defence:")->getValue()
		+ gui->getSlider("Hitpoints:")->getValue();

	return total_stat_value;
}

void Gui::setRandomStats(int player_id) {
	ofxDatGui* gui;
	if (player_id == 1) {
		gui = player_one_gui;
	}
	else if (player_id == 2) {
		gui = player_two_gui;
	}
	else {
		return;
	}

	//generate a value from 1 to 99
	gui->getSlider("Attack:")->setValue(rand() % 100 + 1);
	gui->getSlider("Strength:")->setValue(rand() % 100 + 1);
	gui->getSlider("Defence:")->setValue(rand() % 100 + 1);

	//generate a value from 10 to 99
	gui->getSlider("Hitpoints:")->setValue(rand() % 100 + 10);
}

void Gui::parsePlayerJson(ofxDatGui* gui, std::string url) {
	bool parsingSuccessful = player_json_result.open(url);

	if (parsingSuccessful)
	{
		if (player_json_result["status"].asString() == "success") {
			gui->getTextInput("Player name:")
				->setBackgroundColor(ofColor(0, 0, 0));
			gui->getSlider("Attack:")
				->setValue(stoi(player_json_result["stats"]["attack"]["level"].asString()));
			gui->getSlider("Strength:")
				->setValue(stoi(player_json_result["stats"]["strength"]["level"].asString()));
			gui->getSlider("Defence:")
				->setValue(stoi(player_json_result["stats"]["defence"]["level"].asString()));
			gui->getSlider("Hitpoints:")
				->setValue(stoi(player_json_result["stats"]["hitpoints"]["level"].asString()));
		}
		else {
			gui->getTextInput("Player name:")->setText("##INVALID_NAME##");
			gui->getTextInput("Player name:")
				->setBackgroundColor(ofColor(255, 0, 0));
		}
	}
}

void Gui::parseWeaponJson() {
	weapon_attack_styles.push_back(unarmed_attack_styles);

	for (int weapon_i = 1; weapon_i < weapons.size(); weapon_i++) {
		std::string url = "https://www.osrsbox.com/osrsbox-db/items-json/"
		+ weapon_ids.at(weapon_i) + ".json";

		bool parsingSuccessful = weapon_json_result.open(url);

		if (parsingSuccessful) {
			weapon_attack_reqs.push_back(
			std::stoi(weapon_json_result["equipment"]["requirements"]["attack"].asString()));
		
			std::vector<std::string> styles;

			for (Json::ArrayIndex stance_i = 0;
				stance_i < weapon_json_result["weapon"]["stances"].size(); stance_i++) {
				std::string attack_style = "";
				attack_style.append(weapon_json_result["weapon"]["stances"]
					[stance_i]["combat_style"].asString());
				attack_style.append(" (");
				attack_style.append(weapon_json_result["weapon"]["stances"]
					[stance_i]["attack_style"].asString());
				attack_style.append(", ");
				attack_style.append(weapon_json_result["weapon"]["stances"]
					[stance_i]["attack_type"].asString());
				attack_style.append(")");
				styles.push_back(attack_style);
			}

			if (weapon_json_result["weapon"]["stances"].size() < max_num_attack_styles) {
				styles.push_back(styles.at(0));
			}

			weapon_attack_styles.push_back(styles);
		}
	}
}

void Gui::onNameInputEvent(ofxDatGuiTextInputEvent e) {
	if (!e.target->is("Player name:")) {
		return;
	}

	//user input is truncated at 12 characters
	if (player_one_gui->getTextInput("Player name:")
		->getText().length() > max_username_length) {
		player_one_gui->getTextInput("Player name:")
			->setText(player_one_gui->getTextInput("Player name:")
				->getText().substr(0, max_username_length));
	}

	if (player_two_gui->getTextInput("Player name:")
		->getText().length() > max_username_length) {
		player_two_gui->getTextInput("Player name:")
			->setText(player_two_gui->getTextInput("Player name:")
				->getText().substr(0, max_username_length));
	}
}

void Gui::onLookupEvent(ofxDatGuiButtonEvent e) {
	ofxDatGui* gui;
	int player_id = 0;
	if (e.target->is("LOOKUP PLAYER ONE")) {
		gui = player_one_gui;
		player_id = 1;
	}
	else  if (e.target->is("LOOKUP PLAYER TWO")) {
		gui = player_two_gui;
		player_id = 2;
	}
	else {
		return;
	}

	if (gui->getTextInput("Player name:")->getText() == "") {
		gui->getTextInput("Player name:")->setBackgroundColor(ofColor(0, 0, 0));
		return;
	}

	std::string url = "http://www.sudo.tf/api/hiscores/grab.php?rsn="
		+ gui->getTextInput("Player name:")->getText();

	parsePlayerJson(gui, url);
}

void Gui::onStatChangeEvent(ofxDatGuiSliderEvent e) {
	if (e.target->is("Hitpoints:") && e.target->getValue() < 10) {
		e.target->setValue(10);
	}

	//Enforce attack requirements for weapons
	if (e.target->is("Attack:")) {
		for (int weapon_i = 0; weapon_i < weapons.size(); weapon_i++) {
			if ((player_one_gui->getDropdown("Weapon")
				->getSelected()->getLabel() == weapons.at(weapon_i)) 
				&& player_one_gui->getSlider("Attack:")
				->getValue() < weapon_attack_reqs.at(weapon_i)) {

				player_one_gui->getSlider("Attack:")
					->setValue(weapon_attack_reqs.at(weapon_i));
				return;
			}

			if ((player_two_gui->getDropdown("Weapon")
				->getSelected()->getLabel() == weapons.at(weapon_i))
				&& player_two_gui->getSlider("Attack:")
				->getValue() < weapon_attack_reqs.at(weapon_i)) {

				player_two_gui->getSlider("Attack:")
					->setValue(weapon_attack_reqs.at(weapon_i));
				return;
			}
		}
	}

	player_one_gui->getSlider("Total value:")->setValue(computePlayerTotal(1));
	player_two_gui->getSlider("Total value:")->setValue(computePlayerTotal(2));
}

void Gui::onWeaponChangeEvent(ofxDatGuiDropdownEvent e) {
	ofxDatGui* gui;

	for (int gui_i = 1; gui_i <= 2; gui_i++) {
		if (gui_i == 1) {
			gui = player_one_gui;
		}
		else {
			gui = player_two_gui;
		}

		//Enforce the attack requirements for weapons.
		if (e.target->is("Weapon")) {
			for (int weapon_i = 0; weapon_i < weapons.size(); weapon_i++) {
				if ((gui->getDropdown("Weapon")
					->getSelected()->getLabel() == weapons.at(weapon_i))) {

					//"Unarmed" and weapons with three attack styles have the fourth attack
					//style hidden.
					if (weapons.at(weapon_i) == "Unarmed" ||
						(weapon_attack_styles.at(weapon_i).at(0) ==
							weapon_attack_styles.at(weapon_i).at(max_num_attack_styles - 1))) {

						gui->getDropdown("Attack style")
							->getChildAt(max_num_attack_styles - 1)->setVisible(false);
					}
					else {
						gui->getDropdown("Attack style")
							->getChildAt(max_num_attack_styles - 1)->setVisible(true);
					}


					for (int style_i = 0;
						style_i < weapon_attack_styles.at(weapon_i).size(); style_i++) {
						gui->getDropdown("Attack style")
							->getChildAt(style_i)
							->setLabel(weapon_attack_styles.at(weapon_i).at(style_i));
					}


					gui->getDropdown("Attack style")->setLabel(
						gui->getDropdown("Attack style")
						->getSelected()->getLabel());

					if (gui->getSlider("Attack:")
						->getValue() < weapon_attack_reqs.at(weapon_i)) {

						gui->getSlider("Attack:")
							->setValue(weapon_attack_reqs.at(weapon_i));
					}
					break;
				}
			}

		}
	}
}

void Gui::onRandomizeEvent(ofxDatGuiButtonEvent e) {
	if (!e.target->is("Generate random players")) {
		return;
	}

	player_one_gui->getTextInput("Player name:")->setText("");
	player_two_gui->getTextInput("Player name:")->setText("");

	bool generate_similar = randomizer_gui
		->getToggle("Ensure similar total stats?")->getChecked();

	if (generate_similar) {
		setRandomStats(1);
		int player_one_total_stat_value = computePlayerTotal(1);
		int player_two_total_stat_value = 0; //ensure new data is generated

		//generate random player data for the second player until the total 
		//value of their stats are within a similarity threshold compared 
		//to the first player
		int stats_similarity_threshold = randomizer_gui
			->getSlider("Max total dif:")->getValue();

		while (!(abs(player_two_total_stat_value - player_one_total_stat_value) 
			<= stats_similarity_threshold)) {
			setRandomStats(2);
			player_two_total_stat_value = computePlayerTotal(2);
		}
	}
	else {
		setRandomStats(1);
		setRandomStats(2);
	}
}

void Gui::onSetRunsEvent(ofxDatGuiSliderEvent e) {
	if (e.target->is("Total runs:")
		&& duel_runner_gui->getSlider("Total runs:")->getValue() < 100) {
		duel_runner_gui->getSlider("Total runs:")->setValue(100);
	}
}

void Gui::onRunEvent(ofxDatGuiButtonEvent e) {
	if (e.target->is("Run simulation")) {
		
		Duel::Player player_one;
		player_one.setStats(1, player_one_gui->getSlider("Attack:")->getValue(),
			player_one_gui->getSlider("Strength:")->getValue(),
			player_one_gui->getSlider("Defence:")->getValue(),
			player_one_gui->getSlider("Hitpoints:")->getValue());

		player_one.setCombatSpecs(player_one_gui->getDropdown("Attack style")->getSelected()->getLabel(),
			player_one_gui->getDropdown("Weapon")->getSelected()->getLabel(),
			weapon_ids.at(player_one_gui->getDropdown("Weapon")->getSelected()->getIndex()),
			weapon_attack_styles.at(player_one_gui->getDropdown("Weapon")->getSelected()->getIndex()),
			player_one_gui->getToggle("Alternate attack style?")->getChecked());

		Duel::Player player_two;
		player_two.setStats(2, player_two_gui->getSlider("Attack:")->getValue(),
			player_two_gui->getSlider("Strength:")->getValue(),
			player_two_gui->getSlider("Defence:")->getValue(),
			player_two_gui->getSlider("Hitpoints:")->getValue());

		player_two.setCombatSpecs(player_two_gui->getDropdown("Attack style")->getSelected()->getLabel(),
			player_two_gui->getDropdown("Weapon")->getSelected()->getLabel(),
			weapon_ids.at(player_two_gui->getDropdown("Weapon")->getSelected()->getIndex()),
			weapon_attack_styles.at(player_two_gui->getDropdown("Weapon")->getSelected()->getIndex()),
			player_two_gui->getToggle("Alternate attack style?")->getChecked());

		duel.runSimulation(player_one, player_two,
			duel_runner_gui->getToggle("Run long-run analysis?")->getChecked(),
			duel_runner_gui->getSlider("Total runs:")->getValue());
	}
}