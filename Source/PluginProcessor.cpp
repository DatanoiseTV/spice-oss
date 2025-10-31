#include "PluginProcessor.h"
#include "PluginEditor.h"

SpiceAudioProcessor::SpiceAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    apvts(*this, nullptr, "Parameters", createParameterLayout()),
    oversampling(2, 2, Oversampling::filterHalfBandPolyphaseIIR),
    presetManager(apvts)
{
    presetManager.setProcessor(this);
    
    inputGainParam = apvts.getRawParameterValue("inputGain");
    driveParam = apvts.getRawParameterValue("drive");
    mixParam = apvts.getRawParameterValue("mix");
    outputParam = apvts.getRawParameterValue("output");
    modelParam = apvts.getRawParameterValue("model");
    toneParam = apvts.getRawParameterValue("tone");
    biasParam = apvts.getRawParameterValue("bias");
    qualityParam = apvts.getRawParameterValue("quality");
    // compactViewParam = apvts.getRawParameterValue("compactView");
    lowCutParam = apvts.getRawParameterValue("lowCut");
    highCutParam = apvts.getRawParameterValue("highCut");
    gateThresholdParam = apvts.getRawParameterValue("gateThreshold");
    gateEnabledParam = apvts.getRawParameterValue("gateEnabled");
    cabinetModelParam = apvts.getRawParameterValue("cabinetModel");
    cabinetPresenceParam = apvts.getRawParameterValue("cabinetPresence");
    cabinetMixParam = apvts.getRawParameterValue("cabinetMix");
    cabinetEnabledParam = apvts.getRawParameterValue("cabinetEnabled");
    limiterEnabledParam = apvts.getRawParameterValue("limiterEnabled");
    midSideEnabledParam = apvts.getRawParameterValue("midSideEnabled");
    midGainParam = apvts.getRawParameterValue("midGain");
    sideGainParam = apvts.getRawParameterValue("sideGain");
    stereoWidthParam = apvts.getRawParameterValue("stereoWidth");
    autoGainParam = apvts.getRawParameterValue("autoGain");
}

SpiceAudioProcessor::~SpiceAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SpiceAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Input gain parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("inputGain", 4), "Input Gain", 
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0.0f));
    
    // Drive parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("drive", 4), "Drive", 
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 30.0f));
    
    // Mix parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("mix", 4), "Mix", 
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));
    
    // Output parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("output", 4), "Output", 
        juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    
    // Model parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("model", 4), "Model", 
        juce::StringArray{"Tube Saturation", "Transistor Drive", "Transformer Saturation", "Tape Compression", "Diode Clipping", "Vintage Console", "Warm Plexi", "Bright Crystal", "Fuzz Box", "Overdrive", "12AX7 Tube"}, 0));
    
    // Tone parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("tone", 4), "Tone", 
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 50.0f));
    
    // Bias parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("bias", 4), "Bias", 
        juce::NormalisableRange<float>(-50.0f, 50.0f, 0.1f), 0.0f));
    
    // Quality parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("quality", 4), "Quality", 
        juce::StringArray{"Eco", "Pro", "Ultra"}, 1));
    
    // Bypass parameter with version hint in constructor
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("bypass", 4), "Bypass", false));
    
    // Compact view parameter removed in version 2
    
    // Pre-FX filter parameters (new in version 2)
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("lowCut", 4), "Low Cut", 
        juce::NormalisableRange<float>(20.0f, 1000.0f, 1.0f, 0.3f), 20.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("highCut", 4), "High Cut", 
        juce::NormalisableRange<float>(1000.0f, 20000.0f, 1.0f, 0.3f), 20000.0f));
    
    // Noise gate parameters (new in version 2)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("gateEnabled", 4), "Gate Enable", false));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("gateThreshold", 4), "Gate Threshold", 
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f), -40.0f));
    
    // Cabinet simulation parameters (new in version 2)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("cabinetEnabled", 4), "Cabinet Enable", false));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        juce::ParameterID("cabinetModel", 4), "Cabinet Model", 
        juce::StringArray{"1x12 Vintage", "2x10 Tweed", "1x15 Bass", "2x12 Modern", "4x10 Clean", "1x12 British", "4x12 Vintage", "4x12 Modern", "1x12 Jazz", "2x12 Vintage"}, 0));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("cabinetPresence", 4), "Mic Distance", 
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 30.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("cabinetMix", 4), "Cabinet Mix", 
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 100.0f));
    
    // Limiter parameter (new in current version)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("limiterEnabled", 4), "Limiter Enable", false));
    
    // Mid-side processing parameters (new in version 5)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("midSideEnabled", 5), "Mid-Side Enable", false));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("midGain", 5), "Mid Gain", 
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("sideGain", 5), "Side Gain", 
        juce::NormalisableRange<float>(-24.0f, 24.0f, 0.1f), 0.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("stereoWidth", 5), "Stereo Width", 
        juce::NormalisableRange<float>(0.0f, 300.0f, 1.0f), 100.0f));
    
    // Auto-gain compensation parameter (new in version 6)
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("autoGain", 6), "Auto Gain", false));
    
    return { params.begin(), params.end() };
}

