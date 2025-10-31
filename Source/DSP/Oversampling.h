#pragma once

#include <JuceHeader.h>

class Oversampling
{
public:
    enum FilterType
    {
        filterHalfBandPolyphaseIIR = 0,
        filterHalfBandFIREquiripple
    };
    
    Oversampling(int numChannels, int factor, FilterType type);
    ~Oversampling() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void updateQuality(int factor, const juce::dsp::ProcessSpec& spec);
    
    juce::dsp::AudioBlock<float> processSamplesUp(const juce::dsp::AudioBlock<float>& inputBlock);
    void processSamplesDown(juce::dsp::AudioBlock<float>& outputBlock);
    
    int getOversamplingFactor() const { return oversamplingFactor; }
    
private:
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    int oversamplingFactor;
};