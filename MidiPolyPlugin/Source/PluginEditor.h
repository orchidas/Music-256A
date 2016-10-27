/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class MidiPolyPluginAudioProcessorEditor  : public AudioProcessorEditor,
private Timer,
private Slider::Listener,
private ComboBox::Listener,
private MidiInputCallback
{
public:
    MidiPolyPluginAudioProcessorEditor (MidiPolyPluginAudioProcessor&);
    ~MidiPolyPluginAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    void sliderValueChanged (Slider* slider) override;
    void comboBoxChanged (ComboBox* box) override;
    void handleIncomingMidiMessage (MidiInput*, const MidiMessage& message) override;
    void timerCallback() override;
    void setMidiInput (int index);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiPolyPluginAudioProcessor& processor;
    
    //declare midi sliders
    Slider tremoloSlider;
    Slider distSlider;
    Slider echoSlider;
    
    Label tremoloLabel;
    Label distLabel;
    Label echoLabel;
    
    MidiKeyboardComponent keyboardComponent;
    MidiMessageCollector midiCollector;
    
    //AudioDeviceManager audioDeviceManager;
    //MidiKeyboardState keyboardState;
    //AudioSourcePlayer audioSourcePlayer;
    //GuitarAudioSource synthAudioSource;

    ComboBox midiInputList;
    Label midiInputListLabel;
    
    int lastMidiInputIndex;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiPolyPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
