#pragma once

#include <JuceHeader.h>

class LevelMeter : public juce::Component, public juce::Timer
{
public:
    LevelMeter();
    ~LevelMeter() override;
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    void timerCallback() override;
    
    void setLevel(float newLevel);
    void setPeakLevel(float newPeak);
    
private:
    float level = 0.0f;
    float peakLevel = 0.0f;
    float smoothedLevel = 0.0f;
    float displayPeak = 0.0f;
    
    float peakHoldTime = 0.0f;
    const float peakHoldDuration = 2.0f;
    const float peakFallRate = 20.0f;
    
    juce::Colour meterGradientStart;
    juce::Colour meterGradientMid;
    juce::Colour meterGradientEnd;
    juce::Colour peakColour;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
};