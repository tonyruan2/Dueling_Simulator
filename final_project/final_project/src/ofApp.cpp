#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofxDatGuiLog::quiet();
	ofSetWindowTitle("Old School RuneScape Duel Simulator");
	gui_.setup();
	background_.load("background_solid.png");
	gui_.duel_.simulation_.setup();
	ofSetFrameRate(30);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	background_.draw(0, 0, ofGetWidth(), ofGetHeight());
	if (should_draw_) {
		gui_.duel_.simulation_.draw();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	//Pause displaying the simulation for improvements in performance.
	if (key == OF_KEY_F5) {
		should_draw_ = !should_draw_;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}