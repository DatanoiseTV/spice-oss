#include "CabinetSimulator.h"

CabinetSimulator::CabinetSimulator()
{
    // Define realistic combo cabinet models with detailed speaker and cabinet characteristics
    
    // Combo 1x12 Vintage - Classic Fender-style combo (Celestion Vintage 30 style)
    cabinetResponses[0] = {
        12.0f,          // speakerSize
        5200.0f,        // speakerCutoff - Natural speaker rolloff
        3100.0f,        // breakupFreq - Cone breakup adds character
        0.8f,           // breakupQ
        1.8f,           // cabinetSize - Medium cabinet volume
        85.0f,          // portTuning - Bass reflex tuning
        1.2f,           // resonanceQ
        120.0f,         // closeProximity - Close mic bass boost frequency
        280.0f,         // roomReflection - Room reflection low frequency
        8500.0f         // airLoss - High frequency air absorption
    };
    
    // Combo 2x10 Tweed - Vintage tweed combo warmth (Jensen P10R style)
    cabinetResponses[1] = {
        10.0f,          // speakerSize
        4800.0f,        // speakerCutoff - Earlier rolloff for warmth
        2800.0f,        // breakupFreq - Lower breakup for vintage character
        0.6f,           // breakupQ
        2.2f,           // cabinetSize - Larger tweed cabinet
        75.0f,          // portTuning - Lower tuning for warmth
        0.9f,           // resonanceQ
        110.0f,         // closeProximity
        250.0f,         // roomReflection
        7800.0f         // airLoss
    };
    
    // Combo 1x15 Bass - Bass combo cabinet (Eminence style)
    cabinetResponses[2] = {
        15.0f,          // speakerSize
        3200.0f,        // speakerCutoff - Lower cutoff for bass response
        1800.0f,        // breakupFreq - Much lower breakup
        1.0f,           // breakupQ
        4.5f,           // cabinetSize - Large cabinet for bass extension
        45.0f,          // portTuning - Low bass reflex tuning
        1.5f,           // resonanceQ
        80.0f,          // closeProximity
        160.0f,         // roomReflection
        6000.0f         // airLoss
    };
    
    // Combo 2x12 Modern - Modern high-gain combo (Celestion V30 + G12T-75)
    cabinetResponses[3] = {
        12.0f,          // speakerSize
        6500.0f,        // speakerCutoff - Extended high frequency response
        3800.0f,        // breakupFreq - Higher breakup for clarity
        1.2f,           // breakupQ
        3.2f,           // cabinetSize - Large 2x12 cabinet
        95.0f,          // portTuning - Tight bass response
        1.8f,           // resonanceQ
        130.0f,         // closeProximity
        320.0f,         // roomReflection
        9200.0f         // airLoss
    };
    
    // Combo 4x10 Clean - Clean Fender-style 4x10 (Jensen C10Q style)
    cabinetResponses[4] = {
        10.0f,          // speakerSize
        5800.0f,        // speakerCutoff - Clean, clear high end
        3500.0f,        // breakupFreq - Clean breakup character
        0.4f,           // breakupQ - Gentle breakup
        4.8f,           // cabinetSize - Large 4x10 cabinet
        90.0f,          // portTuning - Balanced bass response
        0.8f,           // resonanceQ
        125.0f,         // closeProximity
        300.0f,         // roomReflection
        8800.0f         // airLoss
    };
    
    // Combo 1x12 British - British-voiced combo (Celestion Blue style)
    cabinetResponses[5] = {
        12.0f,          // speakerSize
        4600.0f,        // speakerCutoff - Classic British rolloff
        2400.0f,        // breakupFreq - Early musical breakup
        0.7f,           // breakupQ
        1.5f,           // cabinetSize - Compact British cab
        70.0f,          // portTuning - Tight, punchy bass
        1.4f,           // resonanceQ
        105.0f,         // closeProximity
        230.0f,         // roomReflection
        7200.0f         // airLoss
    };
    
    // Stack 4x12 Vintage - Classic Marshall stack (Celestion G12M style)
    cabinetResponses[6] = {
        12.0f,          // speakerSize
        5800.0f,        // speakerCutoff - Marshall stack presence
        2800.0f,        // breakupFreq - Classic rock breakup
        0.9f,           // breakupQ
        8.0f,           // cabinetSize - Large 4x12 cabinet
        85.0f,          // portTuning - Punchy bass response
        2.0f,           // resonanceQ - Strong cabinet resonance
        140.0f,         // closeProximity
        350.0f,         // roomReflection
        9000.0f         // airLoss
    };
    
    // Stack 4x12 Modern - Modern high-gain stack (Mesa/Orange style)
    cabinetResponses[7] = {
        12.0f,          // speakerSize
        6800.0f,        // speakerCutoff - Extended high frequency
        3600.0f,        // breakupFreq - Tight modern breakup
        1.3f,           // breakupQ
        8.5f,           // cabinetSize - Oversized modern cab
        95.0f,          // portTuning - Tight, focused bass
        2.2f,           // resonanceQ
        150.0f,         // closeProximity
        380.0f,         // roomReflection
        10500.0f        // airLoss
    };
    
    // Combo 1x12 Jazz - Jazz combo with smooth response (JBL style)
    cabinetResponses[8] = {
        12.0f,          // speakerSize
        7500.0f,        // speakerCutoff - Smooth extended highs
        4200.0f,        // breakupFreq - Clean, minimal breakup
        0.3f,           // breakupQ - Very gentle breakup
        2.0f,           // cabinetSize - Medium jazz combo
        80.0f,          // portTuning - Balanced bass response
        0.7f,           // resonanceQ - Controlled resonance
        115.0f,         // closeProximity
        270.0f,         // roomReflection
        8200.0f         // airLoss
    };
    
    // Combo 2x12 Vintage - Vintage 2x12 combo warmth (Fender Twin style)
    cabinetResponses[9] = {
        12.0f,          // speakerSize
        5400.0f,        // speakerCutoff - Vintage warmth
        2900.0f,        // breakupFreq - Musical vintage breakup
        0.6f,           // breakupQ
        3.8f,           // cabinetSize - Large vintage 2x12
        78.0f,          // portTuning - Warm bass tuning
        1.1f,           // resonanceQ
        125.0f,         // closeProximity
        290.0f,         // roomReflection
        7600.0f         // airLoss
    };
    
    // Setup speaker saturation curve for realistic cone behavior
    speakerSaturation.functionToUse = [](float x) {
        // Soft saturation that mimics speaker cone compression
        float absX = std::abs(x);
        if (absX < 0.3f)
            return x;
        else if (absX < 0.7f)
            return x * (1.0f - 0.1f * (absX - 0.3f));
        else
            return juce::jlimit(-0.85f, 0.85f, x * 0.9f + std::tanh(x * 0.3f) * 0.1f);
    };
}

