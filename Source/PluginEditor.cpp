#include "PluginProcessor.h"
#include "PluginEditor.h"   

SpiceAudioProcessorEditor::SpiceAudioProcessorEditor (SpiceAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), gateIndicatorLED(p)
{
    // Load custom fonts
    auto piximData = BinaryData::Pixim_otf;
    auto piximDataSize = BinaryData::Pixim_otfSize;
    aveschonFont = juce::Font(juce::Typeface::createSystemTypefaceFor(piximData, piximDataSize));
    
    auto dirtyHaroldData = BinaryData::DirtyHarold_ttf;
    auto dirtyHaroldDataSize = BinaryData::DirtyHarold_ttfSize;
    dirtyHaroldFont = juce::Font(juce::Typeface::createSystemTypefaceFor(dirtyHaroldData, dirtyHaroldDataSize));
    
    lookAndFeel.setCustomFont(dirtyHaroldFont.withHeight(16.0f));
    setLookAndFeel(&lookAndFeel);
    
    inputGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    inputGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    inputGainSlider.setPopupDisplayEnabled(true, false, this);
    inputGainSlider.setTextValueSuffix(" dB");
    inputGainSlider.setTooltip("Input gain - drive the saturation harder");
    addAndMakeVisible(inputGainSlider);
    inputGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "inputGain", inputGainSlider);
    
    inputGainLabel.setText("INPUT", juce::dontSendNotification);
    inputGainLabel.setJustificationType(juce::Justification::centred);
    inputGainLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(inputGainLabel);
    
    driveSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    driveSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    driveSlider.setPopupDisplayEnabled(true, false, this);
    driveSlider.setTextValueSuffix(" %");
    driveSlider.setTooltip("Turn up the heat! Controls how spicy your signal gets");
    addAndMakeVisible(driveSlider);
    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "drive", driveSlider);
    
    driveLabel.setText("DRIVE", juce::dontSendNotification);
    driveLabel.setJustificationType(juce::Justification::centred);
    driveLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(driveLabel);
    
    mixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    mixSlider.setPopupDisplayEnabled(true, false, this);
    mixSlider.setTextValueSuffix(" %");
    mixSlider.setTooltip("Mix your spice blend - from mild to wild!");
    addAndMakeVisible(mixSlider);
    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "mix", mixSlider);
    
    mixLabel.setText("MIX", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    mixLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(mixLabel);
    
    outputSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    outputSlider.setPopupDisplayEnabled(true, false, this);
    outputSlider.setTextValueSuffix(" dB");
    outputSlider.setTooltip("Adjust the final taste - how loud do you want it?");
    addAndMakeVisible(outputSlider);
    outputAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "output", outputSlider);
    
    outputLabel.setText("OUTPUT", juce::dontSendNotification);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(outputLabel);
    
    toneSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    toneSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    toneSlider.setPopupDisplayEnabled(true, false, this);
    toneSlider.setTextValueSuffix(" %");
    toneSlider.setTooltip("Season to taste: Left = Smoky/Dark, Right = Bright/Tangy");
    addAndMakeVisible(toneSlider);
    toneAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "tone", toneSlider);
    
    toneLabel.setText("TONE", juce::dontSendNotification);
    toneLabel.setJustificationType(juce::Justification::centred);
    toneLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(toneLabel);
    
    biasSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    biasSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    biasSlider.setPopupDisplayEnabled(true, false, this);
    biasSlider.setTooltip("Add some zest! Controls harmonic complexity and character");
    addAndMakeVisible(biasSlider);
    biasAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "bias", biasSlider);
    
    biasLabel.setText("BIAS", juce::dontSendNotification);
    biasLabel.setJustificationType(juce::Justification::centred);
    biasLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(biasLabel);
    
    // Pre-FX Controls
    lowCutSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    lowCutSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    lowCutSlider.setPopupDisplayEnabled(true, false, this);
    lowCutSlider.setTextValueSuffix(" Hz");
    lowCutSlider.setTooltip("Low cut frequency - removes unwanted low-end rumble");
    addAndMakeVisible(lowCutSlider);
    lowCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "lowCut", lowCutSlider);
    
    lowCutLabel.setText("LOW CUT", juce::dontSendNotification);
    lowCutLabel.setJustificationType(juce::Justification::centred);
    lowCutLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(lowCutLabel);
    
    highCutSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    highCutSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    highCutSlider.setPopupDisplayEnabled(true, false, this);
    highCutSlider.setTextValueSuffix(" Hz");
    highCutSlider.setTooltip("High cut frequency - tames harsh highs");
    addAndMakeVisible(highCutSlider);
    highCutAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "highCut", highCutSlider);
    
    highCutLabel.setText("HIGH CUT", juce::dontSendNotification);
    highCutLabel.setJustificationType(juce::Justification::centred);
    highCutLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(highCutLabel);
    
    gateThresholdSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    gateThresholdSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    gateThresholdSlider.setPopupDisplayEnabled(true, false, this);
    gateThresholdSlider.setTextValueSuffix(" dB");
    gateThresholdSlider.setTooltip("Noise gate threshold - cuts signal below this level");
    addAndMakeVisible(gateThresholdSlider);
    gateThresholdAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "gateThreshold", gateThresholdSlider);
    
    gateLabel.setText("GATE", juce::dontSendNotification);
    gateLabel.setJustificationType(juce::Justification::centred);
    gateLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(gateLabel);
    
    // Gate enable button
    gateEnabledButton.setOnOffText("GATE ON", "GATE OFF");
    addAndMakeVisible(gateEnabledButton);
    gateEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "gateEnabled", gateEnabledButton);
    
    // Gate indicator LED
    addAndMakeVisible(gateIndicatorLED);
    
    // Cabinet controls
    cabinetModelSelector.addItem("1x12 Vintage", 1);
    cabinetModelSelector.addItem("2x10 Tweed", 2);
    cabinetModelSelector.addItem("1x15 Bass", 3);
    cabinetModelSelector.addItem("2x12 Modern", 4);
    cabinetModelSelector.addItem("4x10 Clean", 5);
    cabinetModelSelector.addItem("1x12 British", 6);
    cabinetModelSelector.addItem("4x12 Vintage", 7);
    cabinetModelSelector.addItem("4x12 Modern", 8);
    cabinetModelSelector.addItem("1x12 Jazz", 9);
    cabinetModelSelector.addItem("2x12 Vintage", 10);
    addAndMakeVisible(cabinetModelSelector);
    cabinetModelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "cabinetModel", cabinetModelSelector);
    
    cabinetModelLabel.setText("CABINET", juce::dontSendNotification);
    cabinetModelLabel.setJustificationType(juce::Justification::centred);
    cabinetModelLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(cabinetModelLabel);
    
    cabinetPresenceSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    cabinetPresenceSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    cabinetPresenceSlider.setPopupDisplayEnabled(true, false, this);
    cabinetPresenceSlider.setTextValueSuffix(" %");
    cabinetPresenceSlider.setTooltip("Cabinet presence - controls high frequency character");
    addAndMakeVisible(cabinetPresenceSlider);
    cabinetPresenceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "cabinetPresence", cabinetPresenceSlider);
    
    cabinetPresenceLabel.setText("MIC DIST", juce::dontSendNotification);
    cabinetPresenceLabel.setJustificationType(juce::Justification::centred);
    cabinetPresenceLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(cabinetPresenceLabel);
    
    cabinetMixSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    cabinetMixSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    cabinetMixSlider.setPopupDisplayEnabled(true, false, this);
    cabinetMixSlider.setTextValueSuffix(" %");
    cabinetMixSlider.setTooltip("Cabinet mix - blend between dry and processed signal");
    addAndMakeVisible(cabinetMixSlider);
    cabinetMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "cabinetMix", cabinetMixSlider);
    
    cabinetMixLabel.setText("MIX", juce::dontSendNotification);
    cabinetMixLabel.setJustificationType(juce::Justification::centred);
    cabinetMixLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(cabinetMixLabel);
    
    // Cabinet enable button
    // Cabinet enable button removed - now using power button next to POST-FX title
    
    // Section title labels
    preFXTitleLabel.setText("PRE-FX", juce::dontSendNotification);
    preFXTitleLabel.setJustificationType(juce::Justification::centred);
    preFXTitleLabel.setFont(dirtyHaroldFont.withHeight(20.0f));
    preFXTitleLabel.setColour(juce::Label::textColourId, juce::Colour(0xffff9f43));
    addAndMakeVisible(preFXTitleLabel);
    
    postFXTitleLabel.setText("POST-FX", juce::dontSendNotification);
    postFXTitleLabel.setJustificationType(juce::Justification::centredLeft);
    postFXTitleLabel.setFont(dirtyHaroldFont.withHeight(20.0f));
    postFXTitleLabel.setColour(juce::Label::textColourId, juce::Colour(0xffff9f43));
    addAndMakeVisible(postFXTitleLabel);
    
    // Small power button for POST-FX
    addAndMakeVisible(postFXPowerButton);
    cabinetEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "cabinetEnabled", postFXPowerButton);
    
    modelSelector.addItem("TUBE SATURATION", 1);
    modelSelector.addItem("TRANSISTOR DRIVE", 2);
    modelSelector.addItem("TRANSFORMER SATURATION", 3);
    modelSelector.addItem("TAPE COMPRESSION", 4);
    modelSelector.addItem("DIODE CLIPPING", 5);
    modelSelector.addItem("VINTAGE CONSOLE", 6);
    modelSelector.addItem("WARM PLEXI", 7);
    modelSelector.addItem("BRIGHT CRYSTAL", 8);
    modelSelector.addItem("FUZZ BOX", 9);
    modelSelector.addItem("OVERDRIVE", 10);
    modelSelector.addItem("12AX7 TUBE", 11);
    addAndMakeVisible(modelSelector);
    modelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "model", modelSelector);
    
    previousModelButton.addListener(this);
    previousModelButton.setLookAndFeel(&lookAndFeel);
    previousModelButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0a0a0a));
    previousModelButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff606060));
    addAndMakeVisible(previousModelButton);
    
    nextModelButton.addListener(this);
    nextModelButton.setLookAndFeel(&lookAndFeel);
    nextModelButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0a0a0a));
    nextModelButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff606060));
    addAndMakeVisible(nextModelButton);
    
    modelLabel.setText("MODEL", juce::dontSendNotification);
    modelLabel.setJustificationType(juce::Justification::centred);
    modelLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(modelLabel);
    
    qualitySelector.addItem("ECO", 1);
    qualitySelector.addItem("PRO", 2);
    qualitySelector.addItem("ULTRA", 3);
    addAndMakeVisible(qualitySelector);
    qualityAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        audioProcessor.getAPVTS(), "quality", qualitySelector);
    
    qualityLabel.setText("QUALITY", juce::dontSendNotification);
    qualityLabel.setJustificationType(juce::Justification::centred);
    qualityLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(qualityLabel);
    
    // Level meters with ff_meters
    inputMeter.setLookAndFeel(&meterLookAndFeel);
    outputMeter.setLookAndFeel(&meterLookAndFeel);
    
    inputMeter.setMeterSource(&audioProcessor.getInputMeterSource());
    outputMeter.setMeterSource(&audioProcessor.getOutputMeterSource());
    
    // Configure meters with dark styling
    meterLookAndFeel.setColour(foleys::LevelMeter::lmBackgroundColour, juce::Colour(0xff000000));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterBackgroundColour, juce::Colour(0xff0a0a0a));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterOutlineColour, juce::Colour(0xff1a1a1a));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterGradientLowColour, juce::Colour(0xff2ecc71));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterGradientMidColour, juce::Colour(0xffffb347));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterGradientMaxColour, juce::Colour(0xffff4757));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmTicksColour, juce::Colour(0xff606060));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmOutlineColour, juce::Colour(0xff2c2c2c));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmTextColour, juce::Colour(0xfff1f2f6));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterMaxNormalColour, juce::Colour(0xfff1f2f6));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterMaxWarnColour, juce::Colour(0xffffb347));
    meterLookAndFeel.setColour(foleys::LevelMeter::lmMeterMaxOverColour, juce::Colour(0xffff4757));
    
    // Set meter flags for vintage VU meter appearance without dB labels
    inputMeter.setMeterFlags(foleys::LevelMeter::Default); // No dB values displayed
    outputMeter.setMeterFlags(foleys::LevelMeter::Default); // No dB values displayed
    
    // Set refresh rate
    inputMeter.setRefreshRateHz(30);
    outputMeter.setRefreshRateHz(30);
    
    addAndMakeVisible(inputMeter);
    addAndMakeVisible(outputMeter);
    
    inputMeterLabel.setText("INPUT", juce::dontSendNotification);
    inputMeterLabel.setJustificationType(juce::Justification::centred);
    inputMeterLabel.setColour(juce::Label::textColourId, juce::Colour(0xffe8e8e8));
    inputMeterLabel.setFont(dirtyHaroldFont.withHeight(14.0f));
    addAndMakeVisible(inputMeterLabel);
    
    outputMeterLabel.setText("OUTPUT", juce::dontSendNotification);
    outputMeterLabel.setJustificationType(juce::Justification::centred);
    outputMeterLabel.setColour(juce::Label::textColourId, juce::Colour(0xffe8e8e8));
    outputMeterLabel.setFont(dirtyHaroldFont.withHeight(14.0f));
    addAndMakeVisible(outputMeterLabel);
    
    // Waveform visualizer
    addAndMakeVisible(waveformDisplay);
    
    // Analog saturation lamp
    addAndMakeVisible(saturationLamp);
    
    // Preset controls
    presetSelector.addListener(this);
    addAndMakeVisible(presetSelector);
    refreshPresetList();
    
    previousPresetButton.addListener(this);
    previousPresetButton.setLookAndFeel(&lookAndFeel);
    previousPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0a0a0a));
    previousPresetButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff606060));
    addAndMakeVisible(previousPresetButton);
    
    nextPresetButton.addListener(this);
    nextPresetButton.setLookAndFeel(&lookAndFeel);
    nextPresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0a0a0a));
    nextPresetButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff606060));
    addAndMakeVisible(nextPresetButton);
    
    savePresetButton.addListener(this);
    savePresetButton.setLookAndFeel(&lookAndFeel);
    savePresetButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0a0a0a));
    savePresetButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff606060));
    addAndMakeVisible(savePresetButton);
    
    // Bypass button
    bypassButton.setOnOffText("BYPASS ON", "BYPASS OFF");
    addAndMakeVisible(bypassButton);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "bypass", bypassButton);
    
    // Limiter button
    limiterEnabledButton.setOnOffText("LIMITER ON", "LIMITER OFF");
    addAndMakeVisible(limiterEnabledButton);
    limiterEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "limiterEnabled", limiterEnabledButton);
    
    // Mid-Side processing button
    addAndMakeVisible(midSideEnabledButton);
    midSideEnabledAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "midSideEnabled", midSideEnabledButton);
    
    // Auto-gain button
    autoGainButton.setOnOffText("AUTO GAIN ON", "AUTO GAIN OFF");
    addAndMakeVisible(autoGainButton);
    autoGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        audioProcessor.getAPVTS(), "autoGain", autoGainButton);
    
    // Mid-side section title
    midSideTitleLabel.setText("MID-SIDE", juce::dontSendNotification);
    midSideTitleLabel.setJustificationType(juce::Justification::centred);
    midSideTitleLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    midSideTitleLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(midSideTitleLabel);
    
    // Mid gain slider
    midGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    midGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    midGainSlider.setPopupDisplayEnabled(true, false, this);
    midGainSlider.setTextValueSuffix(" dB");
    midGainSlider.setTooltip("Mid channel gain - controls center/mono information");
    addAndMakeVisible(midGainSlider);
    midGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "midGain", midGainSlider);
    
    midGainLabel.setText("MID", juce::dontSendNotification);
    midGainLabel.setJustificationType(juce::Justification::centred);
    midGainLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(midGainLabel);
    
    // Side gain slider
    sideGainSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    sideGainSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    sideGainSlider.setPopupDisplayEnabled(true, false, this);
    sideGainSlider.setTextValueSuffix(" dB");
    sideGainSlider.setTooltip("Side channel gain - controls stereo width information");
    addAndMakeVisible(sideGainSlider);
    sideGainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "sideGain", sideGainSlider);
    
    sideGainLabel.setText("SIDE", juce::dontSendNotification);
    sideGainLabel.setJustificationType(juce::Justification::centred);
    sideGainLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(sideGainLabel);
    
    // Stereo width slider
    stereoWidthSlider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    stereoWidthSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 60, 20);
    stereoWidthSlider.setPopupDisplayEnabled(true, false, this);
    stereoWidthSlider.setTextValueSuffix(" %");
    stereoWidthSlider.setTooltip("Stereo width - 0% = mono, 100% = normal, 300% = super wide");
    addAndMakeVisible(stereoWidthSlider);
    stereoWidthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        audioProcessor.getAPVTS(), "stereoWidth", stereoWidthSlider);
    
    stereoWidthLabel.setText("WIDTH", juce::dontSendNotification);
    stereoWidthLabel.setJustificationType(juce::Justification::centred);
    stereoWidthLabel.setFont(dirtyHaroldFont.withHeight(16.0f));
    addAndMakeVisible(stereoWidthLabel);
    
    // Compact view button with dark theme styling
    compactViewButton.setClickingTogglesState(true);
    compactViewButton.setColour(juce::TextButton::buttonColourId, juce::Colour(0xff0a0a0a));
    compactViewButton.setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xff1a4a66)); // Dark blue when active
    compactViewButton.setColour(juce::TextButton::textColourOffId, juce::Colour(0xff606060));
    compactViewButton.setColour(juce::TextButton::textColourOnId, juce::Colour(0xff57c4f1));
    compactViewButton.setColour(juce::ComboBox::outlineColourId, juce::Colours::transparentBlack);
    compactViewButton.addListener(this);
    addAndMakeVisible(compactViewButton);
    // compactViewAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
    //     audioProcessor.getAPVTS(), "compactView", compactViewButton);
    
    presetLabel.setText("PRESET", juce::dontSendNotification);
    presetLabel.setJustificationType(juce::Justification::centred);
    presetLabel.setFont(dirtyHaroldFont.withHeight(18.0f));
    addAndMakeVisible(presetLabel);
    
    setSize (900, 680);
    startTimerHz(30);
}