const juce::String SpiceAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SpiceAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SpiceAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SpiceAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SpiceAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SpiceAudioProcessor::getNumPrograms()
{
    return 1;
}

int SpiceAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SpiceAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SpiceAudioProcessor::getProgramName (int index)
{
    return {};
}

void SpiceAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void SpiceAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    storedSpec = spec;
    
    oversampling.prepare(spec);
    
    auto oversampledSpec = spec;
    oversampledSpec.sampleRate *= oversampling.getOversamplingFactor();
    
    saturationProcessor.prepare(oversampledSpec);
    filterChain.prepare(oversampledSpec);
    
    // Prepare cabinet simulator at normal sample rate (post-fx)
    cabinetSimulator.prepare(spec);
    
    // Prepare mid-side processor
    midSideProcessor.prepare(spec);
    
    inputGain.prepare(spec);
    dryWetMixer.prepare(spec);
    cabinetDryWetMixer.prepare(spec);
    outputGain.prepare(spec);
    
    // Prepare limiter
    limiter.prepare(spec);
    limiter.setThreshold(0.0f);  // 0 dB threshold
    limiter.setRelease(10.0f);   // 10ms release
    
    // Prepare pre-FX filters and noise gate
    lowCutFilter.prepare(spec);
    highCutFilter.prepare(spec);
    gateEnvelope.resize(spec.numChannels, 0.0f);
    smoothedGate.resize(spec.numChannels, 1.0f);
    
    dcBlocker.prepare(spec);
    inputMeterDCBlocker.prepare(spec);
    outputMeterDCBlocker.prepare(spec);
    
    dryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    cabinetDryWetMixer.setMixingRule(juce::dsp::DryWetMixingRule::linear);
    
    // Initialize DC blocker with high-pass at 5Hz
    *dcBlocker.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 5.0f);
    
    // Initialize meter DC blockers with high-pass at 10Hz for more aggressive DC removal
    *inputMeterDCBlocker.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 10.0f);
    *outputMeterDCBlocker.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 10.0f);
    
    // Initialize pre-FX filters
    updatePreFXFilters(sampleRate);
    
    // Initialize meters
    inputMeterSource.resize(spec.numChannels, spec.sampleRate * 0.1 / spec.maximumBlockSize);
    outputMeterSource.resize(spec.numChannels, spec.sampleRate * 0.1 / spec.maximumBlockSize);
    
    // Initialize parameter smoothing to avoid clicks - instant response
    inputGainSmoothed.reset(sampleRate, 0.002);  // 2ms for input gain
    driveSmoothed.reset(sampleRate, 0.001);   // 1ms for instant response
    mixSmoothed.reset(sampleRate, 0.001);     // 1ms
    outputSmoothed.reset(sampleRate, 0.002);  // 2ms for output gain
    toneSmoothed.reset(sampleRate, 0.001);    // 1ms
    biasSmoothed.reset(sampleRate, 0.001);    // 1ms
    bypassSmoothed.reset(sampleRate, 0.002);  // 2ms for bypass - fast response while still avoiding clicks
    autoGainCompensation.reset(sampleRate, 0.5);  // 500ms for smooth auto-gain adjustments
    
    // Initialize bypass smoothed value with current parameter state
    auto* bypassParam = apvts.getRawParameterValue("bypass");
    if (bypassParam)
    {
        float initialBypassValue = bypassParam->load() > 0.5f ? 1.0f : 0.0f;
        bypassSmoothed.setCurrentAndTargetValue(initialBypassValue);
    }
    
    // Update oversampling based on quality setting
    int qualityLevel = static_cast<int>(*qualityParam);
    int oversamplingFactor = (qualityLevel == 0) ? 1 : (qualityLevel == 1) ? 2 : 4;
    // Note: For dynamic quality changes, we'd need to re-prepare the oversampling
    // For now, oversampling factor is set at construction time
    
    // Initialize RMS buffers for auto-gain compensation
    rmsBufferSize = static_cast<int>(sampleRate * rmsWindowMs / 1000.0f);
    inputRmsBuffer.resize(rmsBufferSize, 0.0f);
    outputRmsBuffer.resize(rmsBufferSize, 0.0f);
    rmsWritePos = 0;
    autoGainCompensation.setCurrentAndTargetValue(1.0f);
}

