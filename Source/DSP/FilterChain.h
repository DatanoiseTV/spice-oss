#pragma once

#include <JuceHeader.h>

class FilterChain
{
public:
    FilterChain();
    ~FilterChain() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    
    void setTone(float tone);
    
private:
    juce::dsp::ProcessorChain<
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>,
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>,
        juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>
    > filterChain;
    
    float currentTone = 0.5f;
    float sampleRate = 44100.0f;
    
    void updateFilters();
};