SpiceAudioProcessorEditor::~SpiceAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel(nullptr);
}

void SpiceAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Load and draw background texture
    if (!backgroundTexture.isValid())
    {
        auto bgData = BinaryData::BgTexture_jpg;
        auto bgSize = BinaryData::BgTexture_jpgSize;
        backgroundTexture = juce::ImageCache::getFromMemory(bgData, bgSize);
    }
    
    if (backgroundTexture.isValid())
    {
        // Draw texture at 1:1 scale, anchored at top-left
        g.setOpacity(0.8f); // Slightly transparent to keep it subtle
        
        // Draw the image at its natural size, starting from top-left corner
        g.drawImageAt(backgroundTexture, 0, 0);
    }
    else
    {
        // Fallback to gradient if texture fails to load
        juce::ColourGradient darkGradient(
            juce::Colour(0xff000000), bounds.getX(), bounds.getY(),
            juce::Colour(0xff050505), bounds.getX(), bounds.getBottom(), false);
        darkGradient.addColour(0.3, juce::Colour(0xff020202));
        darkGradient.addColour(0.7, juce::Colour(0xff010101));
        g.setGradientFill(darkGradient);
        g.fillAll();
    }
    
    // Premium glass panel effect
    auto mainPanel = bounds.reduced(8);
    
    // Dark glass background with subtle effect
    juce::ColourGradient glassGradient(
        juce::Colour(0x10ffffff), mainPanel.getX(), mainPanel.getY(),
        juce::Colour(0x05ffffff), mainPanel.getX(), mainPanel.getBottom(), false);
    g.setGradientFill(glassGradient);
    g.fillRoundedRectangle(mainPanel, 16);
    
    // Subtle dark border
    g.setColour(juce::Colour(0x20ffffff));
    g.drawRoundedRectangle(mainPanel, 16, 1.5f);
    
    // Subtle inner glow
    g.setColour(juce::Colour(0x10ff4757));
    g.drawRoundedRectangle(mainPanel.reduced(1), 15, 1.0f);
    
    // Dynamic title with glow effects
    auto headerArea = mainPanel.removeFromTop(80);
    headerArea.translate(0, 10); // Move title 10 pixels down
    
    // Dynamic brand glow based on saturation level - COMMENTED OUT
    // auto saturationLevel = waveformDisplay.getSaturationLevel();
    // auto isClipping = waveformDisplay.isClipping();
    
    // Fixed white color for logo
    juce::Colour glowColour = juce::Colour(0x40ff4757);
    juce::Colour brandColour = juce::Colour(0xfff1f2f6);
    
    // COMMENTED OUT - Dynamic color behavior
    // if (isClipping) {
    //     glowColour = juce::Colour(0x80ff1744); // Intense red glow when clipping
    //     brandColour = juce::Colour(0xffff4757); // Red brand text
    // } else if (saturationLevel > 0.5f) {
    //     auto intensity = (saturationLevel - 0.5f) * 2.0f; // 0-1 range
    //     glowColour = juce::Colour(0x40ff9f43).withAlpha(0.2f + intensity * 0.6f); // Warm amber glow
    //     brandColour = juce::Colour(0xfff1f2f6).interpolatedWith(juce::Colour(0xffff9f43), intensity * 0.3f);
    // }
    
    // Brand with glow effect
    g.setColour(glowColour);
    g.setFont(aveschonFont.withHeight(48.0f));
    g.drawText("SPICE", headerArea.expanded(2), juce::Justification::centred, true);
    
    // Main brand text
    g.setColour(brandColour);
    g.setFont(aveschonFont.withHeight(46.0f));
    g.drawText("SPICE", headerArea, juce::Justification::centred, true);
    
    // Removed section separators for cleaner look
    
    // Subtle ambient light effects in corners
    drawAmbientLight(g, bounds.getTopLeft(), juce::Colour(0x10ff4757));
    drawAmbientLight(g, bounds.getTopRight(), juce::Colour(0x10ff9f43));
    drawAmbientLight(g, bounds.getBottomLeft(), juce::Colour(0x10ff4757));
    drawAmbientLight(g, bounds.getBottomRight(), juce::Colour(0x10ff9f43));
    
    // Draw vintage meter panel with bezels
    auto meterPanelBounds = juce::Rectangle<float>(
        inputMeter.getX() - 15,
        inputMeter.getY() - 40,
        (outputMeter.getRight() - inputMeter.getX()) + 30,
        inputMeter.getHeight() + 60
    );
    
    // Panel background - dark grey matching color scheme
    juce::ColourGradient panelGradient(
        juce::Colour(0xff151515), meterPanelBounds.getTopLeft(),
        juce::Colour(0xff0a0a0a), meterPanelBounds.getBottomRight(), true);
    g.setGradientFill(panelGradient);
    g.fillRoundedRectangle(meterPanelBounds, 6.0f);
    
    // Panel edge highlight
    g.setColour(juce::Colour(0xff252525));
    g.drawRoundedRectangle(meterPanelBounds, 6.0f, 2.0f);
    
    // Inner shadow
    g.setColour(juce::Colour(0x80000000));
    g.drawRoundedRectangle(meterPanelBounds.reduced(1), 5.0f, 1.0f);
    
    // Draw screws
    auto drawScrew = [&](float x, float y)
    {
        auto screwRadius = 4.0f;
        // Screw head - metallic grey
        juce::ColourGradient screwGrad(
            juce::Colour(0xff404040), x - screwRadius, y - screwRadius,
            juce::Colour(0xff1a1a1a), x + screwRadius, y + screwRadius, true);
        g.setGradientFill(screwGrad);
        g.fillEllipse(x - screwRadius, y - screwRadius, screwRadius * 2, screwRadius * 2);
        
        // Screw rim
        g.setColour(juce::Colour(0xff2a2a2a));
        g.drawEllipse(x - screwRadius, y - screwRadius, screwRadius * 2, screwRadius * 2, 0.5f);
        
        // Screw slot
        g.setColour(juce::Colour(0xff000000));
        g.drawLine(x - screwRadius * 0.6f, y, x + screwRadius * 0.6f, y, 1.5f);
    };
    
    // Corner screws
    drawScrew(meterPanelBounds.getX() + 10, meterPanelBounds.getY() + 10);
    drawScrew(meterPanelBounds.getRight() - 10, meterPanelBounds.getY() + 10);
    drawScrew(meterPanelBounds.getX() + 10, meterPanelBounds.getBottom() - 10);
    drawScrew(meterPanelBounds.getRight() - 10, meterPanelBounds.getBottom() - 10);
    
    // Individual meter bezels
    auto drawMeterBezel = [&](juce::Rectangle<int> meterBounds)
    {
        auto bezelBounds = meterBounds.toFloat().expanded(3);
        
        // Bezel gradient - dark grey
        juce::ColourGradient bezelGrad(
            juce::Colour(0xff303030), bezelBounds.getTopLeft(),
            juce::Colour(0xff121212), bezelBounds.getBottomRight(), true);
        g.setGradientFill(bezelGrad);
        g.fillRoundedRectangle(bezelBounds, 3.0f);
        
        // Inner edge
        g.setColour(juce::Colour(0xff0a0a0a));
        g.drawRoundedRectangle(bezelBounds.reduced(1), 2.0f, 1.0f);
    };
    
    drawMeterBezel(inputMeter.getBounds());
    drawMeterBezel(outputMeter.getBounds());
    
    // Draw lamp panel with screws
    auto lampBounds = saturationLamp.getBounds().toFloat();
    auto lampPanelBounds = lampBounds.expanded(8).translated(0, -2); // Much smaller panel, slight position adjustment
    
    // Lamp panel background
    juce::ColourGradient lampPanelGrad(
        juce::Colour(0xff1a1a1a), lampPanelBounds.getTopLeft(),
        juce::Colour(0xff0a0a0a), lampPanelBounds.getBottomRight(), true);
    g.setGradientFill(lampPanelGrad);
    g.fillRoundedRectangle(lampPanelBounds, 4.0f);
    
    // Panel edge
    g.setColour(juce::Colour(0xff2a2a2a));
    g.drawRoundedRectangle(lampPanelBounds, 4.0f, 1.0f);
    
    // Lamp screws - 4 around the lamp (closer to lamp)
    drawScrew(lampPanelBounds.getX() + 6, lampPanelBounds.getY() + 6);
    drawScrew(lampPanelBounds.getRight() - 6, lampPanelBounds.getY() + 6);
    drawScrew(lampPanelBounds.getX() + 6, lampPanelBounds.getBottom() - 6);
    drawScrew(lampPanelBounds.getRight() - 6, lampPanelBounds.getBottom() - 6);
}

void SpiceAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(16);
    area.removeFromTop(88); // Space for stunning header
    
    // Revolutionary circular layout inspired by premium hardware
    auto centerX = area.getCentreX();
    auto centerY = area.getCentreY() + 20;
    auto mainRadius = 180;
    
    // Central waveform display - the heart of the plugin
    auto waveformSize = 160;
    waveformDisplay.setBounds(centerX - waveformSize/2, centerY - waveformSize/2, waveformSize, waveformSize);
    
    // Model selector at the top left (made narrower)
    auto modelArea = juce::Rectangle<int>(20, area.getY() - 10, 280, 45);
    modelLabel.setBounds(modelArea.removeFromTop(22));
    auto modelControls = modelArea;
    previousModelButton.setBounds(modelControls.removeFromLeft(30));
    modelControls.removeFromLeft(2);
    modelSelector.setBounds(modelControls.removeFromLeft(214));
    modelControls.removeFromLeft(2);
    nextModelButton.setBounds(modelControls.removeFromLeft(30));
    
    // Preset controls in the middle top
    auto presetArea = juce::Rectangle<int>(centerX - 125, area.getY() - 10, 250, 45);
    presetLabel.setBounds(presetArea.removeFromTop(22));
    auto presetControls = presetArea;
    previousPresetButton.setBounds(presetControls.removeFromLeft(30));
    presetControls.removeFromLeft(2);
    nextPresetButton.setBounds(presetControls.removeFromLeft(30));
    presetControls.removeFromLeft(5);
    presetSelector.setBounds(presetControls.removeFromLeft(130));
    presetControls.removeFromLeft(5);
    savePresetButton.setBounds(presetControls);
    
    // Quality selector in bottom right corner - narrower and positioned better
    auto qualityArea = juce::Rectangle<int>(getWidth() - 120, area.getBottom() - 50, 100, 45);
    qualityLabel.setBounds(qualityArea.removeFromTop(22));
    qualitySelector.setBounds(qualityArea);
    
    // Circular arrangement of controls around the center
    auto knobSize = 100;
    auto labelHeight = 25;
    
    // HEAT at top-left (11 o'clock position)
    auto heatAngle = -juce::MathConstants<float>::pi * 0.25f;
    auto heatX = centerX + std::cos(heatAngle) * mainRadius - knobSize/2;
    auto heatY = centerY + std::sin(heatAngle) * mainRadius - knobSize/2;
    driveLabel.setBounds(heatX, heatY - labelHeight - 5, knobSize, labelHeight);
    driveSlider.setBounds(heatX, heatY, knobSize, knobSize);
    
    // FLAVOR at top-right (1 o'clock position)
    auto flavorAngle = -juce::MathConstants<float>::pi * 0.75f;
    auto flavorX = centerX + std::cos(flavorAngle) * mainRadius - knobSize/2;
    auto flavorY = centerY + std::sin(flavorAngle) * mainRadius - knobSize/2;
    toneLabel.setBounds(flavorX, flavorY - labelHeight - 5, knobSize, labelHeight);
    toneSlider.setBounds(flavorX, flavorY, knobSize, knobSize);
    
    // TASTE (OUTPUT) at bottom-left (7 o'clock position) - swapped positions
    auto tasteAngle = juce::MathConstants<float>::pi * 0.25f;
    auto tasteX = centerX + std::cos(tasteAngle) * mainRadius - knobSize/2;
    auto tasteY = centerY + std::sin(tasteAngle) * mainRadius - knobSize/2;
    outputLabel.setBounds(tasteX, tasteY + knobSize + 5, knobSize, labelHeight);
    outputSlider.setBounds(tasteX, tasteY, knobSize, knobSize);
    
    // BLEND (MIX) at bottom-right (5 o'clock position) - swapped positions
    auto blendAngle = juce::MathConstants<float>::pi * 0.75f;
    auto blendX = centerX + std::cos(blendAngle) * mainRadius - knobSize/2;
    auto blendY = centerY + std::sin(blendAngle) * mainRadius - knobSize/2;
    mixLabel.setBounds(blendX, blendY + knobSize + 5, knobSize, labelHeight);
    mixSlider.setBounds(blendX, blendY, knobSize, knobSize);
    
    // Pre-FX section variables needed for INPUT positioning
    auto preFXKnobSize = 60;
    auto preFXLabelHeight = 18;
    auto leftColumnX = 30;
    auto preFXStartY = centerY - 90; // Positioned on left side
    
    // INPUT now positioned where lamp was (above waveform) - moved further up
    auto inputKnobSize = 80;
    auto inputLabelHeight = 20;
    inputGainLabel.setBounds(centerX - 50, centerY - waveformSize/2 - 130, inputKnobSize + 20, inputLabelHeight);
    inputGainSlider.setBounds(centerX - 40, centerY - waveformSize/2 - 105, inputKnobSize, inputKnobSize);
    
    // ZEST at left side (9 o'clock position)
    auto zestAngle = juce::MathConstants<float>::pi;
    auto zestX = centerX + std::cos(zestAngle) * mainRadius - knobSize/2;
    auto zestY = centerY + std::sin(zestAngle) * mainRadius - knobSize/2;
    biasLabel.setBounds(zestX - 30, zestY - labelHeight - 5, knobSize + 60, labelHeight);
    biasSlider.setBounds(zestX, zestY, knobSize, knobSize);
    
    // Premium meters positioned elegantly on the right with wider vintage design
    auto meterX = centerX + mainRadius + 70;
    auto meterWidth = 70; // Even wider to accommodate level text
    auto meterHeight = 180;
    auto meterY = centerY - meterHeight/2;
    auto meterSpacing = 80; // Increased spacing for wider meters
    
    inputMeterLabel.setBounds(meterX - 5, meterY - 25, meterWidth + 10, 20);
    inputMeter.setBounds(meterX, meterY, meterWidth, meterHeight);
    
    outputMeterLabel.setBounds(meterX + meterSpacing - 5, meterY - 25, meterWidth + 10, 20);
    outputMeter.setBounds(meterX + meterSpacing, meterY, meterWidth, meterHeight);
    
    // Analog lamp positioned at bottom center
    saturationLamp.setBounds(centerX - 30, area.getBottom() - 100, 60, 60);
    
    // Mid-Side processing section positioned above pre-fx
    auto midSideKnobSize = 50;
    auto midSideLabelHeight = 16;
    auto midSideStartX = leftColumnX;
    auto midSideStartY = preFXStartY - 110; // Above pre-fx - moved 40px up total
    auto midSideSpacing = 65;
    
    // MID-SIDE title and enable button
    midSideTitleLabel.setBounds(midSideStartX - 10, midSideStartY - 20, 120, 20);
    midSideEnabledButton.setBounds(midSideStartX + 110, midSideStartY - 20, 50, 20); // ON/OFF button
    
    // Mid gain control
    midGainLabel.setBounds(midSideStartX, midSideStartY + 10, midSideKnobSize + 10, midSideLabelHeight);
    midGainSlider.setBounds(midSideStartX, midSideStartY + 10 + midSideLabelHeight + 3, midSideKnobSize, midSideKnobSize);
    
    // Side gain control
    sideGainLabel.setBounds(midSideStartX + midSideSpacing, midSideStartY + 10, midSideKnobSize + 10, midSideLabelHeight);
    sideGainSlider.setBounds(midSideStartX + midSideSpacing, midSideStartY + 10 + midSideLabelHeight + 3, midSideKnobSize, midSideKnobSize);
    
    // Stereo width control
    stereoWidthLabel.setBounds(midSideStartX + midSideSpacing * 2, midSideStartY + 10, midSideKnobSize + 10, midSideLabelHeight);
    stereoWidthSlider.setBounds(midSideStartX + midSideSpacing * 2, midSideStartY + 10 + midSideLabelHeight + 3, midSideKnobSize, midSideKnobSize);
    
    // Pre-FX section layout continues (moved down)
    auto preFXSpacing = 80;
    auto preFXActualStartY = midSideStartY + 120; // Below mid-side section with more gap
    
    // PRE-FX title
    preFXTitleLabel.setBounds(leftColumnX - 10, preFXActualStartY - 30, preFXKnobSize + 20, 25);
    
    // Low Cut
    lowCutLabel.setBounds(leftColumnX - 10, preFXActualStartY, preFXKnobSize + 20, preFXLabelHeight);
    lowCutSlider.setBounds(leftColumnX, preFXActualStartY + preFXLabelHeight + 5, preFXKnobSize, preFXKnobSize);
    
    // High Cut  
    highCutLabel.setBounds(leftColumnX - 10, preFXActualStartY + preFXSpacing, preFXKnobSize + 20, preFXLabelHeight);
    highCutSlider.setBounds(leftColumnX, preFXActualStartY + preFXSpacing + preFXLabelHeight + 5, preFXKnobSize, preFXKnobSize);
    
    // Gate controls
    gateLabel.setBounds(leftColumnX - 10, preFXActualStartY + preFXSpacing * 2, preFXKnobSize + 20, preFXLabelHeight);
    gateThresholdSlider.setBounds(leftColumnX, preFXActualStartY + preFXSpacing * 2 + preFXLabelHeight + 5, preFXKnobSize, preFXKnobSize);
    gateEnabledButton.setBounds(leftColumnX, preFXActualStartY + preFXSpacing * 2 + preFXLabelHeight + preFXKnobSize + 10, preFXKnobSize, 20);
    
    // Gate indicator LED - positioned above the gate threshold knob on the right top side with padding
    auto gateKnobBounds = juce::Rectangle<int>(leftColumnX, preFXActualStartY + preFXSpacing * 2 + preFXLabelHeight + 5, preFXKnobSize, preFXKnobSize);
    gateIndicatorLED.setBounds(gateKnobBounds.getRight() - 8, gateKnobBounds.getY() - 10, 8, 8);
    
    // Post-FX section positioned to align with VU meters group - moved slightly right
    auto postFXKnobSize = 50;
    auto postFXLabelHeight = 16;
    auto vuGroupCenterX = meterX + (meterWidth + meterSpacing) / 2;
    auto postFXStartX = vuGroupCenterX - 50; // Moved slightly more to the right
    auto postFXStartY = area.getY() - 40; // Even higher up
    auto postFXSpacing = 65;
    
    // POST-FX title and power button - moved 20px down
    postFXTitleLabel.setBounds(postFXStartX - 10, postFXStartY, 80, 20);
    postFXPowerButton.setBounds(postFXStartX + 70, postFXStartY, 50, 20); // ON/OFF button
    
    // Cabinet model selector
    cabinetModelLabel.setBounds(postFXStartX, postFXStartY + 35, 100, postFXLabelHeight);
    cabinetModelSelector.setBounds(postFXStartX, postFXStartY + 35 + postFXLabelHeight + 3, 100, 25);
    
    // Cabinet presence control
    cabinetPresenceLabel.setBounds(postFXStartX, postFXStartY + 90, 80, postFXLabelHeight);
    cabinetPresenceSlider.setBounds(postFXStartX, postFXStartY + 90 + postFXLabelHeight + 3, postFXKnobSize, postFXKnobSize);
    
    // Cabinet mix control
    cabinetMixLabel.setBounds(postFXStartX + 60, postFXStartY + 90, 60, postFXLabelHeight);
    cabinetMixSlider.setBounds(postFXStartX + 60, postFXStartY + 90 + postFXLabelHeight + 3, postFXKnobSize, postFXKnobSize);
    
    // Bypass button positioned below meters - moved down
    bypassButton.setBounds(meterX - 10, meterY + meterHeight + 25, meterWidth + meterSpacing + 20, 25);
    
    // Limiter button positioned below bypass button
    limiterEnabledButton.setBounds(meterX - 10, meterY + meterHeight + 55, meterWidth + meterSpacing + 20, 25);
    
    // Auto-gain button positioned below limiter button
    autoGainButton.setBounds(meterX - 10, meterY + meterHeight + 85, meterWidth + meterSpacing + 20, 25);
    
    
    // Compact view button positioned below limiter button (disabled)
    compactViewButton.setBounds(meterX - 10, meterY + meterHeight + 95, meterWidth + meterSpacing + 20, 30);
    
    compactViewButton.setVisible(false); // Hide compact view button
    
    // updateLayoutForCompactView();
}

