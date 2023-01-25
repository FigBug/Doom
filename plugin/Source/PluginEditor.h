#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
class DoomAudioProcessorEditor : public gin::ProcessorEditor
{
public:
    DoomAudioProcessorEditor (DoomAudioProcessor&);
    ~DoomAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    DoomAudioProcessor& proc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoomAudioProcessorEditor)
};
