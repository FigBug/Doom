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
    juce::ignoreUnused (midi);
    juce::ScopedNoDenormals noDenormals;

    juce::ScopedLock sl (lock);
    buffer.clear();

    for (auto e :engines)
        e->processBlock (buffer, (int)getSampleRate());
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

void DoomAudioProcessor::registerEngine (gin::DoomAudioEngine& e)
{
    juce::ScopedLock sl (lock);
    engines.add (&e);
}

void DoomAudioProcessor::unregisterEngine (gin::DoomAudioEngine& e)
{
    juce::ScopedLock sl (lock);
    engines.removeFirstMatchingValue (&e);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DoomAudioProcessor();
}
