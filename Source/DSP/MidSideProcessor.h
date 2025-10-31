#pragma once

#include <JuceHeader.h>

class MidSideProcessor
{
public:
    MidSideProcessor();
    ~MidSideProcessor();
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    
    // Processing methods
    void process(juce::dsp::ProcessContextReplacing<float>& context);
    void processStereoToMidSide(juce::AudioBuffer<float>& buffer);
    void processMidSideToStereo(juce::AudioBuffer<float>& buffer);
    
    // Parameter setters
    void setMidSideEnabled(bool enabled) { midSideEnabled = enabled; }
    void setMidGain(float gain) { midGain = gain; }
    void setSideGain(float gain) { sideGain = gain; }
    void setMidSideBalance(float balance) { midSideBalance = balance; }
    void setStereoWidth(float width) { stereoWidth = juce::jlimit(0.0f, 2.0f, width); }
    
    // Getters
    bool isMidSideEnabled() const { return midSideEnabled; }
    float getMidGain() const { return midGain; }
    float getSideGain() const { return sideGain; }
    float getMidSideBalance() const { return midSideBalance; }
    float getStereoWidth() const { return stereoWidth; }
    
private:
    // Mid-side conversion utilities
    static void convertStereoToMidSide(float& left, float& right);
    static void convertMidSideToStereo(float& mid, float& side);
    
    // Parameters
    bool midSideEnabled = false;
    float midGain = 1.0f;        // Mid channel gain
    float sideGain = 1.0f;       // Side channel gain
    float midSideBalance = 0.0f; // -1 = all mid, +1 = all side, 0 = balanced
    float stereoWidth = 1.0f;    // 0 = mono, 1 = normal, 2 = super wide
    
    // Internal buffers for mid-side processing
    juce::AudioBuffer<float> midSideBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidSideProcessor)
};