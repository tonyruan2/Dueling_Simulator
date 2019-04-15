#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxJSON.h"

class Gui {

public:
	void setup();

private:
	ofxDatGui* player_one_gui = new ofxDatGui(0, ofGetHeight() / 2);
	ofxDatGui* player_two_gui = new ofxDatGui(ofGetWidth() / 2, ofGetHeight() / 2);
	ofxDatGui* randomizer_gui = new ofxDatGui(ofxDatGuiAnchor::TOP_LEFT);
	ofxJSONElement result;

	void setupPlayer(int player_id);
	void setupRandomizer();

	void onLookupEvent(ofxDatGuiButtonEvent e);
	void onSliderEvent(ofxDatGuiSliderEvent e);
	void setRandomStats(int player_id);
	void onRandomizeEvent(ofxDatGuiButtonEvent e);

	int computePlayerTotal(int player_id);
};