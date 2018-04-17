/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PpmAudioProcessorEditor::PpmAudioProcessorEditor (PpmAudioProcessor& p)
    : AudioProcessorEditor (&p),  processor (p)
{
    setSize (220, 170);
    addAndMakeVisible(m_meterComponent);
    startTimer(10);
}

PpmAudioProcessorEditor::~PpmAudioProcessorEditor()
{
    
}

void PpmAudioProcessorEditor::timerCallback() {
    if (processor.m_bIsready)
        m_meterComponent.setValue(processor.getMaxSinceLastCall());
}

//==============================================================================
void PpmAudioProcessorEditor::paint (Graphics& g)
{
}

void PpmAudioProcessorEditor::resized()
{
    int width = 120;
    int height = 20;
    m_meterComponent.setBounds(getWidth() / 2 - width / 2, getHeight() / 2 - height / 2, width, height);
}