// Compact view functionality disabled - function removed to prevent crashes
// void SpiceAudioProcessorEditor::updateLayoutForCompactView()
// {
//     // Implementation removed
// }

void SpiceAudioProcessorEditor::timerCallback()
{
    // ff_meters update themselves automatically
    
    // Update waveform
    if (inputBuffer.getNumChannels() == 0 || inputBuffer.getNumSamples() != 512)
    {
        inputBuffer.setSize(2, 512);
        outputBuffer.setSize(2, 512);
    }
    
    audioProcessor.copyInputBuffer(inputBuffer);
    audioProcessor.copyOutputBuffer(outputBuffer);
    
    waveformDisplay.pushInputBuffer(inputBuffer);
    waveformDisplay.pushOutputBuffer(outputBuffer);
    
    // Update analog lamp
    saturationLamp.setSaturationLevel(waveformDisplay.getSaturationLevel());
    saturationLamp.setClipping(waveformDisplay.isClipping());
    
    // Update preset selector if needed (e.g., after state restoration)
    auto currentPreset = audioProcessor.getPresetManager().getCurrentPreset();
    if (presetSelector.getText() != currentPreset)
    {
        presetSelector.setText(currentPreset, juce::dontSendNotification);
    }
    
    // Update gate LED
    gateIndicatorLED.repaint();
    
   
}

