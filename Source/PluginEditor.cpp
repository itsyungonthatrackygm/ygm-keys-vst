#include "PluginEditor.h"

YGMKeysAudioProcessorEditor::YGMKeysAudioProcessorEditor (YGMKeysAudioProcessor& p) : AudioProcessorEditor (&p), processor (p)
{
    setSize (420, 260);
    title.setText ("YGM KEYS", juce::dontSendNotification);
    title.setFont (juce::Font (28.0f, juce::Font::bold));
    title.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (title);
    auto setup = [this] (juce::Slider& s, const juce::String& name) { s.setSliderStyle (juce::Slider::RotaryVerticalDrag); s.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 80, 20); s.setName (name); addAndMakeVisible (s); };
    setup (attack, "Attack"); setup (release, "Release"); setup (gain, "Gain");
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.parameters, "attack", attack);
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.parameters, "release", release);
    gainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (processor.parameters, "gain", gain);
}

void YGMKeysAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (18, 18, 24));
    g.setColour (juce::Colour::fromRGB (125, 70, 220));
    g.fillRoundedRectangle (20.0f, 20.0f, 380.0f, 220.0f, 18.0f);
}

void YGMKeysAudioProcessorEditor::resized()
{
    title.setBounds (40, 35, 300, 40);
    attack.setBounds (45, 100, 100, 120);
    release.setBounds (160, 100, 100, 120);
    gain.setBounds (275, 100, 100, 120);
}

juce::AudioProcessorEditor* YGMKeysAudioProcessor::createEditor() { return new YGMKeysAudioProcessorEditor (*this); }
