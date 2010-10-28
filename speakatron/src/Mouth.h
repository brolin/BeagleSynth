/*
 *  Mouth.h
 *  mouthsynth
 *
 *  Created by Marek Bereza on 04/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */
#include "ofMain.h"
#include "ofxCvGrayscaleImage.h"
#include "ofxCvContourFinder.h"
class Mouth: public ofRectangle {
public:
	Mouth(){justOpened = false;};
	void update(const ofRectangle &rect, ofxCvGrayscaleImage *_img);	
	void cropToMouth();	
	void draw();
	bool blobIsNotAtEdge(const ofRectangle &rect);
	
	ofxCvGrayscaleImage *img;
	static float widthCrop;
	static float heightStart;
	static float heightEnd;
	static ofxCvContourFinder *contourFinder;
	static int threshold;
	static int minSize;
	ofxCvGrayscaleImage mouthImg;
	ofRectangle openingRect;
	
	bool isOpen() { return state; }
	float getPitch() { return pitch; }
	float getFilter() { return filter; }
	float getPan() { return pan; }
	bool state;
	float filter;
	float pan;
	float pitch;
	bool lastState;
	bool justOpened;
	ofPoint getPos() {
		return pos;
	}
	ofPoint pos;
	bool mouthJustOpened() {
		return justOpened;
	}
};