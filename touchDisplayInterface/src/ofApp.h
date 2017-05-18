#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxTrueTypeFontUC.h"

#define W 1920
#define H 1080

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
    
    float easeInOutQuadratic(float t, float b, float c, float d) ;
    float easeInOutCubic(float t, float b, float c, float d) ;
    float easeOutCubic(float t, float b, float c, float d) ;
    float easeLinear(float t, float b, float c, float d) ;
    int getIndex(int x, int y);
    int getIndexft(int x, int y, int f, int t);
    int getIndexftex(int x, int y, int f, int t, int ex);
    int getIndexp(int x, int y, int p);
    void areaText();
    void cityText();
    void backButton(int x, int y);
    bool backButtonPressed(int x, int y, int mx, int my);
    void goButton(int x, int y);
    bool goButtonPressed(int x, int y, int mx, int my);
    void detText(int n);
    void opCircle(int x);
    void flash(int area, int n);
    void annotation(int n, int alphaFrom, int alphaTo, float d);
    void cityPointDraw(int f, int t, int alphaFrom, int alphaTo, float d);
    void cityPointDraw(int f, int t, int ex, int alphaFrom, int alphaTo, float d);
    void cityPointDraw(int p, int alphaFrom, int alphaTo, float d);

    ofxOscReceiver receiver;
    ofxOscSender sender;
    
    enum Status { ST_BLWAIT, ST_TOARANIM, ST_ARWAIT, ST_TOCIANIM, ST_TOCIANIM_BK, ST_CIWAIT, ST_TODEANIM, ST_DEWAIT, ST_FADEOUT, ST_TEST };
    enum Status stat;
    
    float tick, timeStamp, mapScale, tickEnd, circleDia, timeStampDia, dia, imageScale;
    
    bool trigger, debug = true;
    
    int value0, area, cityIndex, circleAlpha, frameStamp, cityFrom, cityTo, localTime, alphaFrom, alphaTo, d;

    ofPath path1;
    
    vector <string> splitString;
    const float b12 = W / 6.5,
    b23 = b12 + W / 6.5,
    b34 = b23 + W / 4.4,
    b45 = b34 + W / 6.8,
    b56 = b45 + W / 5.4,
    easeEnd = 1,
    diameter = 5,
    numOffset = 100,
    scAdjust = 2.37756;
    
    ofCamera cam;
    ofVec3f pointToView, camPos, camPosBK;
    ofNode node;
    
    ofFbo fbo, maskFbo;
        
    ofImage img, maskedImage, backButtonUI, goButtonUI;
    ofImage cityImage[44];
    ofColor col1 = ofColor(230, 125, 138, 220);
    ofColor col2 = ofColor(255, 255, 255, 255);
    
    ofTrueTypeFont fontAreaNum, fontArea, fontSelectCity, fontCity;
    ofxTrueTypeFontUC fontSelectArea, fontDet;
    const ofVec3f
    area1 = ofVec3f(b12 / 2, H - 150),
    area2 = ofVec3f((b23 + b12) / 2, H - 150),
    area3 = ofVec3f((b34 + b23) / 2, H - 150),
    area4 = ofVec3f((b45 + b34) / 2, H - 150),
    area5 = ofVec3f((b56 + b45) / 2, H - 150),
    area6 = ofVec3f((W + b56) / 2, H - 150),
    anim1 = ofVec3f(- W / 2 + b12 - 50, - 300, -600),
    anim2 = ofVec3f(- W / 2 + b23 - 140, - 270, -600),
    anim3 = ofVec3f(- W / 2 + b34 - 100, - 20, -520),
    anim4 = ofVec3f(- W / 2 + b45 - 100, -70, -300),
    anim5 = ofVec3f(- W / 2 + b56 - 120, - 240, -520),
    anim6 = ofVec3f(- W / 3 + b56 - 260, - 100, -450)
    ;
    
    ofVec3f camDefault = ofVec3f(W / 2, H / 2, 937);
    ofVec2f imageSize = ofVec2f(129, 86);
};
