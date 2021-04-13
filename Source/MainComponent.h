/*
  ==============================================================================

    MainComponent.h
    Created: 18 Feb 2020 5:24:02pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "RingBuffer.h"
#include "MyLookAndFeel.h"
#include "IsaiahCustomComponents.h"

//==============================================================================









class MainComponent  final  : public Component, public Slider::Listener, public Button::Listener, public ComboBox::Listener, private Timer
{
public:
    MainComponent(AudioProcessor& processor, RingBuffer<float>*, int*, bool*);
    ~MainComponent();

    
    void paint (Graphics& g) override;
    void resized() override;
    void updateLogoColor(Drawable* d, DrawableButton* b, Colour logo, Colour hy);
    void drawLines (Graphics& g);
    

    void  timerCallback() override;
    
    
    //these 'settings' functions are just me being lazy, they set things up for multiple objects which need to look or function similarly.
    
    void knobSettings (Slider& s);
    void pixelKnobSettings (Slider& s);
    void colourChannelKnobSettings (Slider& s);
    
    int knobSize = 100;
    void sliderValueChanged (Slider* slider) override;
    
    void comboBoxChanged (ComboBox *c) override;
    
    void buttonClicked (Button* b) override;
    void buttonStateChanged (Button* b) override;
    
    
    void channelModeComboBoxInit (ComboBox& c);
    void comboBoxSettings (ComboBox& c);
    
//    void drawColorModeButton(Graphics& g, TextButton& b);
    
    float ratio = .74;
    int componentDefaultSize = 550;
    
    int pluginWidth = componentDefaultSize;
    int pluginHeight = componentDefaultSize / ratio;
    
    static const int maxPixels = 256; //maximum of each pixel knob (x pixels / y pixels)
    static const int minPixels = 1;
    static const int defaultPixels = 32;

    int xImgPixels = defaultPixels;
    int yImgPixels = defaultPixels;
    int imgPixels = xImgPixels * yImgPixels;
    
    static const int averageNumber = 1;
    
    const float offsetPercent = 0.1;
    const float offsetPercentInverse = (offsetPercent * -2) + 1;
    int Offset = offsetPercent * pluginWidth;

    
    
    //=========  o b j e c t   d e c l a r a t i o n s  ============
    
    MyLookAndFeel myLookAndFeel;
    

    
    const File MainDir = File("/Users/isaiahsugar/programming/audio/plugins/sc-lin/sc-lin"); // CHANGE THIS TO THE SC-LIN DIRECTORY!! I DO NOT KNOW WHAT I AM DOING SO THIS IS MY SOLUTION!! CHANGE THIS!!
    
   // const File testFileThing = MainDir.getChildFile ("sclin full logo seperate colors.svg");
    
    const File sclinLogoSeperateColorsSVG = MainDir.getChildFile ("Source/sclin full logo seperate colors.svg");
    const File freezeIconSVG = MainDir.getChildFile("Source/snowflake.svg");
    const Point<float> freezeIconWH = Point<float>(167.00723, 145.31050); //width and height of freezeIconSVG
    const Point<float> freezeIconAspect = Point<float>(freezeIconWH.x / freezeIconWH.y, 1);

    std::unique_ptr<Drawable> sclinLogoDrawableNormal =  Drawable::createFromSVGFile(sclinLogoSeperateColorsSVG);
    std::unique_ptr<Drawable> sclinLogoDrawablePressed = Drawable::createFromSVGFile(sclinLogoSeperateColorsSVG);
    
    std::unique_ptr<Drawable> freezeIconDrawableNormal = Drawable::createFromSVGFile(freezeIconSVG);
    std::unique_ptr<Drawable> freezeIconDrawablePressed = Drawable::createFromSVGFile(freezeIconSVG);
    
    DrawableButton settingsMenuButton;
    
    RoundednessSlider myRoundednessSlider;
    DrawableButton freezeButton;
    

    
    Slider widthKnob;
    Label XPixels;
    Slider heightKnob;
    Label YPixels;
    
    
    ToggleButton colorMode;
    Label RGBLabel;
    Label HSVLabel;

    
    Rectangle<float> notRoundedLabelBounds;
    Rectangle<float> roundedLabelBounds;
    
    
    Label channelLabel1;
    Label channelLabel2;
    Label channelLabel3;
    
    ComboBox channelMode1;
    ComboBox channelMode2;
    ComboBox channelMode3;
    
    Slider channelKnob1;
    Slider channelKnob2;
    Slider channelKnob3;
    
    ConnectingLine connectingLine1;
    ConnectingLine connectingLine2;
    ConnectingLine connectingLine3;

    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    String currentSizeAsString;
    RingBuffer<float>* buffer;

    
    bool isRounded = false;
    
    
    int cornerSize = Offset / 10;
    
    
    float xDrawPixels = pluginWidth * offsetPercentInverse;
    float yDrawPixels = pluginWidth * offsetPercentInverse;
    


    float xPixelSize = xDrawPixels/xImgPixels;
    float yPixelSize = yDrawPixels/yImgPixels;
    
    float addXPixels;
    float addYPixels;

    int channelMode[3] = {1, 1, 1};
    float channelKnobValue[3] = {1.0, 1.0, 1.0};
    
    const float displayBorderFactor = 0.1;
    float displayBorderPixels = Offset * displayBorderFactor;
    Rectangle <float> outerSquare = Rectangle <float> (Offset - (Offset * 0.1), Offset - (Offset * 0.1), int(xDrawPixels) + 1 + (Offset * 0.2), int(yDrawPixels) + 1 + (Offset * 0.2));
    
    int* localCurrentNotePointer;
    int localPreviousNote;
    bool* localIsOnPointer;
    
    //unsigned long debugSampleCountThing = 0;
    
    AudioBuffer<float> theSound = AudioBuffer<float>(2, imgPixels * averageNumber);
    
};