void SpiceAudioProcessor::releaseResources()
{
    oversampling.reset();
    saturationProcessor.reset();
    filterChain.reset();
    cabinetSimulator.reset();
    midSideProcessor.reset();
    inputGain.reset();
    dryWetMixer.reset();
    cabinetDryWetMixer.reset();
    outputGain.reset();
    limiter.reset();
    lowCutFilter.reset();
    highCutFilter.reset();
    dcBlocker.reset();
    inputMeterDCBlocker.reset();
    outputMeterDCBlocker.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SpiceAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SpiceAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);
    
    // Create DC-blocked copies for meter measurement
    juce::AudioBuffer<float> inputMeterBuffer = buffer;
    juce::dsp::AudioBlock<float> inputMeterBlock(inputMeterBuffer);
    juce::dsp::ProcessContextReplacing<float> inputMeterContext(inputMeterBlock);
    inputMeterDCBlocker.process(inputMeterContext);
    
    // Apply noise gate to meter signal only (not audio output)
    const float meterNoiseGate = 0.00001f; // -100dB threshold
    for (int channel = 0; channel < inputMeterBuffer.getNumChannels(); ++channel)
    {
        auto* channelData = inputMeterBuffer.getWritePointer(channel);
        for (int sample = 0; sample < inputMeterBuffer.getNumSamples(); ++sample)
        {
            if (std::abs(channelData[sample]) < meterNoiseGate)
                channelData[sample] = 0.0f;
        }
    }
    
    // Measure input levels with DC-blocked signal
    inputMeterSource.measureBlock(inputMeterBuffer);
    
    {
        const juce::ScopedLock sl(visualizationLock);
        inputVisualizationBuffer = buffer;
    }
    
    // Update bypass smoothing
    auto* bypassParam = apvts.getRawParameterValue("bypass");
    bypassSmoothed.setTargetValue(bypassParam->load() > 0.5f ? 1.0f : 0.0f);
    
    // Check if we're fully bypassed (no ramping needed)
    bool isFullyBypassed = bypassSmoothed.isSmoothing() == false && bypassSmoothed.getCurrentValue() > 0.5f;
    
    // If fully bypassed, skip all processing
    if (isFullyBypassed)
    {
        // Measure output for meters (bypassed signal)
        outputMeterSource.measureBlock(buffer);
        
        {
            const juce::ScopedLock sl(visualizationLock);
            outputVisualizationBuffer = buffer;
        }
        
        return; // Skip all processing - pure bypass
    }
    
    // Create a copy of the dry input for bypass crossfading (only if ramping)
    juce::AudioBuffer<float> dryBuffer;
    if (bypassSmoothed.isSmoothing())
    {
        dryBuffer.makeCopyOf(buffer);
    }
    
    // Apply pre-FX processing (filters and noise gate)
    updatePreFXFilters(getSampleRate());
    
    // Apply low cut filter
    if (lowCutParam->load() > 20.0f)
        lowCutFilter.process(context);
    
    // Apply high cut filter  
    if (highCutParam->load() < 20000.0f)
        highCutFilter.process(context);
    
    // Apply noise gate
    if (gateEnabledParam->load() > 0.5f)
    {
        auto gateThreshold = gateThresholdParam->load();
        auto numChannels = block.getNumChannels();
        auto numSamples = block.getNumSamples();
        
        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* channelData = block.getChannelPointer(channel);
            for (size_t sample = 0; sample < numSamples; ++sample)
            {
                channelData[sample] = applyNoiseGate(channelData[sample], static_cast<int>(channel), gateThreshold, true);
            }
        }
    }
    
    // Store dry signal
    dryWetMixer.pushDrySamples(block);
    
    int qualityLevel = static_cast<int>(*qualityParam);
    
    int oversamplingFactor = (qualityLevel == 0) ? 1 : (qualityLevel == 1) ? 2 : 4;
    oversampling.updateQuality(oversamplingFactor, storedSpec);
    
    // Update smoothed parameters
    inputGainSmoothed.setTargetValue(inputGainParam->load());
    driveSmoothed.setTargetValue(driveParam->load());
    mixSmoothed.setTargetValue(mixParam->load() / 100.0f);
    outputSmoothed.setTargetValue(outputParam->load());
    toneSmoothed.setTargetValue(toneParam->load() / 100.0f);
    biasSmoothed.setTargetValue(biasParam->load() / 50.0f); // -1 to 1 range for stronger effect
    
    // If we're loading a preset, use longer smoothing times to avoid clicks
    if (isLoadingPreset)
    {
        const double presetSmoothTime = 0.05; // 50ms for preset changes
        inputGainSmoothed.reset(getSampleRate(), presetSmoothTime);
        driveSmoothed.reset(getSampleRate(), presetSmoothTime);
        mixSmoothed.reset(getSampleRate(), presetSmoothTime);
        outputSmoothed.reset(getSampleRate(), presetSmoothTime);
        toneSmoothed.reset(getSampleRate(), presetSmoothTime);
        biasSmoothed.reset(getSampleRate(), presetSmoothTime);
        bypassSmoothed.reset(getSampleRate(), presetSmoothTime);
        autoGainCompensation.reset(getSampleRate(), presetSmoothTime);
        isLoadingPreset = false; // Reset flag after applying smoothing
    }
    
    // Skip to current values immediately for instant response (except bypass)
    inputGainSmoothed.skip(buffer.getNumSamples());
    driveSmoothed.skip(buffer.getNumSamples());
    mixSmoothed.skip(buffer.getNumSamples());
    outputSmoothed.skip(buffer.getNumSamples());
    toneSmoothed.skip(buffer.getNumSamples());
    biasSmoothed.skip(buffer.getNumSamples());
    // Don't skip bypass smoothing - we want it to ramp
    
    // Apply input gain first, before any processing
    inputGain.setGainDecibels(inputGainSmoothed.getCurrentValue());
    inputGain.process(context);
    
    // Capture post-input-gain signal for auto-gain compensation
    juce::AudioBuffer<float> preProcessingBuffer;
    if (autoGainParam->load() > 0.5f)
    {
        preProcessingBuffer.makeCopyOf(buffer);
    }
    
    // Apply mid-side processing if enabled
    if (midSideEnabledParam->load() > 0.5f)
    {
        // Convert to mid-side
        midSideProcessor.processStereoToMidSide(buffer);
        
        // Apply mid-side gains directly to the buffer
        auto midGain = juce::Decibels::decibelsToGain(midGainParam->load());
        auto sideGain = juce::Decibels::decibelsToGain(sideGainParam->load());
        auto width = stereoWidthParam->load() / 100.0f; // Convert from 0-300% to 0-3
        
        auto* leftData = buffer.getWritePointer(0);   // Now contains mid
        auto* rightData = buffer.getWritePointer(1);  // Now contains side
        auto numSamples = buffer.getNumSamples();
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            leftData[sample] *= midGain;   // Apply mid gain
            rightData[sample] *= sideGain * width; // Apply side gain and width
        }
    }
    
    // Update processors with current smoothed values
    saturationProcessor.setDrive(driveSmoothed.getCurrentValue());
    saturationProcessor.setModel(static_cast<SaturationProcessor::Model>(modelParam->load()));
    saturationProcessor.setBias(biasSmoothed.getCurrentValue());
    filterChain.setTone(toneSmoothed.getCurrentValue());
    
    // Process with oversampling
    auto oversampledBlock = oversampling.processSamplesUp(block);
    juce::dsp::ProcessContextReplacing<float> oversampledContext(oversampledBlock);
    saturationProcessor.process(oversampledContext);
    filterChain.process(oversampledContext);
    oversampling.processSamplesDown(block);
    
    // Apply dry/wet mix
    dryWetMixer.setWetMixProportion(mixSmoothed.getCurrentValue());
    dryWetMixer.mixWetSamples(block);
    
    // Apply cabinet simulation (post-fx) with dry/wet mixing - license required
    if (cabinetEnabledParam->load() > 0.5f)
    {
        auto cabinetModel = static_cast<CabinetSimulator::CabinetModel>(static_cast<int>(cabinetModelParam->load()));
        auto cabinetPresence = cabinetPresenceParam->load() / 100.0f; // Convert to 0-1 range
        auto cabinetMix = cabinetMixParam->load() / 100.0f; // Convert to 0-1 range
        
        // Store dry signal for cabinet mixing
        cabinetDryWetMixer.pushDrySamples(block);
        
        cabinetSimulator.setCabinetModel(cabinetModel);
        cabinetSimulator.setPresence(cabinetPresence);
        cabinetSimulator.setResonance(0.5f); // Fixed resonance for simplicity
        cabinetSimulator.process(context);
        
        // Apply cabinet dry/wet mix
        cabinetDryWetMixer.setWetMixProportion(cabinetMix);
        cabinetDryWetMixer.mixWetSamples(block);
    }
    
    // Apply output gain with auto-gain compensation if enabled
    float outputGainDb = outputSmoothed.getCurrentValue();
    
    if (autoGainParam->load() > 0.5f && preProcessingBuffer.getNumChannels() > 0)
    {
        // Update auto-gain compensation based on pre/post processing levels
        updateAutoGainCompensation(preProcessingBuffer, buffer);
        
        // Apply compensation (convert linear gain to dB and add to output gain)
        float compensationDb = juce::Decibels::gainToDecibels(autoGainCompensation.getNextValue());
        outputGainDb += compensationDb;
    }
    
    outputGain.setGainDecibels(outputGainDb);
    outputGain.process(context);
    
    // Apply limiter if enabled
    if (limiterEnabledParam->load() > 0.5f)
    {
        limiter.process(context);
    }
    
    // Convert back from mid-side to stereo if mid-side processing was enabled
    if (midSideEnabledParam->load() > 0.5f)
    {
        midSideProcessor.processMidSideToStereo(buffer);
    }
    
    // Apply DC blocking filter to remove any DC offset
    dcBlocker.process(context);
    
    // Apply smooth bypass crossfade only if we're ramping
    if (bypassSmoothed.isSmoothing())
    {
        auto numSamples = buffer.getNumSamples();
        auto numChannels = buffer.getNumChannels();
        
        for (int channel = 0; channel < numChannels; ++channel)
        {
            auto* wetData = buffer.getWritePointer(channel);
            auto* dryData = dryBuffer.getReadPointer(channel);
            
            for (int sample = 0; sample < numSamples; ++sample)
            {
                auto bypassAmount = bypassSmoothed.getNextValue();
                auto wetAmount = 1.0f - bypassAmount;
                
                // Crossfade between wet (processed) and dry (bypass) signals
                wetData[sample] = wetData[sample] * wetAmount + dryData[sample] * bypassAmount;
            }
        }
    }
    
    // Create DC-blocked copy for output meter measurement
    juce::AudioBuffer<float> outputMeterBuffer = buffer;
    juce::dsp::AudioBlock<float> outputMeterBlock(outputMeterBuffer);
    juce::dsp::ProcessContextReplacing<float> outputMeterContext(outputMeterBlock);
    outputMeterDCBlocker.process(outputMeterContext);
    
    // Apply noise gate to meter signal only (not audio output)
    for (int channel = 0; channel < outputMeterBuffer.getNumChannels(); ++channel)
    {
        auto* channelData = outputMeterBuffer.getWritePointer(channel);
        for (int sample = 0; sample < outputMeterBuffer.getNumSamples(); ++sample)
        {
            if (std::abs(channelData[sample]) < meterNoiseGate)
                channelData[sample] = 0.0f;
        }
    }
    
    // Measure output levels with DC-blocked signal
    outputMeterSource.measureBlock(outputMeterBuffer);
    
    {
        const juce::ScopedLock sl(visualizationLock);
        outputVisualizationBuffer = buffer;
    }
}

