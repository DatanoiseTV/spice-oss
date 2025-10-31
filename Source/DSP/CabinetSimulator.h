#pragma once

#include <JuceHeader.h>

/// Advanced cabinet simulation with combo cab modeling and mic distance effects
class CabinetSimulator
{
public:
    enum class CabinetModel
    {
        Combo_1x12_Vintage = 0,  // Classic Fender-style combo cab
        Combo_2x10_Tweed,        // Vintage tweed combo sound
        Combo_1x15_Bass,         // Bass combo cabinet
        Combo_2x12_Modern,       // Modern high-gain combo
        Combo_4x10_Clean,        // Clean Fender-style 4x10
        Combo_1x12_British,      // British-voiced combo cab
        Stack_4x12_Vintage,      // Classic Marshall 4x12 stack
        Stack_4x12_Modern,       // Modern Mesa/Orange 4x12 stack
        Combo_1x12_Jazz,         // Jazz combo with smooth response
        Combo_2x12_Vintage       // Vintage 2x12 combo warmth
    };
    
    CabinetSimulator();
    ~CabinetSimulator();
    
    /// Prepare the processor for playback
    void prepare(const juce::dsp::ProcessSpec& spec);
    
    /// Reset the processor state
    void reset();
    
    /// Process audio block
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    
    /// Set the cabinet model
    void setCabinetModel(CabinetModel model);
    
    /// Set cabinet presence (simulates mic distance and angle)
    void setPresence(float presence); // 0.0 = close mic, 1.0 = room mic
    
    /// Set cabinet resonance (cabinet size and port tuning)  
    void setResonance(float resonance); // 0.0 to 1.0
    
    /// Get the current cabinet model
    CabinetModel getCurrentModel() const { return currentModel; }
    
private:
    void updateFilters(double sampleRate);
    void setupCabinetResponse(CabinetModel model);
    void updateMicDistance();
    
    // Current settings
    CabinetModel currentModel = CabinetModel::Combo_1x12_Vintage;
    float currentPresence = 0.3f; // Default close mic
    float currentResonance = 0.5f;
    double currentSampleRate = 44100.0;
    
    // Multi-stage filtering for realistic cabinet response
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> speakerLowPass;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> cabinetResonance;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> speakerBreakup;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> micProximity;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> roomAmbience;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, 
                                   juce::dsp::IIR::Coefficients<float>> airAbsorption;
    
    // Cabinet-specific parameters for realistic modeling
    struct CabinetResponse
    {
        // Speaker characteristics
        float speakerSize;        // 10", 12", 15" etc
        float speakerCutoff;      // Natural rolloff frequency
        float breakupFreq;        // Speaker cone breakup frequency
        float breakupQ;           // Breakup resonance sharpness
        
        // Cabinet characteristics  
        float cabinetSize;        // Internal volume effect
        float portTuning;         // Bass reflex port frequency
        float resonanceQ;         // Cabinet resonance sharpness
        
        // Mic modeling
        float closeProximity;     // Close mic bass boost
        float roomReflection;     // Room reflection frequency
        float airLoss;            // High frequency air absorption
    };
    
    CabinetResponse cabinetResponses[10];
    
    // Nonlinear saturation for speaker modeling
    juce::dsp::WaveShaper<float> speakerSaturation;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CabinetSimulator)
};