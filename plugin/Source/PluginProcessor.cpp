#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DoomAudioProcessor::DoomAudioProcessor()
{
}

DoomAudioProcessor::~DoomAudioProcessor()
{
}

//==============================================================================
void DoomAudioProcessor::stateUpdated()
{
}

void DoomAudioProcessor::updateState()
{
}

//==============================================================================
void DoomAudioProcessor::reset()
{
    Processor::reset();
}

void DoomAudioProcessor::prepareToPlay (double newSampleRate, int newSamplesPerBlock)
{
    Processor::prepareToPlay (newSampleRate, newSamplesPerBlock);
}

void DoomAudioProcessor::releaseResources()
{
}

void DoomAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
}

//==============================================================================
bool DoomAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* DoomAudioProcessor::createEditor()
{
    return new DoomAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DoomAudioProcessor();
}
