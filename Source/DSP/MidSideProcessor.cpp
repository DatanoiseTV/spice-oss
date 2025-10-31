#include "MidSideProcessor.h"

MidSideProcessor::MidSideProcessor()
{
}

MidSideProcessor::~MidSideProcessor()
{
}

void MidSideProcessor::prepare(const juce::dsp::ProcessSpec& spec)
{
    // Prepare internal buffer for mid-side processing
    midSideBuffer.setSize(static_cast<int>(spec.numChannels), static_cast<int>(spec.maximumBlockSize));
}

void MidSideProcessor::reset()
{
    midSideBuffer.clear();
}

void MidSideProcessor::process(juce::dsp::ProcessContextReplacing<float>& context)
{
    if (!midSideEnabled)
        return;
        
    auto& audioBlock = context.getOutputBlock();
    auto numChannels = audioBlock.getNumChannels();
    auto numSamples = audioBlock.getNumSamples();
    
    // Only process stereo signals
    if (numChannels != 2)
        return;
    
    // Get pointers to left and right channels
    auto* leftChannel = audioBlock.getChannelPointer(0);
    auto* rightChannel = audioBlock.getChannelPointer(1);
    
    // Convert stereo to mid-side and apply processing
    for (size_t sample = 0; sample < numSamples; ++sample)
    {
        float left = leftChannel[sample];
        float right = rightChannel[sample];
        
        // Convert to mid-side
        convertStereoToMidSide(left, right);
        
        // Now left = mid, right = side
        float mid = left;
        float side = right;
        
        // Apply gains
        mid *= midGain;
        side *= sideGain;
        
        // Apply stereo width control
        side *= stereoWidth;
        
        // Apply mid-side balance
        if (midSideBalance > 0.0f)
        {
            // Favor sides
            mid *= (1.0f - midSideBalance);
        }
        else if (midSideBalance < 0.0f)
        {
            // Favor mid
            side *= (1.0f + midSideBalance);
        }
        
        // Convert back to stereo
        convertMidSideToStereo(mid, side);
        
        leftChannel[sample] = mid;   // mid becomes left after conversion
        rightChannel[sample] = side; // side becomes right after conversion
    }
}

void MidSideProcessor::processStereoToMidSide(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() != 2)
        return;
        
    auto* leftData = buffer.getWritePointer(0);
    auto* rightData = buffer.getWritePointer(1);
    auto numSamples = buffer.getNumSamples();
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float left = leftData[sample];
        float right = rightData[sample];
        
        convertStereoToMidSide(left, right);
        
        leftData[sample] = left;   // Now contains mid
        rightData[sample] = right; // Now contains side
    }
}

void MidSideProcessor::processMidSideToStereo(juce::AudioBuffer<float>& buffer)
{
    if (buffer.getNumChannels() != 2)
        return;
        
    auto* midData = buffer.getWritePointer(0);
    auto* sideData = buffer.getWritePointer(1);
    auto numSamples = buffer.getNumSamples();
    
    for (int sample = 0; sample < numSamples; ++sample)
    {
        float mid = midData[sample];
        float side = sideData[sample];
        
        convertMidSideToStereo(mid, side);
        
        midData[sample] = mid;   // Now contains left
        sideData[sample] = side; // Now contains right
    }
}

void MidSideProcessor::convertStereoToMidSide(float& left, float& right)
{
    const float mid = (left + right) * 0.5f;   // Sum (center/mono information)
    const float side = (left - right) * 0.5f;  // Difference (stereo width information)
    
    left = mid;
    right = side;
}

void MidSideProcessor::convertMidSideToStereo(float& mid, float& side)
{
    const float left = mid + side;   // Mid + Side = Left
    const float right = mid - side;  // Mid - Side = Right
    
    mid = left;   // Reuse mid variable for left
    side = right; // Reuse side variable for right
}