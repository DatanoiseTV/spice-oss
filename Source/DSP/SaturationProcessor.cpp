#include "SaturationProcessor.h"

SaturationProcessor::SaturationProcessor()
{
}

void SaturationProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = static_cast<float>(spec.sampleRate);
    
    prevSample.resize(spec.numChannels, 0.0f);
    hysteresis.resize(spec.numChannels, 0.0f);
    
    reset();
}

void SaturationProcessor::reset()
{
    std::fill(prevSample.begin(), prevSample.end(), 0.0f);
    std::fill(hysteresis.begin(), hysteresis.end(), 0.0f);
}

void SaturationProcessor::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    auto& block = context.getOutputBlock();
    auto numChannels = block.getNumChannels();
    auto numSamples = block.getNumSamples();
    
    for (size_t channel = 0; channel < numChannels; ++channel)
    {
        auto* channelData = block.getChannelPointer(channel);
        
        for (size_t sample = 0; sample < numSamples; ++sample)
        {
            channelData[sample] = processSample(channelData[sample], static_cast<int>(channel));
        }
    }
}

float SaturationProcessor::processSample(float input, int channel)
{
    // More reasonable drive scaling: 0-100% maps to 0-20dB of gain
    float normalizedDrive = drive / 100.0f;
    float driveGain = juce::Decibels::decibelsToGain(normalizedDrive * 20.0f);
    
    // Apply drive gain first
    float sample = input * driveGain;
    
    // Apply bias AFTER gain for more audible asymmetric saturation
    // Bias range is now -1 to 1 for stronger effect
    float biasAmount = bias * 0.3f * (1.0f + normalizedDrive); // Scale bias with drive
    sample += biasAmount;
    
    switch (model)
    {
        case Model::Tube:
            sample = tubeSaturation(sample);
            break;
        case Model::Transistor:
            sample = transistorSaturation(sample);
            break;
        case Model::Transformer:
            sample = transformerSaturation(sample);
            break;
        case Model::Tape:
            sample = tapeSaturation(sample);
            break;
        case Model::Diode:
            sample = diodeSaturation(sample);
            break;
        case Model::Vintage:
            sample = vintageSaturation(sample);
            break;
        case Model::Warm:
            sample = warmSaturation(sample);
            break;
        case Model::Bright:
            sample = brightSaturation(sample);
            break;
        case Model::FuzzBox:
            sample = fuzzBoxSaturation(sample);
            break;
        case Model::Overdrive:
            sample = overdriveSaturation(sample);
            break;
        case Model::Tube12AX7:
            sample = tube12AX7Saturation(sample);
            break;
    }
    
    // Smooth compensation curve to avoid clicks
    float compensation = 1.0f;
    if (driveGain > 1.0f)
    {
        // Smooth transition using the full range of drive
        float compensationAmount = (driveGain - 1.0f) * normalizedDrive * 0.5f;
        compensation = 1.0f / std::sqrt(1.0f + compensationAmount);
    }
    return sample * compensation;
}

float SaturationProcessor::tubeSaturation(float input)
{
    const float threshold = 0.7f;
    float x = juce::jlimit(-3.0f, 3.0f, input);
    
    if (std::abs(x) < threshold)
    {
        return x;
    }
    
    float sign = (x < 0.0f) ? -1.0f : 1.0f;
    x = std::abs(x);
    
    float y = threshold + (1.0f - threshold) * std::tanh((x - threshold) * 2.0f);
    
    y += 0.05f * std::sin(2.0f * PI * x);
    y += 0.02f * std::sin(3.0f * PI * x);
    
    return sign * y;
}

float SaturationProcessor::transistorSaturation(float input)
{
    float x = juce::jlimit(-2.0f, 2.0f, input);
    
    float y = x;
    if (std::abs(x) > 0.5f)
    {
        float sign = (x < 0.0f) ? -1.0f : 1.0f;
        float absX = std::abs(x);
        
        y = sign * (0.5f + 0.5f * std::tanh(2.0f * (absX - 0.5f)));
        
        y *= 1.0f + 0.1f * (1.0f - std::abs(y));
    }
    
    y += 0.03f * x * x * x;
    
    return juce::jlimit(-1.0f, 1.0f, y);
}

