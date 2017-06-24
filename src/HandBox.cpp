//
//  HandBox.cpp
//  gropeMachine
//
//  Created by Sukanya Aneja on 28/06/16.
//
//

#include "HandBox.h"

void HandBox::setup() {
  boundingBox = ofRectangle(0,0,0,0);
  type = TOP_RIGHT;
}


void HandBox::update(ofPixels& pix, ofRectangle blobBox, int totalWidth, float handW, float handH) {
  
  float blobW = blobBox.getWidth();
  float blobH = blobBox.getHeight();
  ofPoint blobC = blobBox.getCenter();
  int blobMinX = blobBox.getMinX();
  int blobMaxX = blobBox.getMaxX();
  int blobMinY = blobBox.getMinY();
  int blobMaxY = blobBox.getMaxY();
  cout << "type " << type << endl;
  
  
   if(blobW>=handW && blobH>=handH) {
    if(type==TOP_RIGHT) {
      int startY = floor(blobC.y - 1.2*handH);
      int endY = floor(startY + handH);
      if(endY != startY && endY <= blobMaxY && startY > blobMinY) {
        ofLog(OF_LOG_NOTICE, "here- top right");
        int edgeSummed = 0;
        int count = 0;
        for(int y=startY; y<endY; y++) {
          for(int x=blobC.x; x>blobMinX; x--) {
            if(pix[y*totalWidth+x]==0) {
              edgeSummed += x;
              count++;
              break;
            }
          }
        }
        if(count < (endY-startY)) {
          edgeSummed = 0; count = 0;
          for(int y=startY; y<endY; y++) {
            for(int x=blobMinX; x<=blobMaxX; x++) {
              if(pix[y*totalWidth+x]==255) {
                edgeSummed += x;
                count++;
                break;
              }
            }
          }
        }
        int avrgEdge = floor(edgeSummed/(endY-startY));
        int handStartX = avrgEdge;// - handW;
        if(handStartX >  blobMinX) {

//          boundingBox.setPosition(0.3*boundingBox.getPosition().x + 0.7*handStartX, 0.3*boundingBox.getPosition().y + 0.7*startY);
//          boundingBox.setWidth(0.3*boundingBox.getWidth() + 0.7*handW);
//          boundingBox.setHeight(0.3*boundingBox.getHeight() + 0.7*handH);

          
          boundingBox.setPosition(handStartX, startY);
          boundingBox.setWidth(handW);
          boundingBox.setHeight(handH);
        }
      }
     
    }
    
    else if(type==TOP_LEFT) {
      int startY = floor(blobC.y - 0.8*handH);
      int endY = floor(startY + handH);
      if(endY != startY && endY <= blobMaxY && startY > blobMinY) {
        ofLog(OF_LOG_NOTICE, "here- top left");
        int edgeSummed = 0;
        int count = 0;
        for(int y=startY; y<endY; y++) {
          for(int x=blobC.x; x<blobMaxX; x++) {
            if(pix[y*totalWidth+x]==0) {
              edgeSummed += x;
              count++;
              break;
            }
          }
        }
        if(count < (endY-startY)) {
          edgeSummed = 0; count = 0;
          for(int y=startY; y<endY; y++) {
            for(int x=blobMaxX; x>=blobMinX; x--) {
              if(pix[y*totalWidth+x]==255) {
                edgeSummed += x;
                count++;
                break;
              }
            }
          }
        }
        int avrgEdge = floor(edgeSummed/(endY-startY));
        int handStartX = avrgEdge - handW;
        if(handStartX >  blobMinX) {
          boundingBox.setPosition(handStartX, startY);
          boundingBox.setWidth(handW);
          boundingBox.setHeight(handH);
        }
      }
    }
    
    else if(type==BOTTOM_LEFT) {

      int startY = floor(blobC.y+0.9*handH);
      int endY = floor(startY + handH);
      if(endY != startY && endY <= blobMaxY) {
        ofLog(OF_LOG_NOTICE, "here- bottom left");
        int edgeSummed = 0;
        int count = 0;
//        for(int y=startY; y<endY; y++) {
//          for(int x=blobC.x; x<blobMaxX; x++) {
//            if(pix[y*totalWidth+x]==0) {
//              edgeSummed += x;
//              count++;
//              break;
//            }
//          }
//        }
//        if(count < (endY-startY)) {
//          edgeSummed = 0; count = 0;
          for(int y=startY; y<endY; y++) {
            for(int x=blobMaxX; x>=blobMinX; x--) {
              if(pix[y*totalWidth+x]==255) {
                edgeSummed += x;
                count++;
                break;
              }
            }
          }
//        }
        int avrgEdge = floor(edgeSummed/(endY-startY));
        int handStartX = avrgEdge - handW;
        if(handStartX >  blobMinX) {
          boundingBox.setPosition(handStartX, startY);
          boundingBox.setWidth(handW);
          boundingBox.setHeight(handH);
        }
      }
    }
    
    else if(type==BOTTOM_RIGHT) {
      int startY = floor(blobC.y+0.5*handH);
      int endY = floor(startY + handH);
      if(endY != startY && endY <= blobMaxY) {
        ofLog(OF_LOG_NOTICE, "here- bottom right");
        int edgeSummed = 0;
        int count = 0;
//        for(int y=startY; y<endY; y++) {
//          for(int x=blobC.x; x>blobMinX; x--) {
//            if(pix[y*totalWidth+x]==0) {
//              edgeSummed += x;
//              count++;
//              break;
//            }
//          }
//        }
//        if(count < (endY-startY)) {
//          edgeSummed = 0; count = 0;
          for(int y=startY; y<endY; y++) {
            for(int x=blobMinX; x<=blobMaxX; x++) {
              if(pix[y*totalWidth+x]==255) {
                edgeSummed += x;
                count++;
                break;
              }
            }
          }
//        }
        int avrgEdge = floor(edgeSummed/(endY-startY));
        int handStartX = avrgEdge;
        if(handStartX >  blobMinX) {
          boundingBox.setPosition(handStartX, startY);
          boundingBox.setWidth(handW);
          boundingBox.setHeight(handH);
        }
      }

    }

  }
  
}