/*
  ==============================================================================

    MainComponent.cpp
    Created: 18 Feb 2020 5:24:02pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"


//==============================================================================
MainComponent::MainComponent(AudioProcessor& processor, RingBuffer<float>* r, int* currentNotePointer, bool* isOnPointer):
settingsMenuButton ("", DrawableButton::ButtonStyle::ImageFitted),
connectingLine1(myLookAndFeel.pointerColour, myLookAndFeel.backgroundColour),
connectingLine2(myLookAndFeel.pointerColour, myLookAndFeel.backgroundColour),
connectingLine3(myLookAndFeel.pointerColour, myLookAndFeel.backgroundColour),
myRoundednessSlider(myLookAndFeel.greyedTextColour, myLookAndFeel.backgroundColour)
{
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    startTimerHz(60);
    r->catchup();
    buffer = r;
    

    //make an svg button for the settings menu
    sclinLogoDrawableNormal = Drawable::createFromSVGFile(sclinLogoSeperateColorsSVG);
    sclinLogoDrawableNormal->setTransformToFit(Rectangle<float>(0, 0, 100, 100), RectanglePlacement::fillDestination);
    updateLogoColor(sclinLogoDrawableNormal.get(), &settingsMenuButton, myLookAndFeel.darkTrim, myLookAndFeel.mainTextColour);
    
    sclinLogoDrawablePressed = Drawable::createFromSVGFile(sclinLogoSeperateColorsSVG);
    sclinLogoDrawablePressed->setTransformToFit(Rectangle<float>(0, 0, 100, 100), RectanglePlacement::fillDestination);
    updateLogoColor(sclinLogoDrawablePressed.get(), &settingsMenuButton, myLookAndFeel.greyedTextColour, myLookAndFeel.darkTrim);
    
    
    settingsMenuButton.setClickingTogglesState(true);
    settingsMenuButton.setColour(DrawableButton::backgroundOnColourId, Colour::fromFloatRGBA(0, 0, 0, 0));
    settingsMenuButton.setImages(sclinLogoDrawableNormal.get(), nullptr, nullptr, nullptr, sclinLogoDrawablePressed.get());
    addAndMakeVisible(settingsMenuButton);
    
    addAndMakeVisible(myRoundednessSlider);
    
    
    addAndMakeVisible(connectingLine1);
    addAndMakeVisible(connectingLine2);
    addAndMakeVisible(connectingLine3);
    
    
    
    
    //set up the width/height knobs
    pixelKnobSettings(heightKnob); //settings for x and y pixel count knobs
    knobSettings(heightKnob); //settings for all knobs
    

    pixelKnobSettings(widthKnob); //settings for x and y pixel count knobs
    knobSettings(widthKnob); //settings for all knobs
    
    
    //set up x and y pixels labels.
    addAndMakeVisible(XPixels);
    XPixels.setText("X PIXELS", dontSendNotification);
    XPixels.setJustificationType(Justification::centred);
    XPixels.setFont(myLookAndFeel.goodFont);

    addAndMakeVisible(YPixels);
    YPixels.setText("Y PIXELS", dontSendNotification);
    YPixels.setJustificationType(Justification::centred);
    YPixels.setFont(myLookAndFeel.goodFont);
    
    
    //colorMode is the toggleButton which switches between RGB and HSV modes.
    addAndMakeVisible(colorMode);
    colorMode.addListener(this);
    colorMode.setLookAndFeel(&myLookAndFeel);
    

    
    //set up the rgb/hsv labels
    
    RGBLabel.setText("RGB", dontSendNotification);
    RGBLabel.setJustificationType(Justification::centred);
    RGBLabel.setColour(Label::textColourId, myLookAndFeel.mainTextColour);
    RGBLabel.setFont(myLookAndFeel.goodFont);
    addAndMakeVisible(RGBLabel);
    
    HSVLabel.setText("HSV", dontSendNotification);
    HSVLabel.setJustificationType(Justification::centred);
    HSVLabel.setColour(Label::textColourId, myLookAndFeel.greyedTextColour);
    RGBLabel.setFont(myLookAndFeel.goodFont);
    addAndMakeVisible(HSVLabel);
    
    
    
    
    //set up the channel labels
    
    channelLabel1.setText("RED", dontSendNotification);
    channelLabel1.setJustificationType(Justification::centred);
    channelLabel1.setColour(Label::textColourId, myLookAndFeel.mainTextColour);
    channelLabel1.setFont(myLookAndFeel.goodFont);
    addAndMakeVisible(channelLabel1);
    
    channelLabel2.setText("GREEN", dontSendNotification);
    channelLabel2.setJustificationType(Justification::centred);
    channelLabel2.setColour(Label::textColourId, myLookAndFeel.mainTextColour);
    channelLabel2.setFont(myLookAndFeel.goodFont);
    addAndMakeVisible(channelLabel2);
    
    channelLabel3.setText("BLUE", dontSendNotification);
    channelLabel3.setJustificationType(Justification::centred);
    channelLabel3.setColour(Label::textColourId, myLookAndFeel.mainTextColour);
    channelLabel3.setFont(myLookAndFeel.goodFont);
    addAndMakeVisible(channelLabel3);
    
    
    
    channelModeComboBoxInit(channelMode1);
    comboBoxSettings(channelMode1);
    channelModeComboBoxInit(channelMode2);
    comboBoxSettings(channelMode2);
    channelModeComboBoxInit(channelMode3);
    comboBoxSettings(channelMode3);

    
    colourChannelKnobSettings(channelKnob1);
    knobSettings(channelKnob1);
    
    colourChannelKnobSettings(channelKnob2);
    knobSettings(channelKnob2);
    
    colourChannelKnobSettings(channelKnob3);
    knobSettings(channelKnob3);
    
    //local pointers to midi data
    localCurrentNotePointer = currentNotePointer;
    localPreviousNote = *localCurrentNotePointer;
    localIsOnPointer = isOnPointer;

};




void MainComponent::timerCallback() {
      repaint();
};




MainComponent::~MainComponent() {
};






void MainComponent::paint (Graphics& g) {

    

        
    
    if (*localIsOnPointer && localPreviousNote != *localCurrentNotePointer) {
        xImgPixels = *localIsOnPointer;
        widthKnob.setValue(*localCurrentNotePointer);
        localPreviousNote = *localCurrentNotePointer;
    }
    
    g.fillAll (myLookAndFeel.backgroundColour);   // clear the background

    /*
    // simple display of incoming midi values
    if (*localIsOnPointer){
        g.setColour (Colours::white);
        g.drawFittedText(std::to_string(*localCurrentNotePointer), 0, 0, 200, 50, 50, .2);
    }
    */
