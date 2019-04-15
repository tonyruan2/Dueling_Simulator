#include "ofApp.h"
#include "gui.h"

void Gui::setup() {
	gui->addLabel("PLAYER ONE");
	gui->addTextInput("Player name:", "");

	gui->addButton("Lookup player one");
	gui->onButtonEvent(this, &Gui::onButtonEvent);

	gui->addSlider("Attack:", 1, 99, 1)->setPrecision(0);
	gui->addSlider("Strength:", 1, 99, 1)->setPrecision(0);
	gui->addSlider("Defence:", 1, 99, 1)->setPrecision(0); //spelling is Defence in game
	gui->addSlider("Hitpoints:", 10, 99, 1)->setPrecision(0);

	std::vector<std::string> attack_styles = { "ACCURATE", "AGGRESSIVE", "DEFENSIVE" };
	gui->addDropdown("Attack style", attack_styles)->select(0);

	gui->addToggle("Alternate attack style?", false);

	gui->addToggle("Use weapon?", false);
}

void Gui::draw() {

}

void Gui::onButtonEvent(ofxDatGuiButtonEvent e) {
	std::cout << e.target->getLabel() << " was clicked!" << std::endl;

	std::string username = gui->getTextInput("Player name:")->getText();
	//http://www.sudo.tf/api/hiscores/grab.php?rsn=Zezima
	//https://www.osrsbox.com/osrsbox-db/items-json/4151.json

	std::string url = "http://www.sudo.tf/api/hiscores/grab.php?rsn=" + username;

	// Now parse the JSON
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
			//do something to tell user about bad input
		}
	}
	else
	{
		ofLogNotice("ofApp::setup") << "Failed to parse JSON" << endl;
	}
}
