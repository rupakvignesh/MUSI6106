/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "MeterComponent.h"
//==============================================================================
/**
*/
class PpmAudioProcessorEditor  : public AudioProcessorEditor, private Timer
{
public:
    PpmAudioProcessorEditor (PpmAudioProcessor&);
    ~PpmAudioProcessorEditor();
    void timerCallback() override;
    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PpmAudioProcessor& processor;
    MeterComponent m_meterComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PpmAudioProcessorEditor)
};
