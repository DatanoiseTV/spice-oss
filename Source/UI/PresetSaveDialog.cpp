#include "PresetSaveDialog.h"

PresetSaveDialog::PresetSaveDialog(const juce::String& currentPresetName)
{
    // Load custom font
    auto dirtyHaroldData = BinaryData::DirtyHarold_ttf;
    auto dirtyHaroldDataSize = BinaryData::DirtyHarold_ttfSize;
    labelFont = juce::Font(juce::Typeface::createSystemTypefaceFor(dirtyHaroldData, dirtyHaroldDataSize));
    titleFont = labelFont.withHeight(20.0f);
    
    // Title
    titleLabel.setText("SAVE PRESET", juce::dontSendNotification);
    titleLabel.setFont(titleFont);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setColour(juce::Label::textColourId, juce::Colour(0xffff9f43));
    addAndMakeVisible(titleLabel);
    
    // Text editor with custom styling
    textEditor.setText(currentPresetName);
    textEditor.setFont(labelFont.withHeight(16.0f));
    textEditor.setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xff1a1a1a));
    textEditor.setColour(juce::TextEditor::textColourId, juce::Colour(0xfff1f2f6));
    textEditor.setColour(juce::TextEditor::outlineColourId, juce::Colour(0xff3a3a3a));
    textEditor.setColour(juce::TextEditor::focusedOutlineColourId, juce::Colour(0xffff4757));
    textEditor.setColour(juce::TextEditor::highlightColourId, juce::Colour(0x60ff4757));
    textEditor.setColour(juce::TextEditor::highlightedTextColourId, juce::Colour(0xffffffff));
    textEditor.setJustification(juce::Justification::centred);
    textEditor.setSelectAllWhenFocused(true);
    addAndMakeVisible(textEditor);
    
    // Save button with custom styling
    saveButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xffff4757));
    saveButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xffff6b6b));
    saveButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffffffff));
    saveButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffffffff));
    saveButton.addListener(this);
    addAndMakeVisible(saveButton);
    
    // Cancel button with custom styling
    cancelButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff3a3a3a));
    cancelButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff4a4a4a));
    cancelButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xffcccccc));
    cancelButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xffffffff));
    cancelButton.addListener(this);
    addAndMakeVisible(cancelButton);
    
    setSize(350, 200);
}

PresetSaveDialog::~PresetSaveDialog()
{
}

void PresetSaveDialog::paint(juce::Graphics& g)
{
    // Dark background with gradient
    auto bounds = getLocalBounds().toFloat();
    juce::ColourGradient backgroundGradient(
        juce::Colour(0xff0a0a0a), bounds.getX(), bounds.getY(),
        juce::Colour(0xff1a1a1a), bounds.getX(), bounds.getBottom(), false);
    g.setGradientFill(backgroundGradient);
    g.fillAll();
    
    // Outer border
    g.setColour(juce::Colour(0xff2a2a2a));
    g.drawRect(bounds, 2.0f);
    
    // Inner glow
    g.setColour(juce::Colour(0x20ff4757));
    g.drawRect(bounds.reduced(2), 1.0f);
    
    // Top accent line
    g.setColour(juce::Colour(0xffff4757));
    g.fillRect(0.0f, 0.0f, bounds.getWidth(), 2.0f);
}

void PresetSaveDialog::resized()
{
    auto area = getLocalBounds().reduced(20);
    
    titleLabel.setBounds(area.removeFromTop(30));
    area.removeFromTop(10);
    
    textEditor.setBounds(area.removeFromTop(40));
    area.removeFromTop(20);
    
    auto buttonArea = area.removeFromTop(35);
    auto buttonWidth = 100;
    auto spacing = 20;
    auto totalWidth = buttonWidth * 2 + spacing;
    auto startX = (getWidth() - totalWidth) / 2;
    
    cancelButton.setBounds(startX, buttonArea.getY(), buttonWidth, buttonArea.getHeight());
    saveButton.setBounds(startX + buttonWidth + spacing, buttonArea.getY(), buttonWidth, buttonArea.getHeight());
}

void PresetSaveDialog::buttonClicked(juce::Button* button)
{
    if (button == &saveButton)
    {
        if (onComplete)
            onComplete(true);
    }
    else if (button == &cancelButton)
    {
        if (onComplete)
            onComplete(false);
    }
}