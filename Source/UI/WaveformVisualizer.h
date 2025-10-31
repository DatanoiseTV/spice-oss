#pragma once

#include <JuceHeader.h>

class WaveformVisualizer : public juce::Component, public juce::Timer
{
public:
    WaveformVisualizer();
    ~WaveformVisualizer() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    void pushInputBuffer(const juce::AudioBuffer<float>& buffer);
    void pushOutputBuffer(const juce::AudioBuffer<float>& buffer);
    
    float getSaturationLevel() const { return saturationLevel; }
    bool isClipping() const { return clippingDetected; }
    
private:
    static constexpr int bufferSize = 256;
    std::array<float, bufferSize> inputData;
    std::array<float, bufferSize> outputData;
    std::array<float, bufferSize> inputDisplay;
    std::array<float, bufferSize> outputDisplay;
    int writePosition = 0;
    
    juce::Path inputPath;
    juce::Path outputPath;
    
    void createWaveformPaths();
    
    // Saturation and clipping detection
    std::atomic<float> saturationLevel{0.0f};
    std::atomic<bool> clippingDetected{false};
    juce::Time lastClipTime;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformVisualizer)
};