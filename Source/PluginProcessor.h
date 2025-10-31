#pragma once

#include <JuceHeader.h>
#include "ff_meters.h"
#include "DSP/SaturationProcessor.h"
#include "DSP/Oversampling.h"
#include "DSP/FilterChain.h"
#include "DSP/CabinetSimulator.h"
#include "DSP/MidSideProcessor.h"
#include "PresetManager.h"
    

class SpiceAudioProcessor : public juce::AudioProcessor
{
public:
    SpiceAudioProcessor();
    ~SpiceAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    PresetManager& getPresetManager() { return presetManager; }
    
    bool isTrialNotificationDismissed() const { return trialNotificationDismissed; }
    void setTrialNotificationDismissed(bool dismissed) { trialNotificationDismissed = dismissed; }
    
    // Metering with ff_meters
    foleys::LevelMeterSource& getInputMeterSource() { return inputMeterSource; }
    foleys::LevelMeterSource& getOutputMeterSource() { return outputMeterSource; }
    
    // Visualization buffers
    void copyInputBuffer(juce::AudioBuffer<float>& dest) const
    {
        const juce::ScopedLock sl(visualizationLock);
        dest = inputVisualizationBuffer;
    }
    
    void copyOutputBuffer(juce::AudioBuffer<float>& dest) const
    {
        const juce::ScopedLock sl(visualizationLock);
        dest = outputVisualizationBuffer;
    }
    
    // Gate level monitoring for LED
    float getGateInputLevel() const { return gateInputLevel.load(); }
    
    // Preset loading mode for smoother transitions
    void setPresetLoadingMode(bool loading) { isLoadingPreset = loading; }

private:
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    void updatePreFXFilters(double sampleRate);
    float applyNoiseGate(float sample, int channel, float threshold, bool enabled);
    void updateAutoGainCompensation(const juce::AudioBuffer<float>& inputBuffer, 
                                   const juce::AudioBuffer<float>& outputBuffer);
    float calculateRMS(const std::vector<float>& buffer);
    
    SaturationProcessor saturationProcessor;
    Oversampling oversampling;
    FilterChain filterChain;
    CabinetSimulator cabinetSimulator;
    MidSideProcessor midSideProcessor;
    
    juce::dsp::Gain<float> inputGain;
    juce::dsp::DryWetMixer<float> dryWetMixer;
    juce::dsp::DryWetMixer<float> cabinetDryWetMixer;
    juce::dsp::Gain<float> outputGain;
    juce::dsp::Limiter<float> limiter;
    
    // Pre-FX filters
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> lowCutFilter;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> highCutFilter;
    
    // Noise gate
    std::vector<float> gateEnvelope;
    std::vector<float> smoothedGate;
    std::atomic<float> gateInputLevel {0.0f};
    
    // DC blocking filters
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> dcBlocker;
    
    // DC blocking for meter displays
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> inputMeterDCBlocker;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> outputMeterDCBlocker;
    
    std::atomic<float>* inputGainParam = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* mixParam = nullptr;
    std::atomic<float>* outputParam = nullptr;
    std::atomic<float>* modelParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* biasParam = nullptr;
    std::atomic<float>* qualityParam = nullptr;
    // std::atomic<float>* compactViewParam = nullptr;
    std::atomic<float>* lowCutParam = nullptr;
    std::atomic<float>* highCutParam = nullptr;
    std::atomic<float>* gateThresholdParam = nullptr;
    std::atomic<float>* gateEnabledParam = nullptr;
    std::atomic<float>* cabinetModelParam = nullptr;
    std::atomic<float>* cabinetPresenceParam = nullptr;
    std::atomic<float>* cabinetMixParam = nullptr;
    std::atomic<float>* cabinetEnabledParam = nullptr;
    std::atomic<float>* limiterEnabledParam = nullptr;
    std::atomic<float>* midSideEnabledParam = nullptr;
    std::atomic<float>* midGainParam = nullptr;
    std::atomic<float>* sideGainParam = nullptr;
    std::atomic<float>* stereoWidthParam = nullptr;
    std::atomic<float>* autoGainParam = nullptr;
    
    // Parameter smoothing
    juce::SmoothedValue<float> inputGainSmoothed;
    juce::SmoothedValue<float> driveSmoothed;
    juce::SmoothedValue<float> mixSmoothed;
    juce::SmoothedValue<float> outputSmoothed;
    juce::SmoothedValue<float> toneSmoothed;
    juce::SmoothedValue<float> biasSmoothed;
    juce::SmoothedValue<float> bypassSmoothed;
    
    // Auto-gain compensation
    juce::SmoothedValue<float> autoGainCompensation;
    float inputRmsLevel = 0.0f;
    float outputRmsLevel = 0.0f;
    static constexpr float rmsWindowMs = 300.0f; // 300ms RMS window
    std::vector<float> inputRmsBuffer;
    std::vector<float> outputRmsBuffer;
    int rmsBufferSize = 0;
    int rmsWritePos = 0;
    
    // Metering with ff_meters
    foleys::LevelMeterSource inputMeterSource;
    foleys::LevelMeterSource outputMeterSource;
    
    // Visualization
    mutable juce::CriticalSection visualizationLock;
    juce::AudioBuffer<float> inputVisualizationBuffer;
    juce::AudioBuffer<float> outputVisualizationBuffer;
    
    // Stored spec for dynamic quality updates
    juce::dsp::ProcessSpec storedSpec;
    
    // Preset manager
    PresetManager presetManager;

    
    // Trial notification state (persists across editor recreation)
    bool trialNotificationDismissed = false;
    
    // Preset loading state
    bool isLoadingPreset = false;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpiceAudioProcessor)
};
