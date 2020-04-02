#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    //set up camera and tracker - set all ints to 0;
    cam.setup(1280,720);
    frameCount = 0;
    imgcount = 0;
    imgcountF = 0;
    tracker.setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    cam.update();
    if(cam.isFrameNew()){
        //increment framecount
        frameCount++;
        //get half the framerate
        int FR = ofGetFrameRate()/2;
        //provided half the framerate is more than zero
        if(FR > 0){
            // in order to compare frames further apart to allow a chance for a difference - if the frame count is divisable by half the rate get the pixels from the camera and push them as an image to the image vector
            if(frameCount % FR == 0){
            ofPixels pix;
            pix = cam.getPixels();
            ofImage frame;
            frame.setFromPixels(pix);
            
            img.push_back(frame);
            
            
        }
            //to stop the framecount getting too big
            if(frameCount > FR){
                frameCount = 0;
            }
            
        }
        
    }
    //make sure there are only the 2 most recent images in the vector
    if(img.size() > 2){
        img.erase(img.begin());
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
   //once 2 images have been taken compare the pixels - if the values of the pixels in the new image are within 7 of that of the old image increase the truecount which counts the amount of pixels which this is true for as it is incremented in a loop through the number of pixels in one image - both are the same size and should have the same number of pixels so there is no need for a nested loop
    if(img.size() == 2){
        ofPixels & i1p = img[0].getPixels();
        ofPixels & i2p = img[1].getPixels();
        int trueCount = 0;
        for(int i = 0; i < i1p.size(); i++){
            
            if((i2p[i] < (i1p[i] + 7))&&(i2p[i] > (i1p[i] - 7))){
                trueCount++;
            }
        }
        // check if the number of true pixels is over 90% of the total number of pixels
        if(trueCount >= i1p.size()-(i1p.size()/10)){
            // image is considered the same
        }else{
            //if false it is a different image so something has changed - environmental brightness is a factor here - create an image of said frame and save it, then check it for faces. if there is a face crop and save an image of the face
            ofImage movement;
            movement.setFromPixels(img[1].getPixels());
            tracker.update(movement);
            string date = ofToString(ofGetDay()) + "-" + ofToString(ofGetMonth()) + "-" + ofToString(ofGetYear());
            string filename = "movement-"+ date +"/movement-" + ofToString(imgcount) + ".png";
            movement.save(filename);
            imgcount++;
            if(tracker.instances.size()>0){
                for(int j = 0; j < tracker.instances.size(); j++){
                    ofImage face;
                    face.cropFrom(movement, tracker.instances[j].getBoundingBox().x, tracker.instances[j].getBoundingBox().y, tracker.instances[j].getBoundingBox().width, tracker.instances[j].getBoundingBox().height);
                    face.save("faces-"+ date +"/face-" + ofToString(imgcountF) + ".png");
                    imgcountF++;
                }
               
            }
        }
    }
}

