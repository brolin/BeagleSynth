/*
 *  Mouth.cpp
 *  mouthsynth
 *
 *  Created by Marek Bereza on 04/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include "Mouth.h"

float Mouth::widthCrop = 0.1;
float Mouth::heightStart = 0.5;
float Mouth::heightEnd = 0.7;
int Mouth::threshold = 100;
int Mouth::minSize = 10;
ofxCvContourFinder *Mouth::contourFinder = NULL;

void Mouth::update(const ofRectangle &rect, ofxCvGrayscaleImage *_img) { 
	if(contourFinder==NULL) {
		contourFinder = new ofxCvContourFinder();
	}

	state = false;
	filter = 0;
	
	x = rect.x;
	y = rect.y;
	

	
	width = rect.width;
	height = rect.height;
	
	img = _img;
	float pitchInput = y+(height/2);
	pitch = ofMap(pitchInput, 0, 320, 0, 1);
	pitch = ofMap(pitch, 0.6, 0.1, 0, 1);
	pan = ofMap(x + width/2, 0, width, 0, 1);
	
	
	cropToMouth();
	
	pos = ofPoint(x + width/2, y + height/2);
	
	
	// copy pixels
	img->setROI(*this);
	mouthImg.clear();
	mouthImg.allocate(width, height);

	
	mouthImg.setFromPixels(img->getRoiPixels(), width, height);
	
	

	mouthImg.threshold(threshold);
	mouthImg.invert();
	contourFinder->findContours(mouthImg, minSize, (340*240)/3, 10, false);

	float leftMost = FLT_MAX;
	float rightMost = FLT_MIN;
	// find all the blobs, discounting any ones that touch the edges 
	// or are near the edges
	for(int i = 0; i < contourFinder->nBlobs; i++) {
		ofRectangle r = contourFinder->blobs[i].boundingRect;
		if(blobIsNotAtEdge(r)) {
			state = true;
			if(r.x<leftMost) leftMost = r.x;
			if(r.x+r.width>rightMost) rightMost = r.x + r.width;

		}
	}
	if(state==true) {
		filter = (rightMost-leftMost)/width;
	}
	
	// find filter and whether the state is on or not
	
	if(state&&!lastState) {
		justOpened = true;
	} else {
		justOpened = false;
	}
	
	img->resetROI();
	lastState = state;
	
}

bool Mouth::blobIsNotAtEdge(const ofRectangle &rect) {
	return true;
}

void Mouth::cropToMouth() {

	y += height*heightStart;
	height = height*(heightEnd - heightStart);
	
	float widthToCrop = width *widthCrop;
	x += widthToCrop;
	width -= widthToCrop*2;
}

void Mouth::draw() {
	if(!state) return;
	ofRect(x, y, width, height);
	ofFill();
	ofSetColor(0xFFFFFF);
	glPushMatrix();
	glTranslatef(x, y, 0);
	mouthImg.draw(0, 0);
	ofFill();
	ofSetColor(0xFF00FF);
	contourFinder->draw();
	glPopMatrix();
	ofNoFill();
}
