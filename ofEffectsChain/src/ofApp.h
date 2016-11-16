#pragma once

#include "ofMain.h"
#include "effect.h"
#include "circle.h"
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
        effectsName[0] = "distortion";
        effectsName[1] = "tremolo";
        effectsName[2] = "delay";

    }
    ~EffectsChain(){
        delete [] effects;
        delete [] effectsName;
    }

    void setup(){

        map<const string, float> distParam;
        distParam["gain"] = 0.3;
        distParam["drive"] = 0.5;
        vecOfParamMaps.push_back(distParam);

        map<const string, float> tremParam;
        tremParam["gain"] = 0.3;
        tremParam["modFreq"] = 5;
        vecOfParamMaps.push_back(tremParam);

        map<const string, float> delayParam;
        delayParam["gain"] = 0.3;
        delayParam["feedback"] = 0.2;
        delayParam["delayMs"] = 10.0;
        vecOfParamMaps.push_back(delayParam);

        effects[0].setup(effectsName[0], "distortion.jpeg", false, 50.0,
                make_tuple(100.0, 500.0), distParam);
        effects[1].setup(effectsName[1], "tremolo.jpg", false, 50.0,
                         make_tuple(200.0, 500.0), tremParam);
        effects[2].setup(effectsName[2], "delay.jpg", false, 50.0,
                         make_tuple(300.0, 500.0), delayParam);



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
                effects[i].setCenter(make_tuple((ofGetWindowWidth()+200)/maxEffects * i + 100.0, ofGetWindowHeight() * 0.85 + 50.0));
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
                cout << "New coordintates:" << x << "," << y << endl;
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



private:
    Effect *effects;
    const int maxEffects = 3;
    string *effectsName;
    vector<map<const string, float>> vecOfParamMaps;



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

private:
    EffectsChain effc;
    ofImage sqImg;
    Circle input;
};
