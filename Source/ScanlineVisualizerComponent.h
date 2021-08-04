/*
  ==============================================================================

    ScanlineVisualizerComponent.h
    Created: 13 Jul 2021 8:26:33pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ScanlineVisualizerComponent  : public juce::Component
{
public:
    ScanlineVisualizerComponent(juce::AudioBuffer<float>* theSound, int* xImgPixels, juce::AudioParameterInt* yImgPixels, juce::AudioParameterFloat* roundness, float* channelMultipliers, int* channelModes, juce::AudioParameterBool* colorMode);
    ~ScanlineVisualizerComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScanlineVisualizerComponent)
    
    juce::AudioBuffer<float>* theSound;
    
    int* xImgPixels;
    
    juce::AudioParameterInt* yImgPixels;
    int imgPixels;
    
    juce::AudioParameterFloat* roundness;
    
    float* channelMultipliers;
    int* channelModes;
    
    juce::AudioParameterBool* colorMode;
    

    
    float xPixelSize;
    float yPixelSize;
    
    
    
    
    
};
