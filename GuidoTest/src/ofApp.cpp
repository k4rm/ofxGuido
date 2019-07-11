/*
	ofxGuido example and test
	Dan Wilcox (c) ZKM | Hertz-Lab 2019

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License (Version 2),
	as published by the Free Software Foundation.
	A copy of the license can be found online at www.gnu.org/licenses.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.
*/
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(255);

	// create gudio instance with default settings
	guido = new ofxGuido();
	guido->setResize(true);

	// render simple score
	string gmn = "[ c d e f g ]";
	guido->loadString(gmn);
}

//--------------------------------------------------------------
void ofApp::update() {
	if(updateScore) {
		guido->loadString("[ " + score + " ]");
		updateScore = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	guido->draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::exit() {
	delete guido;
}

//--------------------------------------------------------------
// add or clear existing scrore
void ofApp::keyPressed(int key) {
	if(key >= 'a' && key <= 'g') {
		score += " ";
		score += (char)key;
		updateScore = true;
	}
	else if(key >= 'A' && key <= 'G') {
		score += " ";
		score += (char)(key + 32); // shift to lowercase
		score += "2*1/2"; // 2nd octave half notes
		updateScore = true;
	}
	else if(key == ' ' ) {
		score = "";
		guido->clear();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ) {

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
	guido->setSize(w, h);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
