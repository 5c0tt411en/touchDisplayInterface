#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#define RPORT 8101
#define SPORT 8888
#define HOST "localhost"

using namespace std;

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    //OSC
    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    enum Status { ST_BLWAIT, ST_TOARANIM, ST_ARWAIT, ST_TOCIANIM, ST_CIWAIT, ST_TODEANIM, ST_DEWAIT, ST_FADEOUT, ST_TEST } ;
    enum Status stat ;
    float tick, timeStamp ;
    
    bool trigger;
    int value0;
    int area;
    
    float easeInOutQuadratic(float t, float b, float c, float d) ;
    float easeInOutCubic(float t, float b, float c, float d) ;
    float easeOutCubic(float t, float b, float c, float d) ;
    float easeLinear(float t, float b, float c, float d) ;
    int getIndex(int x, int y);
    
    ofVec3f point;
    int cityIndex;
    
    vector <string> splitString;
    const float b12 = ofGetWidth() / 6.5,
                b23 = b12 + ofGetWidth() / 6.5,
                b34 = b23 + ofGetWidth() / 4.4,
                b45 = b34 + ofGetWidth() / 6.8,
                b56 = b45 + ofGetWidth() / 5.4;
    
    ofCamera cam;
    ofVec3f pointToView;
    ofNode node;
    float offsetX, offsetY;
    
    ofFbo fbo;
    
    ofVec3f camPos;
    float tickEnd;
    ofImage img;
    const float easeEnd = 1;
    float mapScale;
    float diameter = 5;
    ofColor col1 = ofColor(240, 125, 138, 220);
    ofColor col2 = ofColor(255);
};
