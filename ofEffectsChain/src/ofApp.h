#pragma once

#include "ofMain.h"
#include "effect.h"
#include "circle.h"
#include "faust/Distortion.h"
#include "faust/Tremolo.h"
#include "faust/Delay.h"
#include "faust/Phaser.h"
#include "faust/Wah.h"
#include "faust/Flanger.h"
#include "faust/Chorus.h"
#include "gun.h"
#include "laser.h"
#include<string>
#include<vector>
#include<map>
#include<tuple>
#include<iterator>

using namespace std;

//-----------------------------------------------------------------------------
// Preprocessor definitions
//-----------------------------------------------------------------------------
#define MY_SRATE         48000           // sample rate
#define MY_CHANNELS      2                // number of channels
#define MY_BUFFERSIZE    256              // number of frames in a buffer
#define MY_NBUFFERS      2                // number of buffers latency

class EffectsChain{
public:
    EffectsChain(){
        effects = new Effect[maxEffects];
        effectsName = new string[maxEffects];
        effectsName[0] = "Distortion";
        effectsName[1] = "Tremolo";
        effectsName[2] = "Delay";
        effectsName[3] = "Phaser";
        effectsName[4] = "Wah";
        effectsName[5] = "Flanger";
        effectsName[6] = "Chorus";

    }
    ~EffectsChain(){
        delete [] effects;
        delete [] effectsName;
    }

    void setup(){

        effcRad = 30.0;

        map<const string, float> distParam;
        distParam["0-drive"] = 0.5;
        distParam["1-gain"] = 0.3; //distortion is too loud otherwise

        map<const string, float> tremParam;
        tremParam["0-frequency"] = 5;
        tremParam["1-gain"] = 0.8; //maxEffects;
        tremParam["2-depth"] = 1.0;

        map<const string, float> delayParam;
        delayParam["0-feedback"] = 0.2;
        delayParam["1-gain"] = 0.8; //maxEffects;
        delayParam["2-delayMs"] = 0.5;

        map<const string, float> phaseParam;
        phaseParam["0-speed"] = 0.5;
        phaseParam["1-gain"] = 0.8; //maxEffects;

        map<const string, float> wahParam;
        wahParam["0-level"] = 0.5;
        wahParam["1-gain"] = 0.8; //maxEffects;

        map<const string, float> flangeParam;
        flangeParam["0-maxDelay"] = 10;
        flangeParam["1-gain"] = 0.8;
        flangeParam["2-level"] = 0.5;

        map<const string, float> chorusParam;
        chorusParam["0-freq"] = 2;
        chorusParam["1-gain"] = 0.8;
        chorusParam["2-depth"] = 0.5;

        effects[0].setup(effectsName[0], "distortion.jpeg", false, effcRad,
                        distParam, make_tuple(0.0,1.0));
        effects[1].setup(effectsName[1], "tremolo.jpg", false, effcRad,
                         tremParam, make_tuple(0.1,15.0));
        effects[2].setup(effectsName[2], "delay.jpg", false, effcRad,
                         delayParam, make_tuple(0.0,1.0));
        effects[3].setup(effectsName[3], "phaser.jpg", false, effcRad,
                         phaseParam, make_tuple(0.0, 10.0));
        effects[4].setup(effectsName[4], "wah.jpg", false, effcRad,
                         wahParam, make_tuple(0.4, 1.0));
        effects[5].setup(effectsName[5], "flanger.jpg", false, effcRad,
                         flangeParam, make_tuple(0, 20));
        effects[6].setup(effectsName[6], "chorus.jpg", false, effcRad,
                         chorusParam, make_tuple(0, 10));

        //load text font here
        myEffectLabel.load("pacifico/Pacifico.ttf", 12);


    }


    //return array of all effects
    Effect* getEffects(){
        return effects;
    }

