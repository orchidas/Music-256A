#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){

    //ofSetFullscreen(true);

    std::cout << "Testing:" << endl;
    effc.setup();
    label.load("pacifico/Pacifico.ttf", 18);


    ofEnableDepthTest();
    //need this to bind textures
    ofDisableArbTex();

    input.setup(30,make_tuple(0.0,0.0),"input.bmp");
    g.setup("gun_texture.png",(0.85*ofGetWindowWidth())/2 + 0.15*ofGetWindowWidth(),0);

    effects = effc.getEffects();

    //audio initialisation

    lAudio.assign(MY_BUFFERSIZE, 0.0);
    rAudio.assign(MY_BUFFERSIZE, 0.0);
    audioBuffer = new float*[2];
    audioBuffer[0] = &lAudio[0];
    audioBuffer[1] = &rAudio[0];

    //setup sound stream
    soundStream.setup(this, MY_CHANNELS, MY_CHANNELS, MY_SRATE, MY_BUFFERSIZE, MY_NBUFFERS);

    dist.init(MY_SRATE);
    dist.buildUserInterface(&distControl);
    distControl.setParamValue("/distortion/gain", effects[0].getParamValue("1-gain"));
    distControl.setParamValue("/distotion/gate", 0);

    trem.init(MY_SRATE);
    trem.buildUserInterface(&tremControl);
    tremControl.setParamValue("/tremolo/gain", effects[1].getParamValue("1-gain"));
    tremControl.setParamValue("/tremolo/gate", 0);
    tremControl.setParamValue("/tremolo/depth", effects[1].getParamValue("2-depth"));

    del.init(MY_SRATE);
    del.buildUserInterface(&delayControl);
    delayControl.setParamValue("/delay/gain", effects[2].getParamValue("1-gain"));
    delayControl.setParamValue("/delay/gate", 0);
    delayControl.setParamValue("/delay/duration", effects[2].getParamValue("2-delayMs"));

    phase.init(MY_SRATE);
    phase.buildUserInterface(&phaseControl);
    phaseControl.setParamValue("/phaser/gain", effects[3].getParamValue("1-gain"));
    phaseControl.setParamValue("/phaser/gate", 0);

    wah.init(MY_SRATE);
    wah.buildUserInterface(&wahControl);
    wahControl.setParamValue("/wah/gain", effects[4].getParamValue("1-gain"));
    wahControl.setParamValue("/wah/gate", 0);

    flange.init(MY_SRATE);
    flange.buildUserInterface(&flangeControl);
    flangeControl.setParamValue("/flanger/gain", effects[5].getParamValue("1-gain"));
    flangeControl.setParamValue("/flanger/gate", 0);
    flangeControl.setParamValue("/flanger/depth", effects[5].getParamValue("2-level"));

    chor.init(MY_SRATE);
    chor.buildUserInterface(&chorControl);
    chorControl.setParamValue("/chorus/gain", effects[6].getParamValue("1-gain"));
    chorControl.setParamValue("/chorus/gate", 0);
    chorControl.setParamValue("/chorus/level", effects[6].getParamValue("2-depth"));


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

    distControl.setParamValue("/distortion/drive", effects[0].getParamValue("0-drive"));
    tremControl.setParamValue("/tremolo/frequency", effects[1].getParamValue("0-frequency"));
    delayControl.setParamValue("/delay/feedback", effects[2].getParamValue("0-feedback"));
    phaseControl.setParamValue("/phase/speed", effects[3].getParamValue("0-speed"));
    wahControl.setParamValue("/wah/level", effects[4].getParamValue("0-level"));
    flangeControl.setParamValue("/flanger/maxDelay", effects[5].getParamValue("0-maxDelay"));
    chorControl.setParamValue("/chorus/freq", effects[6].getParamValue("0-freq"));

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushMatrix();

    ofBackgroundGradient(ofColor(218,28,148), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);
    /*ofPushStyle();
        ofSetColor(220,220,220);
        label.drawString("Effects", 0.08*ofGetWindowWidth() - 35, 30.0);
    ofPopStyle();*/

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

    effc.finalizePosition();

    distControl.setParamValue("/distortion/gate", effects[0].getOnOff());
    tremControl.setParamValue("/tremolo/gate", effects[1].getOnOff());
    delayControl.setParamValue("/delay/gate", effects[2].getOnOff());
    phaseControl.setParamValue("/phaser/gate", effects[3].getOnOff());
    wahControl.setParamValue("/wah/gate", effects[4].getOnOff());
    flangeControl.setParamValue("/flanger/gate",effects[5].getOnOff());
    chorControl.setParamValue("/chorus/gate",effects[6].getOnOff());

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

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    for (int j = 0; j < bufferSize; j++){
            lAudio[j] = input[j*2];
            rAudio[j] =  input[j*2+1];
    }
}

void ofApp::audioOut(float * output, int bufferSize, int nChannels){

    //keep audio thread seperate from graphics thread
    myMutex.lock();
    if(distControl.getParamValue("/distortion/gate") == 1)
        dist.compute(bufferSize, audioBuffer, audioBuffer);

    if(tremControl.getParamValue("/tremolo/gate") == 1)
        trem.compute(bufferSize, audioBuffer, audioBuffer);

    if(delayControl.getParamValue("/delay/gate") == 1)
        del.compute(bufferSize, audioBuffer, audioBuffer);

    if(phaseControl.getParamValue("/phaser/gate") == 1)
        phase.compute(bufferSize, audioBuffer, audioBuffer);

    if(wahControl.getParamValue("/wah/gate") == 1)
        wah.compute(bufferSize, audioBuffer, audioBuffer);

    if(flangeControl.getParamValue("/flanger/gate") == 1)
        flange.compute(bufferSize, audioBuffer, audioBuffer);

    if(chorControl.getParamValue("/chorus/gate") == 1)
        chor.compute(bufferSize, audioBuffer, audioBuffer);

    myMutex.unlock();

    // Interleave the output buffer
        for (int i = 0; i < bufferSize; i++)
        {
            output[2*i] = lAudio[i]; //audioBuffer[0][i];
            output[2*i+1] = rAudio[i]; //audioBuffer[1][i];
        }
}

void ofApp::exit(){
    delete []audioBuffer;
    delete []effects;
    soundStream.close();
}
