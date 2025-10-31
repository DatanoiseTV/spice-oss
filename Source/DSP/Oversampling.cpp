#include "Oversampling.h"

Oversampling::Oversampling(int numChannels, int factor, FilterType type)
    : oversamplingFactor(factor)
{
    auto filterType = (type == filterHalfBandPolyphaseIIR) 
        ? juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR
        : juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple;
    
    oversampler = std::make_unique<juce::dsp::Oversampling<float>>(
        numChannels, 
        factor, 
        filterType, 
        true
    );
}

void Oversampling::prepare(const juce::dsp::ProcessSpec& spec)
{
    oversampler->initProcessing(spec.maximumBlockSize);
    oversampler->reset();
}

void Oversampling::reset()
{
    oversampler->reset();
}

juce::dsp::AudioBlock<float> Oversampling::processSamplesUp(const juce::dsp::AudioBlock<float>& inputBlock)
{
    return oversampler->processSamplesUp(inputBlock);
}

void Oversampling::processSamplesDown(juce::dsp::AudioBlock<float>& outputBlock)
{
    oversampler->processSamplesDown(outputBlock);
}

void Oversampling::updateQuality(int factor, const juce::dsp::ProcessSpec& spec)
{
    if (factor != oversamplingFactor)
    {
        oversamplingFactor = factor;
        oversampler = std::make_unique<juce::dsp::Oversampling<float>>(
            static_cast<int>(spec.numChannels),
            factor,
            juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR,
            true
        );
        oversampler->initProcessing(spec.maximumBlockSize);
        oversampler->reset();
    }
}