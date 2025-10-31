#include "LevelMeter.h"

LevelMeter::LevelMeter()
{
    meterGradientStart = juce::Colour(0xff00ff00);
    meterGradientMid = juce::Colour(0xffffff00);
    meterGradientEnd = juce::Colour(0xffff0000);
    peakColour = juce::Colour(0xffffffff);
    
    startTimerHz(30);
}

LevelMeter::~LevelMeter()
{
    stopTimer();
}

void LevelMeter::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Background
    g.setColour(juce::Colour(0xff1a1a1a));
    g.fillRoundedRectangle(bounds, 2.0f);
    
    // Meter gradient
    auto meterBounds = bounds.reduced(2.0f);
    auto meterHeight = meterBounds.getHeight() * smoothedLevel;
    auto meterRect = meterBounds.removeFromBottom(meterHeight);
    
    if (meterHeight > 0)
    {
        juce::ColourGradient gradient;
        
        if (smoothedLevel < 0.7f)
        {
            gradient = juce::ColourGradient(meterGradientStart, 0, meterRect.getBottom(),
                                           meterGradientStart.darker(0.3f), 0, meterRect.getY(), false);
        }
        else if (smoothedLevel < 0.9f)
        {
            gradient = juce::ColourGradient(meterGradientMid, 0, meterRect.getBottom(),
                                           meterGradientMid.darker(0.3f), 0, meterRect.getY(), false);
        }
        else
        {
            gradient = juce::ColourGradient(meterGradientEnd, 0, meterRect.getBottom(),
                                           meterGradientEnd.darker(0.3f), 0, meterRect.getY(), false);
        }
        
        g.setGradientFill(gradient);
        g.fillRoundedRectangle(meterRect, 1.0f);
    }
    
    // Peak indicator
    if (displayPeak > 0.01f)
    {
        auto peakY = meterBounds.getBottom() - (meterBounds.getHeight() * displayPeak);
        g.setColour(peakColour);
        g.drawLine(meterBounds.getX(), peakY, meterBounds.getRight(), peakY, 2.0f);
    }
    
    // Tick marks with fixed positions
    g.setColour(juce::Colour(0xff606060));
    const float dbValues[] = { 0.0f, -6.0f, -12.0f, -18.0f, -24.0f, -36.0f, -48.0f };
    
    for (float db : dbValues)
    {
        float normalizedPos = (db + 60.0f) / 60.0f; // Normalize -60 to 0 dB range
        float y = meterBounds.getBottom() - (meterBounds.getHeight() * normalizedPos);
        
        g.drawLine(bounds.getX(), y, bounds.getX() + 3, y, 0.5f);
        g.drawLine(bounds.getRight() - 3, y, bounds.getRight(), y, 0.5f);
    }
}

void LevelMeter::resized()
{
}

void LevelMeter::timerCallback()
{
    // Smooth the meter
    const float attack = 0.9f;
    const float release = 0.95f;
    
    if (level > smoothedLevel)
        smoothedLevel = smoothedLevel * attack + level * (1.0f - attack);
    else
        smoothedLevel = smoothedLevel * release + level * (1.0f - release);
    
    // Peak hold and fall
    if (peakLevel > displayPeak)
    {
        displayPeak = peakLevel;
        peakHoldTime = peakHoldDuration;
    }
    else if (peakHoldTime > 0)
    {
        peakHoldTime -= 1.0f / 30.0f;
    }
    else
    {
        displayPeak *= (1.0f - peakFallRate / 30.0f * 0.01f);
    }
    
    level *= 0.95f;
    peakLevel *= 0.95f;
    
    repaint();
}

void LevelMeter::setLevel(float newLevel)
{
    // Convert linear to dB and then to meter position
    float db = juce::Decibels::gainToDecibels(newLevel + 0.00001f);
    db = juce::jlimit(-60.0f, 0.0f, db);
    level = (db + 60.0f) / 60.0f; // Normalize to 0-1 range
}

void LevelMeter::setPeakLevel(float newPeak)
{
    // Convert linear to dB and then to meter position
    float db = juce::Decibels::gainToDecibels(newPeak + 0.00001f);
    db = juce::jlimit(-60.0f, 0.0f, db);
    peakLevel = (db + 60.0f) / 60.0f; // Normalize to 0-1 range
}