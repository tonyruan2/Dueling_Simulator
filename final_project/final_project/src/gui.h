#pragma once

#include "ofMain.h"
#include "ofxDatGui.h"
#include "ofxJSON.h"

class Gui {

public:
	void setup();
	void draw();
	void onButtonEvent(ofxDatGuiButtonEvent e);

	ofxDatGui* gui = new ofxDatGui(0, ofGetHeight() / 2);
	//ofTrueTypeFont myFont;
	ofxJSONElement result;

};