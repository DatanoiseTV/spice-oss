#include "WaveformVisualizer.h"

WaveformVisualizer::WaveformVisualizer()
{
    std::fill(inputData.begin(), inputData.end(), 0.0f);
    std::fill(outputData.begin(), outputData.end(), 0.0f);
    std::fill(inputDisplay.begin(), inputDisplay.end(), 0.0f);
    std::fill(outputDisplay.begin(), outputDisplay.end(), 0.0f);
    
    startTimerHz(60); // Faster refresh for real-time feel
}

WaveformVisualizer::~WaveformVisualizer()
{
    stopTimer();
}

void WaveformVisualizer::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat();
    
    // Black frame around waveform
    auto frameThickness = 6.0f;
    auto innerBounds = bounds.reduced(frameThickness / 2);
    
    // Outer shadow
    g.setColour(juce::Colour(0x40000000));
    g.drawRoundedRectangle(bounds, 12.0f, frameThickness + 2);
    
    // Main black frame
    g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(bounds, 12.0f, frameThickness);
    
    // Inner highlight
    g.setColour(juce::Colour(0x20ffffff));
    g.drawRoundedRectangle(innerBounds, 8.0f, 1.0f);
    
    // Waveform background with subtle CRT effect
    g.setColour(juce::Colour(0xff0a0a0a));
    g.fillRoundedRectangle(innerBounds, 8.0f);
    
    // Subtle scanline effect
    for (int y = static_cast<int>(innerBounds.getY()); y < innerBounds.getBottom(); y += 3)
    {
        g.setColour(juce::Colour(0x08000000));
        g.drawHorizontalLine(y, innerBounds.getX(), innerBounds.getRight());
    }
    
    // Subtle noise texture
    juce::Random random;
    for (int i = 0; i < 50; ++i)
    {
        float x = innerBounds.getX() + random.nextFloat() * innerBounds.getWidth();
        float y = innerBounds.getY() + random.nextFloat() * innerBounds.getHeight();
        g.setColour(juce::Colour(0x10ffffff));
        g.fillEllipse(x, y, 1.0f, 1.0f);
    }
    
    // Clipping indicator background
    if (clippingDetected) {
        g.setColour(juce::Colour(0x20ff4757));
        g.fillRoundedRectangle(innerBounds, 8.0f);
    }
    
    // Grid with separate areas for input and output
    g.setColour(juce::Colour(0xff202020));
    float centerY = innerBounds.getCentreY();
    float topCenterY = innerBounds.getY() + innerBounds.getHeight() * 0.25f;
    float bottomCenterY = innerBounds.getY() + innerBounds.getHeight() * 0.75f;
    
    // Center line dividing input and output areas
    g.setColour(juce::Colour(0xff404040));
    g.drawLine(innerBounds.getX() + 10, centerY, innerBounds.getRight() - 10, centerY, 1.0f);
    
    // Zero lines for input and output
    g.setColour(juce::Colour(0xff202020));
    g.drawLine(innerBounds.getX() + 10, topCenterY, innerBounds.getRight() - 10, topCenterY, 0.5f);
    g.drawLine(innerBounds.getX() + 10, bottomCenterY, innerBounds.getRight() - 10, bottomCenterY, 0.5f);
    
    // Vertical grid lines
    for (int i = 1; i < 4; ++i)
    {
        float x = innerBounds.getX() + innerBounds.getWidth() * i / 4.0f;
        g.drawLine(x, innerBounds.getY() + 10, x, innerBounds.getBottom() - 10, 0.3f);
    }
    
    // Draw input waveform (green) in top half with analog phosphor glow
    if (!inputPath.isEmpty())
    {
        auto inputColour = juce::Colour(0xff2ecc71);
        
        // Outer glow layer - wide and faint
        g.setColour(inputColour.withAlpha(0.1f));
        g.strokePath(inputPath, juce::PathStrokeType(6.0f));
        
        // Middle glow layer
        g.setColour(inputColour.withAlpha(0.2f));
        g.strokePath(inputPath, juce::PathStrokeType(4.0f));
        
        // Inner glow - brighter
        g.setColour(inputColour.withAlpha(0.4f));
        g.strokePath(inputPath, juce::PathStrokeType(2.5f));
        
        // Main waveform line - bright core
        g.setColour(inputColour.brighter(0.3f));
        g.strokePath(inputPath, juce::PathStrokeType(1.2f));
    }
    
    // Draw output waveform (red) in bottom half with dynamic coloring and analog glow
    if (!outputPath.isEmpty())
    {
        auto satLevel = saturationLevel.load();
        auto outputColour = juce::Colour(0xffe74c3c); // Red
        
        if (satLevel > 0.5f) {
            outputColour = juce::Colour(0xfff39c12); // Orange when saturating
        }
        if (clippingDetected) {
            outputColour = juce::Colour(0xffc0392b); // Dark red when clipping
        }
        
        // Multi-layer phosphor glow effect
        // Outer glow - wide and faint
        g.setColour(outputColour.withAlpha(0.1f));
        g.strokePath(outputPath, juce::PathStrokeType(6.0f));
        
        // Middle glow
        g.setColour(outputColour.withAlpha(0.2f));
        g.strokePath(outputPath, juce::PathStrokeType(4.0f));
        
        // Inner glow - brighter
        g.setColour(outputColour.withAlpha(0.4f));
        g.strokePath(outputPath, juce::PathStrokeType(2.5f));
        
        // Main waveform line - bright core
        g.setColour(outputColour.brighter(0.3f));
        g.strokePath(outputPath, juce::PathStrokeType(1.2f));
    }
    
    // Status indicators
    auto statusArea = innerBounds.removeFromTop(16).reduced(8, 2);
    g.setFont(juce::FontOptions(9.0f).withStyle("Bold"));
    
    // Input label (green)
    g.setColour(juce::Colour(0xff2ecc71));
    g.drawText("IN", statusArea, juce::Justification::topLeft);
    
    // Output label (red)
    g.setColour(juce::Colour(0xffe74c3c));
    g.drawText("OUT", statusArea, juce::Justification::topRight);
    
    // CRT glow effect at edges
    juce::ColourGradient crtGlow(
        juce::Colour(0x10ffffff), innerBounds.getCentreX(), innerBounds.getCentreY(),
        juce::Colour(0x00ffffff), innerBounds.getX(), innerBounds.getY(), true);
    g.setGradientFill(crtGlow);
    g.fillRoundedRectangle(innerBounds, 8.0f);
}

