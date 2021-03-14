/*
  ==============================================================================

    MyLookAndFeel.h
    Created: 30 Jul 2020 2:58:03pm
    Author:  Isaiah Sugar

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>



class MyLookAndFeel : public LookAndFeel_V4
{
    public:

    //define some colors
    Colour darkTrim = Colour::fromHSV(0.0, 0.0, 0.07, 1.0);
    Colour pointerColour = Colour::fromHSV(0.0f, 0.0f, 0.2f, 1.0f);
    Colour backgroundColour = Colour::fromHSV (0.0f, 0.0f, 0.24f, 1.0f);
    Colour pointerColourLighter = Colour::fromHSV(0.0f, 0.0f, 0.25f, 1.0f);
    Colour backgroundColourDarkerish = Colour::fromHSV(0.0f, 0.0f, 0.37f, 1.0f);
    Colour backgroundColourDarker = Colour::fromHSV(0.0f, 0.0f, 0.42f, 1.0f);
    Colour nearBackgroundColour = Colour::fromHSV(0.0f, 0.0f, 0.44f, 1.0f);
    Colour buttonBackgroundColour = Colour::fromHSV(0.0f, 0.0f, 0.45f, 1.0f);
    Colour greyedTextColour = Colour::fromHSV(0.0f, 0.0f, 0.65f, 1.0f);
    Colour mainTextColour = Colour::fromHSV(0.0f, 0.0f, 0.95f, 1.0f);
    
    
    Colour pointerColourDarker = pointerColour.darker(.13);
    
    
    //KNOB DRAWING CODE
    void drawRotarySlider(Graphics &g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, Slider &s) override
    {

        float diameter =.95 * jmin(width, height);
        float radius = diameter / 2.0;
        float thickness = 7 * (diameter / 100);
        float halfThickness = thickness / 2;
        float centerX = (width / 2) + x;
        float centerY = (height / 2) + y;
        float angle = M_PI + rotaryStartAngle + (sliderPos * (rotaryEndAngle - rotaryStartAngle));

        
        Rectangle<float> knobRect((centerX - radius), (centerY - radius), diameter, diameter);
        
        
        Path pointer;
        pointer.addRectangle(0 - halfThickness, 0 , thickness, radius);
        pointer.addEllipse(0 - halfThickness, radius - halfThickness, thickness, halfThickness);
//        pointer.addTriangle(0 - (thickness * 5), radius - (thickness * 5), 0 + (thickness * 5), radius - (thickness * 5), 0, radius + (thickness * 5));
        
        
//        ColourGradient(buttonBackgroundColour, centerX, centerY, backgroundColourDarkerish, centerX - (radius * .8), centerY + (radius * .8), false);
        
        
        
        ColourGradient knobCenterGradient = ColourGradient (buttonBackgroundColour , centerX, centerY, buttonBackgroundColour.darker(.3), x, centerY, true);
        knobCenterGradient.addColour (.25, knobCenterGradient.getColour(0));
        //g.setColour(buttonBackgroundColour);
        g.setGradientFill (knobCenterGradient);
        g.fillEllipse (knobRect);
        
        /*
        g.setColour(nearBackgroundColour);
        g.drawEllipse ((centerX - radius) + (thickness * 2.5), (centerY - radius) + (thickness * 2.5), diameter - (thickness * 5), diameter - (thickness * 5), thickness);
        g.setColour(backgroundColourDarker);
        g.drawEllipse ((centerX - radius) + (thickness * 1.1), (centerY - radius) + (thickness * 1.1), diameter - (thickness * 2.2), diameter - (thickness * 2.2), thickness * 1.5);
        */
        g.setColour(pointerColour);
        g.drawEllipse (knobRect, halfThickness);
        
        
        
        
        g.setColour(pointerColour);
        g.fillPath(pointer, AffineTransform::rotation(angle).translated(centerX, centerY));
        g.fillEllipse(centerX - halfThickness, centerY - halfThickness, thickness, thickness);
        //that last ellipse is to make sure there's not a rectangular edge
        //in the middle of the slider. comment it out to see exactly what it does.
        
    };
        Label* createSliderTextBox (Slider& slider) override
    {
        Label* l = LookAndFeel_V3::createSliderTextBox (slider);
        l->setFont(goodFont);
        return l;
    }

    
    
    //DROPDOWN DRAWING CODE
    void drawComboBox (Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, ComboBox& c) override
    {
        c.setOpaque(true);
        float Thickness =  (buttonH / 4);
        
        if (isButtonDown){
            g.setColour(backgroundColourDarkerish);
//            g.fillRect((buttonX + buttonW) - width, buttonY, width, buttonH);
//            g.setColour(backgroundColourDarker);
//            g.drawRect((buttonX + buttonW) - width, buttonY, width, buttonH, int(Thickness));
//            g.setColour(buttonBackgroundColour);
//            g.drawRect((buttonX + buttonW) - width, buttonY, width, buttonH, 2);
            
            
            
            g.setColour(pointerColourLighter);
        }
        else{
            g.setColour(buttonBackgroundColour);
            g.fillRect((buttonX + buttonW) - width, buttonY, width, buttonH);
            
            
            g.setColour(pointerColour);
        }

        
        Path arrow;
        /*
        Line (buttonX, buttonY - (buttonH / 6), buttonX + buttonW, buttonY - (buttonH / 6));
        arrow.addLineSegment(, halfThickness);
        g.drawLine(buttonX + buttonW, buttonY, buttonX + buttonW, buttonY, halfThickness);
        */
        
        arrow.addTriangle(
                        buttonX, buttonY + Thickness,
                        buttonX + buttonW - Thickness, buttonY + Thickness,
                        buttonX + ((buttonW - Thickness) / 2), (buttonY + buttonH) - (Thickness * 1.5));
        g.fillPath(arrow);
        
        g.setColour(pointerColour);
        g.drawRect((buttonX + buttonW) - width, buttonY, width, buttonH, 1);
        
    };
    
    
    
    
    //set font for dropdown
        Font getComboBoxFont (ComboBox& /*box*/) override {
        return goodFont;
    }
    
    Font getPopupMenuFont() override {
        return goodFont;
    }
    

    
    
    void drawToggleButton(Graphics &g, ToggleButton &b, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override {


        Rectangle<int> myBounds = b.getLocalBounds();
        Rectangle<float> smallerRectangle (
                        myBounds.getX() + (myBounds.getWidth() / 6),
                        myBounds.getY() + (myBounds.getHeight() / 8),
                        (myBounds.getWidth() * 2) / 3,
                        (myBounds.getHeight() * 3) / 4);
        Point<int> circleCenterPoint;
        Point<int> gradientPoint;
        int currentButtonHeight;
        float thickness = 4 * (smallerRectangle.getHeight() / 20);
        float shadowOffset = 1;
        float falloff = 1;
        
        //g.setColour(Colours::red);
        //g.drawRect(myBounds); //REMOVE THIS, MEANT ONLY FOR DEBUG
        
        circleCenterPoint.setX(smallerRectangle.getX() + (smallerRectangle.getWidth() / 2));
        gradientPoint.setX(circleCenterPoint.getX());
        
        if (b.getToggleState())
        {
            currentButtonHeight = smallerRectangle.getY() + smallerRectangle.getHeight() - smallerRectangle.getWidth();
            circleCenterPoint.setY (currentButtonHeight - (smallerRectangle.getWidth() / 2) - (thickness * shadowOffset));
//            gradientPoint.setY(circleCenterPoint.getY() - (thickness * falloff));
            gradientPoint.setY(circleCenterPoint.getY() - (smallerRectangle.getWidth() * falloff));
        }
        else
        {
            currentButtonHeight = smallerRectangle.getY();
            circleCenterPoint.setY(currentButtonHeight + (smallerRectangle.getWidth() / 2) + (thickness * shadowOffset));
//            gradientPoint.setY(circleCenterPoint.getY() + (thickness * falloff));
            gradientPoint.setY(circleCenterPoint.getY() + (smallerRectangle.getWidth() * falloff));
            
        }
        
        
        g.setGradientFill(ColourGradient (
                                pointerColourDarker,
//                                Colours::white,
                                smallerRectangle.getX() + (smallerRectangle.getWidth() / 2),
                                currentButtonHeight + smallerRectangle.getWidth() / 2,
                                pointerColour,
//                                Colours::black,
                                smallerRectangle.getX() + (smallerRectangle.getWidth() / 2),
                                currentButtonHeight + (smallerRectangle.getWidth() / 2) + thickness * 2,
                                true));
        
        
        
        g.fillRoundedRectangle (smallerRectangle, smallerRectangle.getWidth() / 2);
        
        
        
        g.setColour(buttonBackgroundColour);
        g.drawEllipse(smallerRectangle.getX(),
                      currentButtonHeight,
                      smallerRectangle.getWidth(),
                      smallerRectangle.getWidth(),
                      2);
        g.setColour(pointerColourLighter);
        g.fillEllipse(myBounds.getX() + (myBounds.getWidth() / 6),
                      currentButtonHeight,
                      smallerRectangle.getWidth(),
                      smallerRectangle.getWidth());
        
        
    };
     
    
    
    //              do not shame my font
    Font goodFont = Font("Tahoma", 18, Font::plain); //this font is also used in mainComponent btw
    

};
