#include "ofxMidiOutClock.h"

void ofxMidiOutClock::setupParams() {
	ofLogNotice("ofxMidiOutClock") << "SetupParams()";

	bPlay.set("Play", false);
	vReconnect.set("Reconnect");
	vResetBpm.set("Reset Bpm");
	bDebug.set("Debug", true);
	portNumber.set("Port #", -1, -1, -1);
	portName.set("Port Name", "none");
	bpm.set("Bpm", 120.0f, OFXMIDIOUTCLOCK_MIN_BPM, OFXMIDIOUTCLOCK_MAX_BPM);

	params.setName("ofxMidiOutClock");
	params.add(bPlay);
	params.add(vReconnect);
	params.add(portNumber);
	params.add(portName);
	params.add(bpm);
	params.add(vResetBpm);
	params.add(bDebug);

	bPlayListener = bPlay.newListener([this](bool& b) {
		ofLogNotice("ofxMidiOutClock: ") << "Play " << (b ? "TRUE" : "FALSE");
		if (b) this->play();
		else this->stop();
		});

	vReconnectListener = vReconnect.newListener([this](const void* sender) {
		ofParameter<void>* p = (ofParameter<void> *)sender;
		string name = p->getName();
		ofLogNotice("ofxMidiOutClock: ") << name;
		midiOut.closePort();
		refreshMidiPorts();
		midiOut.openPort(portNumber);
		});

	vResetBpmListener = vResetBpm.newListener([this](const void* sender) {
		ofParameter<void>* p = (ofParameter<void> *)sender;
		string name = p->getName();
		ofLogNotice("ofxMidiOutClock: ") << name;
		this->setBpm(120.f);
		});

	BpmListener = bpm.newListener([this](float& v) {
		ofLogNotice("ofxMidiOutClock: Bpm ") << v;
		clockThread.setBpm(bpm);
		});

	portNumberListener = portNumber.newListener([this](int& v) {
		ofLogNotice("ofxMidiOutClock: portNumber ") << v;
		portName = portNames[portNumber];
		midiOut.closePort();
		midiOut.openPort(portNumber);
		});
}

void ofxMidiOutClock::refreshMidiPorts() {
	ofLogNotice("ofxMidiOutClock") << "refreshMidiPorts()";

	midiOut.listOutPorts();
	portNames = midiOut.getOutPortList();

	const int portDefault = 1;

	if (portNames.size() > 0) {
		portNumber.setMin(0);
		portNumber.setMax(portNames.size() - 1);
		if (portNumber == -1) {
			ofLogNotice() << "Port number was settled as none";
			portNumber = portDefault;
		}
		portName = portNames[portNumber];
	}
}

void ofxMidiOutClock::setup() {
	ofLogNotice("ofxMidiOutClock") << "Setup()";

	setupParams();

	refreshMidiPorts();

	// Connect
	midiOut.openPort(portNumber);

	clockThread.setBpm(bpm);
	clockThread.setup(&midiOut);
}

void ofxMidiOutClock::update() {
	if (bpm != clockThread.getBpm()) bpm.set(clockThread.getBpm());
}

void ofxMidiOutClock::draw() {
	if (!bDebug) return;

	ofPushStyle();
	ofSetColor(0);
	stringstream text;
	text << endl << endl
		<< "ofxMidiOutClock" << endl << endl
		<< "MIDI OUT CLOCK" << endl << endl
		<< "CONNECTED PORT " << midiOut.getPort() << endl
		<< "\"" << midiOut.getName() << "\"" << endl << endl
		<< "KEYS" << endl
		<< "Space:      Start/Stop" << endl
		<< "Up/Down:    Tempo +/-" << endl
		<< "            MouseDrag y" << endl
		<< "BackSpace:  Reset" << endl << endl
		<< "CLOCK:      " << (this->isClockRunning() ? "ON" : "OFF") << endl
		<< "BPM:        " << ofToString(this->getBpm(), 2) << endl;

	ofDrawBitmapString(text.str(), 20, ofGetHeight() - 300);

	if (ofGetElapsedTimeMillis() % 2000 == 0)
		ofLogNotice("ofxMidiOutClock") << "MIDI Clock BPM: " << this->getBpm();
	ofPopStyle();
}

void ofxMidiOutClock::keyPressed(int key) {
	if (!bDebug) return;

	ofLogNotice("ofxMidiOutClock") << "keyPressed()" << char(key);
	switch (key) {
	case ' ': // Start/Stop MIDI clock
		toggleClock();
		break;

	case OF_KEY_UP: // Increase tempo
		setBpm(this->getBpm() + 1.0f);
		break;

	case OF_KEY_DOWN: // Decrease tempo
		setBpm(this->getBpm() - 1.0f);
		break;

	case OF_KEY_BACKSPACE: // Reset
		setBpm(120.0f);
		break;
	}
}

void ofxMidiOutClock::mouseDragged(int x, int y, int button) {
	if (!bDebug) return;

	ofLogNotice("ofxMidiOutClock") << "mouseDragged()";
	// Set bpm
	float v = ofMap(x, 0, ofGetHeight(), OFXMIDIOUTCLOCK_MAX_BPM, OFXMIDIOUTCLOCK_MIN_BPM);
	this->setBpm(v);
}

void ofxMidiOutClock::exit() {
	ofLogNotice("ofxMidiOutClock") << "exit()";
	this->stop();
	midiOut.closePort();
}

void ofxMidiOutClock::toggleClock() {
	//clockThread.toggleClock();
	bPlay = !bPlay.get();
}

void ofxMidiOutClock::play() {
	clockThread.play();
	if (!bPlay.get()) bPlay.setWithoutEventNotifications(true);

}

void ofxMidiOutClock::stop() {
	clockThread.stop();
	if (bPlay.get()) bPlay.setWithoutEventNotifications(false);
}

float ofxMidiOutClock::getBpm() {
	return clockThread.getBpm();
}

void ofxMidiOutClock::setBpm(float newBpm) {
	clockThread.setBpm(newBpm);
}

bool ofxMidiOutClock::isClockRunning() const {
	return clockThread.isClockRunning();
}