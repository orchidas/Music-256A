#ifndef EFFECT_H
#define EFFECT_H


#include"parameter.h"
#include"circle.h"
#include<string>
#include<tuple>
#include<map>
#include<iterator>

using namespace std;

class Effect{
    //this class contains all effect controllers and parameters
public:
    Effect(){
        param = new Parameter();
    }
    ~Effect(){
        delete param;
    }

    void setup(string &n,const string &p_img, bool on, float r, tuple<float,float> centerCoordinates,
              map<const string, float> parameters){
        setName(n);
        setOnOff(on);
        setIsPressed(false);
        createParameterList(parameters);
        circle.setup(r, centerCoordinates, p_img, ofColor(236,236,236));

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
            param->setParam(it->first, it->second);
        }
    }

    void setParam(const string &n, float val){
        param->setParam(n, val);
    }

    float getParamValue(const string &n){
        return param->getParamValue(n);
    }

    map<const string, float> getParamList(){
        return param->getParamList();
    }

    void setCenter(tuple<float,float> center){
        circle.setCenter(center);
    }

    tuple<float, float> getCenter(){
        return circle.getCenter();
    }


    void update(float){ }
    void draw(){
        circle.draw();
    }



private:
    string name;
    bool on;
    //param should be a separate class
    Parameter *param;
    //so should circle
    Circle circle;
    bool isPressed;

};


#endif // EFFECT_H
