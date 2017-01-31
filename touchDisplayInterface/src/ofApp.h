#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTrueTypeFontUC.h"

#define RPORT 8101
#define SPORT 8888
#define HOST "localhost"

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
    ofImage img, maskedImage;
    const float easeEnd = 1;
    float mapScale;
    float diameter = 5;
    ofColor col1 = ofColor(230, 125, 138, 220);
    ofColor col2 = ofColor(255, 255, 255, 255);
    
    ofTrueTypeFont fontAreaNum, fontArea, fontSelectCity;
    ofxTrueTypeFontUC fontSelectArea, fontBackButtonn, fontGoButton;
    ofVec3f area1 = ofVec3f(b12 / 2, ofGetHeight() - 150),
    area2 = ofVec3f((b23 + b12) / 2, ofGetHeight() - 150),
    area3 = ofVec3f((b34 + b23) / 2, ofGetHeight() - 150),
    area4 = ofVec3f((b45 + b34) / 2, ofGetHeight() - 150),
    area5 = ofVec3f((b56 + b45) / 2, ofGetHeight() - 150),
    area6 = ofVec3f((ofGetWidth() + b56) / 2, ofGetHeight() - 150);
    float numOffset = 100;
    
    void areaText();
    void cityText(int n);
    
    void backButton(int x, int y);
    bool backButtonPressed(int x, int y, int mx, int my);
    void goButton(int x, int y);
    bool goButtonPressed(int x, int y, int mx, int my);
    
    void detText(int n);
    
    ofPath path1;
    ofFbo maskFbo;
    void opCircle(int x);
    void flash(int area, int n);
    void debugScale();
    
    void annotation(int n);
    int circleAlpha;
    float circleDia;
    float timeStampDia;
    int frameStamp;
    float dia;
};
