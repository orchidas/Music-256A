// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple sine wave oscillator

#ifndef SINE_H_INCLUDED
#define SINE_H_INCLUDED


class Sine{
private:
    double currentSampleRate, currentAngle, angleDelta;
    const double PI = 3.1415926535897931;
    
public:
    Sine():currentSampleRate(0.0),currentAngle(0.0),angleDelta(0.0){}
    
    ~Sine(){}
    
    // sampling rate must be set to get an accurate frequency
    void setSamplingRate(int samplingRate){
        currentSampleRate = samplingRate;
    }
    
    int getSamplingRate(){
        return currentSampleRate;
    }
    
    void setFrequency(float frequency){
        const double cyclesPerSample = frequency / currentSampleRate;
        angleDelta = cyclesPerSample * 2.0 * PI;
    }
    
    // compute one sample
    double tick(){
        const float currentSample = (double) std::sin (currentAngle); //sine function in STD
        currentAngle += angleDelta;
        return currentSample;
    }
};


#endif  // SINE_H_INCLUDED
