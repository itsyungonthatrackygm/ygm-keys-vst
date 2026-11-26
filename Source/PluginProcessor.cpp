#include "PluginProcessor.h"
#include "PluginEditor.h"

bool YGMKeysVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<YGMKeysSound*> (sound) != nullptr;
}

void YGMKeysVoice::startNote (int note, float velocity, juce::SynthesiserSound*, int)
{
    phase = 0.0;
    level = velocity;
    phaseDelta = juce::MidiMessage::getMidiNoteInHertz (note) / sampleRate;
    adsr.noteOn();
}

void YGMKeysVoice::stopNote (float, bool allowTailOff)
{
    if (allowTailOff) adsr.noteOff();
    else { adsr.reset(); clearCurrentNote(); }
}

void YGMKeysVoice::setEnvelope (float attackSeconds, float releaseSeconds)
{
    juce::ADSR::Parameters p;
    p.attack = attackSeconds;
    p.decay = 0.15f;
    p.sustain = 0.8f;
    p.release = releaseSeconds;
    adsr.setParameters (p);
}

void YGMKeysVoice::renderNextBlock (juce::AudioBuffer<float>& output, int start, int count)
{
    while (count-- > 0)
    {
        if (! adsr.isActive()) { clearCurrentNote(); break; }
        const auto envelope = adsr.getNextSample();
        const auto sample = std::sin (juce::MathConstants<double>::twoPi * phase)
                          + 0.18 * std::sin (juce::MathConstants<double>::twoPi * phase * 2.0);
        const auto value = static_cast<float> (sample * level * envelope * 0.22);
        for (int channel = 0; channel < output.getNumChannels(); ++channel)
            output.addSample (channel, start, value);
        phase = std::fmod (phase + phaseDelta, 1.0);
        ++start;
    }
}

YGMKeysAudioProcessor::YGMKeysAudioProcessor()
    : AudioProcessor (BusesProperties().withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      parameters (*this, nullptr, "PARAMETERS", {
          std::make_unique<juce::AudioParameterFloat> ("attack", "Attack", juce::NormalisableRange<float> (0.001f, 2.0f), 0.02f),
          std::make_unique<juce::AudioParameterFloat> ("release", "Release", juce::NormalisableRange<float> (0.01f, 4.0f), 0.7f),
          std::make_unique<juce::AudioParameterFloat> ("gain", "Gain", juce::NormalisableRange<float> (0.0f, 1.0f), 0.8f) })
{
    for (int i = 0; i < 16; ++i) synth.addVoice (new YGMKeysVoice());
    synth.addSound (new YGMKeysSound());
}

void YGMKeysAudioProcessor::prepareToPlay (double rate, int)
{
    synth.setCurrentPlaybackSampleRate (rate);
    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<YGMKeysVoice*> (synth.getVoice (i)))
        { voice->setSampleRate (rate); voice->setEnvelope (*parameters.getRawParameterValue ("attack"), *parameters.getRawParameterValue ("release")); }
}

bool YGMKeysAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono()
        || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void YGMKeysAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)
{
    buffer.clear();
    synth.renderNextBlock (buffer, midi, 0, buffer.getNumSamples());
    buffer.applyGain (*parameters.getRawParameterValue ("gain"));
}

void YGMKeysAudioProcessor::getStateInformation (juce::MemoryBlock& dest)
{
    if (auto xml = parameters.copyState().createXml()) copyXmlToBinary (*xml, dest);
}

void YGMKeysAudioProcessor::setStateInformation (const void* data, int size)
{
    if (auto xml = getXmlFromBinary (data, size)) parameters.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new YGMKeysAudioProcessor(); }
