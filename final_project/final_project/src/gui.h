#pragma once

#include "ofxDatGui.h"
#include "ofxJSON.h"

class Gui {

public:
	void setup();

private:
	ofxDatGui* randomizer_gui = new ofxDatGui(0, 0);
	ofxDatGui* player_one_gui = new ofxDatGui(0, 0.15 * ofGetWindowHeight());
	ofxDatGui* player_two_gui = new ofxDatGui(0, 0.50 * ofGetWindowHeight());
	ofxDatGui* duel_runner_gui = new ofxDatGui(0, 0.85 * ofGetWindowHeight());
	ofxJSONElement result;

	void setupPlayer(int player_id);
	void setupRandomizer();
	void setupDuelRunner();

	void onLookupEvent(ofxDatGuiButtonEvent e);
	int computePlayerTotal(int player_id);
	void onStatChangeEvent(ofxDatGuiSliderEvent e);
	void setRandomStats(int player_id);
	void onRandomizeEvent(ofxDatGuiButtonEvent e);
	void onRunEvent(ofxDatGuiButtonEvent e);
	void onSetRunsEvent(ofxDatGuiSliderEvent e);

};