    void update(){
        for(int i=0;i< maxEffects;i++){
            if(!effects[i].getIsPressed() && !effects[i].getOnOff()){
                //effect stays in menu-bar
                effects[i].setCenter(make_tuple(0.08*ofGetWindowWidth(), ofGetWindowHeight()/maxEffects * i + 35));
                //add labels to effects in menu bar
                ofPushStyle();
                    ofSetColor(200,200,200);
                    myEffectLabel.drawString(effectsName[i], 0.08*ofGetWindowWidth()- effcRad + 5, ofGetWindowHeight()/maxEffects * i + 35 + effcRad + 25);
                ofPopStyle();
            }     
            else if(!effects[i].getIsPressed() && effects[i].getOnOff()){
                //make effects move in a circle
                effects[i].moveCenterInCircle();

            }
            effects[i].draw();
        }
    }


    void checkIfEffectSelected(int x, int y){
        float limitx, limity, r;
        cout << "Mouse pressed" << endl;
        for(int i=0;i< maxEffects;i++){
            limitx = get<0>(effects[i].getCenter());
            limity = get<1>(effects[i].getCenter());
            r = effects[i].getRadius();
            if(x > limitx-r && x < limitx + r
                    && y > limity-r && y < limity + r){
                cout << effects[i].getName() << " is  selected" <<endl;
                effects[i].setIsPressed(true);
            }
        }

    }

    void checkIfEffectDragged(int x, int y){
        for(int i=0;i< maxEffects;i++){
            if(effects[i].getIsPressed()){
                cout << effects[i].getName() << " is pressed:" << effects[i].getIsPressed() << endl;
                effects[i].setCenter(make_tuple((float)x,(float)y));
                effects[i].setPathRadius();
                cout << "New coordinates:" << x << "," << y << endl;
                cout << get<0>(effects[i].getCenter()) << "," << get<1>(effects[i].getCenter()) << endl;
            }
        }
    }

    void finalizePosition(){
        for(int i=0;i< maxEffects;i++){
            if(effects[i].getIsPressed()){
                effects[i].setOnOff(checkIfInsideMenu(i));
                effects[i].setIsPressed(false);
                //this needs to be called to set effect parameter value according to new radius
                effects[i].changeParameter();
            }
        }
    }


    bool checkIfInsideMenu(int i){
        if(get<0>(effects[i].getCenter()) - effects[i].getRadius() < 0.15*ofGetWindowWidth()){
           cout << effects[i].getName() << " should be turned off" << endl;
           return false;
        }
        else{
            cout << effects[i].getName() << " should be turned on" << endl;
            return true;
        }
    }

    bool isHitByLaser(tuple <float, float> laserPos){
        for(int i=0;i< maxEffects;i++){
            if(effects[i].checkIfHit(laserPos)){
                cout << effectsName[i] << " was hit by laser." << endl;
                effects[i].randomiseCirclePath();
                //if this function is not called,
                //then change in orbital radius won't take place
                effects[i].setPathRadius();
                effects[i].changeParameter();
                return true;
            }
        }
        return false;
    }


private:
    Effect *effects;
    const int maxEffects = 7;
    string *effectsName;
    float effcRad;
    ofTrueTypeFont myEffectLabel;

};

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    void audioIn(float *input, int bufferSize, int nChannels);
    void audioOut(float *output, int bufferSize, int nChannels);
    void exit();

private:
    EffectsChain effc;
    ofImage sqImg;
    Circle input;
    Gun g;
    //laser beams need to be in a dynamic list
    vector<Laser> l;
    ofTrueTypeFont label;
    Effect *effects;

    //all audio objects
    vector<float> lAudio;
    vector<float> rAudio;

    ofSoundStream soundStream;
    float **audioBuffer;
    ofMutex myMutex;

    //All faust objects - effects + UI
    Distortion dist;
    MapUI distControl;

    Tremolo trem;
    MapUI tremControl;

    Delay del;
    MapUI delayControl;

    Phaser phase;
    MapUI phaseControl;

    Wah wah;
    MapUI wahControl;

    Flanger flange;
    MapUI flangeControl;

    Chorus chor;
    MapUI chorControl;

};


//============================================================================================================
//============================================================================================================

int main( ){
    ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());

}

