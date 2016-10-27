/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include "faust/Guitar.h"
#include <cmath>
#include <string>

//============================================================================
class GuitarSound: public SynthesiserSound
{
    public:
    GuitarSound() {}
    
    bool appliesToNote (int /*midiNoteNumber*/) override  { return true; }
    bool appliesToChannel (int /*midiChannel*/) override  { return true; }
};

class GuitarVoice : public SynthesiserVoice
{
    
    public:
        GuitarVoice():
            level(0.0),
            onOff(0),
            trem(0.0),
            drive(0.0),
            fb(0.0),
            envelope(0.0),
            tailOff(true){
                guit.init(this->getSampleRate());
                guit.buildUserInterface(& guitControl);
                audioBuffer = new float*[1];
				audioBuffer[0] = new float[1];
                guitControl.setParamValue("/karplus/resonator/attenuation", 0.01);
                guitControl.setParamValue("/karplus/excitator/level" ,0.5);
                guitControl.setParamValue("/karplus/am/modIndex", 1);
                guitControl.setParamValue("/karplus/am/modFreq", 0.1); 
                guitControl.setParamValue("/karplus/distortion/drive" , 0.0);
                guitControl.setParamValue("/karplus/delay/echoDuration", 0.5);
                
            }
            
    //destructor
    ~GuitarVoice()
    {
        delete []audioBuffer;
    }
            
    bool canPlaySound (SynthesiserSound* sound) override
    {
        return dynamic_cast<GuitarSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    SynthesiserSound* /*sound*/,
                    int /*currentPitchWheelPosition*/) override
    {
        level = velocity;
        float frequency = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        duration = this->getSampleRate()/frequency;
        onOff = 1;
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        onOff = 0;
        level = 0;
        tailOff = allowTailOff;
        
    }

    void pitchWheelMoved (int newValue) override
    {
        float freq = pow(2, 1.0/12.0) / 127.0 * newValue;
        duration += this->getSampleRate()/freq;
        // can't be bothered implementing this for the demo!
    }

    void controllerMoved (int controllerNumber, int newValue) override
    {
        if (controllerNumber == 3){
            trem  = (double)newValue/127.0 * 10.0;
        }
        else if(controllerNumber == 4){
            drive = (double)newValue/127.0;
        }
        else if(controllerNumber == 5){
            fb = (double)newValue/127.0;
        }
            
        // not interested in controllers in this case.
    }
    
     void renderNextBlock (AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
         
         
        // only compute block if note is on!
        
        if(envelope != 0 || onOff == 1){
            
            envelope = level;
            
            guitControl.setParamValue("/karplus/gain", envelope);
            guitControl.setParamValue("/karplus/resonator/duration", duration);
            guitControl.setParamValue("/karplus/excitator/play" , onOff);
            guitControl.setParamValue("/karplus/am/modFreq", trem);
            guitControl.setParamValue("/karplus/distortion/drive", drive);
            guitControl.setParamValue("/karplus/delay/echoFeedback" , fb);
            
            
            while (--numSamples >= 0){
                for (int i = outputBuffer.getNumChannels(); --i >= 0;){
					guit.compute(1, NULL, audioBuffer);
                    outputBuffer.addSample (i, startSample, audioBuffer[0][0]);
                }
                ++startSample;
            }
                
                
            
            for (int i = 0; i < outputBuffer.getNumSamples(); i++){
                envelope += abs(outputBuffer.getSample(0,i));
            }
            
            envelope /= outputBuffer.getNumSamples();
            if(onOff == 0 && (envelope < 0.001 || !tailOff)){
                envelope = 0;
                clearCurrentNote();
            }
        }
    }
    
    //set parameter according to slider value
    void setParameter(String whichSlider, double val){
        if(whichSlider.compare("tremolo") == 0){
            trem =  val;//guitControl.setParamValue("/karplus/am/modFreq", val);
        }
        else if(whichSlider.compare("distortion") == 0){
            drive = val;//guitControl.setParamValue("/karplus/distortion/drive", val);
        }
        else if(whichSlider.compare("echo") == 0){
            fb = val;
        }
    }
    