void WaveformVisualizer::resized()
{
    createWaveformPaths();
}

void WaveformVisualizer::timerCallback()
{
    // Smooth display data with faster response
    const float smoothing = 0.5f;
    for (int i = 0; i < bufferSize; ++i)
    {
        inputDisplay[i] = inputDisplay[i] * smoothing + inputData[i] * (1.0f - smoothing);
        outputDisplay[i] = outputDisplay[i] * smoothing + outputData[i] * (1.0f - smoothing);
    }
    
    createWaveformPaths();
    repaint();
}

void WaveformVisualizer::pushInputBuffer(const juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    
    if (numChannels == 0 || numSamples == 0)
        return;
    
    // Downsample and store
    int downsampleFactor = std::max(1, numSamples / 64);
    
    for (int i = 0; i < numSamples; i += downsampleFactor)
    {
        float sum = 0.0f;
        int count = 0;
        
        for (int j = i; j < std::min(i + downsampleFactor, numSamples); ++j)
        {
            for (int ch = 0; ch < numChannels; ++ch)
            {
                sum += buffer.getSample(ch, j);
                count++;
            }
        }
        
        if (count > 0)
        {
            inputData[writePosition] = sum / count;
            writePosition = (writePosition + 1) % bufferSize;
        }
    }
}

void WaveformVisualizer::pushOutputBuffer(const juce::AudioBuffer<float>& buffer)
{
    auto numSamples = buffer.getNumSamples();
    auto numChannels = buffer.getNumChannels();
    
    if (numChannels == 0 || numSamples == 0)
        return;
    
    // Downsample and store with saturation/clipping detection
    int downsampleFactor = std::max(1, numSamples / 32);
    float maxSample = 0.0f;
    float saturationSum = 0.0f;
    int saturationCount = 0;
    
    for (int i = 0; i < numSamples; i += downsampleFactor)
    {
        float sum = 0.0f;
        int count = 0;
        
        for (int j = i; j < std::min(i + downsampleFactor, numSamples); ++j)
        {
            for (int ch = 0; ch < numChannels; ++ch)
            {
                auto sample = buffer.getSample(ch, j);
                sum += sample;
                count++;
                
                // Track peak levels for clipping detection
                auto absSample = std::abs(sample);
                maxSample = std::max(maxSample, absSample);
                
                // Track RMS for saturation level
                saturationSum += sample * sample;
                saturationCount++;
            }
        }
        
        if (count > 0)
        {
            outputData[writePosition] = sum / count;
            writePosition = (writePosition + 1) % bufferSize;
        }
    }
    
    // Update saturation level - use RMS for more musical response
    float rmsLevel = 0.0f;
    if (saturationCount > 0)
    {
        rmsLevel = std::sqrt(saturationSum / saturationCount);
    }
    // Combine peak and RMS for responsive yet smooth behavior
    saturationLevel = maxSample * 0.3f + rmsLevel * 0.7f;
    
    // Update clipping detection
    if (maxSample >= 0.95f) {
        clippingDetected = true;
        lastClipTime = juce::Time::getCurrentTime();
    } else if (juce::Time::getCurrentTime() - lastClipTime > juce::RelativeTime::seconds(1.0)) {
        clippingDetected = false;
    }
}

void WaveformVisualizer::createWaveformPaths()
{
    // Account for the beautiful frame
    auto bounds = getLocalBounds().toFloat().reduced(12, 20);
    inputPath.clear();
    outputPath.clear();
    
    if (bounds.getWidth() <= 0)
        return;
    
    float xScale = bounds.getWidth() / static_cast<float>(bufferSize);
    float yScale = bounds.getHeight() * 0.2f; // Reduced scale for two separate areas
    float topCenterY = bounds.getY() + bounds.getHeight() * 0.25f; // Top quarter for input
    float bottomCenterY = bounds.getY() + bounds.getHeight() * 0.75f; // Bottom quarter for output
    
    int startIdx = (writePosition + 1) % bufferSize;
    
    // Create input path (top half)
    for (int i = 0; i < bufferSize; ++i)
    {
        int idx = (startIdx + i) % bufferSize;
        float x = bounds.getX() + i * xScale;
        float y = topCenterY - inputDisplay[idx] * yScale;
        
        if (i == 0)
            inputPath.startNewSubPath(x, y);
        else
            inputPath.lineTo(x, y);
    }
    
    // Create output path (bottom half)
    for (int i = 0; i < bufferSize; ++i)
    {
        int idx = (startIdx + i) % bufferSize;
        float x = bounds.getX() + i * xScale;
        float y = bottomCenterY - outputDisplay[idx] * yScale;
        
        if (i == 0)
            outputPath.startNewSubPath(x, y);
        else
            outputPath.lineTo(x, y);
    }
}