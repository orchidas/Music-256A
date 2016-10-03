// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Romain Michon (rmichonATccrmaDOTstanfordDOTedu)
// Description: Simple JUCE sine wave synthesizer

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include <cmath>

class MainContentComponent :
    //this class extends the following classes
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener
    
{
public:
    
    //constructor
    MainContentComponent() : samplingRate(0.0)
    {
        numSine = 6;
        osc = new Sine[numSine];
        frequencySlider = new Slider[numSine];
        gainSlider = new Slider[numSine];
        onOffButton = new ToggleButton[numSine];
        frequencyLabel = new Label[numSine];
        gainLabel = new Label[numSine];
        onOffLabel = new Label[numSine];
        onOff = new int[numSine];
        gain = new float[numSine];
        
        
        for(int n = 0; n < numSine; n++){
        
            // configuring frequency slider and adding it to the main window
        addAndMakeVisible (frequencySlider[n]);
        frequencySlider[n].setRange (50.0, 5000.0);
        frequencySlider[n].setSkewFactorFromMidPoint (500.0);
        frequencySlider[n].setValue(1000); // will also set the default frequency of the sine osc
        frequencySlider[n].addListener (this);
        
        // configuring frequency label box and adding it to the main window
        addAndMakeVisible(frequencyLabel[n]);
        frequencyLabel[n].setText ("Frequency", dontSendNotification);
        frequencyLabel[n].attachToComponent (&frequencySlider[n], true);
        
        
        // configuring gain slider and adding it to the main window
        addAndMakeVisible (gainSlider[n]);
        gainSlider[n].setRange (0.0, (1.0 / numSine));
        gainSlider[n].setValue(0.5 * (1.0 / numSine)); 
        gainSlider[n].addListener (this);
        
        
        // configuring gain label and adding it to the main window
        addAndMakeVisible(gainLabel[n]);
        gainLabel[n].setText ("Gain", dontSendNotification);
        gainLabel[n].attachToComponent (&gainSlider[n], true);
        
        
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(onOffButton[n]);
        onOffButton[n].addListener(this);
        
        
        // configuring on/off label and adding it to the main window
        addAndMakeVisible(onOffLabel[n]);
        onOffLabel[n].setText ("On/Off", dontSendNotification);
        onOffLabel[n].attachToComponent (&onOffButton[n], true);
        
        gain[n] = 0.0;
        onOff[n] = 0;
        }
        
        //add button to change state of sine waves
        addAndMakeVisible(changeStateButton);
        changeStateButton.setButtonText("Change State");
        //changeStateButton.changeWidthToFitText();
        changeStateButton.addListener(this);
        
        setSize (600, 100);
        nChans = numSine; //number of output channels
        setAudioChannels (0, nChans); // no inputs, n outputs
        
    }
    
    
    //destrcuctor
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    //must be declared - called when user changes size of window
    void resized() override
    {
        // placing the UI elements in the main window
        // getWidth has to be used in case the window is resized by the user
        const int sliderLeft = 80;
        //change size of UI elements depending on size of window
        
        for (int n = 0; n < numSine; n++){
            frequencySlider[n].setBounds (sliderLeft, 10 + 90*n, getWidth() - sliderLeft - 20, 20);
            gainSlider[n].setBounds (sliderLeft, 40 + 90*n, getWidth() - sliderLeft - 20, 20);
            onOffButton[n].setBounds (sliderLeft, 70 + 90*n, getWidth() - sliderLeft - 20, 20);
        }
        
        changeStateButton.setBounds(sliderLeft + 500, 90*numSine + 20, getWidth() - sliderLeft - 20 , 20);
        changeStateButton.changeWidthToFitText(50);
    }
    
    //must be declared (virtual method) - called when slider value changed
    void sliderValueChanged (Slider* slider) override
    {
        if (samplingRate > 0.0){
            for (int n = 0; n < numSine; n++)
            {
                if (slider == &frequencySlider[n]){
                    osc[n].setFrequency(frequencySlider[n].getValue());
                }
                else if (slider == &gainSlider[n]){
                    gain[n] = gainSlider[n].getValue();
                }
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        // turns audio on or off if OnOffButton is pressed
        for (int n = 0; n < numSine; n++)
        {
            if(button == &onOffButton[n]){
                if(onOffButton[n].getToggleState())
                    onOff[n] = 1;
            
                else
                    onOff[n] = 0;
            }
        }
        
        //if changeStateButton is pressed, then set each sin wave's frequency
        //to a random musical pitch and onOff value to 0 or 1.
        if(button == &changeStateButton){
            Random r (Time::getCurrentTime().currentTimeMillis());
            int note = 0;
            for (int n = 0; n < numSine; n++){
                
                onOff[n] = r.nextInt(2);
                if(onOff[n] == 1)
                    onOffButton[n].setToggleState(true, dontSendNotification);
                else
                    onOffButton[n].setToggleState(false, dontSendNotification);
                
                note = r.nextInt(13);
                float freq = generateNote(note);
                frequencySlider[n].setValue(freq);
                osc[n].setFrequency(freq);
            }
        }
            
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate; //'this' is automatically called by the system
        for (int n = 0; n < numSine; n++){
            osc[n].setSamplingRate(sampleRate);
        }
    }
    
    void releaseResources() override
    {
    }
    
    
    // method runs on a high priority thread because it's an audio thread
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // getting the audio output buffer to be filled
        float* buffer[numSine];
        
        for(int n = 0; n < numSine; n++){
        buffer[n] = bufferToFill.buffer->getWritePointer (n, bufferToFill.startSample);
        // computing one block
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {   
            // tick computes next sample of sine wave
            // onOff gives result of the button OnOff in application - see method buttonClicked
            if(onOff[n] == 1) buffer[n][sample] = osc[n].tick() * gain[n];
            else buffer[n][sample] = 0.0;
            
        }
        }
    }
    
    
    //method to generate frequency corresponding to a musical note
    float generateNote(int semitone){
        float baseFreq = 440.0; //refers to A4
        float freq = pow(2.0, (semitone/12.0)) * baseFreq;
        return freq;
    }
    
    
private:
    // UI Elements
    Slider *frequencySlider;
    Slider *gainSlider;
    ToggleButton *onOffButton;
    TextButton changeStateButton;
    Label *frequencyLabel, *gainLabel, *onOffLabel, changeStateLabel;
    Sine *osc; //array of sine oscillators
    
    // Global Variables
    float *gain;
    int *onOff, samplingRate, nChans;
    int numSine;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