bool SpiceAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* SpiceAudioProcessor::createEditor()
{
    return new SpiceAudioProcessorEditor (*this);
}

void SpiceAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    
    // Add current preset to the saved state
    state.setProperty("currentPreset", presetManager.getCurrentPreset(), nullptr);
    
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void SpiceAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            auto newState = juce::ValueTree::fromXml(*xmlState);
            apvts.replaceState(newState);
            
            // Restore the current preset if it was saved
            if (newState.hasProperty("currentPreset"))
            {
                auto savedPresetName = newState.getProperty("currentPreset").toString();
                if (savedPresetName.isNotEmpty())
                {
                    presetManager.loadPreset(savedPresetName);
                }
            }
        }
}

void SpiceAudioProcessor::updatePreFXFilters(double sampleRate)
{
    // Update low cut filter
    auto lowCutFreq = lowCutParam->load();
    if (lowCutFreq > 20.0f)
    {
        *lowCutFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, lowCutFreq);
    }
    
    // Update high cut filter
    auto highCutFreq = highCutParam->load();
    if (highCutFreq < 20000.0f)
    {
        *highCutFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, highCutFreq);
    }
}

float SpiceAudioProcessor::applyNoiseGate(float sample, int channel, float threshold, bool enabled)
{
    if (!enabled || channel >= static_cast<int>(gateEnvelope.size()))
        return sample;
    
    // Convert threshold from dB to linear
    auto thresholdLinear = juce::Decibels::decibelsToGain(threshold);
    
    // Get absolute level of current sample
    auto level = std::abs(sample);
    
    // Update gate input level for LED (only from first channel to avoid rapid changes)
    if (channel == 0)
    {
        gateInputLevel.store(level);
    }
    
    // Simple gate with fast attack, medium release
    auto& envelope = gateEnvelope[channel];
    auto& gateSmooth = smoothedGate[channel];
    
    if (level > thresholdLinear)
    {
        // Fast attack (1ms)
        auto attackCoeff = std::exp(-1.0f / (getSampleRate() * 0.001f));
        envelope = level + attackCoeff * (envelope - level);
    }
    else
    {
        // Medium release (50ms)
        auto releaseCoeff = std::exp(-1.0f / (getSampleRate() * 0.05f));
        envelope = envelope * releaseCoeff;
    }
    
    // Apply gate
    auto gateReduction = envelope > thresholdLinear ? 1.0f : 0.0f;
    
    // Smooth the gate to avoid clicks
    auto smoothingCoeff = std::exp(-1.0f / (getSampleRate() * 0.001f));
    gateSmooth = gateReduction + smoothingCoeff * (gateSmooth - gateReduction);
    
    return sample * gateSmooth;
}

