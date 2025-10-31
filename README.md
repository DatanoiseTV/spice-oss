# Spice FX - Open Source Edition

A high-quality analog saturation VST3/AU/LV2 plugin with multiple distortion models.
Open sourced in October 2025.

## Features

- Multiple analog saturation models:
  - Tube (12AX7 vacuum tube warmth and harmonics)
  - Transistor (solid-state edge and grit)
  - Transformer (iron core saturation)
  - Tape (magnetic tape compression)
  - Diode (asymmetric clipping)
  - Germanium (vintage transistor warmth)
  - FET (field-effect transistor smoothness)
  - Op-Amp (operational amplifier precision)
  - Fuzz (extreme distortion)
  - Ribbon (ribbon mic saturation)
- Advanced features:
  - Mid-side processing
  - Cabinet simulation (10 models)
  - Noise gate
  - Output limiter
  - Auto-gain compensation
- Multi-stage processing chain
- High-quality oversampling (up to 4x)
- Professional analog-modeled UI
- Full automation support
- 50+ factory presets

## Installation

### Windows Security Notice
**Windows users**: You may see a SmartScreen warning when running Spice FX. This is because the software is not digitally signed (code signing certificates are expensive). The software is safe - see [WINDOWS_SECURITY_NOTICE.md](WINDOWS_SECURITY_NOTICE.md) for details and verification instructions.


### Installation by Platform

#### Windows
1. Download `Spice-Windows.zip` and the corresponding `-checksums.txt` file
2. Verify the download integrity using the checksums (see security notice)
3. Extract the ZIP file
4. Copy the VST3 folder to `C:\Program Files\Common Files\VST3`
5. Run the standalone exe from any location

#### macOS
1. Download `Spice-macOS.dmg`
2. Open the DMG and drag the plugins to your plugin folders:
   - VST3: `/Library/Audio/Plug-Ins/VST3`
   - AU: `/Library/Audio/Plug-Ins/Components`
   - Standalone: `/Applications`

#### Linux
1. Download the appropriate version for your distribution
2. Extract the archive
3. Copy the plugins to:
   - VST3: `~/.vst3` or `/usr/lib/vst3`
   - LV2: `~/.lv2` or `/usr/lib/lv2`
   - Standalone: Run from any location

## Building from Source

### Requirements
- C++17 or later
- CMake 3.15 or later
- JUCE 8.0.8
- Platform-specific build tools:
  - Windows: Visual Studio 2022
  - macOS: Xcode
  - Linux: GCC/Clang, GTK3 development headers

### Build Instructions

```bash
# Clone with submodules
git clone --recursive https://github.com/DatanoiseTV/spice-oss.git
cd spice-prod

# Configure and build
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release

# The built plugins will be in:
# build/Spice_artefacts/Release/
```