CabinetSimulator::~CabinetSimulator()
{
}

void CabinetSimulator::prepare(const juce::dsp::ProcessSpec& spec)
{
    currentSampleRate = spec.sampleRate;
    
    // Prepare all filter stages
    speakerLowPass.prepare(spec);
    cabinetResonance.prepare(spec);
    speakerBreakup.prepare(spec);
    micProximity.prepare(spec);
    roomAmbience.prepare(spec);
    airAbsorption.prepare(spec);
    
    // Prepare speaker saturation
    speakerSaturation.prepare(spec);
    
    updateFilters(spec.sampleRate);
}

void CabinetSimulator::reset()
{
    speakerLowPass.reset();
    cabinetResonance.reset();
    speakerBreakup.reset();
    micProximity.reset();
    roomAmbience.reset();
    airAbsorption.reset();
    speakerSaturation.reset();
}

void CabinetSimulator::process(const juce::dsp::ProcessContextReplacing<float>& context)
{
    // Apply realistic cabinet simulation in proper order
    
    // 1. Cabinet resonance (bass response and cabinet size effects)
    cabinetResonance.process(context);
    
    // 2. Speaker cone breakup (adds character and compression)
    speakerBreakup.process(context);
    
    // 3. Speaker saturation (cone compression at higher levels)
    speakerSaturation.process(context);
    
    // 4. Speaker natural low-pass (cone and magnet system rolloff)
    speakerLowPass.process(context);
    
    // 5. Microphone proximity effect (bass boost when close)
    micProximity.process(context);
    
    // 6. Room ambience (depends on mic distance)
    roomAmbience.process(context);
    
    // 7. Air absorption (high frequency loss over distance)
    airAbsorption.process(context);
}

