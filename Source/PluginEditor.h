#pragma once

#include <JuceHeader.h>
#include "ff_meters.h"
#include "PluginProcessor.h"
#include "UI/LookAndFeel.h"
#include "UI/WaveformVisualizer.h"
#include "UI/AnalogLamp.h"
#include "UI/OnOffButton.h"
#include "UI/PresetSaveDialog.h"


class SpiceAudioProcessorEditor : public juce::AudioProcessorEditor,
                                        public juce::Timer,
                                        public juce::ComboBox::Listener,
                                        public juce::Button::Listener
{
public:
    SpiceAudioProcessorEditor (SpiceAudioProcessor&);
    ~SpiceAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
    void comboBoxChanged(juce::ComboBox* comboBox) override;
    void buttonClicked(juce::Button* button) override;
    
private:
    void refreshPresetList();
    void savePresetDialog();
    void drawSectionSeparator(juce::Graphics& g, juce::Rectangle<float> area, float y, const juce::String& label);
    void drawAmbientLight(juce::Graphics& g, juce::Point<float> position, juce::Colour colour);
    void drawVintageMeterBezel(juce::Graphics& g, juce::Rectangle<int> bounds);
    SpiceAudioProcessor& audioProcessor;
    
    SpiceLookAndFeel lookAndFeel;
    
    juce::Slider inputGainSlider;
    juce::Slider driveSlider;
    juce::Slider mixSlider;
    juce::Slider outputSlider;
    juce::Slider toneSlider;
    juce::Slider biasSlider;
    juce::Slider lowCutSlider;
    juce::Slider highCutSlider;
    juce::Slider gateThresholdSlider;
    juce::ComboBox cabinetModelSelector;
    juce::Slider cabinetPresenceSlider;
    juce::Slider cabinetMixSlider;
    juce::Slider midGainSlider;
    juce::Slider sideGainSlider;
    juce::Slider stereoWidthSlider;
    
    juce::ComboBox modelSelector;
    juce::TextButton previousModelButton {"<"};
    juce::TextButton nextModelButton {">"};
    juce::ComboBox qualitySelector;
    juce::ComboBox presetSelector;
    juce::TextButton previousPresetButton {"<"};
    juce::TextButton nextPresetButton {">"};
    juce::TextButton savePresetButton {"SAVE"};
    OnOffButton bypassButton;
    juce::TextButton compactViewButton {"COMPACT"};
    OnOffButton gateEnabledButton;
    OnOffButton cabinetEnabledButton;
    OnOffButton limiterEnabledButton;
    OnOffButton midSideEnabledButton;
    OnOffButton autoGainButton;
    
    juce::Label inputGainLabel;
    juce::Label driveLabel;
    juce::Label mixLabel;
    juce::Label outputLabel;
    juce::Label toneLabel;
    juce::Label biasLabel;
    juce::Label modelLabel;
    juce::Label qualityLabel;
    juce::Label presetLabel;
    juce::Label lowCutLabel;
    juce::Label highCutLabel;
    juce::Label gateLabel;
    juce::Label cabinetModelLabel;
    juce::Label cabinetPresenceLabel;
    juce::Label cabinetMixLabel;
    juce::Label midGainLabel;
    juce::Label sideGainLabel;
    juce::Label stereoWidthLabel;
    juce::Label midSideTitleLabel;
    juce::Label preFXTitleLabel;
    juce::Label postFXTitleLabel;

    OnOffButton postFXPowerButton;
    
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> toneAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> biasAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> lowCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> highCutAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gateThresholdAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> cabinetModelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cabinetPresenceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cabinetMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> modelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> qualityAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
    // std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> compactViewAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> gateEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> cabinetEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> limiterEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> midSideEnabledAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> autoGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> midGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sideGainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> stereoWidthAttachment;
    
    // Visualization
    foleys::LevelMeter inputMeter;
    foleys::LevelMeter outputMeter;
    WaveformVisualizer waveformDisplay;
    AnalogLamp saturationLamp;
    
    // Gate indicator LED
    class GateIndicatorLED : public juce::Component
    {
    public:
        GateIndicatorLED(SpiceAudioProcessor& proc) : processor(proc) {}
        
        void paint(juce::Graphics& g) override
        {
            auto gateEnabled = processor.getAPVTS().getRawParameterValue("gateEnabled")->load() > 0.5f;
            
            // LED background (bezel)
            g.setColour(juce::Colour(0xff1a1a1a));
            g.fillEllipse(getLocalBounds().toFloat());
            
            if (gateEnabled)
            {
                // Check if gate is actively gating by looking at input level vs threshold
                auto gateThreshold = processor.getAPVTS().getRawParameterValue("gateThreshold")->load();
                auto inputLevel = processor.getGateInputLevel();
                auto thresholdLinear = juce::Decibels::decibelsToGain(gateThreshold);
                bool gateOpen = inputLevel > thresholdLinear;
                
                if (gateOpen)
                {
                    // Gate is open - bright green
                    g.setColour(juce::Colour(0xff00ff00).withAlpha(0.9f));
                    g.fillEllipse(getLocalBounds().reduced(1).toFloat());
                    
                    // Inner highlight
                    g.setColour(juce::Colour(0xff80ff80));
                    g.fillEllipse(getLocalBounds().reduced(2).toFloat());
                }
                else
                {
                    // Gate is closed - dim red
                    g.setColour(juce::Colour(0xffff0000).withAlpha(0.4f));
                    g.fillEllipse(getLocalBounds().reduced(1).toFloat());
                }
            }
            else
            {
                // Gate disabled - dark
                g.setColour(juce::Colour(0xff004400).withAlpha(0.3f));
                g.fillEllipse(getLocalBounds().reduced(1).toFloat());
            }
            
            // LED rim
            g.setColour(juce::Colour(0xff404040));
            g.drawEllipse(getLocalBounds().toFloat(), 0.5f);
        }
        
    private:
        SpiceAudioProcessor& processor;
    } gateIndicatorLED;
    
    juce::Label inputMeterLabel;
    juce::Label outputMeterLabel;
    
    // ff_meters look and feel
    foleys::LevelMeterLookAndFeel meterLookAndFeel;
    
    // Store buffers for visualization
    juce::AudioBuffer<float> inputBuffer;
    juce::AudioBuffer<float> outputBuffer;
    
    // Custom fonts
    juce::Font aveschonFont;
    juce::Font dirtyHaroldFont;
    
    // Background texture
    juce::Image backgroundTexture;
    
    // Compact view functionality disabled
    // bool isCompactView = false;
    // static constexpr int normalHeight = 600;
    // static constexpr int compactHeight = 400;
    
    // void updateLayoutForCompactView();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpiceAudioProcessorEditor)
};