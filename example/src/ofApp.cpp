#include "ofApp.h"

void ofApp::setup() {
	ofSetVerticalSync(true);
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);

	midiOutClock.setup();

	channel = 1;
	note = 0;
	velocity = 0;

	gui.setup(midiOutClock.params);
	gui.setPosition(ofGetWidth() / 2 - gui.getWidth() / 2, ofGetHeight() / 2 - gui.getHeight() / 2);//center
}

void ofApp::update() {
	midiOutClock.update();
}

void ofApp::draw() {
	// We can acces the MIDI port object
	//midiOutClock.getMidiOutPointer()->getPort()

	ofPushStyle();
	ofSetColor(0);
	stringstream text;
	text << endl << endl
		<< "ofApp \nMIDI OUT" << endl << endl
		<< "CONNECTED PORT " << midiOutClock.getMidiOutPointer()->getPort() << endl
		<< "\"" << midiOutClock.getMidiOutPointer()->getName() << "\"" << endl << endl
		<< "TEST NOTES" << endl
		<< "MousePress x" << endl
		<< "Note:       " << note << endl
		<< "Velocity:   " << velocity;
	ofDrawBitmapString(text.str(), 20, 20);
	ofPopStyle();

	midiOutClock.draw();

	gui.draw();
}

void ofApp::keyPressed(int key) {
	midiOutClock.keyPressed(key);
}

void ofApp::mousePressed(int x, int y, int button) {
	// Trigs a random noteOn + noteOff
	int note_ = ofMap(x, 0, ofGetWidth(), 0, 127);
	int velocity_ = ofRandom(32, 128);

	// Send a Note On message
	midiOutClock.getMidiOutPointer()->sendNoteOn(channel, note_, velocity_);

	// Store the note and velocity values for future reference
	note = note_;
	velocity = velocity_;

	// Change the velocity to zero to send a Note Off message
	int velocityOff = 0;
	midiOutClock.getMidiOutPointer()->sendNoteOff(channel, note_, velocityOff);
}

void ofApp::mouseDragged(int x, int y, int button) {
	midiOutClock.mouseDragged(x, y, button);
}

void ofApp::exit() {
	midiOutClock.exit();
}