float SaturationProcessor::transformerSaturation(float input)
{
    float x = juce::jlimit(-2.0f, 2.0f, input);
    int channel = 0;
    
    float hyst = hysteresis[channel];
    float delta = x - prevSample[channel];
    
    hyst += delta * 0.3f;
    hyst *= 0.95f;
    
    float y = std::tanh(x * 1.5f + hyst * 0.2f);
    
    y += 0.02f * std::sin(2.0f * PI * x);
    y += 0.01f * std::sin(4.0f * PI * x);
    
    prevSample[channel] = x;
    hysteresis[channel] = hyst;
    
    return y;
}

float SaturationProcessor::tapeSaturation(float input)
{
    float x = juce::jlimit(-1.5f, 1.5f, input);
    
    float y = x - 0.15f * x * x * x;
    
    if (std::abs(x) > 0.7f)
    {
        float sign = (x < 0.0f) ? -1.0f : 1.0f;
        y = sign * (0.7f + 0.3f * std::tanh(3.0f * (std::abs(x) - 0.7f)));
    }
    
    float compression = 1.0f - 0.2f * std::abs(y);
    y *= compression;
    
    y += 0.01f * std::sin(1.5f * PI * x);
    
    return y;
}

float SaturationProcessor::diodeSaturation(float input)
{
    const float threshold = 0.3f;
    float x = juce::jlimit(-2.0f, 2.0f, input);
    
    if (x > threshold)
    {
        float excess = x - threshold;
        x = threshold + std::tanh(excess * 3.0f) * 0.5f;
    }
    else if (x < -threshold * 1.2f)
    {
        float excess = x + threshold * 1.2f;
        x = -threshold * 1.2f + std::tanh(excess * 2.0f) * 0.6f;
    }
    
    x += 0.02f * x * x;
    
    return juce::jlimit(-1.0f, 1.0f, x);
}

void SaturationProcessor::setDrive(float newDrive)
{
    drive = newDrive;
}

void SaturationProcessor::setModel(Model newModel)
{
    model = newModel;
}

void SaturationProcessor::setBias(float newBias)
{
    bias = newBias;
}

float SaturationProcessor::vintageSaturation(float input)
{
    float x = juce::jlimit(-2.0f, 2.0f, input);
    
    // Vintage console saturation with smooth compression
    float y = std::tanh(x * 1.2f);
    
    // Add subtle harmonic content
    y += 0.08f * std::sin(2.0f * PI * x);
    y += 0.04f * std::sin(3.0f * PI * x);
    y += 0.02f * std::sin(5.0f * PI * x);
    
    // Gentle high-frequency roll-off
    y *= 1.0f - 0.1f * std::abs(x);
    
    return y * 0.8f;
}

float SaturationProcessor::warmSaturation(float input)
{
    float x = juce::jlimit(-1.8f, 1.8f, input);
    
    // Warm, musical saturation with even harmonics
    float y = x - 0.33f * x * x * x;
    
    // Add warmth with even harmonics
    y += 0.06f * x * x;
    y += 0.03f * x * x * x * x;
    
    // Smooth limiting
    if (std::abs(y) > 0.9f)
    {
        float sign = (y < 0.0f) ? -1.0f : 1.0f;
        y = sign * (0.9f + 0.1f * std::tanh(5.0f * (std::abs(y) - 0.9f)));
    }
    
    return y;
}

float SaturationProcessor::brightSaturation(float input)
{
    float x = juce::jlimit(-2.2f, 2.2f, input);
    
    // Bright, crisp saturation with high-frequency emphasis
    float y = std::atanh(juce::jlimit(-0.95f, 0.95f, x * 0.7f)) * 1.2f;
    
    // Add brightness with odd harmonics
    y += 0.1f * std::sin(3.0f * PI * x);
    y += 0.05f * std::sin(5.0f * PI * x);
    y += 0.025f * std::sin(7.0f * PI * x);
    
    // High-frequency boost
    y *= 1.0f + 0.2f * std::abs(x);
    
    return juce::jlimit(-1.0f, 1.0f, y);
}

