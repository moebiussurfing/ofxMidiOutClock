#pragma once

#include "ofMain.h"
#include "ofxMidiOutClock.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void mouseDragged(int x, int y, int button);

	// Test MIDI notes using internal MIDI out port
	int channel;
	int note, velocity;

	ofxPanel gui;

private:
	ofxMidiOutClock midiOutClock;
};
