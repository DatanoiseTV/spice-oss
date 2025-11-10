cmake_minimum_required(VERSION 3.15)
project(Spice VERSION 1.0.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE subdirectory
add_subdirectory(JUCE)

# Set platform-specific icon
if(APPLE)
    set(ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.icns")
else()
    set(ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.icns")
endif()

# Define the plugin
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # iOS specific plugin configuration
    juce_add_plugin(Spice
        COMPANY_NAME "DatanoiseTV"
        BUNDLE_ID "com.datanoise.spicefx"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE Dnug
        PLUGIN_CODE Spce
        FORMATS AUv3 Standalone
        PRODUCT_NAME "Spice FX"
        IPHONE_SCREEN_ORIENTATIONS UIInterfaceOrientationLandscapeLeft UIInterfaceOrientationLandscapeRight
        IPAD_SCREEN_ORIENTATIONS UIInterfaceOrientationLandscapeLeft UIInterfaceOrientationLandscapeRight
        REQUIRES_FULL_SCREEN FALSE
        ICON_BIG "${ICON_FILE}"
        ICON_SMALL "${ICON_FILE}")
elseif(WIN32)
    # On Windows, use Windows icon file
    juce_add_plugin(Spice
        COMPANY_NAME "DatanoiseTV"
        BUNDLE_ID "com.datanoise.spicefx"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Dnug
        PLUGIN_CODE Spce
        FORMATS AU VST3 Standalone LV2
        PRODUCT_NAME "Spice FX"
        LV2URI "http://datanoise.com/plugins/spice"
        APP_SANDBOX_ENABLED TRUE
        APP_SANDBOX_OPTIONS com.apple.security.files.user-selected.read-write
        PLIST_TO_MERGE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info-Standalone.plist"
        ICON_BIG "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.ico"
        ICON_SMALL "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.ico")
else()
    juce_add_plugin(Spice
        COMPANY_NAME "DatanoiseTV"
        BUNDLE_ID "com.datanoise.spicefx"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Dnug
        PLUGIN_CODE Spce
        FORMATS AU VST3 Standalone LV2
        PRODUCT_NAME "Spice FX"
        LV2URI "http://datanoise.com/plugins/spice"
        APP_SANDBOX_ENABLED TRUE
        APP_SANDBOX_OPTIONS com.apple.security.files.user-selected.read-write
        PLIST_TO_MERGE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info-Standalone.plist"
        ICON_BIG "${ICON_FILE}"
        ICON_SMALL "${ICON_FILE}")
endif()

# Plugin version info
target_compile_definitions(Spice
    PUBLIC
        JUCE_USE_CURL=0
        JUCE_WEB_BROWSER=0
        JUCE_JACK=1
        JUCE_VST3_CAN_REPLACE_VST2=0)

# Generate JuceHeader.h
juce_generate_juce_header(Spice)

# Source files
target_sources(Spice
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginProcessor.h
        Source/PluginEditor.cpp
        Source/PluginEditor.h
        Source/DSP/SaturationProcessor.cpp
        Source/DSP/SaturationProcessor.h
        Source/DSP/Oversampling.cpp
        Source/DSP/Oversampling.h
        Source/DSP/FilterChain.cpp
        Source/DSP/FilterChain.h
        Source/DSP/CabinetSimulator.cpp
        Source/DSP/CabinetSimulator.h
        Source/DSP/MidSideProcessor.cpp
        Source/DSP/MidSideProcessor.h
        Source/UI/LookAndFeel.cpp
        Source/UI/LookAndFeel.h
        Source/UI/WaveformVisualizer.cpp
        Source/UI/WaveformVisualizer.h
        Source/UI/AnalogLamp.cpp
        Source/UI/AnalogLamp.h
        Source/UI/OnOffButton.h
        Source/PresetManager.cpp
        Source/PresetManager.h
        Source/UI/PresetSaveDialog.cpp
        Source/UI/PresetSaveDialog.h
        # ff_meters sources
        ff_meters/ff_meters.cpp
        ff_meters/ff_meters.h
        ff_meters/LevelMeter/LevelMeter.cpp
        ff_meters/LevelMeter/LevelMeter.h
        ff_meters/LevelMeter/LevelMeterSource.h
        ff_meters/LookAndFeel/LevelMeterLookAndFeel.h
        ff_meters/LookAndFeel/LevelMeterLookAndFeelMethods.h)

# Binary data
juce_add_binary_data(BinaryData
    SOURCES
        Resources/background.png
        Resources/BgTexture.jpg
        Aveschon.otf
        DirtyHarold.ttf
        Pixim.otf)

# Include directories
target_include_directories(Spice
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/ff_meters)

# Link libraries
target_link_libraries(Spice
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

# Platform-specific configuration
if(APPLE)
    # Check if building for iOS
    if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        # iOS/iPadOS specific settings
        set(CMAKE_OSX_DEPLOYMENT_TARGET "12.0" CACHE STRING "Minimum iOS version")
        
        # iOS bundle settings
        if(TARGET Spice_AUv3)
            set_target_properties(Spice_AUv3 PROPERTIES
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.datanoise.spicefx.auv3"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${IOS_DEVELOPMENT_TEAM}"
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"  # iPhone and iPad
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "${CMAKE_OSX_DEPLOYMENT_TARGET}"
            XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
            )
        endif()
        
        if(TARGET Spice_Standalone)
            set_target_properties(Spice_Standalone PROPERTIES
            XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.datanoise.spicefx"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "${IOS_DEVELOPMENT_TEAM}"
            XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2"  # iPhone and iPad
            XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET "${CMAKE_OSX_DEPLOYMENT_TARGET}"
            XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
            XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
            XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info-iOS.plist"
            )
        endif()
        
        # iOS capabilities
        if(TARGET Spice_AUv3)
            set_target_properties(Spice_AUv3 PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Spice-iOS.entitlements"
            )
        endif()
        
        if(TARGET Spice_Standalone)
            set_target_properties(Spice_Standalone PROPERTIES
                XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Spice-iOS.entitlements"
            )
        endif()
        
    else()
        # macOS specific settings
        # Copy icon to standalone app bundle
        add_custom_command(TARGET Spice_Standalone POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.icns"
        "$<TARGET_BUNDLE_DIR:Spice_Standalone>/Contents/Resources/Icon.icns"
        COMMENT "Copying Icon.icns to app bundle")
    
    # Set minimum deployment target (can be overridden by CI)
    if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
        set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11")
    endif()
    
    # Universal binary support (can be overridden by CI)
    if(NOT CMAKE_OSX_ARCHITECTURES)
        set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
    endif()
    
    # Enable dSYM generation for release builds
    set(CMAKE_XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym")
    set(CMAKE_XCODE_ATTRIBUTE_DWARF_DSYM_FOLDER_PATH "${CMAKE_BINARY_DIR}/dSYMs")
    set(CMAKE_XCODE_ATTRIBUTE_DWARF_DSYM_FILE_SHOULD_ACCOMPANY_PRODUCT YES)
    
    # Standalone app configuration for App Store
    set_target_properties(Spice_Standalone PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.datanoise.spicefx"
        XCODE_ATTRIBUTE_MARKETING_VERSION "1.0.0"
        XCODE_ATTRIBUTE_CURRENT_PROJECT_VERSION "1"
        XCODE_ATTRIBUTE_GENERATE_INFOPLIST_FILE "NO"
        XCODE_ATTRIBUTE_INFOPLIST_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Info-Standalone.plist"
        XCODE_ATTRIBUTE_ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon"
        XCODE_ATTRIBUTE_ENABLE_HARDENED_RUNTIME "YES"
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Spice.entitlements"
    )
    
    # Apply entitlements to AU and VST3 plugins
    set_target_properties(Spice_AU PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Spice.entitlements"
    )
    
    set_target_properties(Spice_VST3 PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Spice.entitlements"
    )
    
    # Apply entitlements to LV2 if it exists
    if(TARGET Spice_LV2)
        set_target_properties(Spice_LV2 PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Spice.entitlements"
        )
    endif()
    
    # Code signing for local development only
    if(DEFINED ENV{XCODE_SIGN_IDENTITY})
        set_target_properties(Spice_AU PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "$ENV{XCODE_SIGN_IDENTITY}"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{XCODE_TEAM_ID}"
            XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
        )
        
        set_target_properties(Spice_VST3 PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "$ENV{XCODE_SIGN_IDENTITY}"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{XCODE_TEAM_ID}"
            XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
        )
        
        set_target_properties(Spice_Standalone PROPERTIES
            XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "$ENV{XCODE_SIGN_IDENTITY}"
            XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{XCODE_TEAM_ID}"
            XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
        )
        
        if(TARGET Spice_LV2)
            set_target_properties(Spice_LV2 PROPERTIES
                XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "$ENV{XCODE_SIGN_IDENTITY}"
                XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "$ENV{XCODE_TEAM_ID}"
                XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
            )
        endif()
    endif()
    endif()  # End of iOS check
endif()

# Windows-specific configuration
if(WIN32)
    # Add Windows resource file for icon
    target_sources(Spice PRIVATE Resources/Spice.rc)
    
    # Enable Windows 10 features
    target_compile_definitions(Spice PRIVATE WINVER=0x0A00 _WIN32_WINNT=0x0A00)
    
    # Optimize Windows builds
    if(MSVC)
        # Enable multiprocessor compilation
        target_compile_options(Spice PRIVATE /MP)
        
        # Use faster linker options in Release mode
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            set_target_properties(Spice PROPERTIES
                LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LTCG /OPT:REF /OPT:ICF")
        endif()
    endif()
endif()

# Testing
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(Tests)
endif()
