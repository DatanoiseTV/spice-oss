#pragma once

#include <JuceHeader.h>

class PresetManager
{
public:
    static constexpr const char* defaultPresetName = "Init";
    static constexpr const char* presetFileExtension = ".spice";
    
    PresetManager(juce::AudioProcessorValueTreeState& apvts);
    ~PresetManager() = default;
    
    void setProcessor(juce::AudioProcessor* proc) { processor = proc; }
    
    void savePreset(const juce::String& presetName);
    void loadPreset(const juce::String& presetName);
    void loadPresetSmooth(const juce::String& presetName);
    void deletePreset(const juce::String& presetName);
    
    juce::StringArray getAllPresets();
    juce::String getCurrentPreset() const { return currentPresetName; }
    
    void loadNextPreset();
    void loadPreviousPreset();
    
    juce::File getPresetsDirectory() const;
    
private:
    void saveDefaultPreset();
    void createPresetsDirectory();
    void createFactoryPresets();
    
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::String currentPresetName;
    juce::AudioProcessor* processor = nullptr;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
};