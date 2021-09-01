/*
  ==============================================================================

    ScanlineVisualizerComponent.cpp
    Created: 13 Jul 2021 8:26:33pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ScanlineVisualizerComponent.h"

//==============================================================================
ScanlineVisualizerComponent::ScanlineVisualizerComponent(juce::AudioBuffer<float>* i_theSound, int* i_xImgPixels, juce::AudioParameterInt* i_yImgPixels, juce::AudioParameterFloat* i_roundness, float* i_channelMultipliers, int* i_channelModes, juce::AudioParameterBool* i_colorMode)
{
    theSound = i_theSound;
    
    xImgPixels = i_xImgPixels;
    yImgPixels = i_yImgPixels;
    imgPixels = *xImgPixels * yImgPixels->get();
    
    roundness = i_roundness;
    
    channelMultipliers = i_channelMultipliers;
    channelModes = i_channelModes;
    
    colorMode = i_colorMode;
    /*
    xPixelSize = getWidth() / *xImgPixels;
    yPixelSize = getHeight() / yImgPixels->get();
     */
}

ScanlineVisualizerComponent::ScanlineVisualizerComponent(ScanlineVisualizerComponent* the_one_to_copy) {
    theSound = the_one_to_copy->theSound;
    
    xImgPixels = the_one_to_copy->xImgPixels;
    yImgPixels = the_one_to_copy->yImgPixels;
    imgPixels = *xImgPixels * yImgPixels->get();
    
    roundness = the_one_to_copy->roundness;
    
    channelMultipliers = the_one_to_copy->channelMultipliers;
    channelModes = the_one_to_copy->channelModes;
    
    colorMode = the_one_to_copy->colorMode;
}


ScanlineVisualizerComponent::~ScanlineVisualizerComponent()
{
}

