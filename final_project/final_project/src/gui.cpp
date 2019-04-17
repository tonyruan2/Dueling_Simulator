#include "gui.h"
#include <cstdlib>

void Gui::setup() {
	setupPlayer(1);
	setupPlayer(2);
	setupRandomizer();
	setupDuelRunner();
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

	gui->addHeader(":: PLAYER " + player_label + " ::");
	gui->addTextInput("Player name:", "");
	gui->addButton("LOOKUP PLAYER " + player_label)
		->setLabelAlignment(ofxDatGuiAlignment::CENTER);

	gui->addSlider("Attack:", 1, 99, 1)->setPrecision(0);
	gui->addSlider("Strength:", 1, 99, 1)->setPrecision(0);
	//spelling is Defence in game
	gui->addSlider("Defence:", 1, 99, 1)->setPrecision(0);
	//min hitpoints is 1 for visual comparison to other stats, min value in-game is 10
	gui->addSlider("Hitpoints:", 1, 99, 1)->setPrecision(0);
	//to display as an int rather than the default float
	gui->getSlider("Hitpoints:")->setValue(10);
	gui->addSlider("Total value:", 1, 396, 1)
		->setEnabled(false);
	gui->getSlider("Total value:")->setPrecision(0);
	//to display as an int rather than the default float
	gui->getSlider("Total value:")->setValue(computePlayerTotal(player_id));

	std::vector<std::string> attack_styles 
		= { "ACCURATE", "AGGRESSIVE", "DEFENSIVE" };
	gui->addDropdown("Attack style", attack_styles)->select(0);

	std::vector<std::string> weapons
		= { "Unarmed", "Abyssal Tentacle" };
	gui->addDropdown("Weapon", weapons)->select(0);

	gui->addToggle("Alternate attack style?", false);

	gui->onButtonEvent(this, &Gui::onLookupEvent);
	gui->onSliderEvent(this, &Gui::onStatChangeEvent);
	gui->onDropdownEvent(this, &Gui::onWeaponChangeEvent);
}

void Gui::onWeaponChangeEvent(ofxDatGuiDropdownEvent e) {
	//Selecting abyssal tencale changes a player's attack level to 75 if it is below it
	//This is a stat requirement
	if (e.target->is("Weapon")) {
		if ((player_one_gui->getDropdown("Weapon")
			->getSelected()->getLabel() == "Abyssal Tentacle")
			&& player_one_gui->getSlider("Attack:")->getValue() < 75) {
			player_one_gui->getSlider("Attack:")->setValue(75);
		}
		if ((player_two_gui->getDropdown("Weapon")
			->getSelected()->getLabel() == "Abyssal Tentacle")
			&& player_two_gui->getSlider("Attack:")->getValue() < 75) {
			player_two_gui->getSlider("Attack:")->setValue(75);
		}
	}
}

void Gui::setupRandomizer() {
	randomizer_gui->addHeader("::Randomizer::");
	randomizer_gui->addButton("Generate random players")
		->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	randomizer_gui->addToggle("Ensure similar total stats?", false);
	randomizer_gui->addSlider("Max total dif:", 0, 383, 1)->setPrecision(0);
	//to display as an int rather than the default float
	randomizer_gui->getSlider("Max total dif:")->setValue(10);
	randomizer_gui->onButtonEvent(this, &Gui::onRandomizeEvent);
}

void Gui::onSetRunsEvent(ofxDatGuiSliderEvent e) {
	if (e.target->is("Total runs:")
		&& duel_runner_gui->getSlider("Total runs:")->getValue() < 100) {
		duel_runner_gui->getSlider("Total runs:")->setValue(100);
	}
}
void Gui::onRunEvent(ofxDatGuiButtonEvent e) {
	if (e.target->is("Run simulation")) {
		std::cout << "run simulation" << std::endl;
	}
}