void SpiceAudioProcessorEditor::drawSectionSeparator(juce::Graphics& g, juce::Rectangle<float> area, float y, const juce::String& label)
{
    auto lineY = area.getY() + y;
    auto lineStart = area.getX() + 30;
    auto lineEnd = area.getRight() - 30;
    
    // Glowing line effect
    g.setColour(juce::Colour(0x40ff4757));
    g.drawLine(lineStart, lineY - 1, lineEnd, lineY - 1, 3.0f);
    
    g.setColour(juce::Colour(0xffff4757));
    g.drawLine(lineStart, lineY, lineEnd, lineY, 1.0f);
    
    // Section label with glow
    auto labelArea = juce::Rectangle<float>(lineStart, lineY - 15, lineEnd - lineStart, 30);
    g.setColour(juce::Colour(0x40ff9f43));
    g.setFont(juce::FontOptions(9.0f).withStyle("Bold"));
    g.drawText(label, labelArea.expanded(1), juce::Justification::centred, true);
    
    g.setColour(juce::Colour(0xffff9f43));
    g.drawText(label, labelArea, juce::Justification::centred, true);
}

void SpiceAudioProcessorEditor::drawAmbientLight(juce::Graphics& g, juce::Point<float> position, juce::Colour colour)
{
    juce::ColourGradient ambientGlow(
        colour, position.x, position.y,
        colour.withAlpha(0.0f), position.x, position.y, true);
    ambientGlow.addColour(0.0, colour);
    ambientGlow.addColour(0.4, colour.withAlpha(0.3f));
    ambientGlow.addColour(1.0, colour.withAlpha(0.0f));
    
    g.setGradientFill(ambientGlow);
    g.fillEllipse(position.x - 80, position.y - 80, 160, 160);
}

