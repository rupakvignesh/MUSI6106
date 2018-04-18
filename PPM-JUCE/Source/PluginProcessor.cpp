/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <iostream>
//==============================================================================
PpmAudioProcessor::PpmAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        m_bIsready(false),
                        m_fMaxSinceLastCall(0),
                       m_pCPpm(0)

#endif
{
    CPpm::createInstance(m_pCPpm);
}

PpmAudioProcessor::~PpmAudioProcessor()
{
    m_pCPpm->destroyInstance(m_pCPpm);
    m_pCPpm = NULL;
}

//==============================================================================
const String PpmAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PpmAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PpmAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PpmAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PpmAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PpmAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PpmAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PpmAudioProcessor::setCurrentProgram (int index)
{
}

const String PpmAudioProcessor::getProgramName (int index)
{
    return {};
}

void PpmAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PpmAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    m_pCPpm->init(sampleRate, getTotalNumInputChannels(), 0.01, 1.5);
    m_bIsready = true;
}

void PpmAudioProcessor::releaseResources()
{
    //m_pCPpm->destroyInstance(m_pCPpm);
    //m_pCPpm = NULL;
    
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PpmAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void PpmAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    
    const float** ppfReadPointer = buffer.getArrayOfReadPointers();
    m_pCPpm->process(ppfReadPointer, buffer.getNumSamples());
//    m_set.insert(m_pCPpm->getMaxPpm());
    if(m_fMaxSinceLastCall<m_pCPpm->getMaxPpm()){
        m_fMaxSinceLastCall = m_pCPpm->getMaxPpm();
    }
    //std::cout << 20*log10f(m_pCPpm->getMaxPpm()) << std::endl;
}

//==============================================================================
bool PpmAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* PpmAudioProcessor::createEditor()
{
    return new PpmAudioProcessorEditor (*this);
}

//==============================================================================
void PpmAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void PpmAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
float PpmAudioProcessor::getMaxSinceLastCall() {
    
    float result = m_fMaxSinceLastCall;
    m_fMaxSinceLastCall = 0.0;
    return result;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PpmAudioProcessor();
}
