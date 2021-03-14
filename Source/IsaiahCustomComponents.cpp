/*
  ==============================================================================

    IsaiahCustomComponents.cpp
    Created: 3 Aug 2020 8:31:41pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#include <JuceHeader.h>
#include "IsaiahCustomComponents.h"




ConnectingLine::ConnectingLine(Colour A, Colour B) {
    centerColor = A;
    backgroundColor = B;
}


ConnectingLine::~ConnectingLine() {
    
}


void ConnectingLine::paint(Graphics& g) {
    
    int centerStart = getWidth() / 2;
    int centerEnd = getWidth() - centerStart;
    
    
    g.setGradientFill(ColourGradient(
                                     centerColor,
                                     centerStart,
                                     0,
                                     backgroundColor,
                                     0,
                                     0,
                                     false));
    
    g.fillRect(0, 0, centerStart, getHeight());
    
    g.setGradientFill(ColourGradient(
                                     centerColor,
                                     centerEnd,
                                     0,
                                     backgroundColor,
                                     getWidth(),
                                     0,
                                     false));
    
    g.fillRect(centerEnd, 0, getWidth(), getHeight());
    
//    g.setColour(centerColor);
//    g.fillRect(centerStart, 0, centerEnd, getHeight());
    
    /*
    g.setColour(juce::Colours::red);
    g.drawRect(0, 0, getWidth(), getHeight());
    g.drawRect(centerStart, 0.0, centerEnd, float(getHeight()));
    */
    
//    std::printf("x: %d y: %d width: %f height: %d centerstart: %d centerend: %f \n", getX(), getY(), float(getWidth()), getHeight(), centerStart, centerEnd);
}
/*
void ConnectingLine::resized() {
    
    
}
 
void ConnectingLine::setBounds(int setX, int setY, int setWidth, int setHeight) {
    
    
}
*/






RoundednessSlider::RoundednessSlider(Colour A, Colour B) {
    this->setSliderStyle(RotaryHorizontalVerticalDrag);
    this->setTextBoxStyle(NoTextBox, true, 4, 2); //since its set to have NoTextBox, the other arguments don't matter, but something has to be there.
    this->setMouseDragSensitivity(80);
    this->setRange(0, 1);
    this->setValue(1.0);
    
    currentValue = this->getValue();
    squareColour = A;
    backgroundColour = B;
    
}

RoundednessSlider::~RoundednessSlider() {
}



void RoundednessSlider::paint(Graphics& g) {
    /*
    draw value as text, for testing
    g.setFont(Font(20.0, 0));
    const float currentValue = this->getValue();
    g.drawText (std::to_string(currentValue), 0, 0, getWidth(), getHeight(), Justification::centred, true);

     print value, for testing
     std::printf("%f", this->getValue());
    */
    
    Path squarePath;
    
    if (currentValue == 1.0)
        squarePath.addRectangle(drawDimensions);
    else if (currentValue == 0.0)
        squarePath.addEllipse(drawDimensions);
    else
        squarePath.addRoundedRectangle(drawDimensions, (1 + (currentValue * -1)) * (getHeight() / 2));
         
    g.setColour(squareColour);
    g.strokePath(squarePath, PathStrokeType(lineThickness));
    
}

void RoundednessSlider::resized() {
    drawDimensions.setBounds(lineThickness / 2, lineThickness / 2, getWidth() - lineThickness, getHeight() - lineThickness);
}

void RoundednessSlider::valueChanged() {
    currentValue = this->getValue();
}
