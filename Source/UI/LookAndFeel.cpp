#include "LookAndFeel.h"

SpiceLookAndFeel::SpiceLookAndFeel()
{
    // Stunning modern color palette inspired by premium audio gear
    primaryColour = juce::Colour(0xffff4757);        // Vibrant red-orange
    secondaryColour = juce::Colour(0xff57606f);      // Cool blue-grey
    backgroundColour = juce::Colour(0xff0c0c0c);     // Deep black
    textColour = juce::Colour(0xfff1f2f6);           // Pure clean white
    metalColour = juce::Colour(0xff747d8c);          // Premium steel
    shadowColour = juce::Colour(0xff000000);         // Pure black shadow
    accentGlow = juce::Colour(0xff3742fa);           // Electric blue
    warmGlow = juce::Colour(0xffff9f43);             // Warm amber
    
    // Enhanced analog-style color mapping
    setColour(juce::Slider::rotarySliderFillColourId, primaryColour);
    setColour(juce::Slider::rotarySliderOutlineColourId, metalColour);
    setColour(juce::Slider::thumbColourId, juce::Colour(0xffffcc88));
    setColour(juce::Slider::trackColourId, shadowColour);
    
    setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff3a342d));
    setColour(juce::ComboBox::textColourId, textColour);
    setColour(juce::ComboBox::outlineColourId, metalColour);
    setColour(juce::ComboBox::arrowColourId, primaryColour);
    
    // Popup menu colors for black background
    setColour(juce::PopupMenu::backgroundColourId, juce::Colour(0xff000000));
    setColour(juce::PopupMenu::textColourId, textColour);
    setColour(juce::PopupMenu::headerTextColourId, textColour);
    setColour(juce::PopupMenu::highlightedBackgroundColourId, juce::Colour(0xff1a1a1a));
    setColour(juce::PopupMenu::highlightedTextColourId, primaryColour);
    
    setColour(juce::Label::textColourId, textColour);
    
    // Initialize with default font
    customFont = juce::Font();
}

void SpiceLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                             juce::Slider& slider)
{
    auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
    auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    auto centre = bounds.getCentre();
    
    // Draw tick marks around the knob (before the knob so they're behind)
    auto tickRadius = radius + 6;
    int numTicks = 11; // Number of tick marks
    float angleRange = rotaryEndAngle - rotaryStartAngle;
    
    for (int i = 0; i < numTicks; ++i)
    {
        float tickAngle = rotaryStartAngle + (i / float(numTicks - 1)) * angleRange - juce::MathConstants<float>::halfPi;
        float tickLength = (i == 0 || i == numTicks - 1) ? 8.0f : 5.0f; // Longer ticks at extremes
        
        float innerRadius = tickRadius - tickLength;
        float outerRadius = tickRadius;
        
        float x1 = centre.x + std::cos(tickAngle) * innerRadius;
        float y1 = centre.y + std::sin(tickAngle) * innerRadius;
        float x2 = centre.x + std::cos(tickAngle) * outerRadius;
        float y2 = centre.y + std::sin(tickAngle) * outerRadius;
        
        g.setColour(juce::Colour(0xff606060));
        g.drawLine(x1, y1, x2, y2, 1.5f);
    }
    
    // Main knob body - larger and more realistic
    auto knobRadius = radius * 0.85f;
    
    // Outer bezel
    juce::ColourGradient bezelGradient(
        juce::Colour(0xff3a3a3a), centre.x - knobRadius, centre.y - knobRadius,
        juce::Colour(0xff0a0a0a), centre.x + knobRadius, centre.y + knobRadius, true);
    g.setGradientFill(bezelGradient);
    g.fillEllipse(centre.x - knobRadius, centre.y - knobRadius, knobRadius * 2.0f, knobRadius * 2.0f);
    
    // Inner knob with realistic gradient
    auto innerKnobRadius = knobRadius * 0.9f;
    juce::ColourGradient knobGradient(
        juce::Colour(0xff1a1a1a), centre.x, centre.y - innerKnobRadius,
        juce::Colour(0xff050505), centre.x, centre.y + innerKnobRadius, false);
    knobGradient.addColour(0.5f, juce::Colour(0xff0f0f0f));
    g.setGradientFill(knobGradient);
    g.fillEllipse(centre.x - innerKnobRadius, centre.y - innerKnobRadius, innerKnobRadius * 2.0f, innerKnobRadius * 2.0f);
    
    // Subtle texture - concentric circles for realistic feel
    g.setColour(juce::Colour(0x10ffffff));
    for (float r = innerKnobRadius * 0.3f; r < innerKnobRadius * 0.9f; r += 3.0f)
    {
        g.drawEllipse(centre.x - r, centre.y - r, r * 2.0f, r * 2.0f, 0.3f);
    }
    
    // Position indicator - single white line like real hardware
    auto pointerLength = innerKnobRadius * 0.7f;
    auto pointerAngle = toAngle - juce::MathConstants<float>::halfPi;
    auto pointerStartRadius = innerKnobRadius * 0.3f;
    
    auto pointerX1 = centre.x + std::cos(pointerAngle) * pointerStartRadius;
    auto pointerY1 = centre.y + std::sin(pointerAngle) * pointerStartRadius;
    auto pointerX2 = centre.x + std::cos(pointerAngle) * pointerLength;
    auto pointerY2 = centre.y + std::sin(pointerAngle) * pointerLength;
    
    // Pointer shadow
    g.setColour(juce::Colour(0x80000000));
    g.drawLine(pointerX1 + 1, pointerY1 + 1, pointerX2 + 1, pointerY2 + 1, 3.0f);
    
    // Main pointer - bright white
    g.setColour(juce::Colour(0xffe0e0e0));
    g.drawLine(pointerX1, pointerY1, pointerX2, pointerY2, 2.5f);
    
    // Center cap removed - cleaner look without screw head
    
    // Subtle highlights for 3D effect
    g.setColour(juce::Colour(0x20ffffff));
    g.drawEllipse(centre.x - innerKnobRadius * 0.8f, centre.y - innerKnobRadius * 0.8f, 
                  innerKnobRadius * 1.6f, innerKnobRadius * 1.6f, 0.5f);
    
    // Bottom shadow for depth
    juce::ColourGradient shadowGradient(
        juce::Colour(0x40000000), centre.x, centre.y + innerKnobRadius * 0.8f,
        juce::Colour(0x00000000), centre.x, centre.y + innerKnobRadius * 1.2f, false);
    g.setGradientFill(shadowGradient);
    g.fillEllipse(centre.x - innerKnobRadius, centre.y - innerKnobRadius, 
                  innerKnobRadius * 2.0f, innerKnobRadius * 2.0f);
}

void SpiceLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown,
                                         int buttonX, int buttonY, int buttonW, int buttonH,
                                         juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<int>(0, 0, width, height).toFloat();
    auto cornerSize = 4.0f;
    
    // Modern background - very dark
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillRoundedRectangle(bounds, cornerSize);
    
    // Much darker border
    g.setColour(juce::Colour(0xff1a1a1a));
    g.drawRoundedRectangle(bounds.reduced(0.5f), cornerSize, 1.0f);
    
    // Modern dropdown arrow
    auto arrowZone = juce::Rectangle<int>(width - 20, 0, 16, height);
    auto arrowCentreY = arrowZone.getCentreY();
    
    juce::Path arrow;
    arrow.startNewSubPath(arrowZone.getX() + 3, arrowCentreY - 2);
    arrow.lineTo(arrowZone.getCentreX(), arrowCentreY + 2);
    arrow.lineTo(arrowZone.getRight() - 3, arrowCentreY - 2);
    
    g.setColour(primaryColour.withAlpha(isButtonDown ? 1.0f : 0.7f));
    g.strokePath(arrow, juce::PathStrokeType(1.5f, juce::PathStrokeType::curved));
}

void SpiceLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label)
{
    g.fillAll(label.findColour(juce::Label::backgroundColourId));
    
    if (!label.isBeingEdited())
    {
        auto alpha = label.isEnabled() ? 1.0f : 0.5f;
        const juce::Font font(getLabelFont(label));
        
        g.setColour(label.findColour(juce::Label::textColourId).withMultipliedAlpha(alpha));
        g.setFont(font);
        
        auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
        
        g.drawFittedText(label.getText(), textArea, label.getJustificationType(),
                        juce::jmax(1, (int)(textArea.getHeight() / font.getHeight())),
                        label.getMinimumHorizontalScale());
        
        g.setColour(label.findColour(juce::Label::outlineColourId).withMultipliedAlpha(alpha));
    }
    else if (label.isEnabled())
    {
        g.setColour(label.findColour(juce::Label::outlineColourId));
    }
    
    g.drawRect(label.getLocalBounds());
}

juce::Font SpiceLookAndFeel::getComboBoxFont(juce::ComboBox&)
{
    return customFont.getHeight() > 0 ? customFont : juce::Font(16.0f);
}

void SpiceLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button, 
                                           const juce::Colour& backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced(1);
    
    // No border - just a subtle rounded rectangle
    float cornerSize = 6.0f;
    
    // Dark background
    juce::Colour bgColour = backgroundColour;
    
    if (shouldDrawButtonAsDown || button.getToggleState())
    {
        bgColour = bgColour.brighter(0.1f);
    }
    else if (shouldDrawButtonAsHighlighted)
    {
        bgColour = bgColour.brighter(0.05f);
    }
    
    // Subtle shadow
    g.setColour(juce::Colour(0x40000000));
    g.fillRoundedRectangle(bounds.translated(0, 1), cornerSize);
    
    // Main background
    g.setColour(bgColour);
    g.fillRoundedRectangle(bounds, cornerSize);
    
    // Very subtle inner highlight when not pressed
    if (!shouldDrawButtonAsDown && !button.getToggleState())
    {
        g.setColour(juce::Colour(0x10ffffff));
        g.drawRoundedRectangle(bounds.reduced(1), cornerSize - 1, 1.0f);
    }
}

juce::Font SpiceLookAndFeel::getTextButtonFont(juce::TextButton&, int buttonHeight)
{
    if (customFont.getHeight() > 0)
        return customFont.withHeight(buttonHeight * 0.6f);
    return juce::Font(buttonHeight * 0.6f);
}

juce::Label* SpiceLookAndFeel::createSliderTextBox(juce::Slider& slider)
{
    auto* label = LookAndFeel_V4::createSliderTextBox(slider);
    
    // Set black background for the popup value display
    label->setColour(juce::Label::backgroundColourId, juce::Colour(0xff000000));
    label->setColour(juce::Label::textColourId, juce::Colour(0xfff1f2f6));
    label->setColour(juce::Label::outlineColourId, juce::Colour(0xff1a1a1a));
    
    // Set the custom font if available
    if (customFont.getHeight() > 0)
        label->setFont(customFont.withHeight(14.0f));
    
    return label;
}