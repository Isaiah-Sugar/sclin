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
MainComponent::MainComponent(SclinAudioProcessor* processor, RingBuffer<float>* r, float* CurrentFreqPointer, bool* isOnPointer):
settingsMenuButton ("", DrawableButton::ButtonStyle::ImageFitted),
connectingLine1(myLookAndFeel.pointerColour, myLookAndFeel.backgroundColour),
connectingLine2(myLookAndFeel.pointerColour, myLookAndFeel.backgroundColour),
connectingLine3(myLookAndFeel.pointerColour, myLookAndFeel.backgroundColour),
myRoundednessSlider(myLookAndFeel.greyedTextColour, myLookAndFeel.backgroundColour),
freezeButton("", DrawableButton::ButtonStyle::ImageStretched),
scanlineVisualizer(&theSound, &xImgPixels, dynamic_cast<AudioParameterInt*>(processor->myTreeState.getParameter("ypixels")), dynamic_cast<AudioParameterFloat*>(processor->myTreeState.getParameter("roundness")), channelKnobValue, channelMode, dynamic_cast<AudioParameterBool*>(processor->myTreeState.getParameter("colormode")))
{
    startTimerHz(60);
    r->catchup();
    buffer = r;
    
    pointerToAudioProcessor = processor; //pointer to the audioProcessor running this
    
    
    xImgPixels = pointerToAudioProcessor->defaultPixels;
    
    theSound = AudioBuffer<float>(2, imgPixels * averageNumber);
    
    xPixelSize = xDrawPixels/xImgPixels;
    yPixelSize = yDrawPixels/ pointerToAudioProcessor->defaultPixels;
    
    
    
    //make an svg button for the settings menu
    sclinLogoDrawableNormal = Drawable::createFromSVGFile(sclinLogoSeperateColorsSVG);
    sclinLogoDrawableNormal->setTransformToFit(Rectangle<float>(0, 0, 100, 100), RectanglePlacement::fillDestination);
    updateLogoColor(sclinLogoDrawableNormal.get(), &settingsMenuButton, myLookAndFeel.darkTrim, myLookAndFeel.mainTextColour);
    
    sclinLogoDrawablePressed = Drawable::createFromSVGFile(sclinLogoSeperateColorsSVG);
    sclinLogoDrawablePressed->setTransformToFit(Rectangle<float>(0, 0, 100, 100), RectanglePlacement::fillDestination);
    updateLogoColor(sclinLogoDrawablePressed.get(), &settingsMenuButton, myLookAndFeel.greyedTextColour, myLookAndFeel.darkTrim );
    
    
    settingsMenuButton.setClickingTogglesState(true);
    settingsMenuButton.setColour(DrawableButton::backgroundOnColourId, Colour::fromFloatRGBA(0, 0, 0, 0));
    settingsMenuButton.setImages(sclinLogoDrawableNormal.get(), nullptr, nullptr, nullptr, sclinLogoDrawablePressed.get());
    settingsMenuButton.addListener(this);
    addAndMakeVisible(settingsMenuButton);

    //set up buttonAttachment
        midiButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor->myTreeState, "midienable", settingsMenuButton);
    
    
    addAndMakeVisible(myRoundednessSlider);
    
    
    roundnessSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "roundness", myRoundednessSlider);
    
    freezeIconDrawablePressed = Drawable::createFromSVGFile(freezeIconSVG);
    freezeIconDrawablePressed->setTransformToFit(Rectangle<float>(0, 0, 100, 100), RectanglePlacement::fillDestination);
    //freezeIconDrawablePressed->replaceColour(Colour::fromFloatRGBA(0, 0, 0, 0), myLookAndFeel.darkTrim);
    
    freezeIconDrawablePressed = Drawable::createFromSVGFile(freezeIconSVG);
    freezeIconDrawablePressed->setTransformToFit(Rectangle<float>(0, 0, 100, 100), RectanglePlacement::fillDestination);
    freezeIconDrawablePressed->replaceColour(Colour::fromFloatRGBA(0, 0, 0, 1), Colour::fromFloatRGBA(1, 1, 1, 1));
    
    
    freezeButton.setClickingTogglesState(true);
    freezeButton.setColour(DrawableButton::backgroundOnColourId, Colour::fromFloatRGBA(0, 0, 0, 0));
    freezeButton.setImages(freezeIconDrawableNormal.get(), nullptr, nullptr, nullptr, freezeIconDrawablePressed.get());
    addAndMakeVisible(freezeButton);
    
    freezeButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor->myTreeState, "freeze", freezeButton);
    
    

    addAndMakeVisible(scanlineVisualizer);
    
    
    addAndMakeVisible(connectingLine1);
    addAndMakeVisible(connectingLine2);
    addAndMakeVisible(connectingLine3);
    
    
    
    
    //set up the width/height knobs

    
    pixelKnobSettings(widthKnob); //settings for x and y pixel count knobs
    knobSettings(widthKnob); //settings for all knobs
    widthKnob.setTextValueSuffix (" px");
    
        
    pixelKnobSettings(xCyclesKnob);
    knobSettings(xCyclesKnob);
    xCyclesKnob.setTextValueSuffix (" cycle"); //please please please change. 'cycles' is not very descriptive.

    buttonClicked(&settingsMenuButton);
    
    pixelKnobSettings(heightKnob); //settings for x and y pixel count knobs
    knobSettings(heightKnob); //settings for all knobs
    heightKnob.setTextValueSuffix (" px");
    
    
    //set up x and y pixels labels.
    addAndMakeVisible(XPixels);
    XPixels.setText("X PIXELS", dontSendNotification);
    XPixels.setJustificationType(Justification::centred);
    XPixels.setFont(myLookAndFeel.goodFont);

    addAndMakeVisible(YPixels);
    YPixels.setText("Y PIXELS", dontSendNotification);
    YPixels.setJustificationType(Justification::centred);
    YPixels.setFont(myLookAndFeel.goodFont);
    
    //set up sliderAttachment objects
    xPixelsSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "xpixels", widthKnob);
    xCyclesSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "xcycles", xCyclesKnob);
    yPixelsSliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "ypixels", heightKnob);
    
    //colorMode is the toggleButton which switches between RGB and HSV modes.
    addAndMakeVisible(colorMode);
    colorMode.addListener(this);
    colorMode.setLookAndFeel(&myLookAndFeel);
    
    colorModeButtonAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(processor->myTreeState, "colormode", colorMode);
    
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


    
    channelMode1ComboAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor->myTreeState, "channelmode1", channelMode1);
    channelMode2ComboAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor->myTreeState, "channelmode2", channelMode2);
    channelMode3ComboAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor->myTreeState, "channelmode3", channelMode3);
    
    
    //colourChannelKnobSettings(channelKnob1);
    knobSettings(channelKnob1);
    
    //colourChannelKnobSettings(channelKnob2);
    knobSettings(channelKnob2);
    
    //colourChannelKnobSettings(channelKnob3);
    knobSettings(channelKnob3);
    
    channelKnob1SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "channelknob1", channelKnob1);
    channelKnob2SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "channelknob2", channelKnob2);
    channelKnob3SliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor->myTreeState, "channelknob3", channelKnob3);
    
    //local pointers to midi data
    localCurrentFreqPointer = CurrentFreqPointer;
    localPreviousFreq = *localCurrentFreqPointer;
    localIsOnPointer = isOnPointer;
    
    
    imgPixels = xImgPixels * heightKnob.getValue(); //at the end so the heightKnob will get connected to the parameter first

};




