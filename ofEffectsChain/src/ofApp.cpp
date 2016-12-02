#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup(){

    //ofSetFullscreen(true);

    effc.setup();
    label.load("pacifico/Pacifico.ttf", 12);


    ofEnableDepthTest();
    //need this to bind textures
    ofDisableArbTex();

    input.setup(30,make_tuple(0.0,0.0),"input.bmp");
    g.setup("gun_texture.png",(0.85*ofGetWindowWidth())/2 + 0.15*ofGetWindowWidth(),0);

    effects = effc.getEffects();
    record = false;
    play = false;


    //audio initialisation

    lAudio.assign(MY_BUFFERSIZE, 0.0);
    rAudio.assign(MY_BUFFERSIZE, 0.0);
    audioBuffer = new float*[MY_OUT_CHANNELS];
    audioBuffer[0] = &lAudio[0];
    audioBuffer[1] = &rAudio[0];

    //setup sound stream
    soundStream.setup(this, MY_OUT_CHANNELS,MY_OUT_CHANNELS, MY_SRATE, MY_BUFFERSIZE, MY_NBUFFERS);
    //initialise Stk objects
    stk::Stk::setSampleRate((float)MY_SRATE);


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

    //setup osc receiver
    receiver.setup( PORT );
    thetaPrev = 0;
    thetaCur = 0;

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

    //check for OSC messages from Chuck
    while(receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(&m);

        if(m.getAddress() == "/ofEffects/game_track_values"){
            thetaPrev = thetaCur;
            xaxis = m.getArgAsFloat(0);
            yaxis = m.getArgAsFloat(1);
            zaxis = m.getArgAsFloat(2);

            //update gun's position
            //calculate elevation angle of spherical coordinate system
            thetaCur = atan2(yaxis, xaxis);
            cout << "Elevation angle :" << thetaCur*180/PI << endl;
            //this is just to make sure that at the beginning,
            //gun doesn't move due to initial value of theta.
            if(180/PI * abs(thetaCur - thetaPrev) > 1){
                float offset = 0;
                //if(thetaCur >= 0 && thetaCur <= PI)
                //move gun left or right depending on clockwise or anticlockwise
                //motion of gametrack
                if(thetaCur - thetaPrev > 0)
                    offset = 10;
                else
                    offset = -10;
                g.setOffset(g.getOffset() + offset);
            }

            //now see if foot pedal is pressed
            int footPedal = m.getArgAsInt(6);
            if(footPedal){
                g.setShootMode(true);
                Laser newLaser;
                newLaser.setup("laser_texture.png", g.getRad()/2, 50);
                l.push_back(newLaser);
                l.back().setPosition(g.getPosition());
            }
            else
                g.setShootMode(false);
        }
        else{
            cout << "Osc message not recognised." << endl;
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushMatrix();

    ofBackgroundGradient(ofColor(218,28,148), ofColor(0,0,0), OF_GRADIENT_CIRCULAR);

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

    //draw record button
    ofPushMatrix();
        ofFill();
        ofPushStyle();
        if(!record)
            ofSetColor(85,4,101);
        else{
            ofSetColor(38,29,40);
            ofPushStyle();
            ofSetColor(200,200,200);
            ofDrawBitmapString("Recording..", ofGetWindowWidth()*0.85/2 + 0.15*ofGetWindowWidth(),
                               0.95*ofGetWindowHeight());
            ofPopStyle();
        }
        ofDrawRectangle(0.15*ofGetWindowWidth() + 50, 20, 70, 30);

        ofSetColor(200,200,200);
        ofNoFill();
        ofDrawRectangle(0.15*ofGetWindowWidth() + 50, 20, 70, 30);
        ofPopStyle();

        ofPushStyle();
        ofSetColor(155,155,155);
        label.drawString("Record", 0.15*ofGetWindowWidth() + 60, 40);
        ofPopStyle();
    ofPopMatrix();

    //draw play button
    ofPushMatrix();
        ofFill();
        ofPushStyle();
        if(!play)
            ofSetColor(85,4,101);
        else{
            ofSetColor(38,29,40);
            ofPushStyle();
            ofSetColor(200,200,200);
            ofDrawBitmapString("Playing loop..", ofGetWindowWidth()*0.85/2 + 0.15*ofGetWindowWidth(),
                               0.95*ofGetWindowHeight());
            ofPopStyle();
        }

        ofDrawRectangle(ofGetWindowWidth() - 100, 20, 70, 30);

        ofSetColor(200,200,200);
        ofNoFill();
        ofDrawRectangle(ofGetWindowWidth() - 100, 20, 70, 30);
        ofPopStyle();

        ofPushStyle();
        ofSetColor(155,155,155);
        label.drawString("Play", ofGetWindowWidth() - 80, 40);
        ofPopStyle();
    ofPopMatrix();


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

    //check if record or play button is pressed
    if(x >= 0.15*ofGetWindowWidth() + 50 && x <= 0.15*ofGetWindowWidth() + 50 + 70 &&
            y >= 20 && y <= 50){

        //cannot record if playing
        if(!play){
            if(!record){
                cout << "recording" << endl;
                unsigned int nChannels = 2;
                rec.openFile(ofToDataPath("rec.wav", true), nChannels, stk::FileWrite::FILE_WAV, stk::Stk::STK_SINT16);
                record = true;
            }
            else{
                cout << "finished recording" << endl;
                rec.closeFile();
                record = false;
            }
        }
    }

    //check if play button is pressed
    if(x >= ofGetWindowWidth() - 100 && x <= ofGetWindowWidth() -100 + 70 &&
            y >= 20 && y <= 50){

        if(!record){
            if(!play){
                cout << "playing" << endl;
                //first make sure Stk doesn't throw any error because file to be played doesn't exist
                try{
                    loop.normalize(0.5);
                    loop.openFile(ofToDataPath("rec.wav",true));
                }
                catch(stk::StkError e){
                    cout << e.getMessage() << endl;
                    play = false;
                }
                play = true;
            }
            else{
                cout << "finished playing" << endl;      
                loop.closeFile();
                play = false;
            }
        }
    }

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
            rAudio[j] =  input[j*2];
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

    //play audio loop if button is pressed
    if(play){
        stk::StkFrames outframes(bufferSize,2);
        loop.tick(outframes);
        for (int i = 0; i < bufferSize; i++)
        {
            output[2*i] = 0.5*outframes(i,0) + lAudio[i]; //audioBuffer[0][i];
            output[2*i+1] = 0.5*outframes(i,1) + rAudio[i]; //audioBuffer[1][i];
        }
    }

    else{
        for (int i = 0; i < bufferSize; i++)
        {
            output[2*i] = lAudio[i]; //audioBuffer[0][i];
            output[2*i+1] = rAudio[i]; //audioBuffer[1][i];
        }
    }

    if(record){
        stk::StkFrames inframes(bufferSize,2);
        for (int i = 0; i < bufferSize; i++){
            inframes(i,0) = lAudio[i];
            inframes(i,1) =  rAudio[i];
        }

        rec.tick(inframes);
    }
}

void ofApp::exit(){
    delete []audioBuffer;
    delete []effects;
    soundStream.close();
    rec.closeFile();
    loop.closeFile();

    //delete wave file created
    /*try{
        remove("rec.wav");
    }
    catch(const char* e){
        cout << "File did not exist!" << endl;
    }*/
}
