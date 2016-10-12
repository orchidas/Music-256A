/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "faust/Synth.h"
#include "faust/FaustReverb.h"
#include <cmath>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : 
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener,
    private KeyListener
{
public:
    //==============================================================================
    MainContentComponent() : currentSampleRate(0.0)
    {
        sawGainSlider.setSliderStyle(Slider::Rotary);
        sawGainSlider.setRange(0.0,0.5);
        sawGainSlider.setValue(0.5);
        sawGainSlider.addListener(this);
        addAndMakeVisible(sawGainSlider);
        
        addAndMakeVisible(sawGainLabel);
        sawGainLabel.setText ("Sawtooth Gain", dontSendNotification);
        sawGainLabel.attachToComponent (&sawGainSlider, true);
    
        sqrGainSlider.setSliderStyle(Slider::Rotary);
        sqrGainSlider.setRange(0.0,0.5);
        sqrGainSlider.setValue(0.5);
        sqrGainSlider.addListener(this);
        addAndMakeVisible(sqrGainSlider);
        
        addAndMakeVisible(sawGainLabel);
        sqrGainLabel.setText ("Square Gain", dontSendNotification);
        sqrGainLabel.attachToComponent (&sqrGainSlider, true);
        
        cutoffSlider.setSliderStyle(Slider::Rotary);
        cutoffSlider.setRange(500,20000,50);
        cutoffSlider.setValue(2000);
        cutoffSlider.addListener(this);
        addAndMakeVisible(cutoffSlider);
        
        addAndMakeVisible(cutoffLabel);
        cutoffLabel.setText ("Cutoff Frequency", dontSendNotification);
        cutoffLabel.attachToComponent (&cutoffSlider, true);
        
        beatFreqSlider.setSliderStyle(Slider::Rotary);
        beatFreqSlider.setRange(0,5);
        beatFreqSlider.setValue(0);
        beatFreqSlider.addListener(this);
        addAndMakeVisible(beatFreqSlider);
        
        addAndMakeVisible(beatFreqLabel);
        beatFreqLabel.setText ("Beat Frequency", dontSendNotification);
        beatFreqLabel.attachToComponent (&beatFreqSlider, true);
        
        addAndMakeVisible(onOffButton);
        onOffButton.addListener(this);
        
        addAndMakeVisible(onOffLabel);
        onOffLabel.setText ("On/Off", dontSendNotification);
        onOffLabel.attachToComponent (&onOffButton, true);
        
        //add keylistener
        getTopLevelComponent()->addKeyListener(this);
        
       
        
        //change color of sliders and labels to white
        getLookAndFeel().setColour (Slider::rotarySliderFillColourId, Colours::white);
        getLookAndFeel().setColour (Label::textColourId, Colours::white);
        getLookAndFeel().setColour (TextButton::buttonColourId, Colours::white);
        getLookAndFeel().setColour (ToggleButton::tickColourId, Colours::blue);
        setSize (800, 600);
        beatFreq = 0;
        sqrFreq = 220;
        sawFreq = 440;

        // specify the number of input and output channels that we want to open
        nChans = 2;
        setAudioChannels (0, nChans);
        
        audioBuffer = new float*[nChans];
    }

    ~MainContentComponent()
    {
        shutdownAudio();
        delete [] audioBuffer;
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        
        // This function will be called when the audio device is started, or when
        // its settings (i.e. sample rate, block size, etc) are changed.

        // You can use this function to initialise any resources you might need,
        // but be careful - it will be called on the audio thread, not the GUI thread.

        // For more details, see the help for AudioProcessor::prepareToPlay()
        currentSampleRate = sampleRate;
        blockSize = samplesPerBlockExpected;
        
        synth.init(sampleRate); // initializing the Faust module
        synth.buildUserInterface(&synthControl); // linking the Faust module to the controler
        
        reverb.init(sampleRate);
        reverb.buildUserInterface(&reverbControl);
        
        // Print the list of parameters address of "saw"
        // To get the current (default) value of these parameters, sawControl.getParamValue("paramPath") can be used
        for(int i=0; i<synthControl.getParamsCount(); i++){
            std::cout << synthControl.getParamAdress(i) << "\n";
        }
        
        // setting default values for the Faust module parameters
        synthControl.setParamValue("/sub_synth/saw/frequency",sawFreq);
        synthControl.setParamValue("/sub_synth/sqr/frequency", sqrFreq);
        synthControl.setParamValue("/sub_synth/saw/gain",0.5);
        synthControl.setParamValue("/sub_synth/sqr/gain",0.5);
        synthControl.setParamValue("/sub_synth/lpf/cutoff", 2000); 
        
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // Your audio-processing code goes here!
        
        for(int i =0; i < nChans; i++){
             audioBuffer[i] = bufferToFill.buffer->getWritePointer (i, bufferToFill.startSample);
        }
        
        synth.compute(blockSize, NULL, audioBuffer); // computing one block with Faust
        //apply reverb
        reverb.compute(blockSize, audioBuffer, audioBuffer); 

    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);
        g.setFont (Font ("Times New Roman", 40.0f, Font::bold));
        g.setColour (Colours::lightgrey);
        g.drawText ("Sub Synth", 300, 10, 200, 50, true);


        // You can add your drawing code here!
    }

    void resized() override
    {
        
        const int sliderLeft = 200;
        sawGainSlider.setBounds (sliderLeft, 70, getWidth() - sliderLeft - 10, 100);
        sqrGainSlider.setBounds (sliderLeft, 200, getWidth() - sliderLeft - 10, 100);    
        cutoffSlider.setBounds (sliderLeft, 330, getWidth() - sliderLeft - 10, 100);  
        beatFreqSlider.setBounds(sliderLeft, 470, getWidth() - sliderLeft - 10, 100);
        onOffButton.setBounds (sliderLeft + 200, 550, getWidth() - sliderLeft - 10, 20);
        
    }
    
    void sliderValueChanged (Slider* slider) override
    {
        if (currentSampleRate > 0.0){
            if (slider == &sawGainSlider)
                synthControl.setParamValue("/sub_synth/saw/gain", sawGainSlider.getValue());
            else if(slider == &sqrGainSlider)
                synthControl.setParamValue("/sub_synth/sqr/gain", sqrGainSlider.getValue());
            else if(slider == &cutoffSlider)
                synthControl.setParamValue("/sub_synth/lpf/cutoff", cutoffSlider.getValue());
            else if(slider == &beatFreqSlider){
                beatFreq = beatFreqSlider.getValue();
                synthControl.setParamValue("/sub_synth/sqr/frequency", getSqrFrequency() + beatFreq);
            }
        }
    }
    
    void buttonClicked (Button* button) override
    {
        if(button == &onOffButton && onOffButton.getToggleState()){
            synthControl.setParamValue("/sub_synth/env/gate", 1);
        }
        else{
            synthControl.setParamValue("/sub_synth/env/gate", 0);
        }
    }
    
    //map keys to notes
    bool keyPressed(const KeyPress &key, Component* /* originatingComponent */) override
    {
	    if(key.getTextCharacter() >= 'a' && key.getTextCharacter() <= 'n')
	    {
	        sawFreq = generateNote(key.getTextCharacter() - 'a', 0);
	        sqrFreq = generateNote(key.getTextCharacter() - 'a', -1);
	    
	    synthControl.setParamValue("/sub_synth/env/gate", 1);
	    synthControl.setParamValue("/sub_synth/saw/frequency", sawFreq);
	    synthControl.setParamValue("/sub_synth/sqr/frequency", sqrFreq + beatFreq);
	      
        }   
        else{
            synthControl.setParamValue("/sub_synth/env/gate", 0);
        }
            
	        
	return true;
    }
    
    
    bool keyStateChanged(bool isKeyDown, Component* /* originatingComponent */)
    {
        if (!isKeyDown){
           onOffButton.setToggleState(false, dontSendNotification);
           synthControl.setParamValue("/sub_synth/env/gate", 0);
        }
        else{
           synthControl.setParamValue("/sub_synth/env/gate", 1); 
           onOffButton.setToggleState(true, dontSendNotification);
        }
        return true;
    }
            
             
    //return current frequency of square wave
    float getSqrFrequency(){
        return sqrFreq;
    }
    
    float getSawFrequency(){
        return sawFreq;
    }
    
    //method to generate frequency corresponding to a musical note
    float generateNote(int semitone, int oct){
        float baseFreq = 440.0 * pow(2.0,oct); 
        float freq = pow(2.0, (semitone/12.0)) * baseFreq;
        return freq;
    }
    
    
private:
    //==============================================================================

    // Your private member variables go here...
    Synth synth; // the Faust module (Saw.h)
    MapUI synthControl; // used to easily control the Faust module (Saw.h)
    
    Slider sawGainSlider;
    Slider sqrGainSlider;
    Slider cutoffSlider;
    Slider beatFreqSlider;
    ToggleButton onOffButton;
    
    
    Label sawGainLabel;
    Label sqrGainLabel;
    Label cutoffLabel;
    Label beatFreqLabel;
    Label onOffLabel;
    
    int blockSize, nChans;
    double currentSampleRate;
    
    float** audioBuffer; // multichannel audio buffer used both for input and output
    float beatFreq;
    float sqrFreq;
    float sawFreq;
    
    //add reverb
     FaustReverb reverb;
     MapUI reverbControl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
