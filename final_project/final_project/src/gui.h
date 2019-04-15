#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxJSON.h"

class Gui {

public:
	void setup(int player_id);
	void draw();
	void onButtonEvent(ofxDatGuiButtonEvent e);

	ofxDatGui* player_one_gui = new ofxDatGui(0, ofGetHeight() / 2);
	ofxDatGui* player_two_gui = new ofxDatGui(ofGetWidth() / 2, ofGetHeight() / 2);
	//ofTrueTypeFont myFont;
	ofxJSONElement result;

};