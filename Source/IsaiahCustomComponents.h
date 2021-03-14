/*
  ==============================================================================

    IsaiahCustomComponents.h
    Created: 3 Aug 2020 8:22:25pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


class ConnectingLine : public Component {
public:
    const float ratioFilled = 3;
    
    Colour centerColor;
    Colour backgroundColor;
    
    ConnectingLine(Colour A, Colour B);
    ~ConnectingLine();
    void paint(Graphics& g);
//    void resized();
//    void setBounds(int setX, int setY, int setWidth, int setHeight);
    
};





class RoundednessSlider : public Slider {
public:
    
    RoundednessSlider(Colour A, Colour B);
    ~RoundednessSlider();
    void paint(Graphics& g);
    void resized();
    void valueChanged();
    
    
private:
    Rectangle<float> drawDimensions;
    float currentValue;
    const float squareRatio = 0.1;
    int lineThickness = 2;
    Colour squareColour;
    Colour backgroundColour;
};
