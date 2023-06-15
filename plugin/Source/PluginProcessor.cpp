#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DoomAudioProcessor::DoomAudioProcessor()
{
	auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory);
   #if JUCE_MAC
	auto wad = dir.getChildFile ("Application Support/jDoom/DOOM1.WAD");
   #else
	auto wad = dir.getChildFile ("jDoom/DOOM1.WAD");
   #endif

	if (! wad.existsAsFile())
	{
		wad.getParentDirectory().createDirectory();
		wad.replaceWithData (BinaryData::DOOM1_WAD, BinaryData::DOOM1_WADSize);
	}

	doom.startGame (wad);
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

    buffer.clear();
	doom.getAudioEngine().processBlock (buffer, (int)getSampleRate());
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
