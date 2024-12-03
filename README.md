# Overview
OpenFrameworks addon to send MIDI clock to a MIDI port.  

Based on the app example:  
[TEST_MidiOutClock](https://github.com/moebiussurfing/TEST_MidiOutClock)  

![image](https://github.com/user-attachments/assets/e3d33578-6de9-48e6-942d-ee126b28faf1)

Requires [ofxMidi](https://github.com/danomatika/ofxMidi).  
Tested on **Windows 11** and **macOS M1** / **OF 0.12.0**.

The example requires **ofxGui**.  

Platform agnostic using C++ timers.  

## TODO
- Bug: MIDI slave app requires ~10 seconds to pair target BPM.
