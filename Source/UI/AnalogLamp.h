#pragma once

#include <JuceHeader.h>

class AnalogLamp : public juce::Component, public juce::Timer
{
public:
    AnalogLamp();
    ~AnalogLamp() override;
    
    void paint(juce::Graphics& g) override;
    void timerCallback() override;
    
    void setSaturationLevel(float level);
    void setClipping(bool isClipping);
    
private:
    float saturationLevel = 0.0f;
    float targetBrightness = 0.0f;
    float currentBrightness = 0.0f;
    bool clippingState = false;
    
    // Animation
    float glowIntensity = 0.0f;
    float pulsePhase = 0.0f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AnalogLamp)
};