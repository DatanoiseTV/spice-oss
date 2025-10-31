#pragma once

#include <JuceHeader.h>

class SpiceLookAndFeel : public juce::LookAndFeel_V4
{
public:
    SpiceLookAndFeel();
    ~SpiceLookAndFeel() override = default;
    
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                         float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                         juce::Slider& slider) override;
    
    void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                     int buttonX, int buttonY, int buttonW, int buttonH,
                     juce::ComboBox& box) override;
    
    void drawLabel(juce::Graphics& g, juce::Label& label) override;
    
    juce::Font getComboBoxFont(juce::ComboBox&) override;
    
    void drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                             const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted,
                             bool shouldDrawButtonAsDown) override;
    
    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override;
    
    juce::Label* createSliderTextBox(juce::Slider& slider) override;
    
    void setCustomFont(const juce::Font& font) { customFont = font; }
    
private:
    juce::Font customFont;
    juce::Colour primaryColour;
    juce::Colour secondaryColour;
    juce::Colour backgroundColour;
    juce::Colour textColour;
    juce::Colour metalColour;
    juce::Colour shadowColour;
    juce::Colour accentGlow;
    juce::Colour warmGlow;
};