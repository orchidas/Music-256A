#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Make the app full screen
    ofSetFullscreen(true);
    //need this to bind texture around sphere
    ofDisableArbTex();

    // Turn on depth testing so rendering happens according to z-depth rather
    // than draw order.
    ofEnableDepthTest();

    // Black background
    ofBackground(0);

    //setup solar system
    s.setup();

    //setup easyCam for zooming and moving perspective
    cam.setDistance(700);
}

//--------------------------------------------------------------
void ofApp::update(){
    s.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5);
    cam.begin();
    s.draw();
    cam.end();
    ofPopMatrix();

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
