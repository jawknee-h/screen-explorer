#include "ofMain.h"
#include "ofApp.h"
#include <dwmapi.h> // For transparent topmost window
#pragma comment (lib, "dwmapi.lib") //...


//========================================================================

/// makes the window always on top (may want to find a cross-platform solution in the future)
void makeWindowOnTop(HWND& wd)
{
    // making window always stay on top
    SetWindowPos(wd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    // making window transparent
    MARGINS margins;
    margins.cxLeftWidth = -1;
    SetWindowLong(wd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
    DwmExtendFrameIntoClientArea(wd, &margins);

    // making window click-through
    SetWindowLongPtr(wd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
}

int main( ){
	ofSetupOpenGL(600,400,OF_WINDOW);			// <-------- setup the GL context // Set the window size to 1920, 1080 if you want it transparent.

    HWND wnd = WindowFromDC(wglGetCurrentDC());
    //makeWindowOnTop(wnd); // uncomment this to make the window transparent and on-top

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());
}
