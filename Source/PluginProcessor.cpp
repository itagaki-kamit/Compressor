#include "PluginProcessor.h"
#include "PluginEditor.h"

CompresserPluginAudioProcessor::CompresserPluginAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
    ),

    parameters(*this, nullptr, "PARAMETERS",
    {
        std::make_unique<juce::AudioParameterFloat>(
            "threshold",
            "Threshold",
            juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f),
            -12.0f),

        std::make_unique<juce::AudioParameterFloat>(
            "ratio",
            "Ratio",
            juce::NormalisableRange<float>(1.0f, 20.0f, 0.1f),
            4.0f),

        std::make_unique<juce::AudioParameterFloat>(
            "attack",
            "Attack",
            juce::NormalisableRange<float>(1.0f, 200.0f, 0.1f),
            20.0f),

        std::make_unique<juce::AudioParameterFloat>(
            "release",
            "Release",
            juce::NormalisableRange<float>(10.0f, 500.0f, 0.1f),
            100.0f)
    })
{
}

CompresserPluginAudioProcessor::~CompresserPluginAudioProcessor()
{
}

const juce::String CompresserPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool CompresserPluginAudioProcessor::acceptsMidi() const
{
    return false;
}

bool CompresserPluginAudioProcessor::producesMidi() const
{
    return false;
}

bool CompresserPluginAudioProcessor::isMidiEffect() const
{
    return false;
}

double CompresserPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int CompresserPluginAudioProcessor::getNumPrograms()
{
    return 1;
}

int CompresserPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void CompresserPluginAudioProcessor::setCurrentProgram (int)
{
}

const juce::String CompresserPluginAudioProcessor::getProgramName (int)
{
    return {};
}

void CompresserPluginAudioProcessor::changeProgramName (int, const juce::String&)
{
}

void CompresserPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;

    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    compressor.prepare(spec);
}

void CompresserPluginAudioProcessor::releaseResources()
{
}

bool CompresserPluginAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* CompresserPluginAudioProcessor::createEditor()
{
    return new CompresserPluginAudioProcessorEditor (*this);
}


//void CompresserPluginAudioProcessor::processBlock(
//    juce::AudioBuffer<float>& buffer,
//    juce::MidiBuffer&)
//{
//    for (int channel = 0; channel < buffer.getNumChannels(); ++channel)
//    {
//        auto* data = buffer.getWritePointer(channel);
//
//        for (int i = 0; i < buffer.getNumSamples(); ++i)
//        {
//            data[i] *= 0.5f;
//        }
//    }
//}
void CompresserPluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                   juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    auto threshold = parameters.getRawParameterValue("threshold")->load();
    auto ratio     = parameters.getRawParameterValue("ratio")->load();
    auto attack    = parameters.getRawParameterValue("attack")->load();
    auto release   = parameters.getRawParameterValue("release")->load();

    compressor.setThreshold(threshold);
    compressor.setRatio(ratio);
    compressor.setAttack(attack);
    compressor.setRelease(release);

    auto block = juce::dsp::AudioBlock<float>(buffer);

    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    compressor.process(context);
}

void CompresserPluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());

    copyXmlToBinary(*xml, destData);
}

void CompresserPluginAudioProcessor::setStateInformation (const void* data,
                                                          int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(
        getXmlFromBinary(data, sizeInBytes));

    if (xml && xml->hasTagName(parameters.state.getType()))
    {
        parameters.replaceState(
            juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new CompresserPluginAudioProcessor();
}
