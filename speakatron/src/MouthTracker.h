/*
 *  MouthTracker.h
 *  mouthsynth
 *
 *  Created by Marek Bereza on 04/09/2010.
 *  Copyright 2010 Marek Bereza. All rights reserved.
 *
 */

#include <vector>
#include "ofxCvHaarFinder.h"
#include "ofxSimpleGuiToo.h"

#include "Mouth.h"


class MouthTracker {
public:
	MouthTracker() {
	}
	void setup() {
//		finder.setup("haarcascade_mcs_mouth.xml");
		finder.setup("haarcascade_frontalface_default.xml");
		gui.addTitle("Mouth Metrics");
		gui.addSlider("widthCrop", Mouth::widthCrop, 0, 1);
		gui.addSlider("heightStart", Mouth::heightStart, 0, 1);
		gui.addSlider("heightEnd", Mouth::heightEnd, 0, 1);
		gui.addSlider("threshold", Mouth::threshold, 0, 255);
		gui.addSlider("minSize", Mouth::minSize, 1, 100);
		for(int i = 0; i < 4; i++) {
			mouths.push_back(new Mouth());
		}
	}
	
	Mouth *operator[](const int &index) {
		return mouths[index];
	}
	int size() {
		return mouths.size();
	}
	void update(ofxCvGrayscaleImage *img) {

		
		finder.findHaarObjects(*img, 20, 20);

		for(int i = 0; i < 4; i++) {
			if(i < finder.blobs.size()) {
				ofRectangle cur = finder.blobs[i].boundingRect;
				mouths[i]->update(cur, img);
			} else {
				mouths[i]->state = false;
			}
		}
	}
	
	void draw() {

		ofNoFill();
		ofSetColor(0, 255, 0);
		for(int i = 0; i < finder.blobs.size(); i++) {
			ofRectangle cur = finder.blobs[i].boundingRect;
			ofRect(cur.x, cur.y, cur.width, cur.height);
		}
		
		for(int i = 0; i < mouths.size(); i++) {
			ofSetColor(255, 0, 0);
			mouths[i]->draw();
		}
	}
	
private:
	vector<Mouth*> mouths;
	ofxCvHaarFinder finder;
};