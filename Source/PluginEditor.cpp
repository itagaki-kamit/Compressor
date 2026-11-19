// PluginEditor.cpp

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
CompresserPluginAudioProcessorEditor::
CompresserPluginAudioProcessorEditor
    (CompresserPluginAudioProcessor& p)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      resizer(this, &constrainer)
{
    // リサイズ可能
    setResizable(true, true);

    // 最小サイズ
    setResizeLimits(400, 250, 1200, 800);

    setSize (600, 350);
    
    // サイズ制限
    constrainer.setMinimumSize(400, 250);
    constrainer.setMaximumSize(1600, 1200);

    setResizeLimits(400, 250, 1600, 1200);

    // 全辺リサイズ
    resizer.setBorderThickness(
        juce::BorderSize<int>(10));

    addAndMakeVisible(resizer);

    // Threshold
    setupSlider(thresholdSlider, thresholdLabel, "Threshold");

    thresholdAttachment = std::make_unique<Attachment>(
        audioProcessor.parameters,
        "threshold",
        thresholdSlider);

    // Ratio
    setupSlider(ratioSlider, ratioLabel, "Ratio");

    ratioAttachment = std::make_unique<Attachment>(
        audioProcessor.parameters,
        "ratio",
        ratioSlider);

    // Attack
    setupSlider(attackSlider, attackLabel, "Attack");

    attackAttachment = std::make_unique<Attachment>(
        audioProcessor.parameters,
        "attack",
        attackSlider);

    // Release
    setupSlider(releaseSlider, releaseLabel, "Release");

    releaseAttachment = std::make_unique<Attachment>(
        audioProcessor.parameters,
        "release",
        releaseSlider);
}

CompresserPluginAudioProcessorEditor::~CompresserPluginAudioProcessorEditor()
{
}

//==============================================================================
void CompresserPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::white);
//    g.setFont(28.0f);
    g.setFont(proportionOfHeight(0.08f));

    g.drawFittedText("Compressor Plugin",
                     getLocalBounds().removeFromTop(40),
                     juce::Justification::centred,
                     1);
}

void CompresserPluginAudioProcessorEditor::resized()
{
    // size contollo
    auto bounds = getLocalBounds();
    resizer.setBounds(bounds);
    auto area = bounds.reduced(20);
    area.removeFromTop(proportionOfHeight(0.15f));
    auto sliderArea = area;
    int sectionWidth = sliderArea.getWidth() / 4;

    
    auto layoutKnob =
        [this](juce::Rectangle<int> bounds,
               juce::Label& label,
               juce::Slider& slider)
    {
        int labelHeight = proportionOfHeight(0.06f);

        label.setBounds(bounds.removeFromTop(labelHeight));

        int knobSize = juce::jmin(bounds.getWidth(),
                                  bounds.getHeight());

        knobSize = static_cast<int>(knobSize * 0.8f);

        slider.setBounds(
            bounds.withSizeKeepingCentre(
                knobSize,
                knobSize));
    };

    auto t = sliderArea.removeFromLeft(sectionWidth);
    auto r = sliderArea.removeFromLeft(sectionWidth);
    auto a = sliderArea.removeFromLeft(sectionWidth);
    auto rel = sliderArea;

    layoutKnob(t, thresholdLabel, thresholdSlider);
    layoutKnob(r, ratioLabel, ratioSlider);
    layoutKnob(a, attackLabel, attackSlider);
    layoutKnob(rel, releaseLabel, releaseSlider);
}

void CompresserPluginAudioProcessorEditor::setupSlider(
    juce::Slider& slider,
    juce::Label& label,
    const juce::String& labelText)
{
    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow,
                           false,
                           60,
                           20);

    addAndMakeVisible(slider);

    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(label);
}
