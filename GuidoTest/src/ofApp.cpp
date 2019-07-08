#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofBackground(255);

	// create gudio instance with default settings
	GuidoLayoutSettings layoutSettings;
	GuidoGetDefaultLayoutSettings(&layoutSettings);
	guido = new ofxGuido(layoutSettings);

	// render simple score
	string gmn = "[ c d e f g ]";
	guido->compile_string(gmn);

	// size to fit
	guido->guido->setResizePageToMusic(true);

	// // make wide "page"
	// GuidoPageFormat format;
	// GuidoGetDefaultPageFormat(&format);
	// format.width = 5000;
	// format.height = 1000;
	// GuidoSetDefaultPageFormat(&format);

	// // check page format size
	// GuidoGetPageFormat(guido->guido->fGRHandler, 0, &format);
	// ofLog() << "page w: " << format.width << " h: " << format.height;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
	guido->draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::exit(){
	delete guido;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
