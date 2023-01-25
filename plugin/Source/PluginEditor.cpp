#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DoomAudioProcessorEditor::DoomAudioProcessorEditor (DoomAudioProcessor& p_)
    : ProcessorEditor (p_), proc (p_)
{
}

DoomAudioProcessorEditor::~DoomAudioProcessorEditor()
{
}

//==============================================================================
void DoomAudioProcessorEditor::paint (juce::Graphics& g)
{
    ProcessorEditor::paint (g);
}

void DoomAudioProcessorEditor::resized()
{
    ProcessorEditor::resized ();
}