    //return parameter
    float getParameter(String whichParam){
        
        if(whichParam.compare("tremolo") == 0) return trem;
        else if(whichParam.compare("distortion") == 0)  return drive;
        else if(whichParam.compare("echo") == 0)  return fb;
        
    }
            
    
    private:
    
    Guitar guit;
    MapUI guitControl;
    
    double level, trem, drive, fb;
    int onOff;
    bool tailOff;
    float duration, envelope;
    
    float** audioBuffer;
    
};
    
//===================================================================================

//==============================================================================
MidiPolyPluginAudioProcessor::MidiPolyPluginAudioProcessor()
{
    initialiseSynth();
}


MidiPolyPluginAudioProcessor::~MidiPolyPluginAudioProcessor()
{
}

void MidiPolyPluginAudioProcessor::initialiseSynth(){
    
    
      for(int i = nVoices; --i >= 0;){
          synth.addVoice(new GuitarVoice());
      }
            
      synth.clearSounds();
      synth.addSound(new GuitarSound());
}


void MidiPolyPluginAudioProcessor::changeParameters(String whichSlider, double val){
     for(int i = nVoices; --i>=0;){
            GuitarVoice* tempVoice;
            tempVoice = dynamic_cast<GuitarVoice*>(synth.getVoice(i));
            tempVoice->setParameter(whichSlider, val);
        }
 }
 
float MidiPolyPluginAudioProcessor::getParameters(String whichParam){
            GuitarVoice* tempVoice;
            return (tempVoice->getParameter(whichParam));
 
}
    

//==============================================================================
const String MidiPolyPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiPolyPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiPolyPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double MidiPolyPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiPolyPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiPolyPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiPolyPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String MidiPolyPluginAudioProcessor::getProgramName (int index)
{
    return String();
}

void MidiPolyPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void MidiPolyPluginAudioProcessor::prepareToPlay (double sampleRate, int /*samplesPerBlock*/)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
     //midiCollector.reset (sampleRate);
     keyboardState.reset();
     synth.setCurrentPlaybackSampleRate (sampleRate);
}

void MidiPolyPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiPolyPluginAudioProcessor::setPreferredBusArrangement (bool isInput, int bus, const AudioChannelSet& preferredSet)
{
    // Reject any bus arrangements that are not compatible with your plugin

    const int numChannels = preferredSet.size();

   #if JucePlugin_IsMidiEffect
    if (numChannels != 0)
        return false;
   #elif JucePlugin_IsSynth
    if (isInput || (numChannels != 1 && numChannels != 2))
        return false;
   #else
    if (numChannels != 1 && numChannels != 2)
        return false;

    if (! AudioProcessor::setPreferredBusArrangement (! isInput, bus, preferredSet))
        return false;
   #endif

    return AudioProcessor::setPreferredBusArrangement (isInput, bus, preferredSet);
}
#endif




void MidiPolyPluginAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    
    const int numSamples = buffer.getNumSamples();

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, numSamples);

    // Now pass any incoming midi messages to our keyboard state object, and let it
    // add messages to the buffer if the user is clicking on the on-screen keys
    keyboardState.processNextMidiBuffer (midiMessages, 0, numSamples, true);

    // and now get our synth to process these midi events and generate its output.
    synth.renderNextBlock (buffer, midiMessages, 0, numSamples);


    // In case we have more outputs than inputs, we'll clear any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    
}

    


//==============================================================================
bool MidiPolyPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* MidiPolyPluginAudioProcessor::createEditor()
{
    return new MidiPolyPluginAudioProcessorEditor (*this);
}

//==============================================================================
void MidiPolyPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void MidiPolyPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiPolyPluginAudioProcessor();
}


