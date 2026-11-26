#pragma once

#include <JuceHeader.h>

class YGMKeysVoice final : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}
    void renderNextBlock (juce::AudioBuffer<float>&, int startSample, int numSamples) override;

    void setSampleRate (double rate) { sampleRate = rate; }
    void setEnvelope (float attackSeconds, float releaseSeconds);

private:
    double sampleRate = 44100.0;
    double phase = 0.0;
    double phaseDelta = 0.0;
    float level = 0.0f;
    juce::ADSR adsr;
};

class YGMKeysSound final : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

class YGMKeysAudioProcessor final : public juce::AudioProcessor
{
public:
    YGMKeysAudioProcessor();
    ~YGMKeysAudioProcessor() override = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout&) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "YGM Keys"; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}
    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;

    juce::AudioProcessorValueTreeState parameters;

private:
    juce::Synthesiser synth;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (YGMKeysAudioProcessor)
};