void ScanlineVisualizerComponent::paint (juce::Graphics& g)
{
    int currentScanlinePixel = 0;
    int currentXPixel = 0;
    int currentYPixel = 0;

    xPixelSize = getWidth() / float(*xImgPixels);
    yPixelSize = getHeight() / float(yImgPixels->get());
    
    int guiLoopNumber = 0;
    

    
    imgPixels = *xImgPixels * yImgPixels->get();
    
    
    //float addXPixels = 0.0;
    //float addYPixels = 0.0;
    
    
    //g.fillAll (juce::Colour::fromRGB(0, 0, 0));   // clear the background
    g.setColour(juce::Colour::fromRGB(0, 0, 0));
    g.fillRect(xPixelSize / 2, yPixelSize / 2, getWidth() - xPixelSize, getHeight() - yPixelSize);
    g.fillRect(0.0, yPixelSize / 2, xPixelSize, getHeight() - yPixelSize); //left side
    g.fillRect(getWidth() - xPixelSize, yPixelSize / 2, xPixelSize, getHeight() - yPixelSize); //right side
    g.fillRect(xPixelSize / 2, 0.0, getWidth() - xPixelSize, yPixelSize); //top side
    g.fillRect(xPixelSize / 2, getHeight() - yPixelSize, getWidth() - xPixelSize, yPixelSize); //bottom
    
    
    

    //CORRECTLY INVERT NEGATIVE VALUES FOR channelKnob
    //this generates a value which is added later on (in the pixel update loop), to make negative values inverted positives.
    float valueNeededToInvert[3];
            for (int i = 0; i < 3; i++) {
                if (channelMultipliers[i] < 0.0) {
                    valueNeededToInvert[i] = 1.0 ;
                }
                else{
                    valueNeededToInvert[i] = 0.0;
                }
            }
    
    
    
    //create a Path object with a square / rounded rectangle / ellipse, to move around for each pixel.
    Path currentPixelPath;
    if (roundness->get() == 1.0) {
        currentPixelPath.addRectangle(0,  // x
        0,  // y
        xPixelSize, // width
        yPixelSize); // height
    }
    else if (roundness->get() == 0.0) {
        currentPixelPath.addEllipse(0,  // x
        0,  // y
        xPixelSize, // width
        yPixelSize);// height (had + addYPixels)
    }
    else {
        currentPixelPath.addRoundedRectangle(0,  // x
        0,  // y
        xPixelSize,// width
        yPixelSize,
        (std::min(xPixelSize,yPixelSize) / 2) * (1 + (roundness->get() * -1)));
    }
    

    
    
    
        // PIXEL UPDATING LOOP
    while (guiLoopNumber < imgPixels) {
        

        
        
            //UPDATE SAMPLE VALUE
        
        
        const float currentSampleL = theSound->getSample(0, guiLoopNumber);
 
        const float currentSampleR = theSound->getSample(1, guiLoopNumber);
        
        const float currentSampleAVG = (currentSampleL + currentSampleR) / 2;

        
        
        
        
        //UPDATE PIXEL COLOR
        
        
        float ChannelOutValue[3] = {0.0, 0.0, 0.0};
        
        
        //case 1 - value from AVG
        //case 2 - value from left
        //case 3 - value from right
        //case 4 - value from knob
        
        for (int i = 0; i < 3; i++) {
        switch (channelModes[i]){
            case 1:
                ChannelOutValue[i] = (currentSampleAVG * channelMultipliers[i]) + valueNeededToInvert[i];
                break;
            case 2:
                ChannelOutValue[i] = (currentSampleL * channelMultipliers[i]) + valueNeededToInvert[i];
                break;
            case 3:
                ChannelOutValue[i] = (currentSampleR * channelMultipliers[i]) + valueNeededToInvert[i];
                break;
            case 4:
                ChannelOutValue[i] = abs(channelMultipliers[i]);
                break;
        }
        }
        
        //case 0 - RGB routable
        //case 1 - HSV routable
        switch (int(colorMode->get())){
            case 0:
            {
                g.setColour(Colour::fromFloatRGBA(ChannelOutValue[0], ChannelOutValue[1], ChannelOutValue[2], 1));
                }
                break;
            case 1:
            {
                
                g.setColour(Colour::fromHSV(ChannelOutValue[0], ChannelOutValue[1], ChannelOutValue[2], 1));
                }
                break;

        }
        

        /*
        
        // used to be: if (!roundedButton.getToggleState()) {
        if (roundness->get() == 0.0) {
            if(currentXPixel == *xImgPixels){
                addXPixels = 0;
            }
            else{
                addXPixels = 1;
            }
        
            if(currentYPixel == yImgPixels->get()){
                addYPixels = 0;
            }
            else{
                addYPixels = 1;
            }
        }
         */
        
        
        //DRAW CURRENT PIXEL (uses color from above)

        g.fillPath(currentPixelPath, AffineTransform::translation(float(currentXPixel) * xPixelSize, float(currentYPixel) * yPixelSize));
        
        
        //MOVE CURSOR
        if ((currentXPixel + 1) < *xImgPixels) {
            currentXPixel ++;
            currentScanlinePixel ++;
        }
        else {
            currentXPixel = 0;
            if ((currentYPixel + 1) < yImgPixels->get()) {
                currentYPixel ++;
                currentScanlinePixel ++;
            }
            else {
                currentYPixel = 0;
                currentScanlinePixel = 0;
            }
                    
        }
        
        
        guiLoopNumber ++;
    } //that curly brace ends the while() loop. ik its wierd, just deal with it.

    
    
    
}

void ScanlineVisualizerComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}








VisualizerFullscreen::VisualizerFullscreen(class ScanlineVisualizerComponent * theVisuzlizerToCopy) : juce::DocumentWindow("visualizer_fullscreen", juce::Colour::fromHSV(0.0, 0.0, 0.0, 1.0), 0), theComponentWeAreUsing(theVisuzlizerToCopy) {
    setFullScreen(true);
    setVisible(true);
    juce::Desktop::getInstance().setKioskModeComponent(this, false);
    setContentOwned(&theComponentWeAreUsing, false);
}


VisualizerFullscreen::~VisualizerFullscreen() {
    
    
    juce::Desktop::getInstance().setKioskModeComponent(nullptr, false);
    
}