//    drawLines(g);
    
    xDrawPixels = pluginWidth * offsetPercentInverse;
    yDrawPixels = pluginWidth * offsetPercentInverse;
        
    

    
    
    
 //   xImgPixels = widthKnob.getValue() * 4000;
 //   yImgPixels = heightKnob.getValue() * 4000;
    
    imgPixels = xImgPixels * yImgPixels;
    

        
    int currentScanlinePixel = 0;
    int currentXPixel = 0;
    int currentYPixel = 0;

    int guiLoopNumber = 0;
    
    

    
    
    
    AudioBuffer<float> theSound(2, imgPixels * averageNumber);
    theSound.clear();
    
    
    if (buffer->availableSamples() >= imgPixels) {
        
        if (buffer->availableSamples() >= imgPixels * 2) {
            buffer->skipSamples((int(buffer->availableSamples() / imgPixels) * imgPixels) - (imgPixels));
            //figure it out, stupid.
            //welcome to isaiah's helpful comment corner.
        }
        
        buffer->readSamples(theSound, imgPixels * averageNumber);
        /*
        debugSampleCountThing += theSound.getNumSamples();;
        if (debugSampleCountThing >= 100000){
            printf("read %lu samples at %d (millis) \n", debugSampleCountThing, Time::getMillisecondCounter());
            debugSampleCountThing = 0;
        }
        //std::printf("imgPixels - %d, drawing \n", imgPixels);
        */
        
    }
    else {
        
        //std::printf("imgPixels - %d, %d samples available \n", imgPixels, buffer->availableSamples());
        
    }

        
