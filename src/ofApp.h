#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void audioIn(float * input, int bufferSize, int nChannels);
    
    vector <float> left;
    vector <float> right;
    vector <float> volHistory;
    
    float smoothedVol;
    int drawCounter;
    int bufferCounter;
    bool soundOn;
    float currentVol;
    float scaledVol;
		
    ofImage image;
    ofMesh mesh;
    ofEasyCam easyCam;
    vector<ofVec3f> offsets;
    
    ofSoundStream sound;

};
