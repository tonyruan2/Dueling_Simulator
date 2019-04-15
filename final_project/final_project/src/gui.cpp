#include "ofApp.h"
#include "gui.h"
#include <string>
#include <algorithm>

void Gui::setup(int player_id) {
	ofxDatGui* gui;
	std::string player_label;
	if (player_id == 1) {
		gui = player_one_gui;
		player_label = "ONE";
	}
	else {
		gui = player_two_gui;
		player_label = "TWO";
	}

	gui->addHeader(":: PLAYER " + player_label + " ::");
	gui->addTextInput("Player name:", "");

	gui->addButton("LOOKUP PLAYER " + player_label)->setLabelAlignment(ofxDatGuiAlignment::CENTER);

	gui->addSlider("Attack:", 1, 99, 1)->setPrecision(0);
	gui->addSlider("Strength:", 1, 99, 1)->setPrecision(0);
	gui->addSlider("Defence:", 1, 99, 1)->setPrecision(0); //spelling is Defence in game
	gui->addSlider("Hitpoints:", 10, 99, 1)->setPrecision(0);

	std::vector<std::string> attack_styles = { "ACCURATE", "AGGRESSIVE", "DEFENSIVE" };
	gui->addDropdown("Attack style", attack_styles)->select(0);

	gui->addToggle("Alternate attack style?", false);

	gui->addToggle("Use weapon?", false);

	gui->onButtonEvent(this, &Gui::onButtonEvent);
}

void Gui::draw() {

}

void Gui::onButtonEvent(ofxDatGuiButtonEvent e) {
	ofxDatGui* gui;
	if (e.target->is("LOOKUP PLAYER ONE")) {
		gui = player_one_gui;
	}
	else {
		gui = player_two_gui;
	}

	std::string url = "http://www.sudo.tf/api/hiscores/grab.php?rsn=" 
		+ gui->getTextInput("Player name:")->getText();

	bool parsingSuccessful = result.open(url);

	if (parsingSuccessful)
	{
		std::cout << result.getRawString() << std::endl;
		if (result["status"].asString() == "success") {
			gui->getSlider("Attack:")->setValue(stoi(result["stats"]["attack"]["level"].asString()));
			gui->getSlider("Strength:")->setValue(stoi(result["stats"]["strength"]["level"].asString()));
			gui->getSlider("Defence:")->setValue(stoi(result["stats"]["defence"]["level"].asString()));
			gui->getSlider("Hitpoints:")->setValue(stoi(result["stats"]["hitpoints"]["level"].asString()));
		}
		else {
			gui->getTextInput("Player name:")->setText("[INVALID NAME]");
		}
	}
	else
	{
		ofLogNotice("Gui::OnButtonEvent") << "Failed to parse JSON" << endl;
	}
}