void SpiceAudioProcessor::updateAutoGainCompensation(const juce::AudioBuffer<float>& inputBuffer, 
                                                     const juce::AudioBuffer<float>& outputBuffer)
{
    // Calculate RMS for input and output
    float inputSum = 0.0f;
    float outputSum = 0.0f;
    int numSamples = inputBuffer.getNumSamples();
    int numChannels = inputBuffer.getNumChannels();
    
    // Calculate sum of squares for this block
    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* inputData = inputBuffer.getReadPointer(channel);
        const float* outputData = outputBuffer.getReadPointer(channel);
        
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float inputSample = inputData[sample];
            float outputSample = outputData[sample];
            
            inputSum += inputSample * inputSample;
            outputSum += outputSample * outputSample;
        }
    }
    
    // Average across channels
    inputSum /= numChannels;
    outputSum /= numChannels;
    
    // Add to RMS buffers
    for (int i = 0; i < numSamples; ++i)
    {
        if (rmsWritePos >= rmsBufferSize)
            rmsWritePos = 0;
            
        inputRmsBuffer[rmsWritePos] = inputSum / numSamples;
        outputRmsBuffer[rmsWritePos] = outputSum / numSamples;
        rmsWritePos++;
    }
    
    // Calculate RMS values
    inputRmsLevel = calculateRMS(inputRmsBuffer);
    outputRmsLevel = calculateRMS(outputRmsBuffer);
    
    // Calculate gain compensation
    if (outputRmsLevel > 0.0001f && inputRmsLevel > 0.0001f) // Avoid division by zero
    {
        float targetGain = inputRmsLevel / outputRmsLevel;
        
        // Limit compensation range to ±12dB
        targetGain = juce::jlimit(0.25f, 4.0f, targetGain);
        
        // Smooth the compensation
        autoGainCompensation.setTargetValue(targetGain);
    }
}

float SpiceAudioProcessor::calculateRMS(const std::vector<float>& buffer)
{
    float sum = 0.0f;
    
    for (float value : buffer)
    {
        sum += value;
    }
    
    float mean = sum / buffer.size();
    return std::sqrt(mean);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SpiceAudioProcessor();
}