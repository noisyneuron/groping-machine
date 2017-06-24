//
//  HandBox.h
//  gropeMachine
//
//  Created by Sukanya Aneja on 28/06/16.
//
//

#ifndef HandBox_h
#define HandBox_h


#include "ofMain.h"

enum HandType {
  BOTTOM_RIGHT,
  TOP_RIGHT,
  TOP_LEFT,

  BOTTOM_LEFT
};

class HandBox {
public:
  void setup();
  void update(ofPixels& pix, ofRectangle blobBox, int totalWidth, float handW, float handH);
  
  ofRectangle boundingBox;
  HandType type;
  HandType hType;
};




#endif /* HandBox_h */
