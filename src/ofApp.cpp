#include "ofApp.h"

using namespace ofxCv;
using namespace cv;

//--------------------------------------------------------------
void ofApp::setup(){
  ofSetBackgroundColor(0); // make the background black
  ofSetBackgroundAuto(false); //don't redraw the background every frame
  
  h1r.loadMovie("h1r.mov");
  h1r.play();
  
  h2r.loadMovie("h2r.mov");
  h2r.play();
  
  h1l.loadMovie("h1l.mov");
  h1l.play();
  
  h2l.loadMovie("h2l.mov");
  h2l.play();
  
  debugMode = false;
  
  ofSetLogLevel(OF_LOG_VERBOSE);
  
  kinect.setRegistration(true);
  kinect.init();
  kinect.open();
  
  kw = kinect.width;
  kh = kinect.height;
  
  colorImg.allocate(kw, kh);
  grayImg.allocate(kw, kh);
  grayThreshNear.allocate(kw, kh);
  grayThreshFar.allocate(kw, kh);
  
  gui.setup();
  gui.setPosition(0,kh);
  gui.add(nearThresh.set("near", NEAR_THRESH, 0, 255));
  gui.add(farThresh.set("far", FAR_THRESH, 0, 255));
  gui.add(angle.set("angle", ANGLE, -30, 30));
  gui.add(minRadius.set("min-rad", MIN_RADIUS, 1, 300));
  gui.add(maxRadius.set("max-rad", MAX_RADIUS, 300, 10000));
  gui.add(contThresh.set("contour-thresh", CONT_THRESH, 10, 254));
  gui.add(persistence.set("persistence", PERSISTENCE, 2, 30));
  gui.add(maxMove.set("max-move", MAX_MOVE, 10, 80));
  
  handRatio = 640/360;
  
}


//--------------------------------------------------------------
void ofApp::update(){
  h1r.update();
  h2r.update();
  h1l.update();
  h2l.update();
  ww = ofGetWidth();
  wh = ofGetHeight();
  kinect.setCameraTiltAngle(angle);
  kinect.update();
  contourFinder.setMinAreaRadius(minRadius);
  contourFinder.setMaxAreaRadius(maxRadius);
  contourFinder.setThreshold(contThresh);
  contourFinder.getTracker().setPersistence(persistence);
  contourFinder.getTracker().setMaximumDistance(maxMove);
  
  if(kinect.isFrameNew()) {
    grayImg.setFromPixels(kinect.getDepthPixels());
    grayThreshNear = grayImg;
    grayThreshFar = grayImg;
    grayThreshNear.threshold(nearThresh, true);
    grayThreshFar.threshold(farThresh);
    cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImg.getCvImage(), NULL);
    grayImg.flagImageChanged();
    grayImg.dilate();
    grayImg.blurGaussian();
    contourFinder.findContours(grayImg);
    
    RectTracker& tracker = contourFinder.getTracker();

    for(int i=0; i<contourFinder.size(); i++) {
      int label = contourFinder.getLabel(i);
      
      map<int,vector<HandBox>>::iterator it = manyHands.find(label);
      if(it != manyHands.end()) {
        vector<HandBox> hs;
        HandBox h1;
        h1.setup();
        h1.type=TOP_LEFT;
        
        HandBox h2;
        h2.setup();
        h2.type=TOP_RIGHT;
        
        HandBox h3;
        h3.setup();
        h3.type=BOTTOM_LEFT;
        
        HandBox h4;
        h4.setup();
        h4.type=BOTTOM_RIGHT;
        hs.push_back(h1);
        hs.push_back(h2);
        hs.push_back(h3);
        hs.push_back(h4);
        manyHands[label] = hs;
      }

      ofRectangle b = toOf(tracker.getSmoothed(label));
      float d = kinect.getDistanceAt(b.getCenter());
      
      ofPixels &pix = grayImg.getPixels();
      float handW = ofMap(d, MIN_DEPTH, MAX_DEPTH, MAX_WIDTH, MIN_WIDTH, true);
      float handH = handW/handRatio;
      for(int a=0;a<manyHands[label].size();a++){
        manyHands[label][a].update(pix, b, kw, handW, handH);
      }

    }
    
    vector<unsigned int> dead = tracker.getDeadLabels();
    for(int a=0; a<dead.size(); a++) {
      manyHands.erase(dead[a]);
    }

  }
  

}

