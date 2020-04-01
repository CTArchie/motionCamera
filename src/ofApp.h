#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxFaceTracker2.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();


		//create vector to get the current frame - it will hold
    vector<ofImage> img;
    // declare camera
    ofVideoGrabber cam;
    // int for frame and image counts
    int frameCount,imgcount,imgcountF;
    // face tracker
    ofxFaceTracker2 tracker;
};
