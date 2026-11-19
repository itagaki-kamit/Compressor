// PluginEditor.h

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class CompresserPluginAudioProcessorEditor
    : public juce::AudioProcessorEditor
{
public:
    CompresserPluginAudioProcessorEditor (CompresserPluginAudioProcessor&);
    ~CompresserPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    CompresserPluginAudioProcessor& audioProcessor;
    
    juce::ComponentBoundsConstrainer constrainer;
    juce::ResizableBorderComponent resizer;

    //==============================
    // Sliders
    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;

    // Labels
    juce::Label thresholdLabel;
    juce::Label ratioLabel;
    juce::Label attackLabel;
    juce::Label releaseLabel;

    // Attachments
    using Attachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<Attachment> thresholdAttachment;
    std::unique_ptr<Attachment> ratioAttachment;
    std::unique_ptr<Attachment> attackAttachment;
    std::unique_ptr<Attachment> releaseAttachment;

    void setupSlider(juce::Slider& slider,
                     juce::Label& label,
                     const juce::String& labelText);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompresserPluginAudioProcessorEditor)
};
