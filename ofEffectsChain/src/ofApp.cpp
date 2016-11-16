#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    std::cout << "Testing:" << endl;
    effc.setup();
    effc.test();

    ofEnableDepthTest();
    //need this to bind textures
    ofDisableArbTex();

    input.setup(30,make_tuple(0.0,0.0),"input.bmp", ofColor(255,255,255));


}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushMatrix();

    ofBackgroundGradient(ofColor(212,200,200), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);

    //draw line that separates effects circle from menu
    ofPushMatrix(); 
        ofSetColor(255,255,255);
        ofDrawLine(0,  ofGetWindowHeight() * 0.8, ofGetWindowWidth(), ofGetWindowHeight() * 0.8);
    ofPopMatrix();

    //draw audio input at center
    ofPushMatrix();
        ofTranslate(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5);
        input.draw();
    ofPopMatrix();

    //draw audio effects in menu bar
    ofPushMatrix();
        ofTranslate(0, 0);
        effc.update();
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
    effc.checkIfEffectDragged(x,y);

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    effc.checkIfEffectSelected(x,y);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    effc.finalizePosition(x,y);
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