void MainComponent::timerCallback() {
    repaint();
    scanlineVisualizer.repaint();
};




MainComponent::~MainComponent() {
};






void MainComponent::paint (Graphics& g) {

    
    if (*localIsOnPointer && localPreviousFreq != *localCurrentFreqPointer && settingsMenuButton.getToggleState()) {
        
        xImgPixels = int(*localCurrentFreqPointer * int(xCyclesKnob.getValue()));
        imgPixels = xImgPixels * heightKnob.getValue();
        theSound.clear();
        theSound.setSize(2, imgPixels, false, true, true);
        
        localPreviousFreq = *localCurrentFreqPointer;
        
    }
    
    g.fillAll (myLookAndFeel.backgroundColour);   // clear the background

    /*
    // simple display of incoming midi values
    if (*localIsOnPointer){
        g.setColour (Colours::white);
        g.drawFittedText(std::to_string(*localCurrentFreqPointer), 0, 0, 200, 50, 50, .2);
    }
    */
//    drawLines(g);
    
    xDrawPixels = pluginWidth * offsetPercentInverse;
    yDrawPixels = pluginWidth * offsetPercentInverse;
        
    

    
    
    
 //   xImgPixels = widthKnob.getValue() * 4000;
 //   yImgPixels = heightKnob.getValue() * 4000;
    
 //   imgPixels = xImgPixels * yImgPixels;
    //moved that into the sliderValueChanged() function aswell


    
    

    
    
    
//    AudioBuffer<float> theSound(2, imgPixels * averageNumber);
    
    
    if (buffer->availableSamples() >= imgPixels && !pointerToAudioProcessor->myTreeState.getParameter("freeze")->getValue()) {
        theSound.clear();
        theSound.setSize(2, imgPixels * averageNumber, false, true, true);
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
    //int XOffset = ((pluginWidth - ((xDrawPixels/xImgPixels) * xImgPixels)) / 2);
    
    xPixelSize = xDrawPixels / float(xImgPixels);
    yPixelSize = yDrawPixels / float(heightKnob.getValue());
    
    
    //DRAW A very cool border
    outerSquare = Rectangle <float> (Offset - displayBorderPixels, Offset - displayBorderPixels, xDrawPixels + displayBorderPixels * 2, yDrawPixels + displayBorderPixels * 2);
    

    //int cornerSize = (std::min(xPixelSize, yPixelSize) / 2) * (1 + (pointerToAudioProcessor->myTreeState.getParameter("roundness")->getValue() * -1));
    
    int xCornerSize = (xPixelSize / 2) * (1 + (pointerToAudioProcessor->myTreeState.getParameter("roundness")->getValue() * -1));
    
    int yCornerSize = (yPixelSize / 2) * (1 + (pointerToAudioProcessor->myTreeState.getParameter("roundness")->getValue() * -1));
    
    if (xCornerSize < 0) //make sure it is positive
        xCornerSize = 0;
    if (yCornerSize < 0) //make sure it is positive
        yCornerSize = 0;
        
    Path borderPath;
    
    if (pointerToAudioProcessor->myTreeState.getParameter("roundness")->getValue() == 1.0) {
        //rectangle border.
        borderPath.addRectangle(outerSquare);
        
    }
    else if (pointerToAudioProcessor->myTreeState.getParameter("roundness")->getValue() == 0.0) {
        //yes i used rounded rectangles for the ellipse case, one edge may need to be straight while the other is fully rounded.
        borderPath.addRoundedRectangle(outerSquare, xCornerSize, yCornerSize);
    }
    else {
        //rounded rectangle border.
        borderPath.addRoundedRectangle(outerSquare, std::min(xCornerSize, yCornerSize));
    }
    g.setColour(myLookAndFeel.pointerColourDarker);
    //draw border
    g.strokePath(borderPath, PathStrokeType(2));
    


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
    

    
    float knobRescale = pluginWidth * (1.0/600.0);

    displayBorderPixels = Offset * displayBorderFactor;


    settingsMenuButton.setBounds(Offset * 2, displayBorderPixels, pluginWidth - (Offset * 4), Offset - (displayBorderPixels * 3));

    
    myRoundednessSlider.setBounds(
                                  settingsMenuButton.getX() + settingsMenuButton.getHeight() + (Offset / 20),
                                  settingsMenuButton.getY() + (Offset / 20),
                                  settingsMenuButton.getHeight() - (Offset / 10),
                                  settingsMenuButton.getHeight() - (Offset / 10));
    
    freezeButton.setBounds(settingsMenuButton.getX() + settingsMenuButton.getWidth() - (settingsMenuButton.getHeight() * 2) - (Offset / 20),
                           settingsMenuButton.getY() + (Offset / 20),
                           (settingsMenuButton.getHeight() - (Offset / 10)) * freezeIconAspect.x,
                           settingsMenuButton.getHeight() - (Offset / 10));
    
    
    scanlineVisualizer.setBounds(Offset, Offset, pluginWidth - (Offset * 2), pluginWidth - (Offset * 2));
    
    
//    widthKnob.setBounds(Offset, (yPixelSize * yImgPixels) + Offset, knobSize * knobRescale, knobSize * knobRescale);
    
    XPixels.setBounds(Offset,
                      yDrawPixels + (Offset * 1.2),
                      100 * knobRescale,
                      25 * knobRescale);
    
    widthKnob.setBounds(Offset, XPixels.getY() + XPixels.getHeight() - (5 * knobRescale), knobSize * knobRescale, knobSize * knobRescale);
    
    xCyclesKnob.setBounds(Offset, XPixels.getY() + XPixels.getHeight() - (5 * knobRescale), knobSize * knobRescale, knobSize * knobRescale);
    
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
        d->replaceColour(Colour::fromFloatRGBA(1, 0, 0, 1), hy); //color for the hyphen
        d->replaceColour(Colour::fromFloatRGBA(0, 1, 0, 1), logo); //color for the main text
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
    
    
};


void MainComponent::pixelKnobSettings (Slider& s) {
        //lambda function to stop it from showing decimals
        s.textFromValueFunction = [] (double val){
            int i = static_cast<int>(val);
            return String(i);
        };
    //    s.setNumDecimalPlacesToDisplay(1);
    
    //s.setRange(pointerToAudioProcessor->minPixels, pointerToAudioProcessor->maxPixels);
    
};


/*

void MainComponent::colourChannelKnobSettings (Slider& s) {
    

    //s.setRange(-1 * pointerToAudioProcessor->maxMult, pointerToAudioProcessor->maxMult);
    s.setNumDecimalPlacesToDisplay(1);
    s.setTextValueSuffix (" %");
     
    
};
 */



void MainComponent::sliderValueChanged (Slider* s) {
    if (s == &heightKnob){
        imgPixels = xImgPixels * s->getValue();
        theSound.clear();
        theSound.setSize(2, imgPixels);
    }
    else if (s == &widthKnob){
        xImgPixels = int(s->getValue());
        //*pointerToAudioProcessor->xPixelsParam = xImgPixels;
        imgPixels = xImgPixels * heightKnob.getValue();
        theSound.clear();
        theSound.setSize(2, imgPixels);
    }
    else if (s == &xCyclesKnob){
        
    }
    else if (s == &channelKnob1){
        channelKnobValue[0] = (s->getValue() / 100); //percentage perschmentage.
    }
    else if (s == &channelKnob2){
        channelKnobValue[1] = (s->getValue() / 100);
    }
    else if (s == &channelKnob3){
        channelKnobValue[2] = (s->getValue() / 100);
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
    if (b == &settingsMenuButton) {
        if (settingsMenuButton.getToggleState()) {//true means the button is 'pressed down' and midi should be on.
            widthKnob.setVisible(false);
            xCyclesKnob.setVisible(true);

            //widthKnob.setRange(pointerToAudioProcessor->minCycles, pointerToAudioProcessor->maxCycles);
            //widthKnob.setValue(*(pointerToAudioProcessor->xCyclesParam));
            XPixels.setText("X CYCLES", dontSendNotification);
            //ui when midi is enabled. display a whole number of wavecycle(s) (based on midi nore) per line (x-axis).
        }
        else {
            widthKnob.setVisible(true);
            xCyclesKnob.setVisible(false);
            //widthKnob.setRange(pointerToAudioProcessor->minPixels, pointerToAudioProcessor->maxPixels);
            //widthKnob.setValue(*pointerToAudioProcessor->xPixelsParam);
            XPixels.setText("X PIXELS", dontSendNotification);
        }
    }
}


void MainComponent::buttonStateChanged (Button* b) {

    if (b == &colorMode) {
        //*pointerToAudioProcessor->colorModeParam = colorMode.getToggleState();
        if (!b->getToggleState())
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