void CabinetSimulator::setCabinetModel(CabinetModel model)
{
    if (currentModel != model)
    {
        currentModel = model;
        setupCabinetResponse(model);
        updateFilters(currentSampleRate);
    }
}

void CabinetSimulator::setPresence(float presence)
{
    currentPresence = juce::jlimit(0.0f, 1.0f, presence);
    updateMicDistance();
    updateFilters(currentSampleRate);
}

void CabinetSimulator::setResonance(float resonance)
{
    currentResonance = juce::jlimit(0.0f, 1.0f, resonance);
    updateFilters(currentSampleRate);
}

void CabinetSimulator::updateFilters(double sampleRate)
{
    if (sampleRate <= 0.0)
        return;
    
    const auto& response = cabinetResponses[static_cast<int>(currentModel)];
    
    // 1. Cabinet resonance (bass reflex port and cabinet size) - MUCH more dramatic
    auto resonanceGain = 1.0f + currentResonance * 8.0f; // 1-9 dB boost for audible effect
    auto resonanceFreq = response.portTuning * (0.6f + currentResonance * 0.8f); // Wider frequency range
    *cabinetResonance.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, resonanceFreq, response.resonanceQ * (0.5f + currentResonance), 
        juce::Decibels::decibelsToGain(resonanceGain));
    
    // 2. Speaker cone breakup (adds musical distortion) - More pronounced
    auto breakupGain = 0.5f + currentResonance * 4.0f; // More dramatic breakup
    *speakerBreakup.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        sampleRate, response.breakupFreq, response.breakupQ * (0.3f + currentResonance * 0.7f),
        juce::Decibels::decibelsToGain(breakupGain));
    
    // 3. Speaker natural rolloff - More dramatic cutoff control
    auto cutoffFreq = response.speakerCutoff * (0.7f + currentResonance * 0.6f); // Variable cutoff
    *speakerLowPass.state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
        sampleRate, cutoffFreq, 0.8f + currentResonance * 0.4f); // Variable Q
    
    updateMicDistance();
}

void CabinetSimulator::updateMicDistance()
{
    if (currentSampleRate <= 0.0)
        return;
        
    const auto& response = cabinetResponses[static_cast<int>(currentModel)];
    
    // Mic proximity effect (close mic = more bass, room mic = less bass) - MUCH more dramatic
    float proximityGain = (1.0f - currentPresence) * 12.0f; // 0-12 dB bass boost when close - very audible
    *micProximity.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
        currentSampleRate, response.closeProximity, 0.7f,
        juce::Decibels::decibelsToGain(proximityGain));
    
    // Room reflection (more room = more low-mid resonance) - More pronounced
    float roomGain = currentPresence * 6.0f; // 0-6 dB boost for room character - doubled
    *roomAmbience.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
        currentSampleRate, response.roomReflection, 0.6f + currentPresence * 0.4f, // Variable Q
        juce::Decibels::decibelsToGain(roomGain));
    
    // Air absorption (more distance = more high frequency loss) - Much more dramatic
    float airLoss = -currentPresence * 15.0f; // 0 to -15 dB high cut - very audible effect
    *airAbsorption.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
        currentSampleRate, response.airLoss * (0.8f + currentPresence * 0.4f), 0.7f, // Variable frequency
        juce::Decibels::decibelsToGain(airLoss));
}

void CabinetSimulator::setupCabinetResponse(CabinetModel model)
{
    // Cabinet responses are set up in constructor, but this could be extended
    // for loading custom impulse responses or additional cabinet modeling
}