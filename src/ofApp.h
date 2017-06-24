#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "HandBox.h"

#define NEAR_THRESH 188
#define FAR_THRESH 122
#define ANGLE 6
#define MIN_RADIUS 72
#define MAX_RADIUS 8000
#define CONT_THRESH 80
#define PERSISTENCE 15
#define MAX_MOVE 32

#define MIN_DEPTH 572
#define MAX_DEPTH 2063
#define MIN_WIDTH 55
#define MAX_WIDTH 170

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    void exit();

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
  
  ofxKinect kinect;
  ofxCvColorImage colorImg;
  ofxCvGrayscaleImage grayImg;
  ofxCvGrayscaleImage grayThreshNear;
  ofxCvGrayscaleImage grayThreshFar;
  ofxCv::ContourFinder contourFinder;
  int ww;
  int wh;
  int kw;
  int kh;
  float handRatio;
  
  std::map<int, HandBox> hands;
  
  std::map<int, vector<HandBox>> manyHands;
  
  bool debugMode;
  
  ofxPanel gui;
  
  ofParameter<int> nearThresh;
  ofParameter<int> farThresh;
  ofParameter<int> angle;
  ofParameter<int> minRadius;
  ofParameter<int> maxRadius;
  ofParameter<int> contThresh;
  ofParameter<int> persistence;
  ofParameter<int> maxMove;
  
//  Hand h1;
  ofxCvContourFinder cfDebug;
  
  ofVideoPlayer h1r;
  ofVideoPlayer h1l;
  ofVideoPlayer h2r;
  ofVideoPlayer h2l;

  

};
