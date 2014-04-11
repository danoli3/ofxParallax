#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){	
    ofSetOrientation(OF_ORIENTATION_90_RIGHT);

    ofSetFrameRate(60);
    
    blurID = 0;
    
    worldSpeed = 60;
    
    currentImpulse = 0;
    currentImpulseEased = 0;
    
    isTravellingRight = true;
    
    parallax.setup();
//    if(i==0) {
//        theLayer->speed = speed; //5 * 360;
//        //theLayer->isBlurred = true;
//    } else if(i==1){
//        theLayer->speed = 5 * 130;
//        theLayer->isBlurred = true;
//    } else if(i==2) {
//        theLayer->speed = 5 * 80;
//    }
//    parallax.setupLayers(3);
    
    parallax.addNewLayer(0, ofVec2f(-200, 0), ofVec2f(1566, 768), 5 * 360, ofVec2f(-1566, +500));
    parallax.addNewLayer(1, ofVec2f(0, 0), ofVec2f(1236, 768), 5 * 130, ofVec2f(-(1236/2), +(1236/2)));
    parallax.addNewLayer(2, ofVec2f(0, 0), ofVec2f(1236, 768), 5 * 80, ofVec2f(-(1236/2), +(1236/2)));
    parallax.unblurAll();
    
    
    parallax.addImageToLayer(0, "images/3-background.jpg", ofPoint(-400, 0));
    parallax.addImageToLayer(1, "images/2-middle.png", ofPoint(0, 0));
    parallax.addImageToLayer(2, "images/1-foreground.png", ofPoint(0, 0));
}

//--------------------------------------------------------------
void ofApp::update(){
    
    float diff = 0;
    if(isTouched == true) {
        diff = (currentImpulse - currentImpulseEased) * 0.4;
    } else {
        diff = (0.0 - currentImpulseEased) * 0.1;
    }
    currentImpulseEased += diff;
    //cout << currentImpulseEased << endl;
    
    if(currentImpulseEased != 0.0f) {
        parallax.update(currentImpulseEased);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
	parallax.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){

}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    if(touch.id == 0) {
        isTouched = true;
        touchPoint.set(touch.x, touch.y);
        releasePoint.set(touch.x, touch.y);
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    if(touch.id == 0) {
        if(isTouched) {
            touchPoint = releasePoint;
            releasePoint.set(touch.x, touch.y);
            ofVec2f vel = releasePoint - touchPoint;
            
            currentImpulse = vel.x * 0.4;
        }
    }
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    if(touch.id == 0) {
        if(isTouched) {
            touchPoint = releasePoint;
            releasePoint.set(touch.x, touch.y);
            ofVec2f vel = releasePoint - touchPoint;
            
            currentImpulse = vel.x * 0.4;
        }
        
        isTouched = false;
    }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){

    parallax.unblurAll();
    blurID++;
    if(blurID > parallax.getTotalLayersSize()-1) {
        blurID = 0;
    }
    parallax.blurLayer(blurID);
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){

}

//--------------------------------------------------------------
void ofApp::gotFocus(){

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){

}
