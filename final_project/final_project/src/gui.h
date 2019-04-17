#pragma once

#include "ofxDatGui.h"
#include "ofxJSON.h"
#include "duel.h"

//Class the represents the user interface of the duel simulator.
class Gui {

public:

	//Sets up the user interface.
	void setup();

private:

	//Used to run a simulation with the option of a long-run analysis.
	Duel duel;

	//The attack styles that a player can use. Each adds a different set of bonuses to their stats.
	//Users should be wary not all weapons have all options.
	std::vector<std::string> attack_styles
		= { "ACCURATE", "AGGRESSIVE", "CONTROLLED", "DEFENSIVE" };

	//The list of weapons possible to be used in the duel.
	std::vector<std::string> weapons
		= { "Unarmed", "Abyssal Tentacle" };

	//The ids of the weapons (at the same indexes). To be used with an item API.
	std::vector<std::string> weapon_ids
		= { "-1", "12006" };

	//The max length of a username cannot exceed 12 characters.
	int max_username_length = 12;

	//Gui used to randomize players' stats.
	ofxDatGui* randomizer_gui = new ofxDatGui(0, 0);

	//Gui that displays and allows modification of player one's stats. 
	ofxDatGui* player_one_gui = new ofxDatGui(0, 0.15 * ofGetWindowHeight());

	//Gui that displays and allows modification of player two's stats.
	ofxDatGui* player_two_gui = new ofxDatGui(0, 0.50 * ofGetWindowHeight());

	//Gui that allows the user to run a simulation of the duel and generate
	//a long-run analysis.
	ofxDatGui* duel_runner_gui = new ofxDatGui(0, 0.85 * ofGetWindowHeight());

	//Used to parse json.
	ofxJSONElement result;

	//Sets up the player gui for a player based on their id (1 or 2).
	void setupPlayer(int player_id);

	//Sets up the randomizer gui that randomizers the players' stats.
	void setupRandomizer();

	//Sets up the duel runner gui.
	void setupDuelRunner();

	//Computes the combined attack, strength, defence, and hitpoints level
	//of a player.
	int computePlayerTotal(int player_id);

	//Randomizes a player's stats.
	void setRandomStats(int player_id);

	//Parses json data from a url and changes the data displayed by the gui.
	void parseJsonFromUrl(ofxDatGui* gui, std::string url);

	//Called when the user inputs a name to lookup. Enforces name restrictions.
	void onNameInputEvent(ofxDatGuiTextInputEvent e);

	//Called when the user clicks the "Lookup player" button.
	//Attempts to get API data about the inputted username.
	void onLookupEvent(ofxDatGuiButtonEvent e);

	//Called when the user changes a player's stats. Enforces stat restrictions.
	void onStatChangeEvent(ofxDatGuiSliderEvent e);

	//Called when the user changes a player's weapon. Enforces stat restrictions.
	void onWeaponChangeEvent(ofxDatGuiDropdownEvent e);

	//Called when the user clicks the "Generate random players" button.
	void onRandomizeEvent(ofxDatGuiButtonEvent e);

	//Called when the user attempts to set the number of runs to
	//analyze. Enforces number of run restrictions (for long-run analysis).
	void onSetRunsEvent(ofxDatGuiSliderEvent e);

	//Called when the user clicks the "Run simulation" button.
	void onRunEvent(ofxDatGuiButtonEvent e);

};