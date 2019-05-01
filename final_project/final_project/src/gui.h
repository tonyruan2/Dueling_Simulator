#pragma once

#include "ofxDatGui.h"
#include "ofxJSON.h"
#include "duel.h"

//Class that represents the user interface of the duel simulator.
class Gui {

public:
	//Used to run a simulation with the option of a long-run analysis.
	Duel duel_;

	//Sets up the user interface.
	void setup();

private:
	//The weapon styles that a player can use. Each adds a different set of bonuses to their stats.
	std::vector<std::vector<std::string>> weapon_styles_;

	//Unarmed weapon styles. The last weapon style is the same as the first
	//to allow players to change from three style weapons to four style weapons and vice versa.
	std::vector<std::string> unarmed_weapon_styles_
		= { "punch (accurate, crush)", "kick (aggressive, crush)", "block (defensive, crush)", "punch (accurate, crush)" };

	//The list of weapons in the duel.
	std::vector<std::string> weapons_
		= { "Unarmed", "Elder Maul", "Abyssal Tentacle", "Dragon Scimitar" };

	//The ids of the weapons (at the same indexes as above). Used with an item API.
	std::vector<std::string> weapon_ids_
		= { "-1", "21003", "12006", "4587" };

	//Attack requirements to use each weapon. Retrieved from API using above ids.
	//0 is for unarmed.
	std::vector<int> weapon_attack_reqs_ = { 0 };

	//The maximum number of attack styles. Weapons either have 3 or 4 attack styles.
	int max_num_attack_styles_ = 4;

	//The max length of a username cannot exceed 12 characters.
	//User input will be shortened to 12 characters upon being read
	//if the input exceeds the max username length.
	int max_username_length_ = 12;

	//Gui used to randomize players' stats.
	ofxDatGui* randomizer_gui_ = new ofxDatGui(0, 0);

	//Gui that displays and allows modification of player one's stats. 
	ofxDatGui* player_one_gui_ = new ofxDatGui(0, 0.15 * ofGetWindowHeight());

	//Gui that displays and allows modification of player two's stats.
	ofxDatGui* player_two_gui_ = new ofxDatGui(0, 0.50 * ofGetWindowHeight());

	//Gui that allows the user to run a simulation of the duel and generate
	//a long-run analysis.
	ofxDatGui* duel_runner_gui_ = new ofxDatGui(0, 0.85 * ofGetWindowHeight());

	//Used to parse player json data.
	ofxJSONElement player_json_result_;

	//Used to parse weapon json data.
	ofxJSONElement weapon_json_result_;

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

	//Parses player json data from a url and changes the data displayed by the gui.
	void parsePlayerJson(ofxDatGui* gui, std::string url);

	//Parses weapon json data using weapon ids.
	void parseWeaponJson();

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