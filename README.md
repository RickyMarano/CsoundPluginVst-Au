# CsoundPluginVst-Au
Csound plugin wrapper for DAW

A high-performance Csound-based plugin wrapper for real-time synthesis and effects processing. Built with the JUCE Framework, this tool seamlessly integrates the power of Csound into modern DAW workflows.

💻 System Requirements:
Operating System: 
Windows 10/11 or macOS (10.13+).

Csound Engine: 
Csound 6.18 (Installation is required for the plugin to function).

DAW:
Any host supporting VST3 or Audio Units (AU) formats.

🛠️ Installation & Setup:

1. Install Csound 6.18
The plugin relies on system-level Csound libraries. Download and install version 6.18 from the official channels:
Windows/macOS: Csound GitHub Releases (v6.18).

2. Automatic Directory Initialization
Upon the first launch in your DAW, the plugin automatically creates the following working directories in your Documents folder:
CsoundPluginSynth/ (for the instrument version)
CsoundPluginEffect/ (for the effect version)

🚀 Key Features

🎨 Integrated Editor & Scripting:

Syntax Highlighting: Real-time code coloring for Csound opcodes.
Smart Autocomplete: Press F1 to trigger context-aware suggestions while coding.
Dynamic Include Pathing: When using #include "...", the plugin automatically points to the CustomOpcode subfolder within the dedicated directory in your Documents folder.
CSD Export: Save your current script directly as a standalone .csd file.

🎛️ Automation & MIDI:

32 Automation Channels: Pre-configured parameters accessible via chnget "Par01" through chnget "Par32".
MIDI Learn: Native support to map any hardware MIDI controller to the 32 automation parameters for tactile control.

🔊 Manual Channel Configuration:

Users must manually define the nchnls variable in the Csound header to match the DAW track configuration:
nchnls = 1 (Mono)
nchnls = 2 (Stereo)
nchnls = 4 (Stereo + Sidechain)

📝 Code Example: Automation Mapping:

### Automation Example
Below is a basic script showing how to map "Par01" to a filter:

```csound
<CsoundSynthesizer>
<CsOptions>
-M0 -n -d
</CsOptions>
<CsInstruments>
sr = 44100
ksmps = 32
nchnls = 2 ; Must be set manually to match the DAW track
0dbfs = 1

instr 1
    ; "Par01" (0.0 to 1.0) scaled to Filter Cutoff (100Hz-5000Hz)
    kAuto chnget "Par01"
    kCut = 100 + (kAuto * 4900)
    
    aOut vco2 0.5, 440
    aFilt lpf18 aOut, kCut, 0.5, 0.1
    outs aFilt, aFilt
endin
</CsInstruments>
<CsScore>i 1 0 3600</CsScore>
</CsoundSynthesizer>
```
## 🔗 Credits & Resources
This plugin is powered by these amazing open-source projects and communities:

*   **[JUCE Framework](https://juce.com)**: The framework powering the plugin's UI and audio engine.
*   **[Csound Official](https://csound.com)**: The core audio programming language.
*   **[Cabbage Audio](https://cabbageaudio.com)**: Inspiration for Csound-based plugin development.
*   **[Csound VST3](https://github.com/gogins/csound-vst3)**: Michael Gogins' implementation of Csound as a VST3 plugin.
*   **[FLOSS Manuals](https://flossmanual.csound.com)**: The definitive guide for Csound documentation and learning.