float SaturationProcessor::fuzzBoxSaturation(float input)
{
    float x = juce::jlimit(-3.0f, 3.0f, input);
    
    // Aggressive fuzz with hard clipping
    float y = x * 2.0f;
    
    // Hard clipping with some softness
    if (y > 1.0f)
        y = 1.0f - 0.2f * std::exp(-(y - 1.0f) * 3.0f);
    else if (y < -1.0f)
        y = -1.0f + 0.2f * std::exp((y + 1.0f) * 3.0f);
    
    // Add fuzz character with square wave approximation
    y += 0.15f * ((y > 0.0f) ? 1.0f : -1.0f);
    
    // Bit crushing effect
    y = std::round(y * 32.0f) / 32.0f;
    
    return juce::jlimit(-1.0f, 1.0f, y * 0.7f);
}

float SaturationProcessor::overdriveSaturation(float input)
{
    float x = juce::jlimit(-2.5f, 2.5f, input);
    
    // Musical overdrive with asymmetric clipping
    float y = x;
    
    // Asymmetric clipping for even harmonics
    if (x > 0.5f)
    {
        y = 0.5f + 0.5f * std::tanh(2.0f * (x - 0.5f));
    }
    else if (x < -0.7f)
    {
        y = -0.7f + 0.3f * std::tanh(1.5f * (x + 0.7f));
    }
    
    // Add musical harmonics
    y += 0.1f * x * x * x;
    y += 0.05f * x * x;
    
    // Gentle compression
    y *= 1.0f / (1.0f + 0.3f * std::abs(y));
    
    return y;
}

float SaturationProcessor::tube12AX7Saturation(float input)
{
    float x = juce::jlimit(-4.0f, 4.0f, input);
    
    // 12AX7 tube characteristics:
    // - Grid conduction at ~0.3V
    // - Smooth transition to saturation
    // - Asymmetric transfer curve
    // - Rich even and odd harmonics
    
    // Pre-emphasis to model input capacitance
    float preEmphasis = x + 0.1f * (x - prevSample[0]);
    
    // Grid conduction modeling
    float gridCurrent = 0.0f;
    if (preEmphasis > 0.3f)
    {
        gridCurrent = 0.15f * std::tanh((preEmphasis - 0.3f) * 3.0f);
        preEmphasis -= gridCurrent;
    }
    
    // Asymmetric transfer curve modeling
    float y;
    if (preEmphasis >= 0.0f)
    {
        // Positive half - smoother compression
        float drive = 1.0f + preEmphasis * 0.5f;
        y = std::tanh(preEmphasis * drive);
        
        // Cathode follower compression
        y *= 1.0f / (1.0f + 0.2f * y);
    }
    else
    {
        // Negative half - harder clipping
        float drive = 1.0f - preEmphasis * 0.3f;
        y = std::tanh(preEmphasis * drive * 1.2f);
    }
    
    // Harmonic enrichment based on actual 12AX7 measurements
    // Strong 2nd harmonic (even)
    y += 0.12f * std::sin(PI * preEmphasis) * (1.0f - std::abs(y));
    
    // 3rd harmonic (odd)
    y += 0.08f * std::sin(3.0f * PI * preEmphasis) * (1.0f - std::abs(y));
    
    // 5th harmonic
    y += 0.03f * std::sin(5.0f * PI * preEmphasis) * (1.0f - std::abs(y));
    
    // Miller capacitance effect (subtle high-frequency rolloff)
    float millerEffect = 0.95f + 0.05f * (1.0f - std::abs(y));
    y = y * millerEffect + prevSample[0] * (1.0f - millerEffect);
    
    // Output transformer saturation
    if (std::abs(y) > 0.8f)
    {
        float excess = std::abs(y) - 0.8f;
        float sign = (y < 0.0f) ? -1.0f : 1.0f;
        y = sign * (0.8f + 0.2f * std::tanh(excess * 5.0f));
    }
    
    // Add subtle ghost notes (intermodulation)
    y += 0.02f * y * y * y;
    
    // Grid current recovery adds subtle pumping
    y += gridCurrent * 0.3f;
    
    return juce::jlimit(-1.0f, 1.0f, y * 0.85f);
}