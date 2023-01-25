#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DoomAudioProcessorEditor::DoomAudioProcessorEditor (DoomAudioProcessor& p_)
    : ProcessorEditor (p_), proc (p_)
{
    addAndMakeVisible (doom);
    setSize (640, 400);
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

    doom.setBounds (getLocalBounds());
}