//--------------------------------------------------------------
void ofApp::draw(){
  ofSetColor(255);
  
  
  
  if(debugMode) {
    /* DEBUG MODE DRAW */
    ofShowCursor();
    gui.draw();
    kinect.draw(kw,0,kw,kh);
    grayImg.draw(0,0,kw,kh);
    contourFinder.draw();
    RectTracker& tracker = contourFinder.getTracker();
    ofSetColor(255, 0, 255);
    for(int i = 0; i < contourFinder.size(); i++) {
//      ofRectangle handBoundingBox = hands[contourFinder.getLabel(i)].boundingBox;
//      ofDrawRectangle(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
//      h1r.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());

      vector<HandBox> & hs = manyHands[contourFinder.getLabel(i)];
      for(int a=0;a<hs.size();a++) {
        //HandBox h = hs[a];
        ofRectangle handBoundingBox = hs[a].boundingBox;
        if(hs[a].type == TOP_LEFT) {
          h1l.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        } else if(hs[a].type == TOP_RIGHT) {
          h2r.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        } else if(hs[a].type == BOTTOM_LEFT) {
          h2l.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        } else if(hs[a].type == BOTTOM_RIGHT) {
          h1r.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        }
      }
      
      int label = contourFinder.getLabel(i);
      float smoothX = tracker.getSmoothed(label).x + tracker.getSmoothed(label).width*.5;
      float smoothY = tracker.getSmoothed(label).y + tracker.getSmoothed(label).height*.5;
      ofDrawCircle(smoothX,smoothY,4);
      
      ofPushMatrix();
      ofTranslate(contourFinder.getCenter(i).x, contourFinder.getCenter(i).y);
      string msg = ofToString(label) + ":" + ofToString(tracker.getAge(label));
      ofDrawBitmapString(msg, 0, 0);
      ofPopMatrix();
      
    }
  }
  
  
  else {
    /* PRESENTATION MODE DRAW */
    ofPushMatrix();
    ofScale(-1, 1);
    ofTranslate(-ww, 0);
    ofHideCursor();
    //kinect.draw(0,0, ww,wh);
    ofPushMatrix();
     ofScale(ofGetHeight()/kinect.getHeight(),ofGetHeight()/kinect.getHeight());
    kinect.draw(0,0);
   // ofScale(ofGetWidth()/kinect.getWidth(),ofGetHeight()/kinect.getHeight());
    for(int i = 0; i < contourFinder.size(); i++) {
//      ofRectangle handBoundingBox = hands[contourFinder.getLabel(i)].boundingBox;
//      h1r.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
      
      vector<HandBox> & hs = manyHands[contourFinder.getLabel(i)];
      for(int a=0;a<hs.size();a++) {
        //HandBox h = hs[a];
        ofRectangle handBoundingBox = hs[a].boundingBox;
        if(hs[a].type == TOP_LEFT) {
          h1l.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        } else if(hs[a].type == TOP_RIGHT) {
          h2r.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        } else if(hs[a].type == BOTTOM_LEFT) {
          h2l.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        } else if(hs[a].type == BOTTOM_RIGHT) {
          h1r.draw(handBoundingBox.getMinX(), handBoundingBox.getMinY(), handBoundingBox.getWidth(), handBoundingBox.getHeight());
        }
      }

      
    }
    ofPopMatrix();
    ofPopMatrix();
    ofSetFullscreen(true);
    
  }
  
//  h1r.draw(0,0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
  if(key == 'd') {
    debugMode = !debugMode;
  }
}

//--------------------------------------------------------------
void ofApp::exit() {
  kinect.close();
}

void ofApp::keyReleased(int key){
  
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
