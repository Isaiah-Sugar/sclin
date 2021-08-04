/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "MainComponent.h"

//==============================================================================
/**
*/
class SclinAudioProcessorEditor  : public AudioProcessorEditor//, public Slider::Listener
{
public:
    SclinAudioProcessorEditor (SclinAudioProcessor*, RingBuffer<float>*, float*, bool*);
    ~SclinAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    MainComponent mainComponent; //object instanced!
    float ratio = mainComponent.ratio;
    int pluginDefaultSize = mainComponent.componentDefaultSize;
    
//    void sliderValueChanged (Slider* slider) override;
    
    
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SclinAudioProcessor& processor;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SclinAudioProcessorEditor)
};
