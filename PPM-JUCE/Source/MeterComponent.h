/*
  ==============================================================================

    MeterComponent.h
    Created: 17 Apr 2018 12:54:16pm
    Author:  Yongliang He

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MeterComponent    : public Component
{
public:
    MeterComponent():m_fValue(0.5)
    {
    }

    ~MeterComponent()
    {
    }
    
    
    
    void paint (Graphics& g) override
    {
        g.fillAll(Colours::black);
        g.setColour(Colours::green);
        g.fillRoundedRectangle(5.0f, 5.0f, (getWidth() - 10.0f) * m_fValue, getHeight() - 10.0f, 2.0f);
    }

    void resized() override
    {
    }
    
    void setValue(float value) {
        m_fValue = value;
//        repaint();
    }

private:
    float m_fValue;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterComponent)
};
