#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "MouthTracker.h"



//#define _USE_LIVE_VIDEO		// uncomment this to use a live camera
								// otherwise, we'll use a movie file
#include "ofxZenGarden.h"
#include "msaColor.h"

class Particle {
public:
	ofPoint pos;
	ofPoint vel;
	float start;
	float size;
	msaColor color;
	
	
	Particle(ofPoint _pos, float _size) { 
		if(img==NULL) {
			img = new ofImage();
			img->loadImage("bulb.png");
			img->setAnchorPercent(0.5,0.5);
		}
		size = 1 + 12.f*_size + ofRandom(0, 20);
		color.setHSV(ofRandom(50, 99), 1, 1);
		pos = _pos; start = pos.x; vel.y = -2; }
	void draw() {
		color.setGL();
		img->draw(pos.x, pos.y, size, size);
		glColor4f(1, 1, 1, 1);
	}
	void update() {
		color.a = pos.y/240.f;
		size += 1;
		vel.x = -2.5f + 5.f*ofNoise(0.02 + ofGetElapsedTimef()*0.2, 0.3 + start/0.1);
		pos += vel;
	}
	bool isAlive() {
		if(pos.y < -20) return false;
		return true;
	}
	static ofImage *img;
};



class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);

		void audioRequested 	(float * input, int bufferSize, int nChannels);

		  ofVideoGrabber 		vidGrabber;

		  ofVideoPlayer 		vidPlayer;
	bool useLiveVideo;

        ofxCvColorImage		colorImg;
	ofxCvGrayscaleImage bigGrayImage;
        ofxCvGrayscaleImage 	grayImage;
		ofxCvGrayscaleImage 	grayBg;
		ofxCvGrayscaleImage 	grayDiff;

        ofxCvContourFinder 	contourFinder;

		int 				threshold;
		bool				bLearnBakground;

	int WIDTH, HEIGHT;
	bool paused;
	bool rewind;
	bool play;
	MouthTracker mouths;
	ofxZenGarden zg;
	vector<Particle> particles;

};

#endif
