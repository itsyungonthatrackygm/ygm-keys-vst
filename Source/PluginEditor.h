#pragma once
#include "PluginProcessor.h"

class YGMKeysAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit YGMKeysAudioProcessorEditor (YGMKeysAudioProcessor&);
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    YGMKeysAudioProcessor& processor;
    juce::Label title;
    juce::Slider attack, release, gain;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment, releaseAttachment, gainAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YGMKeysAudioProcessorEditor)
};
