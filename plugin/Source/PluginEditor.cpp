#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DoomAudioProcessorEditor::DoomAudioProcessorEditor (DoomAudioProcessor& p_)
    : ProcessorEditor (p_), proc (p_)
{
    addAndMakeVisible (doom);
    setSize (640, 400);

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
