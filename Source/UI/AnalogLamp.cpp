#include "AnalogLamp.h"

AnalogLamp::AnalogLamp()
{
    startTimerHz(120); // 120Hz for instant response
}

AnalogLamp::~AnalogLamp()
{
    stopTimer();
}

void AnalogLamp::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced(10); // Add padding to prevent clipping
    auto center = bounds.getCentre();
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.35f; // Slightly smaller
    
    // Metal bezel around lamp
    juce::ColourGradient bezelGradient(
        juce::Colour(0xff8b8b8b), center.x - radius, center.y - radius,
        juce::Colour(0xff2a2a2a), center.x + radius, center.y + radius, true);
    bezelGradient.addColour(0.5, juce::Colour(0xff5a5a5a));
    g.setGradientFill(bezelGradient);
    g.fillEllipse(center.x - radius - 3, center.y - radius - 3, (radius + 3) * 2, (radius + 3) * 2);
    
    // Inner bevel
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillEllipse(center.x - radius - 1, center.y - radius - 1, (radius + 1) * 2, (radius + 1) * 2);
    
    // Dark glass bulb background
    juce::ColourGradient glassGradient(
        juce::Colour(0xff2a1a1a), center.x, center.y - radius,
        juce::Colour(0xff0a0505), center.x, center.y + radius, false);
    g.setGradientFill(glassGradient);
    g.fillEllipse(center.x - radius, center.y - radius, radius * 2, radius * 2);
    
    // Lamp color based on state - warm amber/orange like old analog bulbs
    juce::Colour lampColor;
    if (clippingState)
    {
        // Bright orange-red when clipping
        lampColor = juce::Colour(0xffff4500);
    }
    else if (saturationLevel > 0.7f)
    {
        // Bright amber when heavily saturating
        lampColor = juce::Colour(0xffffa500);
    }
    else if (saturationLevel > 0.3f)
    {
        // Medium amber when moderately saturating
        lampColor = juce::Colour(0xffffb347);
    }
    else if (saturationLevel > 0.01f)
    {
        // Dim amber when barely active
        lampColor = juce::Colour(0xffd2691e);
    }
    else
    {
        // Very dim orange/brown when off
        lampColor = juce::Colour(0xff5d3a1a);
    }
    
    // Dynamic glow effect - smaller, contained within bounds
    float glowRadius = radius * 1.5f; // Reduced from 2.0f to fit within bounds
    
    // Outer glow - no clipping needed if we keep it within bounds
    if (currentBrightness > 0.01f)
    {
        // Soft outer glow
        juce::ColourGradient outerGlow(
            lampColor.withAlpha(currentBrightness * 0.05f), center.x, center.y,
            lampColor.withAlpha(0.0f), center.x + glowRadius, center.y, true);
        g.setGradientFill(outerGlow);
        g.fillEllipse(center.x - glowRadius, center.y - glowRadius, 
                     glowRadius * 2.0f, glowRadius * 2.0f);
    }
    
    // Middle glow
    if (currentBrightness > 0.1f)
    {
        float midRadius = radius * 1.2f;
        juce::ColourGradient middleGlow(
            lampColor.withAlpha(currentBrightness * 0.3f), center.x, center.y,
            lampColor.withAlpha(0.0f), center.x + midRadius, center.y, true);
        g.setGradientFill(middleGlow);
        g.fillEllipse(center.x - midRadius, center.y - midRadius, 
                     midRadius * 2.0f, midRadius * 2.0f);
    }
    
    // Inner lamp glow
    juce::ColourGradient innerGlow(
        lampColor.withAlpha(currentBrightness), center.x, center.y,
        lampColor.withAlpha(currentBrightness * 0.3f), center.x + radius, center.y, true);
    g.setGradientFill(innerGlow);
    g.fillEllipse(center.x - radius + 2, center.y - radius + 2, 
                 (radius - 2) * 2, (radius - 2) * 2);
    
    // Draw filament structure
    if (currentBrightness > 0.01f)
    {
        // Filament wires - zigzag pattern like vintage bulbs
        auto filamentColor = lampColor.interpolatedWith(juce::Colours::white, currentBrightness * 0.6f);
        g.setColour(filamentColor.withAlpha(0.8f + currentBrightness * 0.2f));
        
        // Main vertical supports
        float supportWidth = 2.0f;
        float supportX1 = center.x - radius * 0.15f;
        float supportX2 = center.x + radius * 0.15f;
        float supportTop = center.y - radius * 0.6f;
        float supportBottom = center.y + radius * 0.6f;
        
        g.fillRect(supportX1 - supportWidth/2, supportTop, supportWidth, supportBottom - supportTop);
        g.fillRect(supportX2 - supportWidth/2, supportTop, supportWidth, supportBottom - supportTop);
        
        // Zigzag filament between supports
        juce::Path filamentPath;
        float zigzagHeight = (supportBottom - supportTop) / 8.0f;
        
        filamentPath.startNewSubPath(supportX1, supportTop);
        for (int i = 1; i <= 8; ++i)
        {
            float y = supportTop + i * zigzagHeight;
            float x = (i % 2 == 0) ? supportX1 : supportX2;
            filamentPath.lineTo(x, y);
        }
        
        // Filament glow
        g.setColour(filamentColor.withAlpha(0.3f));
        g.strokePath(filamentPath, juce::PathStrokeType(4.0f));
        
        g.setColour(filamentColor.withAlpha(0.6f));
        g.strokePath(filamentPath, juce::PathStrokeType(2.0f));
        
        g.setColour(filamentColor);
        g.strokePath(filamentPath, juce::PathStrokeType(1.0f));
        
        // Hot spots along filament when bright
        if (currentBrightness > 0.5f)
        {
            for (int i = 2; i <= 6; i += 2)
            {
                float y = supportTop + i * zigzagHeight;
                float x = (i % 2 == 0) ? supportX1 : supportX2;
                
                float hotSpotSize = 3.0f + currentBrightness * 2.0f;
                juce::ColourGradient hotSpot(
                    juce::Colours::white.withAlpha(currentBrightness * 0.7f), x, y,
                    filamentColor.withAlpha(0.0f), x + hotSpotSize, y, true);
                g.setGradientFill(hotSpot);
                g.fillEllipse(x - hotSpotSize, y - hotSpotSize, hotSpotSize * 2, hotSpotSize * 2);
            }
        }
    }
    
    // Center hot spot (where filament is brightest)
    if (currentBrightness > 0.5f)
    {
        float hotSpotSize = radius * 0.2f;
        juce::ColourGradient hotSpot(
            juce::Colours::white.withAlpha(currentBrightness * 0.5f), center.x, center.y,
            lampColor.withAlpha(0.0f), center.x + hotSpotSize, center.y, true);
        g.setGradientFill(hotSpot);
        g.fillEllipse(center.x - hotSpotSize, center.y - hotSpotSize, 
                     hotSpotSize * 2, hotSpotSize * 2);
    }
    
    // Glass reflection
    auto reflectionBounds = bounds.reduced(radius * 0.3f).withHeight(radius * 0.8f);
    reflectionBounds.setY(center.y - radius * 0.7f);
    
    juce::ColourGradient reflection(
        juce::Colours::white.withAlpha(0.2f), reflectionBounds.getCentreX(), reflectionBounds.getY(),
        juce::Colours::white.withAlpha(0.0f), reflectionBounds.getCentreX(), reflectionBounds.getBottom(), false);
    g.setGradientFill(reflection);
    g.fillEllipse(reflectionBounds);
    
    // No label - removed per user request
}

