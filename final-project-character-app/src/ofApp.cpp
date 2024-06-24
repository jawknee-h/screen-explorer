#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
	ofSetVerticalSync(true);
	//ofSetFrameRate(120); // capping framerate

	// Establish a connection with the arduino through serial port COM7
	serial.setup("COM7", 115200);
}

//--------------------------------------------------------------
void ofApp::update(){

	// Getting delta-time for physics calculations
	dt = ofGetLastFrameTime();

	// ============= CHARACTER ================
	// Only update the character movement if this screen is currently active.
	if (active)
	{
		moveDir = movingRight - movingLeft;
		switch (moveDir)
		{
		case -1:
			character.walkLeft();
			break;
		case 1:
			character.walkRight();
		}
		//character.chase(of_get_mouse_pos());
		character.updatePhysics(dt);
	}

	// If the character has walked off the right side of the screen..
	if ((character.getPosition().x > ofGetWidth() + 70) && active)
	{
		std::cout << "swapping" << std::endl;
		serial.writeByte('b'); // send a msg to move to the oled display.
		active = false; // De-activate the graphics on this screen so the character no longer walks around
	}

	// Recieve serial commands from arduino
	int inByte = 0;
	inByte = serial.readByte();
	if (inByte == OF_SERIAL_NO_DATA)
	{
		// do nothing.
	}
	else if (inByte == OF_SERIAL_ERROR)
	{
		std::cout << "ERROR: serial error." << std::endl;
	}
	else
	{
		// Re-activate the character when 'r' is recieved
		if (inByte == 'r')
		{
			active = true;
			character.walkLeft();
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofClear(ofColor(0, 0, 0, 0)); // clearing for transparent window

	character.draw();

	ofDrawBitmapString(ofGetFrameRate(), 500, 500);

	
	// Drawing the glowing door
	ofPushStyle();
		ofSetLineWidth(2);
		ofEnableBlendMode(OF_BLENDMODE_SCREEN);
		int wobbleAmt = sin(ofGetElapsedTimef()*5) * 10;
		int numLines = 60 + wobbleAmt;
		for (int i = 0; i < numLines; i++)
		{
			int sat = ofMap(i, 0, numLines, 0, 100);
			int bright = ofMap(i, 0, numLines, 255, 0);
			int alpha = numLines - i;
			int xpos = ofGetWidth()-i*2;
			ofSetColor(ofColor::fromHsb(56, sat, bright, alpha));
			ofLine(xpos, ofGetHeight()-155 - i*0.3, xpos, ofGetHeight() - 10);
		}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key)
	{
		case OF_KEY_LEFT:
			if (leftIsDown) { break; }
			movingLeft = true; // moving on the desktop
			if (!active) serial.writeByte('a'); // Send a 'left' command to the arduino
			leftIsDown = true;
			break;
		case OF_KEY_RIGHT:
			if (rightIsDown) { break; }
			movingRight = true; // moving on the desktop
			if (!active) serial.writeByte('d'); // Send a 'right' command to the arduino
			rightIsDown = true;
			break;
		case 'w':
			//character.jump();
			character.jumpTowards(of_get_mouse_pos());
			break;
		case 'd':
			character.switch_draw_mode();
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key)
	{
		case OF_KEY_LEFT:
			leftIsDown = false;
			movingLeft = false;
			if (!active) serial.writeByte('j');
			break;
		case OF_KEY_RIGHT:
			rightIsDown = false;
			movingRight = false;
			if (!active) serial.writeByte('l');
			break;
		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}