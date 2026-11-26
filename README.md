# YGM Keys

A small polyphonic macOS instrument plug-in from YGM. It builds VST3, AU, and Standalone targets with JUCE.

## Build on macOS

Install Xcode and CMake, then run:

```bash
cmake -B build -S . -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel
```

The built plug-ins are copied to the standard user plug-in locations when `COPY_PLUGIN_AFTER_BUILD` is enabled. Open the Standalone target first to test without a DAW.

## Controls

- **Attack** — envelope attack in seconds
- **Release** — envelope release in seconds
- **Gain** — output level

The instrument responds to MIDI notes and uses a warm sine-based oscillator with subtle harmonics.
