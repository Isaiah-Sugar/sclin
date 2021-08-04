/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RingBuffer.h"

//==============================================================================
/*
class MyListener : public Slider::Listener
{
public:
    MyListener();
    ~MyListener();
    void changeListenerCallback(ChangeBroadcaster *source);
    
    
};

*/




class SclinAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SclinAudioProcessor();
    ~SclinAudioProcessor();

    
    
    
    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    /*
    void sliderValueChanged (Slider* s) override;
    
    void comboBoxChanged (ComboBox *c) override;
    
    void buttonClicked (Button* b) override;
    void buttonStateChanged (Button* b) override;
    */
    
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    

    
    AudioProcessorValueTreeState myTreeState;
    //parameters - xpixels, ypixels, xcycles, ycycles, rgb/hsv, redsource, redmult, greensource, greenmult, bluesource, bluemult, roundness, midion, freeze
    
    
    static const int defaultPixels;//default value for pixel knobs (x pixels / y pixels)
    static const int defaultCycles;//default value for pixel knobs (x pixels / y pixels)
    static const bool defaultColorMode;
    static const int defaultMult;
    static const bool defaultMidiState;
    
    static const int maxPixels; //maximum of each pixel knob (x pixels / y pixels)
    static const int minPixels; //minimum of each pixel knob (x pixels / y pixels)
    static const int maxCycles; //maximum of each pixel knob (x pixels / y pixels)
    static const int minCycles; //minimum of each pixel knob (x pixels / y pixels)
    
    static const int maxMult; //maximum value for the channel multipliers, minimum is just negative.
    
    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SclinAudioProcessor)
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    juce::String channelKnobsStringFromValue(float, int);
    
    RingBuffer<float>* ringBuffer;
    
    static const juce::StringArray colorSourceOptionStrings;


    const int representsNoData = -50;
    int currentMidiNote = representsNoData;
    bool isOn = false;
    static const int midiBufferSize = 24; // the number of notes to store, turning midi into one monophonic note.
    int currentMidi[midiBufferSize];
    int lastFilled = 0;
    float currentNoteCycleLength;

    unsigned long numBuffers;
};
