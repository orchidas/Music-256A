#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFullscreen(true);

    std::cout << "Testing:" << endl;
    effc.setup();
    effc.test();
    label.load("pacifico/Pacifico.ttf", 18);


    ofEnableDepthTest();
    //need this to bind textures
    ofDisableArbTex();

    input.setup(30,make_tuple(0.0,0.0),"input.bmp", ofColor(255,255,255));
    g.setup("gun_texture.png",(0.85*ofGetWindowWidth())/2 + 0.15*ofGetWindowWidth(),0);


}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i = 0; i < l.size(); i++){
        //check if any effect is hit by laser beam,
        //then a)effect should change radius b)laser beam should disappear
        if(effc.isHitByLaser(l[i].getPosition())){
            l[i].setShoot(false);
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushMatrix();

    ofBackgroundGradient(ofColor(218,28,148), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);
    ofPushStyle();
        ofSetColor(220,220,220);
        label.drawString("Effects", 0.08*ofGetWindowWidth() - 35, 50.0);
    ofPopStyle();

    //draw line that separates effects circle from menu
    ofPushMatrix(); 
        ofSetColor(255,255,255);
        ofDrawLine(ofGetWindowWidth() * 0.15, 0, ofGetWindowWidth() * 0.15, ofGetWindowHeight());
        ofPushStyle();
        ofSetColor(0,0,0, 127);
        ofDrawRectangle(0, 0, ofGetWindowWidth() * 0.15, ofGetWindowHeight());
        ofPopStyle();
    ofPopMatrix();

    //draw audio input at center
    ofPushMatrix();
        ofTranslate((0.85*ofGetWindowWidth())/2 + 0.15*ofGetWindowWidth(), ofGetWindowHeight()*0.5);
        input.draw();
    ofPopMatrix();

    //draw audio effects in menu bar
    ofPushMatrix();
        ofTranslate(0, 0);
        effc.update();
    ofPopMatrix();

    //draw laser gun
    ofPushMatrix();
        g.draw();
    ofPopMatrix();

    //draw laser beams
    for(int i = 0; i < l.size(); i++){
        if(l[i].getShoot() == true){
            l[i].update();
            l[i].draw();
        }
        else{
            //delete ith element in vector, i.e, ith laser beam
            //that has crossed the entire screen
            l.erase(l.begin() + i);
        }
    }


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //use arrow keys to control gun position along x axis
    if (key == 'a'){
        float offset = g.getOffset();
        g.setOffset(offset - 10);
    }
    else if(key == 'd'){
        float offset = g.getOffset();
        g.setOffset(offset + 10);
    }
    else if(key == 's'){
        g.setShootMode(true);
        Laser newLaser;
        newLaser.setup("laser_texture.png", g.getRad()/2, 50);
        l.push_back(newLaser);
        l.back().setPosition(g.getPosition());
    }

}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(key == 's')
        g.setShootMode(false);

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
    g.setPosition((0.85*w/2) + 0.15*w,0);
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::exit(){}