void SpiceAudioProcessorEditor::refreshPresetList()
{
    presetSelector.clear();
    
    auto& presetManager = audioProcessor.getPresetManager();
    auto presets = presetManager.getAllPresets();
    
    for (int i = 0; i < presets.size(); ++i)
    {
        presetSelector.addItem(presets[i], i + 1);
    }
    
    presetSelector.setText(presetManager.getCurrentPreset(), juce::dontSendNotification);
}

void SpiceAudioProcessorEditor::comboBoxChanged(juce::ComboBox* comboBox)
{
    if (comboBox == &presetSelector)
    {
        auto presetName = presetSelector.getText();
        if (presetName.isNotEmpty())
        {
            audioProcessor.getPresetManager().loadPresetSmooth(presetName);
        }
    }
}

void SpiceAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &previousModelButton)
    {
        int currentIndex = modelSelector.getSelectedItemIndex();
        int newIndex = (currentIndex - 1 + modelSelector.getNumItems()) % modelSelector.getNumItems();
        modelSelector.setSelectedItemIndex(newIndex);
    }
    else if (button == &nextModelButton)
    {
        int currentIndex = modelSelector.getSelectedItemIndex();
        int newIndex = (currentIndex + 1) % modelSelector.getNumItems();
        modelSelector.setSelectedItemIndex(newIndex);
    }
    else if (button == &previousPresetButton)
    {
        audioProcessor.getPresetManager().loadPreviousPreset();
        refreshPresetList();
    }
    else if (button == &nextPresetButton)
    {
        audioProcessor.getPresetManager().loadNextPreset();
        refreshPresetList();
    }
    else if (button == &savePresetButton)
    {
        savePresetDialog();
    }
    
    // Trial notification buttons are now handled by the TrialNotificationComponent itself
    // Compact view functionality disabled
    // else if (button == &compactViewButton)
    // {
    //     isCompactView = compactViewButton.getToggleState();
    //     updateLayoutForCompactView();
    // }
}

