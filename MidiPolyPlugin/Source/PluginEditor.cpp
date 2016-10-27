/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
MidiPolyPluginAudioProcessorEditor::MidiPolyPluginAudioProcessorEditor (MidiPolyPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p),

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    keyboardComponent (processor.keyboardState, MidiKeyboardComponent::horizontalKeyboard),
    //synthAudioSource (p.keyboardState),
    lastMidiInputIndex (0)
    {
        //set your sliders
        tremoloSlider.setSliderStyle(Slider::Rotary);
        tremoloSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        tremoloSlider.setRange(0, 10, 1);
        tremoloSlider.setValue(1);
        tremoloSlider.addListener(this);
        addAndMakeVisible(tremoloSlider);
        
        addAndMakeVisible(tremoloLabel);
        tremoloLabel.setText ("Tremolo Frequency", dontSendNotification);
        tremoloLabel.attachToComponent (&tremoloSlider, true);
    
        distSlider.setSliderStyle(Slider::Rotary);
        distSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        distSlider.setRange(0.0,1.0, 0.01);
        distSlider.setValue(0.0);
        distSlider.addListener(this);
        addAndMakeVisible(distSlider);
        
        addAndMakeVisible(distLabel);
        distLabel.setText ("Distortion Drive", dontSendNotification);
        distLabel.attachToComponent (&distSlider, true);
        
        echoSlider.setSliderStyle(Slider::Rotary);
        echoSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        echoSlider.setRange(0.0, 1.0, 0.01);
        echoSlider.setValue(0);
        echoSlider.addListener(this);
        addAndMakeVisible(echoSlider);
        
        addAndMakeVisible(echoLabel);
        echoLabel.setText ("Delay feedback", dontSendNotification);
        echoLabel.attachToComponent (&echoSlider, true);
        
        /*//other stuff here
        audioDeviceManager.initialise (0, 1, nullptr, true, String(), nullptr); //no input stereo output
        audioSourcePlayer.setSource (&synthAudioSource);

        audioDeviceManager.addAudioCallback (&audioSourcePlayer);
        audioDeviceManager.addMidiInputCallback (String(), &(synthAudioSource.midiCollector));
        */
        
        // MIDI Inputs
        addAndMakeVisible (midiInputListLabel);
        midiInputListLabel.setText ("MIDI Input:", dontSendNotification);
        midiInputListLabel.attachToComponent (&midiInputList, true);

        // menu listing all the available MIDI inputs
        addAndMakeVisible (midiInputList);
        midiInputList.setTextWhenNoChoicesAvailable ("No MIDI Inputs Enabled");
        const StringArray midiInputs (MidiInput::getDevices());
        midiInputList.addItemList (midiInputs, 1);
        midiInputList.setSelectedId(0); // default
        midiInputList.addListener (this);

        // by default we use the first MIDI input
        setMidiInput(0);

        // The MIDI keyboard
        addAndMakeVisible(keyboardComponent);

        setSize (600, 300);
        
        midiCollector.reset(processor.getSampleRate());
        
    }

    
MidiPolyPluginAudioProcessorEditor::~MidiPolyPluginAudioProcessorEditor()
{
     midiInputList.removeListener (this);
}

//==============================================================================
void MidiPolyPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colours::lightgrey);
    
}

void MidiPolyPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    const int labelWidth = 90;
    midiInputList.setBounds (labelWidth, 10, getWidth()-labelWidth-10, 20);
    keyboardComponent.setBounds (5, 150, getWidth()-10,100);
        
    const int sliderLeft = 100;
    tremoloSlider.setBounds(sliderLeft, 50, 70, 70);
    tremoloLabel.setBounds(sliderLeft - 20, 120, 150, 20);
    distSlider.setBounds(sliderLeft + 150, 50, 70, 70);
    distLabel.setBounds(sliderLeft + 150 - 10, 120, 100, 20);
    echoSlider.setBounds(sliderLeft + 300, 50, 70, 70);
    echoLabel.setBounds(sliderLeft + 300 - 10, 120, 100, 20);
}

 void MidiPolyPluginAudioProcessorEditor::sliderValueChanged (Slider* slider) 
    {
        for(int i = 0; i < processor.synth.getNumVoices(); i++){
        if (slider == &tremoloSlider){
            processor.changeParameters("tremolo", tremoloSlider.getValue());
            //processor.synth.getVoice(i)->setParameter("tremolo", tremoloSlider.getValue());
            //processor.myVoice[i]->controllerMoved(3, tremoloSlider.getValue());
        }
        else if(slider == &distSlider){
            processor.changeParameters("distortion", distSlider.getValue());
            //processor.synth.getVoice(i)->setParameter("distortion", distSlider.getValue());
             //processor.myVoice[i]->controllerMoved(4, distSlider.getValue());
        }
        else if(slider == &echoSlider){
            processor.changeParameters("echo", echoSlider.getValue());
            //processor.synth.getVoice(i)->setParameter("echo", echoSlider.getValue());
            //processor.myVoice[i]->controllerMoved(5, echoSlider.getValue());
        }
        }
    }
    
    
 void MidiPolyPluginAudioProcessorEditor::comboBoxChanged (ComboBox* box)
    {
        if (box == &midiInputList) setMidiInput  (midiInputList.getSelectedItemIndex());
    }
    
 //set which MIDI input is being used.
    void MidiPolyPluginAudioProcessorEditor::setMidiInput (int index)
    {
        const StringArray list (MidiInput::getDevices());

        const String newInput (list[index]);

        midiInputList.setSelectedId (index + 1, dontSendNotification);

        lastMidiInputIndex = index;
    }

    // Needed virtual function
    void MidiPolyPluginAudioProcessorEditor::handleIncomingMidiMessage (MidiInput* midiCollector, const MidiMessage& message) {
        int midiValue = message.getControllerValue();
        if(message.getControllerNumber() == 3){
            tremoloSlider.setValue(midiValue/127.0 * 10);
        }
        else if(message.getControllerNumber() == 4){
            distSlider.setValue(midiValue/127.0);
        }
        else if(message.getControllerNumber() == 5){
            echoSlider.setValue(midiValue/127.0);
        }
        
    }
    
    void MidiPolyPluginAudioProcessorEditor::timerCallback()
    {
    
    }
    
   