//    const float** happyLilData = theSound.getArrayOfReadPointers();
    
    //OFFSET TO KEEP THE X AXIS CENTERED
    int XOffset = ((pluginWidth - ((xDrawPixels/xImgPixels) * xImgPixels)) / 2);
    
    xPixelSize = xDrawPixels / float(xImgPixels);
    yPixelSize = yDrawPixels / float(yImgPixels);
    
    
    //DRAW A FILLED IN BLACK SQUARE (or rounded rectangle), TO COVER THE SPACE BEHIND THE DISPLAY. also, makes a non-filled in border, to help show whether it's rounded or not. also because it looks cool.
    outerSquare = Rectangle <float> (Offset - displayBorderPixels, Offset - displayBorderPixels, xDrawPixels + displayBorderPixels * 2, yDrawPixels + displayBorderPixels * 2);
    

    cornerSize = (std::min(xPixelSize, yPixelSize) / 2) * (1 + (myRoundednessSlider.getValue() * -1));
    if (cornerSize < 0)
        cornerSize = 0;
        
    Path borderPath;
    
    if (myRoundednessSlider.getValue() == 1.0){
        //fill in a black box behind the pixel display, in case something shows through.
        g.setColour(Colours::black);
        g.fillRect(float(Offset), float(Offset), xDrawPixels, yDrawPixels);
        
        //rectangle border.
        borderPath.addRectangle(outerSquare);
        
    }
    else {
        //fill in a black rounded rectangle behind the pixel display, it shows through between pixels.
        g.setColour(Colours::black);
        g.fillRoundedRectangle(Offset, Offset, xDrawPixels + 1, yDrawPixels + 1, cornerSize);
        
        
        //rounded rectangle border.
        borderPath.addRoundedRectangle(outerSquare, 2 * cornerSize, Offset * displayBorderFactor);
        
    }
    g.setColour(myLookAndFeel.pointerColourDarker);
    //draw border
    g.strokePath(borderPath, PathStrokeType(2));
    
    
    //CORRECTLY INVERT NEGATIVE VALUES FOR channelKnob
    //this generates a value which is added later on (in the pixel update loop), to make negative values inverted positives.
    float valueNeededToInvert[3];
            for (int i = 0; i < 3; i++) {
                if (channelKnobValue[i] < 0.0) {
                    valueNeededToInvert[i] = 1.0 ;
                }
                else{
                    valueNeededToInvert[i] = 0.0;
                }
            }
    
    bool localToggleStateRgbHsv = colorMode.getToggleState();
    
    
    
    Path currentPixelPath;
    if (myRoundednessSlider.getValue() == 1.0) {
        currentPixelPath.addRectangle(XOffset,  // x
        Offset,  // y
        xPixelSize,// + addXPixels,  // width
        yPixelSize); // height (had + addYPixels)
    }
    else if (myRoundednessSlider.getValue() == 0.0){
        currentPixelPath.addEllipse(Offset,  // x
        Offset,  // y
        xPixelSize,// + addXPixels,  // width
        yPixelSize);// height (had + addYPixels)
    }
    else {
        currentPixelPath.addRoundedRectangle(float(XOffset),  // x
        (float(currentYPixel) * yPixelSize) + float(Offset),  // y
        xPixelSize,// + addXPixels,  // width
        yPixelSize,// + addYPixels,
        cornerSize);
    }
    
    
    
    
        // PIXEL UPDATING LOOP
    while (guiLoopNumber/averageNumber < imgPixels) {
        
                //update random number (for dev)
     //   float randomNumber = Random::getSystemRandom().nextFloat();
        
        
            //UPDATE SAMPLE VALUE
        
        
        const float currentSampleL = theSound.getSample(0, guiLoopNumber);
        const float currentSampleR = theSound.getSample(1, guiLoopNumber);
        
        const float currentSampleAVG = (currentSampleL + currentSampleR) / 2;

        
        
        
        
        //UPDATE PIXEL COLOR
        
        
        float ChannelOutValue[3] = {0.0, 0.0, 0.0};
        
        
        //case 1 - value from AVG
        //case 2 - value from left
        //case 3 - value from right
        //case 4 - value from knob
        
        for (int i = 0; i < 3; i++) {
        switch (channelMode[i]){
            case 1:
                ChannelOutValue[i] = (currentSampleAVG * channelKnobValue[i]) + valueNeededToInvert[i];
                break;
            case 2:
                ChannelOutValue[i] = (currentSampleL * channelKnobValue[i]) + valueNeededToInvert[i];
                break;
            case 3:
                ChannelOutValue[i] = (currentSampleR * channelKnobValue[i]) + valueNeededToInvert[i];
                break;
            case 4:
                ChannelOutValue[i] = abs(channelKnobValue[i]);
                break;
        }
        }
        
        //case 0 - RGB routable
        //case 1 - HSV routable
        switch (int(localToggleStateRgbHsv)){
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
        
        
        
        
        
        //happyLilData[];
    /*
        for(int i = guiLoopNumber * averageNumber; i < guiLoopNumber * averageNumber + averageNumber; i++){
            //currentSample += happyLilData[0][i];
            currentSample += theSound.getSample(0, i);
        }
        currentSample /= averageNumber;
        */
        //random pixel color (for dev, otherwise leave it turned off)
        //g.setColour (Colour::fromHSV (0.0f, 0.0f, randomNumber, 1.0f));
        
        
        // used to be: if (!roundedButton.getToggleState()) {
        if (myRoundednessSlider.getValue() == 0.0) {
            if(currentXPixel == xImgPixels){
                addXPixels = 0;
            }
            else{
                addXPixels = 1;
            }
        
            if(currentYPixel == yImgPixels){
                addYPixels = 0;
            }
            else{
                addYPixels = 1;
            }
        }

        
        
        //DRAW CURRENT PIXEL (uses color from above)

        g.fillPath(currentPixelPath, AffineTransform::translation(float(currentXPixel) * xPixelSize, float(currentYPixel) * yPixelSize));
        
        
        /*
        // used to be: if (roundedButton.getToggleState()) {
        
        if (myRoundednessSlider.getValue() == 0.0) {
            g.fillRoundedRectangle(
                (float(currentXPixel) * xPixelSize) + float(XOffset),  // x
                (float(currentYPixel) * yPixelSize) + float(Offset),   // y
                xPixelSize + addXPixels,  // width
                yPixelSize + addYPixels,  // height
                cornerSize); // roundness
        }
        else {
            g.fillRect (
                (float(currentXPixel) * xPixelSize) + float(XOffset),  // x
                (float(currentYPixel) * yPixelSize) + float(Offset),  // y
                xPixelSize + addXPixels,  // width
                yPixelSize + addYPixels); // height
        }
         */
        
        //MOVE CURSOR
        if ((currentXPixel + 1) < xImgPixels) {
            currentXPixel ++;
            currentScanlinePixel ++;
        }
        else {
            currentXPixel = 0;
            if ((currentYPixel + 1) < yImgPixels) {
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

//    g.setColour(Colours::red);
//    g.fillRect(connectingLine1.getBounds());
//    std::printf("top: %d left: %d, width: %d, height: %d \n", connectingLine1.getY(), connectingLine1.getX(), connectingLine1.getWidth(), connectingLine1.getHeight());
    
}







void MainComponent::resized() {
    // This method is where you should set the bounds of any child
    // components that your component contains..
    
    
    pluginWidth = getWidth();
    pluginHeight = getHeight();
    
    Offset = (offsetPercent * pluginWidth);
    

    
    float knobRescale = getWidth() * (1.0/600.0);

    displayBorderPixels = Offset * displayBorderFactor;


    settingsMenuButton.setBounds(Offset * 2, displayBorderPixels, pluginWidth - (Offset * 4), Offset - (displayBorderPixels * 3));

    
    myRoundednessSlider.setBounds(
                                  settingsMenuButton.getX() + settingsMenuButton.getHeight() + (Offset / 20),
                                  settingsMenuButton.getY() + (Offset / 20),
                                  settingsMenuButton.getHeight() - (Offset / 10),
                                  settingsMenuButton.getHeight() - (Offset / 10));
    
    
    
    
//    widthKnob.setBounds(Offset, (yPixelSize * yImgPixels) + Offset, knobSize * knobRescale, knobSize * knobRescale);
    
    XPixels.setBounds(Offset,
                      yDrawPixels + (Offset * 1.2),
                      100 * knobRescale,
                      25 * knobRescale);
    
    widthKnob.setBounds(Offset, XPixels.getY() + XPixels.getHeight() - (5 * knobRescale), knobSize * knobRescale, knobSize * knobRescale);
    
    YPixels.setBounds(Offset,
                      widthKnob.getY() + widthKnob.getHeight() + (10 * knobRescale),
                      100 * knobRescale,
                      25 * knobRescale);
    
    heightKnob.setBounds(Offset, YPixels.getY() + YPixels.getHeight() - (5 * knobRescale), knobSize * knobRescale, knobSize * knobRescale);
    
    
    
    
    

    colorMode.setBounds(widthKnob.getWidth() + widthKnob.getX(),
                        widthKnob.getY() + (widthKnob.getHeight() * (1.0/3.0)),
                        Offset * .75,
                        heightKnob.getY() - widthKnob.getY() + (widthKnob.getHeight() * (1.0/3.0)));
    
    RGBLabel.setBounds(colorMode.getX(), colorMode.getY() - (Offset / 2.5), colorMode.getWidth(), colorMode.getWidth() / 2);
    HSVLabel.setBounds(colorMode.getX(), colorMode.getY() + (Offset / 3) + colorMode.getHeight() - (colorMode.getWidth() / 2), colorMode.getWidth(), colorMode.getWidth() / 2);

    
    

    
    
    int colorModeXOffset = colorMode.getX() + colorMode.getWidth() + (Offset * 0.15);
    
    
    channelMode1.setBounds(colorModeXOffset,
                         colorMode.getY() + ((colorMode.getHeight() / 4)) - (colorMode.getWidth() / 6),
                         (getWidth() - colorModeXOffset - Offset) / 3,
                         (knobSize * knobRescale) / 3);
    
    channelMode2.setBounds(
                     colorModeXOffset + channelMode1.getWidth(),
                     colorMode.getY() + ((colorMode.getHeight() / 4)) - (colorMode.getWidth() / 6),
                     (getWidth() - colorModeXOffset - Offset) / 3,
                     (knobSize * knobRescale) / 3);
    
    channelMode3.setBounds(
                 colorModeXOffset + (channelMode1.getWidth() * 2),
                 colorMode.getY() + ((colorMode.getHeight() / 4)) - (colorMode.getWidth() / 6),
                 (getWidth() - colorModeXOffset - Offset) / 3,
                 (knobSize * knobRescale) / 3);
    
    
    channelKnob1.setBounds(channelMode1.getX() + (channelMode1.getWidth() / 2) - (heightKnob.getWidth() / 2),
                           heightKnob.getY(),
                           heightKnob.getWidth(),
                           heightKnob.getHeight());
    
    channelKnob2.setBounds(channelMode2.getX() + (channelMode2.getWidth() / 2) - (heightKnob.getWidth() / 2),
                           heightKnob.getY(),
                           heightKnob.getWidth(),
                           heightKnob.getHeight());
    
    channelKnob3.setBounds( channelMode3.getX() + (channelMode3.getWidth() / 2) - (heightKnob.getWidth() / 2),
                           heightKnob.getY(),
                           heightKnob.getWidth(),
                           heightKnob.getHeight());
    
    
    channelLabel1.setBounds(channelMode1.getX(),
                            RGBLabel.getY(),
                            channelMode1.getWidth(),
                            RGBLabel.getHeight());

    channelLabel2.setBounds(channelMode2.getX(),
                            RGBLabel.getY(),
                            channelMode2.getWidth(),
                            RGBLabel.getHeight());
    
    channelLabel3.setBounds(channelMode3.getX(),
                            RGBLabel.getY(),
                            channelMode3.getWidth(),
                            RGBLabel.getHeight());
    
    
    
    int newWidth = channelMode1.getWidth() / 25;
//    int newHeight = channelMode1.getY() - channelKnob1.getY();
    int newHeight = channelKnob1.getY() - channelMode1.getY();
    int widthAddend = (channelMode1.getWidth() / 2) - newWidth / 2;
    int newY = channelMode1.getY() + (channelMode1.getHeight() / 2);
    
//    std::printf("x: %d \n", (channelMode1.getX() + widthAddend));
    
    connectingLine1.setBounds(channelMode1.getX() + widthAddend,
                              newY,
                              newWidth,
                              newHeight);
    connectingLine2.setBounds(channelMode2.getX() + widthAddend,
                              newY,
                              newWidth,
                              newHeight);
    connectingLine3.setBounds(channelMode3.getX() + widthAddend,
                              newY,
                              newWidth,
                              newHeight);
    
    
    
    
}


void MainComponent::updateLogoColor(Drawable* d, DrawableButton* b, Colour logo, Colour hy)
{
        //these are the colors of the SVG logo.
        d->replaceColour(Colour::fromFloatRGBA(1, 0, 0, 1), hy);
        d->replaceColour(Colour::fromFloatRGBA(0, 1, 0, 1), logo);
};

void MainComponent::drawLines (Graphics& g) {
    
    //draws the 3 lines visually connecting the color mode comboBoxes to their corresponding sliders.
    g.setColour(myLookAndFeel.darkTrim);
    g.drawLine(
               channelKnob1.getX() + (channelKnob1.getWidth() / 2), // starting x
               (channelKnob1.getY() + (channelKnob1.getWidth() / 2)), // starting y
               channelMode1.getX() + (channelMode1.getWidth() / 2), // ending x
               channelMode1.getY(), // ending y
               1); // thickness
    
    g.drawLine(
               channelKnob2.getX() + (channelKnob2.getWidth() / 2), // starting x
               (channelKnob2.getY() + (channelKnob2.getWidth() / 2)), // starting y
               channelMode2.getX() + (channelMode2.getWidth() / 2), // ending x
               channelMode2.getY(), // ending y
               1); // thickness
    
    g.drawLine(
               channelKnob3.getX() + (channelKnob3.getWidth() / 2), // starting x
               (channelKnob3.getY() + (channelKnob3.getWidth() / 2)), // starting y
               channelMode3.getX() + (channelMode3.getWidth() / 2), // ending x
               channelMode3.getY(), // ending y
               1); // thickness
    
    
    g.setColour(myLookAndFeel.pointerColour);
    g.drawRect((channelKnob1.getX() + (channelKnob1.getWidth() / 2)) - 1, channelMode1.getY(), 3, (channelKnob1.getY() - channelMode1.getY()) + 2);
    g.drawRect((channelKnob2.getX() + (channelKnob2.getWidth() / 2)) - 1, channelMode2.getY(), 3, (channelKnob2.getY() - channelMode2.getY()) + 2);
    g.drawRect((channelKnob3.getX() + (channelKnob3.getWidth() / 2)) - 1, channelMode3.getY(), 3, (channelKnob3.getY() - channelMode3.getY()) + 2);
};


void MainComponent::knobSettings(Slider& s)
{
    addAndMakeVisible(s);
    s.setLookAndFeel(&myLookAndFeel);
    
    s.setDoubleClickReturnValue (true, s.getValue());
    s.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    s.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    s.setTextBoxIsEditable(true);
    s.addListener(this);
    s.setColour(Slider::textBoxHighlightColourId, myLookAndFeel.backgroundColour);
    s.setColour(Slider::textBoxBackgroundColourId, myLookAndFeel.pointerColourDarker);
    
    
    /*
    //Don't need these definitions anymore, since I wrote a lookAndFeel class
    s.setColour(CaretComponent::ColourIds::caretColourId,
                //Colour::fromHSV (0.0f, 0.0f, 0.75f, 1.0f));
                Colour::fromHSV (1.0f, 0.4f, 0.75f, 1.0f));
    
        s.setColour(Slider::thumbColourId, Colour::fromHSV (0.0f, 0.0f, 0.6f, 1.0f));
        s.setColour(Slider::rotarySliderFillColourId, Colour::fromHSV (0.0f, 0.0f, 0.5f, 1.0f));
        s.setColour(Slider::rotarySliderOutlineColourId, Colour::fromHSV (0.0f, 0.0f, 0.3f, 1.0f));
    */
    
};


void MainComponent::pixelKnobSettings (Slider& s) {
        //lamba function to stop it from showing decimals
        s.textFromValueFunction = [] (double val){
            int i = static_cast<int>(val);
            return String(i);
        };
    //    s.setNumDecimalPlacesToDisplay(1);
    
    s.setRange(minPixels, maxPixels);
    s.setValue(defaultPixels);
    s.setTextValueSuffix (" px");
    
};




void MainComponent::colourChannelKnobSettings (Slider& s) {
    

    s.setRange(-200, 200);
    s.setValue(100);
    s.setNumDecimalPlacesToDisplay(1);
    s.setTextValueSuffix (" %");
     
    
};


void MainComponent::sliderValueChanged (Slider* s) {
    if (s == &heightKnob){
        yImgPixels = int(s->getValue());
    }
    else if (s == &widthKnob){
        if (!settingsMenuButton.getToggleState())
            xImgPixels = int(s->getValue());
    }
    else if (s == &channelKnob1){
        channelKnobValue[0] = (s->getValue() / 100.0);
    }
    else if (s == &channelKnob2){
        channelKnobValue[1] = (s->getValue() / 100.0);
    }
    else if (s == &channelKnob3){
        channelKnobValue[2] = (s->getValue() / 100.0);
    }
};

void MainComponent::comboBoxChanged (ComboBox *c) {
    
//    c->getSelectedId();
    
    if (c == &channelMode1){
        channelMode[0] = c->getSelectedId();
    }
    else if (c == &channelMode2){
        channelMode[1] = c->getSelectedId();
    }
    else if (c == &channelMode3){
        channelMode[2] = c->getSelectedId();
    };
    
};




void MainComponent::buttonClicked (Button* b) {

}


void MainComponent::buttonStateChanged (Button* b) {

    if (b == &colorMode) {
        if (!colorMode.getToggleState())
        {
            RGBLabel.setColour(Label::textColourId, myLookAndFeel.mainTextColour);
            HSVLabel.setColour(Label::textColourId, myLookAndFeel.greyedTextColour);
            
            channelLabel1.setText("RED", dontSendNotification);
            channelLabel2.setText("GREEN", dontSendNotification);
            channelLabel3.setText("BLUE", dontSendNotification);
            
        }
        else {
            RGBLabel.setColour(Label::textColourId, myLookAndFeel.greyedTextColour);
            HSVLabel.setColour(Label::textColourId, myLookAndFeel.mainTextColour);
            
            channelLabel1.setText("HUE", dontSendNotification);
            channelLabel2.setText("SATURATION", dontSendNotification);
            channelLabel3.setText("VALUE", dontSendNotification);
            
        }
        
        
    }


}


void MainComponent::channelModeComboBoxInit (ComboBox& c) { //to be run ONCE each time the editor is re-generated, for the channel mode dropdowns.

    c.addItem("AVG", 1);
    c.addItem("LEFT", 2);
    c.addItem("RIGHT", 3);
    c.addItem("KNOB", 4);
    
    c.setSelectedId(1);
    c.addListener(this);
    
};



void MainComponent::comboBoxSettings (ComboBox& c) { //to be run ONCE each time the editor is re-generated, for dropdowns.

    addAndMakeVisible(c);
    c.setLookAndFeel(&myLookAndFeel);
    
    c.setJustificationType(Justification::horizontallyCentred);
    

};
