#pragma once

#include "ofMain.h"
#include "Character.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		float dt;
		Character character{ ofGetHeight()  }; // floor y for fullscreen mode = 'ofGetHeight() - 85.0f'
		bool movingRight = false;
		bool movingLeft = false;
		int moveDir = 0;

		// For communicating with the Arduino
		ofSerial serial;

		// Screen state
		bool active = true; // whether this screen is currently active.

		// Button states
		bool leftIsDown = false;
		bool rightIsDown = false;

		// Character logic
		float doNothingTime = 0;
		bool armsRaised = false;
		int armRaisesToDo = 0;
		float raisedTime = 0.0;
};
