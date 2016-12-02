#ifndef EFFECT_H
#define EFFECT_H


#include "parameter.h"
#include "circle.h"
#include<string>
#include<tuple>
#include<map>
#include<iterator>
#include<vector>
#include "math.h"
//for random number generation
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Effect{
    //this class contains all effect controllers and parameters
public:
    Effect(){
        pathRad = 0.0;
        pathAng = 0.0;
        isHit = false;
    }
    ~Effect(){}

    void setup(string &n,const string &p_img, bool on, float r,
              map<const string, float> parameters, tuple<float,float> minMaxofParam){
        setName(n);
        setOnOff(on);
        setIsPressed(false);
        createParameterList(parameters);
        circle.setup(r, make_tuple(0.0,0.0), p_img);
        param.setMaxMinValues(minMaxofParam);

    }


    void setName( string &n){
        name = n;
    }

    string & getName(){
        return name;
    }

    void setOnOff(bool byp){
        on = byp;
    }


    bool checkIfHit(tuple <float, float> laserPos){
        float lx = get<0>(laserPos);
        float ly = get<1>(laserPos);
        float centerX = get<0>(circle.getCenter());
        float centerY = get<1>(circle.getCenter());
        //check it laser position is inside circle
        float d = sqrt(pow((centerX - lx),2) + pow((centerY - ly),2));
        if(d <= circle.getRadius())
            isHit = true;
        else
            isHit = false;
        return isHit;
    }

    bool getIsHit(){
        return isHit;
    }

    void setPathRadius(){
        float x = (0.85*ofGetWindowWidth()/2 + 0.15*ofGetWindowWidth()) - get<0>(circle.getCenter());
        float y = (0.5*ofGetWindowHeight()) - get<1>(circle.getCenter());
        pathRad = sqrt(pow(x,2) + pow(y, 2));
        //make sure radius doesn't exceed screen size
        if(pathRad > ofGetWindowHeight()/2)
            pathRad = ofGetWindowHeight()/2 - circle.getRadius();
        //or doesn't crash into input
        else if(pathRad <= circle.getRadius()){
            pathRad = circle.getRadius()+20;
        }
        //dunno why but only negating both x and y gives right angle
        pathAng = atan2(-y,-x);
    }

    float getPathRad(){
        return pathRad;
    }

    void moveCenterInCircle(){
        float centerX = (0.85*ofGetWindowWidth()/2 + 0.15*ofGetWindowWidth()) + pathRad * cos(pathAng);
        float centerY = (0.5*ofGetWindowHeight()) + pathRad * sin(pathAng);
        pathAng +=  0.01;
        circle.setCenter(make_tuple(centerX, centerY));
    }

    void randomiseCirclePath(){
        float centerX = get<0>(circle.getCenter());
        float centerY = get<1>(circle.getCenter());
        //initialise seed
        srand (time(NULL));
        //make sure the new radius stays withing screen limits
        float newX = rand() % 100 - 100;
        float newY = rand() % 100 - 100;
        circle.setCenter(make_tuple(centerX + newX, centerY+newY));
        setPathRadius();
        cout << "New path radius : " << pathRad << endl;
    }

    void changeParameter(){
        string keyList;
        map<const string, float> paramList = getParamList();
        map<const string, float> :: iterator it = paramList.begin();
        keyList = it->first;
        setParam(keyList, ofMap(pathRad, 30, 500, get<0>(param.getMinMaxValues()) ,get<1>(param.getMinMaxValues())));
        cout << keyList << " new value:" << getParamValue(keyList) << endl;
    }


    bool getOnOff(){
        return on;
    }

    void setIsPressed(bool p){
        isPressed = p;
    }

    bool getIsPressed(){
        return isPressed;
    }

    void setRadius(float r){
        circle.setRadius(r);
    }

    float getRadius(){
        return circle.getRadius();
    }


    void createParameterList(map<const string, float> parameters) {
        for(map<const string, float> :: iterator it = parameters.begin(); it!= parameters.end(); it++){
            param.setParam(it->first, it->second);
        }
    }

    void setParam(const string &n, float val){
        param.setParam(n, val);
    }

    float getParamValue(const string &n){
        return param.getParamValue(n);
    }

    map<const string, float> getParamList(){
        return param.getParamList();
    }

    void setCenter(tuple<float,float> center){
        circle.setCenter(center);
    }

    tuple<float, float> getCenter(){
        return circle.getCenter();
    }

    void update(){ }
    void draw(){

        //draw orbit path
        if(on){
        ofPushMatrix();
        ofTranslate(0.85*ofGetWindowWidth()/2 + (0.15*ofGetWindowWidth()), ofGetWindowHeight()*0.5);
        ofPushStyle();
            ofNoFill();
            ofSetLineWidth(2);
            ofSetColor(107,12,55, 100);
            ofSetCircleResolution(100);
            ofDrawCircle(0,0,pathRad);
        ofPopStyle();
        ofPopMatrix();
        }

        //draw rotating effects
        circle.draw();

    }



private:
    string name;
    bool on;
    //param should be a separate class
    Parameter param;
    //so should circle
    Circle circle;
    bool isPressed;
    float pathRad;
    float pathAng;
    bool isHit;

};


#endif // EFFECT_H