void SpiceAudioProcessorEditor::savePresetDialog()
{
    auto currentPreset = audioProcessor.getPresetManager().getCurrentPreset();
    auto* dialog = new PresetSaveDialog(currentPreset);
    
    dialog->onComplete = [this, dialog](bool shouldSave)
    {
        if (shouldSave)
        {
            auto presetName = dialog->getPresetName();
            if (presetName.isNotEmpty())
            {
                audioProcessor.getPresetManager().savePreset(presetName);
                refreshPresetList();
            }
        }
        
        if (auto* parent = dialog->getParentComponent())
            parent->removeChildComponent(dialog);
        delete dialog;
    };
    
    // Add as child component and center it
    addAndMakeVisible(dialog);
    dialog->setCentrePosition(getLocalBounds().getCentre());
}

void SpiceAudioProcessorEditor::drawVintageMeterBezel(juce::Graphics& g, juce::Rectangle<int> bounds)
{
    // Outer bezel
    auto bezelBounds = bounds.toFloat().expanded(4);
    
    // Vintage brass/bronze gradient
    juce::ColourGradient bezelGradient(
        juce::Colour(0xff8b7355), bezelBounds.getTopLeft(),
        juce::Colour(0xff3a2a1a), bezelBounds.getBottomRight(), true);
    bezelGradient.addColour(0.5f, juce::Colour(0xff5a4a3a));
    g.setGradientFill(bezelGradient);
    g.fillRoundedRectangle(bezelBounds, 4.0f);
    
    // Inner bevel for depth
    g.setColour(juce::Colour(0xff2a1a0a));
    g.drawRoundedRectangle(bezelBounds.reduced(1), 3.0f, 2.0f);
    
    // Glass effect
    auto glassBounds = bounds.toFloat();
    juce::ColourGradient glassGradient(
        juce::Colour(0x20ffffff), glassBounds.getTopLeft(),
        juce::Colour(0x05000000), glassBounds.getBottomRight(), false);
    g.setGradientFill(glassGradient);
    g.fillRoundedRectangle(glassBounds, 2.0f);
    
    // Screws in corners
    auto screwRadius = 3.0f;
    auto screwInset = 8.0f;
    
    // Draw 4 corner screws
    auto drawScrew = [&](juce::Point<float> pos)
    {
        // Screw head
        juce::ColourGradient screwGrad(
            juce::Colour(0xff4a3a2a), pos.x - screwRadius, pos.y - screwRadius,
            juce::Colour(0xff1a0a00), pos.x + screwRadius, pos.y + screwRadius, true);
        g.setGradientFill(screwGrad);
        g.fillEllipse(pos.x - screwRadius, pos.y - screwRadius, screwRadius * 2, screwRadius * 2);
        
        // Screw slot
        g.setColour(juce::Colour(0xff000000));
        g.drawLine(pos.x - screwRadius * 0.6f, pos.y, pos.x + screwRadius * 0.6f, pos.y, 1.0f);
    };
    
    drawScrew({bezelBounds.getX() + screwInset, bezelBounds.getY() + screwInset});
    drawScrew({bezelBounds.getRight() - screwInset, bezelBounds.getY() + screwInset});
    drawScrew({bezelBounds.getX() + screwInset, bezelBounds.getBottom() - screwInset});
    drawScrew({bezelBounds.getRight() - screwInset, bezelBounds.getBottom() - screwInset});
}