void Gui::setupDuelRunner() {
	duel_runner_gui->addHeader("::Duel Runner::");
	duel_runner_gui->addButton("Run simulation")
		->setLabelAlignment(ofxDatGuiAlignment::CENTER);
	duel_runner_gui->addToggle("Run long-run analysis?");
	duel_runner_gui->addSlider("Total runs:", 0, 10000, 1)->setPrecision(0);
	//to display as an int rather than the default float
	duel_runner_gui->getSlider("Total runs:")->setValue(100);

	duel_runner_gui->onButtonEvent(this, &Gui::onRunEvent);
	duel_runner_gui->onSliderEvent(this, &Gui::onSetRunsEvent);
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

	std::string url = "http://www.sudo.tf/api/hiscores/grab.php?rsn=" 
		+ gui->getTextInput("Player name:")->getText();

	bool parsingSuccessful = result.open(url);

	if (parsingSuccessful)
	{
		if (result["status"].asString() == "success") {
			gui->getSlider("Attack:")
				->setValue(stoi(result["stats"]["attack"]["level"].asString()));
			gui->getSlider("Strength:")
				->setValue(stoi(result["stats"]["strength"]["level"].asString()));
			gui->getSlider("Defence:")
				->setValue(stoi(result["stats"]["defence"]["level"].asString()));
			gui->getSlider("Hitpoints:")
				->setValue(stoi(result["stats"]["hitpoints"]["level"].asString()));
		}
		else {
			gui->getTextInput("Player name:")->setText("[INVALID NAME]");
		}
	}
	else
	{
		ofLogNotice("Gui::OnLookupEvent") << "Failed to parse JSON" << endl;
	}
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
		return -1; //private method should not return -1 if player_id argument is 1 or 2
	}

	int total_stat_value = gui->getSlider("Attack:")->getValue()
		+ gui->getSlider("Strength:")->getValue()
		+ gui->getSlider("Defence:")->getValue()
		+ gui->getSlider("Hitpoints:")->getValue();

	return total_stat_value;
}

void Gui::onStatChangeEvent(ofxDatGuiSliderEvent e) {
	if (e.target->is("Hitpoints:") && e.target->getValue() < 10) {
		e.target->setValue(10);
	}
	
	//Abyssal tentacle requires an attack level of at least 75 to use
	if (e.target->is("Attack:")) {
		if (player_one_gui->getSlider("Attack:")->getValue() < 75
			&& player_one_gui->getDropdown("Weapon")->getSelected()->getLabel() == "Abyssal Tentacle") {
			std::cout << "onStatChange" << std::endl;
			std::cout << player_one_gui->getDropdown("Weapon")->getSelected()->getLabel() << std::endl;
			player_one_gui->getSlider("Attack:")->setValue(75);
		}

		if (player_two_gui->getSlider("Attack:")->getValue() < 75
			&& player_two_gui->getDropdown("Weapon")->getSelected()->getLabel() == "Abyssal Tentacle") {
			player_two_gui->getSlider("Attack:")->setValue(75);
		}
	}

	player_one_gui->getSlider("Total value:")->setValue(computePlayerTotal(1));
	player_two_gui->getSlider("Total value:")->setValue(computePlayerTotal(2));
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

	gui->getSlider("Attack:")->setValue(rand() % 100 + 1); //generate a value from 1 to 99
	gui->getSlider("Strength:")->setValue(rand() % 100 + 1);
	gui->getSlider("Defence:")->setValue(rand() % 100 + 1);
	gui->getSlider("Hitpoints:")->setValue(rand() % 100 + 10); //generate a value from 10 to 99
}

void Gui::onRandomizeEvent(ofxDatGuiButtonEvent e) {
	if (!e.target->is("Generate random players")) {
		return;
	}

	player_one_gui->getTextInput("Player name:")->setText("");
	player_two_gui->getTextInput("Player name:")->setText("");

	bool generate_similar = randomizer_gui->getToggle("Ensure similar total stats?")->getChecked();

	if (generate_similar) {
		setRandomStats(1);
		int player_one_total_stat_value = computePlayerTotal(1);
		int player_two_total_stat_value = 0; //ensure that new player data is generated

		//generate random player data for the second player until the total value of their stats
		//are within a similarity threshold compared to the first player
		int stats_similarity_threshold = randomizer_gui->getSlider("Max total dif:")->getValue();
		while (!(abs(player_two_total_stat_value - player_one_total_stat_value) <= stats_similarity_threshold)) {
			setRandomStats(2);
			player_two_total_stat_value = computePlayerTotal(2);
		}
	}
	else {
		setRandomStats(1);
		setRandomStats(2);
	}
}