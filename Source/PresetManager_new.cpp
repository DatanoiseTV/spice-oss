void PresetManager::createFactoryPresets()
{
    // First delete all existing presets
    auto presetsDir = getPresetsDirectory();
    for (auto file : presetsDir.findChildFiles(juce::File::findFiles, false, "*" + juce::String(presetFileExtension)))
    {
        file.deleteFile();
    }
    
    // Helper lambda to set all parameters
    auto setAllParams = [this](float inputGain, float drive, float mix, float output, int model, 
                               float tone, float bias, int quality, float lowCut, float highCut,
                               bool gateEnabled, float gateThreshold, bool cabinetEnabled, 
                               int cabinetModel, float cabinetPresence, float cabinetMix)
    {
        valueTreeState.getParameter("inputGain")->setValueNotifyingHost(inputGain);
        valueTreeState.getParameter("drive")->setValueNotifyingHost(drive);
        valueTreeState.getParameter("mix")->setValueNotifyingHost(mix);
        valueTreeState.getParameter("output")->setValueNotifyingHost(output);
        valueTreeState.getParameter("model")->setValueNotifyingHost(model / 10.0f);
        valueTreeState.getParameter("tone")->setValueNotifyingHost(tone);
        valueTreeState.getParameter("bias")->setValueNotifyingHost(bias);
        valueTreeState.getParameter("quality")->setValueNotifyingHost(quality / 2.0f);
        valueTreeState.getParameter("lowCut")->setValueNotifyingHost(lowCut);
        valueTreeState.getParameter("highCut")->setValueNotifyingHost(highCut);
        valueTreeState.getParameter("gateEnabled")->setValueNotifyingHost(gateEnabled ? 1.0f : 0.0f);
        valueTreeState.getParameter("gateThreshold")->setValueNotifyingHost(gateThreshold);
        valueTreeState.getParameter("cabinetEnabled")->setValueNotifyingHost(cabinetEnabled ? 1.0f : 0.0f);
        valueTreeState.getParameter("cabinetModel")->setValueNotifyingHost(cabinetModel / 9.0f);
        valueTreeState.getParameter("cabinetPresence")->setValueNotifyingHost(cabinetPresence);
        valueTreeState.getParameter("cabinetMix")->setValueNotifyingHost(cabinetMix);
    };
    
    // 1. Init (Default)
    setAllParams(0.5f, 0.0f, 1.0f, 0.5f, 0, 0.5f, 0.5f, 1, 0.0f, 1.0f, false, 0.333f, false, 0, 0.3f, 1.0f);
    savePreset("Init");
    
    // 2. Clean Warmth
    setAllParams(0.5f, 0.15f, 1.0f, 0.5f, 0, 0.45f, 0.5f, 1, 0.0f, 1.0f, false, 0.333f, false, 0, 0.3f, 1.0f);
    savePreset("Clean Warmth");
    
    // 3. Vintage Console
    setAllParams(0.45f, 0.25f, 1.0f, 0.5f, 5, 0.6f, 0.55f, 2, 0.02f, 0.98f, false, 0.333f, true, 6, 0.4f, 0.8f);
    savePreset("Vintage Console");
    
    // 4. Tape Glue
    setAllParams(0.52f, 0.35f, 1.0f, 0.48f, 3, 0.4f, 0.45f, 1, 0.0f, 0.95f, false, 0.333f, false, 0, 0.3f, 1.0f);
    savePreset("Tape Glue");
    
    // 5. Rock Guitar
    setAllParams(0.58f, 0.65f, 1.0f, 0.45f, 9, 0.7f, 0.6f, 2, 0.08f, 1.0f, true, 0.4f, true, 3, 0.2f, 0.9f);
    savePreset("Rock Guitar");
    
    // 6. Fuzz Madness
    setAllParams(0.62f, 0.8f, 1.0f, 0.4f, 8, 0.8f, 0.7f, 1, 0.0f, 1.0f, false, 0.333f, true, 7, 0.5f, 1.0f);
    savePreset("Fuzz Madness");
    
    // 7. Warm Bass
    setAllParams(0.48f, 0.4f, 1.0f, 0.5f, 6, 0.3f, 0.4f, 1, 0.05f, 1.0f, true, 0.45f, true, 2, 0.3f, 0.7f);
    savePreset("Warm Bass");
    
    // 8. Bright Presence
    setAllParams(0.5f, 0.3f, 1.0f, 0.52f, 7, 0.8f, 0.6f, 2, 0.0f, 1.0f, false, 0.333f, true, 8, 0.6f, 0.5f);
    savePreset("Bright Presence");
    
    // 9. Drum Punch
    setAllParams(0.55f, 0.45f, 1.0f, 0.48f, 1, 0.55f, 0.65f, 1, 0.03f, 1.0f, true, 0.35f, false, 0, 0.3f, 1.0f);
    savePreset("Drum Punch");
    
    // 10. Vocal Silk
    setAllParams(0.47f, 0.2f, 1.0f, 0.5f, 0, 0.5f, 0.45f, 2, 0.08f, 0.98f, true, 0.5f, false, 0, 0.3f, 1.0f);
    savePreset("Vocal Silk");
    
    // 11. Master Bus
    setAllParams(0.5f, 0.1f, 0.8f, 0.5f, 2, 0.5f, 0.5f, 2, 0.0f, 1.0f, false, 0.333f, false, 0, 0.3f, 1.0f);
    savePreset("Master Bus");
    
    // 12. 12AX7 Clean
    setAllParams(0.48f, 0.25f, 1.0f, 0.5f, 10, 0.55f, 0.55f, 1, 0.0f, 1.0f, false, 0.333f, true, 5, 0.25f, 0.9f);
    savePreset("12AX7 Clean");
    
    // 13. 12AX7 Crunch
    setAllParams(0.53f, 0.55f, 1.0f, 0.47f, 10, 0.45f, 0.65f, 2, 0.02f, 1.0f, false, 0.333f, true, 1, 0.15f, 1.0f);
    savePreset("12AX7 Crunch");
    
    // 14. 12AX7 Lead
    setAllParams(0.6f, 0.8f, 1.0f, 0.44f, 10, 0.65f, 0.6f, 2, 0.05f, 0.98f, true, 0.42f, true, 6, 0.1f, 0.95f);
    savePreset("12AX7 Lead");
    
    // 15. Analog Desk
    setAllParams(0.49f, 0.35f, 0.95f, 0.5f, 5, 0.52f, 0.48f, 2, 0.01f, 0.99f, false, 0.333f, true, 4, 0.45f, 0.6f);
    savePreset("Analog Desk");
    
    // 16. Kick Destroyer
    setAllParams(0.65f, 0.7f, 1.0f, 0.42f, 4, 0.35f, 0.7f, 1, 0.1f, 0.9f, true, 0.38f, false, 0, 0.3f, 1.0f);
    savePreset("Kick Destroyer");
    
    // 17. Snare Crack
    setAllParams(0.52f, 0.5f, 1.0f, 0.48f, 1, 0.7f, 0.6f, 1, 0.15f, 1.0f, true, 0.4f, false, 0, 0.3f, 1.0f);
    savePreset("Snare Crack");
    
    // 18. Piano Warmth
    setAllParams(0.46f, 0.18f, 0.85f, 0.5f, 0, 0.4f, 0.45f, 2, 0.0f, 0.95f, false, 0.333f, true, 9, 0.7f, 0.4f);
    savePreset("Piano Warmth");
    
    // 19. String Section
    setAllParams(0.48f, 0.22f, 0.9f, 0.5f, 2, 0.58f, 0.5f, 2, 0.02f, 0.97f, false, 0.333f, true, 5, 0.5f, 0.5f);
    savePreset("String Section");
    
    // 20. Synth Grit
    setAllParams(0.56f, 0.6f, 1.0f, 0.46f, 4, 0.75f, 0.55f, 0, 0.0f, 1.0f, false, 0.333f, true, 3, 0.2f, 0.8f);
    savePreset("Synth Grit");
    
    // 21. Bass DI
    setAllParams(0.51f, 0.32f, 1.0f, 0.5f, 1, 0.35f, 0.52f, 1, 0.08f, 0.98f, true, 0.5f, true, 2, 0.4f, 0.6f);
    savePreset("Bass DI");
    
    // 22. Acoustic Guitar
    setAllParams(0.45f, 0.15f, 0.75f, 0.5f, 7, 0.65f, 0.5f, 2, 0.04f, 1.0f, true, 0.55f, true, 9, 0.8f, 0.3f);
    savePreset("Acoustic Guitar");
    
    // 23. Lo-Fi Magic
    setAllParams(0.54f, 0.55f, 1.0f, 0.45f, 3, 0.25f, 0.65f, 0, 0.2f, 0.7f, false, 0.333f, true, 0, 0.9f, 1.0f);
    savePreset("Lo-Fi Magic");
    
    // 24. Mix Bus Glue
    setAllParams(0.5f, 0.12f, 0.7f, 0.5f, 3, 0.48f, 0.5f, 2, 0.0f, 1.0f, false, 0.333f, false, 0, 0.3f, 1.0f);
    savePreset("Mix Bus Glue");
    
    // 25. Parallel Smash
    setAllParams(0.7f, 0.85f, 0.3f, 0.5f, 8, 0.6f, 0.7f, 1, 0.0f, 1.0f, false, 0.333f, false, 0, 0.3f, 1.0f);
    savePreset("Parallel Smash");
    
    // 26. Radio Voice
    setAllParams(0.52f, 0.4f, 1.0f, 0.52f, 7, 0.85f, 0.55f, 0, 0.15f, 0.8f, true, 0.45f, true, 8, 0.6f, 0.7f);
    savePreset("Radio Voice");
    
    // 27. Drum Bus
    setAllParams(0.53f, 0.38f, 0.9f, 0.49f, 5, 0.55f, 0.58f, 1, 0.02f, 1.0f, true, 0.35f, true, 4, 0.3f, 0.6f);
    savePreset("Drum Bus");
    
    // 28. Electric Lead
    setAllParams(0.57f, 0.72f, 1.0f, 0.45f, 9, 0.62f, 0.65f, 2, 0.1f, 1.0f, true, 0.4f, true, 6, 0.15f, 0.95f);
    savePreset("Electric Lead");
    
    // 29. Smooth Jazz
    setAllParams(0.47f, 0.25f, 0.8f, 0.5f, 6, 0.38f, 0.45f, 2, 0.0f, 0.96f, false, 0.333f, true, 8, 0.75f, 0.4f);
    savePreset("Smooth Jazz");
    
    // 30. Broken Speaker
    setAllParams(0.68f, 0.9f, 1.0f, 0.38f, 8, 0.2f, 0.8f, 0, 0.0f, 0.6f, false, 0.333f, true, 7, 0.1f, 1.0f);
    savePreset("Broken Speaker");
    
    // 31. API Style
    setAllParams(0.49f, 0.28f, 1.0f, 0.51f, 1, 0.62f, 0.55f, 2, 0.01f, 1.0f, false, 0.333f, true, 5, 0.35f, 0.7f);
    savePreset("API Style");
    
    // 32. Neve Style
    setAllParams(0.48f, 0.32f, 1.0f, 0.49f, 2, 0.45f, 0.52f, 2, 0.02f, 0.99f, false, 0.333f, true, 4, 0.5f, 0.6f);
    savePreset("Neve Style");
    
    // 33. SSL Style
    setAllParams(0.5f, 0.24f, 0.95f, 0.5f, 5, 0.68f, 0.5f, 2, 0.0f, 1.0f, false, 0.333f, true, 3, 0.4f, 0.5f);
    savePreset("SSL Style");
    
    // 34. Vocal Thickener
    setAllParams(0.48f, 0.35f, 0.8f, 0.52f, 0, 0.58f, 0.48f, 2, 0.06f, 0.98f, true, 0.48f, false, 0, 0.3f, 1.0f);
    savePreset("Vocal Thickener");
    
    // 35. Drum Room
    setAllParams(0.54f, 0.42f, 0.85f, 0.48f, 3, 0.45f, 0.6f, 1, 0.0f, 1.0f, false, 0.333f, true, 9, 0.8f, 0.5f);
    savePreset("Drum Room");
    
    // 36. Smooth Operator
    setAllParams(0.46f, 0.18f, 0.75f, 0.51f, 6, 0.42f, 0.45f, 2, 0.0f, 1.0f, false, 0.333f, true, 1, 0.6f, 0.3f);
    savePreset("Smooth Operator");
    
    // 37. Gritty Pump
    setAllParams(0.62f, 0.75f, 1.0f, 0.4f, 4, 0.65f, 0.72f, 1, 0.05f, 1.0f, true, 0.38f, true, 7, 0.25f, 0.9f);
    savePreset("Gritty Pump");
    
    // 38. Boutique Preamp
    setAllParams(0.47f, 0.28f, 0.92f, 0.5f, 10, 0.55f, 0.52f, 2, 0.0f, 1.0f, false, 0.333f, true, 5, 0.45f, 0.8f);
    savePreset("Boutique Preamp");
    
    // 39. Vintage Compressor
    setAllParams(0.51f, 0.45f, 0.88f, 0.49f, 2, 0.48f, 0.55f, 2, 0.01f, 0.99f, false, 0.333f, true, 0, 0.55f, 0.5f);
    savePreset("Vintage Compressor");
    
    // 40. Harmonic Enhancer
    setAllParams(0.49f, 0.38f, 0.7f, 0.51f, 10, 0.6f, 0.65f, 2, 0.0f, 1.0f, false, 0.333f, true, 5, 0.3f, 0.6f);
    savePreset("Harmonic Enhancer");
    
    // 41. Retro Channel
    setAllParams(0.48f, 0.32f, 0.95f, 0.5f, 5, 0.52f, 0.5f, 2, 0.01f, 0.98f, false, 0.333f, true, 9, 0.65f, 0.4f);
    savePreset("Retro Channel");
    
    // 42. Distorted Dreams
    setAllParams(0.7f, 0.85f, 1.0f, 0.38f, 8, 0.3f, 0.8f, 0, 0.0f, 0.8f, false, 0.333f, true, 7, 0.05f, 1.0f);
    savePreset("Distorted Dreams");
    
    // 43. Crystal Clean
    setAllParams(0.44f, 0.08f, 0.6f, 0.52f, 7, 0.75f, 0.45f, 2, 0.0f, 1.0f, false, 0.333f, true, 8, 0.85f, 0.2f);
    savePreset("Crystal Clean");
    
    // 44. Dark Matter
    setAllParams(0.58f, 0.65f, 1.0f, 0.42f, 3, 0.25f, 0.7f, 1, 0.08f, 0.85f, true, 0.4f, true, 2, 0.2f, 0.9f);
    savePreset("Dark Matter");
    
    // 45. Sparkle Top
    setAllParams(0.46f, 0.22f, 0.7f, 0.52f, 7, 0.82f, 0.48f, 2, 0.0f, 1.0f, false, 0.333f, true, 8, 0.9f, 0.3f);
    savePreset("Sparkle Top");
    
    // 46. Deep Groove
    setAllParams(0.53f, 0.48f, 0.9f, 0.47f, 1, 0.35f, 0.6f, 1, 0.1f, 0.95f, true, 0.42f, true, 2, 0.35f, 0.7f);
    savePreset("Deep Groove");
    
    // 47. Modern Edge
    setAllParams(0.55f, 0.55f, 1.0f, 0.45f, 4, 0.72f, 0.62f, 2, 0.02f, 1.0f, false, 0.333f, true, 3, 0.25f, 0.85f);
    savePreset("Modern Edge");
    
    // 48. Power Surge
    setAllParams(0.65f, 0.8f, 1.0f, 0.4f, 9, 0.68f, 0.75f, 1, 0.05f, 1.0f, true, 0.38f, true, 6, 0.1f, 1.0f);
    savePreset("Power Surge");
    
    // 49. Space Echo
    setAllParams(0.5f, 0.35f, 0.9f, 0.49f, 3, 0.38f, 0.52f, 2, 0.0f, 0.92f, false, 0.333f, true, 0, 0.95f, 0.6f);
    savePreset("Space Echo");
    
    // 50. Ultimate Spice
    setAllParams(0.6f, 0.7f, 1.0f, 0.43f, 10, 0.6f, 0.65f, 2, 0.03f, 0.98f, true, 0.4f, true, 5, 0.2f, 0.9f);
    savePreset("Ultimate Spice");
    
    // Restore to Init preset
    loadPreset(defaultPresetName);
}