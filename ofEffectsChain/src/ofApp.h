#pragma once

#include "ofMain.h"
#include "effect.h"
#include "circle.h"
#include "gun.h"
#include "laser.h"
#include<string>
#include<vector>
#include<map>
#include<tuple>
#include<iterator>

using namespace std;

class EffectsChain{
public:
    EffectsChain(){
        effects = new Effect[maxEffects];
        effectsName = new string[maxEffects];
        effectsName[0] = "Distortion";
        effectsName[1] = "Tremolo";
        effectsName[2] = "Delay";

    }
    ~EffectsChain(){
        delete [] effects;
        delete [] effectsName;
    }

    void setup(){

        effcRad = 30.0;

        map<const string, float> distParam;
        distParam["0-gain"] = 0.3;
        distParam["1-drive"] = 0.5;
        vecOfParamMaps.push_back(distParam);

        map<const string, float> tremParam;
        tremParam["0-gain"] = 0.3;
        tremParam["1-modFreq"] = 5;
        vecOfParamMaps.push_back(tremParam);

        map<const string, float> delayParam;
        delayParam["0-gain"] = 0.3;
        delayParam["1-feedback"] = 0.2;
        delayParam["2-delayMs"] = 10.0;
        vecOfParamMaps.push_back(delayParam);

        effects[0].setup(effectsName[0], "distortion.jpeg", false, effcRad,
                make_tuple(100.0, 500.0), distParam, ofColor(220,220,220));
        effects[1].setup(effectsName[1], "tremolo.jpg", false, effcRad,
                         make_tuple(200.0, 500.0), tremParam, ofColor(220,220,220));
        effects[2].setup(effectsName[2], "delay.jpg", false, effcRad,
                         make_tuple(300.0, 500.0), delayParam, ofColor(220,220,220));
        //load text font here
        myEffectLabel.load("pacifico/Pacifico.ttf", 12);


    }

    void test(){
        for(int i=0;i < maxEffects; i++){
            cout << effectsName[i] << " parameters :" << endl;
            map<const string, float> myparams = effects[i].getParamList();
             for(map<const string, float> :: iterator it = myparams.begin(); it!= myparams.end(); it++){
                 cout << it->first << " value: " << it->second << endl;
             }
        }
    }

    void update(){
        for(int i=0;i< maxEffects;i++){
            if(!effects[i].getIsPressed() && !effects[i].getOnOff()){
                //effect stays in menu-bar
                effects[i].setCenter(make_tuple((ofGetWindowWidth()+200)/maxEffects * i + 100.0, ofGetWindowHeight() * 0.84 + 50.0));
                //add labels to effects in menu bar
                ofPushStyle();
                    ofSetColor(200,200,200);
                    myEffectLabel.drawString(effectsName[i], (ofGetWindowWidth()+200)/maxEffects * i + 100.0 - effcRad + 5, ofGetWindowHeight() * 0.84 + 50.0 + effcRad + 25);
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
                effects[i].changeParameter();
                cout << "New coordinates:" << x << "," << y << endl;
                cout << get<0>(effects[i].getCenter()) << "," << get<1>(effects[i].getCenter()) << endl;
            }
        }
    }

    void finalizePosition(int x, int y){
        for(int i=0;i< maxEffects;i++){
            if(effects[i].getIsPressed()){
                effects[i].setOnOff(checkIfInsideMenu(i, y));
                effects[i].setIsPressed(false);
            }
        }
    }

    bool checkIfInsideMenu(int i,int mouseY){
        if(mouseY > 0.8*ofGetWindowHeight()){
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
    const int maxEffects = 3;
    string *effectsName;
    float effcRad;
    vector<map<const string, float>> vecOfParamMaps;
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
    void exit();

private:
    EffectsChain effc;
    ofImage sqImg;
    Circle input;
    Gun g;
    //laser beams need to be in a dynamic list
    vector<Laser> l;
};
