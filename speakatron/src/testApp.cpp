#include "testApp.h"
#include "ofxSimpleGuiToo.h"
int preset = 0;
int lastPreset = 0;
ofImage *Particle::img = NULL;
bool showSite = false;
ofImage site;
//--------------------------------------------------------------
void testApp::setup(){
	site.loadImage("site.png");
	site.setAnchorPercent(0.5, 0.5);
	zg.load("patches/mouth1.pd", 0, 2, 44100, 256);
	
	
	ofSetVerticalSync(true);
	ofSetFrameRate(60.f);
	play = false;
	WIDTH = 320;
	HEIGHT = 240;
	useLiveVideo = false;
	int sourceWidth = 320;
	int sourceHeight = 240;

	vidGrabber.setVerbose(true);

	sourceWidth = WIDTH;
	sourceHeight = HEIGHT;

	vidGrabber.initGrabber(WIDTH, HEIGHT);

	
	/*vidPlayer.loadMovie("mouth0.mov");
	vidPlayer.play();
	vidPlayer.setLoopState(OF_LOOP_NORMAL);
	
	sourceWidth = vidPlayer.width;
	sourceHeight = vidPlayer.height;
*/

    colorImg.allocate(sourceWidth, sourceHeight);
	bigGrayImage.allocate(sourceWidth, sourceHeight);
		
	grayImage.allocate(WIDTH, HEIGHT);
	grayBg.allocate(WIDTH, HEIGHT);
	grayDiff.allocate(WIDTH, HEIGHT);
	mouths.setup();

	threshold = 80;
	gui.addButton("rewind", rewind);
	gui.addButton("pause", paused);
	gui.addButton("play", play);
	gui.addToggle("Use Camera", useLiveVideo);
	gui.addContent("Source Image", colorImg);
	gui.loadFromXML();
	gui.setAutoSave(true);
	ofSoundStreamSetup(2,0,this, 44100,256, 1);
}

//--------------------------------------------------------------
void testApp::update(){
	ofBackground(0,0,0);

    bool bNewFrame = false;

	if(useLiveVideo) {
       vidGrabber.grabFrame();
	   bNewFrame = vidGrabber.isFrameNew();
		
		if(rewind) {
			vidPlayer.setFrame(0);
			rewind = false;
		}
		
		if(paused) {
			vidPlayer.setPaused(paused);
			paused = false;
		}
		if(play) {
			vidPlayer.setPaused(false);
			vidPlayer.play();
			paused = false;
		}

		
		
	} else {
    //    vidPlayer.idleMovie();
     //   bNewFrame = vidPlayer.isFrameNew();
	}



	
	
	if (bNewFrame){

		if(useLiveVideo) {
            colorImg.setFromPixels(vidGrabber.getPixels(), vidGrabber.getWidth(), vidGrabber.getHeight());
	    } else {
      //      colorImg.setFromPixels(vidPlayer.getPixels(), vidPlayer.getWidth(), vidPlayer.getHeight());
        }

		bigGrayImage = colorImg;
		
        grayImage.scaleIntoMe(bigGrayImage);
		
		
		mouths.update(&grayImage);





		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
		contourFinder.findContours(grayDiff, 20, (340*240)/3, 10, true);	// find holes
		
		
		float pitch = 0;
		float filter = 0;
		for(int i = 0; i < mouths.size(); i++) {
			
			pitch = mouths[i]->getPitch();
			filter = mouths[i]->getFilter();
			
			zg.sendFloat("volume"+ofToString(i), mouths[i]->isOpen()?1:0);
			if(mouths[i]->isOpen()) {
				zg.sendFloat("pitch"+ofToString(i), pitch);
				zg.sendFloat("filter"+ofToString(i), filter);
				zg.sendFloat("pan"+ofToString(i), mouths[i]->getPan());
			}
		}
		for(int i = 0; i < mouths.size(); i++) {
			if(mouths[i]->isOpen() && ofRandom(0, 1)>0.5) { //mouthJustOpened()) {
				printf("Just opened\n");
				particles.push_back(Particle(mouths[i]->getPos(), mouths[i]->getFilter()));
			}
		}
		
		
	}
	
	for(int i = 0; i < particles.size(); i++) {
		if(particles[i].isAlive()) {
			particles[i].update();
		
		} else {
			particles.erase(particles.begin()+i);
			i--;
		}

	}

}

//--------------------------------------------------------------
void testApp::draw(){

	ofEnableAlphaBlending();
	if(showSite) {
		site.draw(ofGetWidth()/2, ofGetHeight()/2);
	} else {
		
	// draw the incoming, the grayscale, the bg and the thresholded difference
	ofSetColor(0xffffff);
	glPushMatrix();
	glScalef(ofGetWidth()/colorImg.getWidth(), ofGetHeight()/colorImg.getHeight(), 1);
	colorImg.draw(0, 0);
	for(int i = 0; i < particles.size(); i++) {
		particles[i].draw();
	}
	if(gui.isOn())	mouths.draw();
	glPopMatrix();
	
	
	
	
/*
	grayImage.draw(360,20);
	grayBg.draw(20,280);
	grayDiff.draw(360,280);

	// then draw the contours:

	ofFill();
	ofSetColor(0x333333);
	ofRect(360,540,320,240);
	ofSetColor(0xffffff);

	// we could draw the whole contour finder
	//contourFinder.draw(360,540);

	// or, instead we can draw each blob individually,
	// this is how to get access to them:
    for (int i = 0; i < contourFinder.nBlobs; i++){
        contourFinder.blobs[i].draw(360,540);
    }

	// finally, a report:

	ofSetColor(0xffffff);
	char reportStr[1024];
	sprintf(reportStr, "bg subtraction and blob detection\npress ' ' to capture bg\nthreshold %i (press: +/-)\nnum blobs found %i, fps: %f", threshold, contourFinder.nBlobs, ofGetFrameRate());
	ofDrawBitmapString(reportStr, 20, 600);
*/
	gui.draw();
	}
}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

	switch (key){
		case ' ':
			gui.toggleDraw();
			//bLearnBakground = true;
			break;
		case 'v':
			vidGrabber.videoSettings();

			threshold ++;
			if (threshold > 255) threshold = 255;
			break;
		case '-':
			threshold --;
			if (threshold < 0) threshold = 0;
			break;
		case 'f':
			ofToggleFullscreen();
			break;
		case 't':
			showSite ^= true;
			break;
	}
	if(key>='1' && key <= '4') {
		preset = key - '1' + 1;		
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

float stuff[256];

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	if(lastPreset!=preset) {
		string theFile = "patches/mouth";
		theFile += ofToString(preset);
		theFile += ".pd";
		zg.load(theFile, 0, 2, 44100, 256);

	}
	lastPreset = preset;
	


	zg.process(stuff, output, bufferSize);
	
	
}

