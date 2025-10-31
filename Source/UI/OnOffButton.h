#pragma once

#include <JuceHeader.h>

class OnOffButton : public juce::TextButton
{
public:
    OnOffButton(const juce::String& onText = "ON", const juce::String& offText = "OFF")
        : onText(onText), offText(offText)
    {
        setClickingTogglesState(true);
        updateButtonText();
    }

    void setOnOffText(const juce::String& newOnText, const juce::String& newOffText)
    {
        onText = newOnText;
        offText = newOffText;
        updateButtonText();
        repaint();
    }

    void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        auto bounds = getLocalBounds().toFloat().reduced(2);
        
        // Draw background
        if (getToggleState())
        {
            // ON state - green background
            g.setColour(juce::Colour(0xff2a7f3e));
            g.fillRoundedRectangle(bounds, 4.0f);
            
            // Subtle glow effect
            g.setColour(juce::Colour(0x30ffffff));
            g.drawRoundedRectangle(bounds.reduced(1), 4.0f, 1.0f);
        }
        else
        {
            // OFF state - dark grey background
            g.setColour(juce::Colour(0xff303030));
            g.fillRoundedRectangle(bounds, 4.0f);
            
            // Border
            g.setColour(juce::Colour(0xff505050));
            g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
        }
        
        // Hover effect
        if (shouldDrawButtonAsHighlighted)
        {
            g.setColour(juce::Colour(0x20ffffff));
            g.fillRoundedRectangle(bounds, 4.0f);
        }
        
        // Draw text
        g.setColour(getToggleState() ? juce::Colours::white : juce::Colour(0xffaaaaaa));
        g.setFont(juce::Font(12.0f, juce::Font::bold));
        g.drawText(getToggleState() ? onText : offText, bounds, juce::Justification::centred);
    }
    
    void clicked() override
    {
        TextButton::clicked();
        updateButtonText();
    }
    
private:
    void updateButtonText()
    {
        setButtonText(getToggleState() ? onText : offText);
    }
    
    juce::String onText;
    juce::String offText;
};