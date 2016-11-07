#pragma once

#include "ofMain.h"
#include "planet.h"
#include "ofxFft.h"
#include "ofxStk.h"

//-----------------------------------------------------------------------------
// Preprocessor definitions
//-----------------------------------------------------------------------------
#define MY_SRATE         44100           // sample rate
#define MY_CHANNELS      2                // number of channels
#define MY_BUFFERHISTORY 50               // number of buffers to save
#define MY_BUFFERSIZE    512              // number of frames in a buffer
#define MY_NBUFFERS      2                // number of buffers latency

//create class SolarSystem
class SolarSystem{
public:
    SolarSystem(){
        earthRad = 20.0;
        earthSemiMajor = 250;
        earthOrbitAng = 1;
        planet = new Planet[nPlanets];
    }
    ~SolarSystem(){
        delete [] planet;
    }

    void setup(){

        //create sun
        sun.setRadius(5*earthRad);
        sunRotationAngle = 0.0;
        sunTexture.loadImage("sunTexture.png");

        //create planets
        //mercury
        planet[0].setup("Mercury", "mercurymap.jpg", 0.3 * earthRad, 0.5871 * earthSemiMajor, 0.206);
        //venus radius
        planet[1].setup("Venus", "VenusTexture.jpg", 0.9 * earthRad, 0.7233 * earthSemiMajor, 0.007);
        //earth radius
        planet[2].setup("Earth", "EarthTexture.jpg", earthRad, earthSemiMajor, 0.017);
        //mars radius
        planet[3].setup("Mars", "MarsTexture.jpg", 0.5 * earthRad, 1.223 * earthSemiMajor, 0.093);
        //jupiter radius
        planet[4].setup("Jupiter", "JupiterTexture.jpg", 2.5 * earthRad, 1.6028 * earthSemiMajor, 0.048);
        //saturn radius
        planet[5].setup("Saturn", "SaturnTexture.jpg", 2 * earthRad, 2.0388 * earthSemiMajor, 0.56);
        //uranus radius
        planet[6].setup("Uranus", "UranusTexture.jpg", 1.5 * earthRad, 2.3914 * earthSemiMajor, 0.76);
        //nepture radius
        planet[7].setup("Neptune", "NeptuneTexture.jpg",1.25 * earthRad, 2.70611 * earthSemiMajor, 0.80);

    }

    void update(float* orbSpeed){
        sunRotationAngle += 0.1;
        for(int i = 0; i < nPlanets;i++){
            planet[i].setRotationAngle(1.2);
        }

        //keep speed of rotation fixed as of now
        planet[0].update(4.14 * (earthOrbitAng + orbSpeed[0]));
        planet[1].update(1.63 * (earthOrbitAng + orbSpeed[1]));
        planet[2].update(1.0 * (earthOrbitAng + orbSpeed[2]));
        planet[3].update(0.531 * (earthOrbitAng + orbSpeed[3]));
        planet[4].update(0.084 * (earthOrbitAng + orbSpeed[4]));
        planet[5].update(0.034 * (earthOrbitAng + orbSpeed[5]));
        planet[6].update(0.012 * (earthOrbitAng + orbSpeed[6]));
        planet[7].update(0.004 * (earthOrbitAng + orbSpeed[7]));


    }

    void draw(){

        ofPushMatrix();
            //draw the sun
            ofPushMatrix();
                ofRotateZ( sunRotationAngle );
                ofPushStyle(); // Save initial style state
                    //bind sun texture to sphere object
                    sunTexture.bind();
                    // Draw the sun
                    sun.draw();
                ofPopStyle(); // Back to initial style state
            ofPopMatrix();

            //draw planets
            for(int i =0; i< nPlanets;i++)
                planet[i].draw();

        ofPopMatrix();
    }

    const int getNplanets(){
        return nPlanets;
    }


private:
    //Member variables
    ofSpherePrimitive sun;
    float sunRotationAngle;
    const int nPlanets = 8;
    Planet *planet;
    float earthRad;
    float earthSemiMajor;
    float earthOrbitAng;
    ofImage sunTexture;
};



class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    //add this explicitly to destroy objects
    void exit();

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

    // Get audio input samples
    void audioIn(float * input, int bufferSize, int nChannels);
    // Send audio output samples
    void audioOut(float *output, int bufferSize, int nChannels);


private:
    SolarSystem s;
    ofEasyCam cam;

    //soundStream object
    ofSoundStream soundStream;

    //vectors for left and right channel waveforms
    vector<float> left, right;

    //FFT object
    ofxFft* fft;

    //soundPlayer object to play space sounds
    //ofSoundPlayer* snd;

    //Stk FileWvIn array
    stk::FileWvIn* snd;

    //keep track of which wave file is being played
    int whichFile;


};
