#pragma once

#include <JuceHeader.h>

class SaturationProcessor
{
public:
    enum class Model
    {
        Tube = 0,
        Transistor,
        Transformer,
        Tape,
        Diode,
        Vintage,
        Warm,
        Bright,
        FuzzBox,
        Overdrive,
        Tube12AX7
    };
    
    SaturationProcessor();
    ~SaturationProcessor() = default;
    
    void prepare(const juce::dsp::ProcessSpec& spec);
    void reset();
    void process(const juce::dsp::ProcessContextReplacing<float>& context);
    
    void setDrive(float newDrive);
    void setModel(Model newModel);
    void setBias(float newBias);
    
private:
    float processSample(float input, int channel);
    
    float tubeSaturation(float input);
    float transistorSaturation(float input);
    float transformerSaturation(float input);
    float tapeSaturation(float input);
    float diodeSaturation(float input);
    float vintageSaturation(float input);
    float warmSaturation(float input);
    float brightSaturation(float input);
    float fuzzBoxSaturation(float input);
    float overdriveSaturation(float input);
    float tube12AX7Saturation(float input);
    
    float drive = 50.0f;
    float bias = 0.0f;
    Model model = Model::Tube;
    
    std::vector<float> prevSample;
    std::vector<float> hysteresis;
    
    float sampleRate = 44100.0f;
    
    static constexpr float PI = juce::MathConstants<float>::pi;
};