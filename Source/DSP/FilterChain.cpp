#include "FilterChain.h"

FilterChain::FilterChain()
{
}

void FilterChain::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    
    filterChain.prepare(spec);
    
    updateFilters();
}

void FilterChain::reset()
{
    filterChain.reset();
}

void FilterChain::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    filterChain.process(context);
}

void FilterChain::setTone(float tone)
{
    if (std::abs(currentTone - tone) > 0.0001f)
    {
        currentTone = tone;
        updateFilters();
    }
}

void FilterChain::updateFilters()
{
    // Low shelf: boost/cut lows based on tone
    float lowGain = juce::jmap(currentTone, 0.0f, 1.0f, 3.0f, -3.0f);
    auto lowShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        sampleRate, 200.0f, 0.7f, 
        juce::Decibels::decibelsToGain(lowGain)
    );
    
    // High shelf: boost/cut highs based on tone  
    float highGain = juce::jmap(currentTone, 0.0f, 1.0f, -3.0f, 3.0f);
    auto highShelfCoeffs = juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        sampleRate, 4000.0f, 0.7f,
        juce::Decibels::decibelsToGain(highGain)
    );
    
    // Presence peak
    float presenceFreq = juce::jmap(currentTone, 0.0f, 1.0f, 2000.0f, 6000.0f);
    float presenceGain = juce::jmap(currentTone, 0.0f, 1.0f, -1.0f, 2.0f);
    auto presenceCoeffs = juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, presenceFreq, 0.5f, 
        juce::Decibels::decibelsToGain(presenceGain)
    );
    
    *filterChain.get<0>().state = *lowShelfCoeffs;
    *filterChain.get<1>().state = *highShelfCoeffs;
    *filterChain.get<2>().state = *presenceCoeffs;
}