void AnalogLamp::timerCallback()
{
    // Realistic bulb behavior with thermal inertia - more sluggish
    const float attackTime = 0.95f;  // Slower heat up (more smoothing)
    //const float releaseTime = 0.985f; // Slower cool down (more afterglow)
    const float releaseTime = 1.0f;

    if (targetBrightness > currentBrightness)
    {
        // Heating up - faster response
        currentBrightness += (targetBrightness - currentBrightness) * (1.0f - attackTime);
    }
    else
    {
        // Cooling down - slower response with afterglow
        currentBrightness += (targetBrightness - currentBrightness) * (1.0f - releaseTime);
    }
    
    // Ensure we eventually reach zero
    if (currentBrightness < 0.01f && targetBrightness == 0.0f)
        currentBrightness = 0.0f;
    
    // Subtle glow pulsing when hot
    if (currentBrightness > 0.5f)
    {
        pulsePhase += 0.05f;
        glowIntensity = 0.15f + 0.05f * std::sin(pulsePhase);
    }
    else
    {
        glowIntensity = 0.1f + currentBrightness * 0.1f;
    }
    
    repaint();
}

void AnalogLamp::setSaturationLevel(float level)
{
    saturationLevel = juce::jlimit(0.0f, 1.0f, level);
    
    // Direct brightness mapping - shows even small signals
    if (clippingState)
    {
        targetBrightness = 1.0f; // Full brightness when clipping
    }
    else if (saturationLevel > 0.001f) // Very sensitive threshold
    {
        // Exponential curve for better visual response
        float scaledLevel = std::pow(saturationLevel, 0.5f); // Square root for better low-level visibility
        targetBrightness = 0.2f + scaledLevel * 0.8f; // Always at least 20% bright when signal present
    }
    else
    {
        targetBrightness = 0.0f; // Off only when truly silent
    }
}

void AnalogLamp::setClipping(bool isClipping)
{
    clippingState = isClipping;
}