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
#include "ScanlineVisualizerComponent.h"

#include "PluginProcessor.h" //i added this but it maybe shouldn't be here. The goal was to allow this to inherit the SclinAudioProcessor class so it could get data from the parameters.

//==============================================================================









class MainComponent  final  : public Component, public Slider::Listener, public Button::Listener, public ComboBox::Listener, private Timer
{
public:
    MainComponent(SclinAudioProcessor* processor, RingBuffer<float>*, float*, bool*);
    ~MainComponent();

    
    void paint (Graphics& g) override;
    void resized() override;
    void updateLogoColor(Drawable* d, DrawableButton* b, Colour logo, Colour hy);
    void drawLines (Graphics& g);
    

    void  timerCallback() override;
    
    
    //these 'settings' functions are just me being lazy, they set things up for multiple objects which need to look or function similarly.
    
    void knobSettings (Slider& s);
    void pixelKnobSettings (Slider& s);
//    void colourChannelKnobSettings (Slider& s);
    

    void sliderValueChanged (Slider* slider) override;
    
    void comboBoxChanged (ComboBox *c) override;
    
    void buttonClicked (Button* b) override;
    void buttonStateChanged (Button* b) override;
    
    
    void channelModeComboBoxInit (ComboBox& c);
    void comboBoxSettings (ComboBox& c);
    
//    void drawColorModeButton(Graphics& g, TextButton& b);
    
    int knobSize = 100;
    const float ratio = .74;
    const int componentDefaultSize = 550;
    
    int pluginWidth = componentDefaultSize;
    int pluginHeight = componentDefaultSize / ratio;
    



    
    int xImgPixels;
    // int yImgPixels = defaultPixels; //was used pre-parameter
    int imgPixels;
    //int keepXImgPixels = defaultPixels; //was used pre-parameter
    
    // 
    /*
    int xKnobPosMidi
    int yKnobPosMidi
    int xKnobPosNoMidi
    int yKnobPosNoMidi
     */
//    int xCycles = 1;
    
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
    
    ScanlineVisualizerComponent scanlineVisualizer;
    
    Slider widthKnob;
    Slider xCyclesKnob;
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

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midiButtonAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roundnessSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> freezeButtonAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> xPixelsSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> xCyclesSliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> yPixelsSliderAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> colorModeButtonAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> channelMode1ComboAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> channelMode2ComboAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> channelMode3ComboAttachment;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelKnob1SliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelKnob2SliderAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> channelKnob3SliderAttachment;
    
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    String currentSizeAsString;
    RingBuffer<float>* buffer;

    
    bool isRounded = false;
    
    
    float xDrawPixels = pluginWidth * offsetPercentInverse;
    float yDrawPixels = pluginWidth * offsetPercentInverse;
    


    float xPixelSize;
    float yPixelSize;
    

    int channelMode[3] = {1, 1, 1};
    float channelKnobValue[3] = {1.0, 1.0, 1.0};
    
    const float displayBorderFactor = 0.1;
    float displayBorderPixels = Offset * displayBorderFactor;
    Rectangle <float> outerSquare = Rectangle <float> (Offset - (Offset * 0.1), Offset - (Offset * 0.1), int(xDrawPixels) + 1 + (Offset * 0.2), int(yDrawPixels) + 1 + (Offset * 0.2));
    
    float* localCurrentFreqPointer;
    float localPreviousFreq;
    bool* localIsOnPointer;
    
    //unsigned long debugSampleCountThing = 0;
    
    AudioBuffer<float> theSound;
    SclinAudioProcessor* pointerToAudioProcessor; //used to store parent audioProcessor to use later
};
