/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SclinAudioProcessorEditor::SclinAudioProcessorEditor (SclinAudioProcessor* p, RingBuffer<float>* r, float* currentFreqPointer, bool* isOnPointer)
: AudioProcessorEditor (p), mainComponent(p, r, currentFreqPointer, isOnPointer), processor (*p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.


    
    
    setSize (pluginDefaultSize, pluginDefaultSize / ratio);
    setResizable(true, true);
    getConstrainer()->setFixedAspectRatio(ratio);
    setResizeLimits(350, 350 / ratio, 1200, 1200 / ratio);
    addAndMakeVisible(mainComponent);
    
}


SclinAudioProcessorEditor::~SclinAudioProcessorEditor()
{
}

//==============================================================================
void SclinAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)


//    g.fillAll (Colour::fromHSV (0.6f, 0.8f, 0.8f, 1.0f));

}

void SclinAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    SclinAudioProcessorEditor::mainComponent.setBounds(0, 0, getWidth(), getHeight());
}


/*
void SclinAudioProcessorEditor::sliderValueChanged (Slider* s)
{
    if (s == &mainComponent.heightKnob){
        yImgPixels = int(mainComponent.heightKnob.getValue());
    }
    else if (s == &mainCoponent.widthKnob){
        xImgPixels = int(mainComponent.widthKnob.getValue());
    }
};
*/
