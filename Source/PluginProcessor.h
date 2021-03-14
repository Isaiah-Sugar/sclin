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




class SclinAudioProcessor  : public AudioProcessor, public Slider::Listener, public Button::Listener, public ComboBox::Listener
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
    
    void sliderValueChanged (Slider* s) override;
    
    void comboBoxChanged (ComboBox *c) override;
    
    void buttonClicked (Button* b) override;
    void buttonStateChanged (Button* b) override;
    
    
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
    

    
    Slider *widthKnobPointer;
    Slider *heightKnobPointer;
    
    int masterWidthValue;
    int masterHeightValue;
    
    
    ToggleButton *colorModePointer;
    bool masterColorModeState;
    
    
    ComboBox *channelMode1Pointer;
    ComboBox *channelMode2Pointer;
    ComboBox *channelMode3Pointer;
    
    int masterChannelMode1Value;
    int masterChannelMode2Value;
    int masterChannelMode3Value;
    
    
    Slider *channelKnob1Pointer;
    Slider *channelKnob2Pointer;
    Slider *channelKnob3Pointer;

    float masterChannelKnob1Value;
    float masterChannelKnob2Value;
    float masterChannelKnob3Value;

    
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SclinAudioProcessor)
    
    RingBuffer<float>* ringBuffer;
    
    const int representsNoData = -50;
    int currentMidiNote = representsNoData;
    bool isOn = false;
    static const int midiBufferSize = 24; // the number of notes to store, turning midi into one monophonic note.
    int currentMidi[midiBufferSize];
    int lastFilled = 0;

    unsigned long numBuffers;
};
