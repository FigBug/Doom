#pragma once

#include <JuceHeader.h>

//==============================================================================
class DoomAudioProcessor : public gin::Processor
{
public:
    //==============================================================================
    DoomAudioProcessor();
    ~DoomAudioProcessor() override;

    void stateUpdated() override;
    void updateState() override;

    void registerEngine (gin::DoomAudioEngine&);
    void unregisterEngine (gin::DoomAudioEngine&);

    //==============================================================================
    void reset() override;
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

private:
    juce::CriticalSection lock;
    juce::Array<gin::DoomAudioEngine*> engines;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DoomAudioProcessor)
};
