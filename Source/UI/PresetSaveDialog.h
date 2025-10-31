#pragma once

#include <JuceHeader.h>

class PresetSaveDialog : public juce::Component,
                        public juce::Button::Listener
{
public:
    PresetSaveDialog(const juce::String& currentPresetName);
    ~PresetSaveDialog() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;
    
    juce::String getPresetName() const { return textEditor.getText(); }
    
    std::function<void(bool shouldSave)> onComplete;
    
private:
    juce::TextEditor textEditor;
    juce::TextButton saveButton {"SAVE"};
    juce::TextButton cancelButton {"CANCEL"};
    juce::Label titleLabel;
    
    juce::Font titleFont;
    juce::Font labelFont;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetSaveDialog)
};