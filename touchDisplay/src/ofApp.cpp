#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    value0 = 10;
//    ofSetWindowPosition(2000, 0);
//    ofSetFullscreen(TRUE);
    ofBackground(0);
    
    ofBuffer buffer = ofBufferFromFile("list.csv");
    string sp = buffer.getText();
    ofStringReplace(sp, "\r", ",");
    splitString = ofSplitString(sp, ",", true, true);
    
    cam.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, -694);
    
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    img.load("equirectangular.png");
    
    receiver.setup(RPORT);
    sender.setup(HOST, SPORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    //OSC
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage message;
        receiver.getNextMessage(&message);
        value0 = message.getArgAsInt32(0);
    }
    
    pointToView.x = camPos.x;
    pointToView.y = camPos.y;
    pointToView.z = 1000;
    
    node.setPosition(pointToView);
//    node.setOrientation(ofVec3f(0, 0, 1000));
//    node.lookAt(ofVec3f(0, 0, 1));
    
    switch(stat) {
        case ST_BLWAIT:
            if (value0 == 0 || trigger) {
                timeStamp = ofGetElapsedTimef();
                trigger = false;
                stat = ST_TOARANIM;
            }
            value0 = 10;
            break ;
            
        case ST_TOARANIM:
            if (tick > 3) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_ARWAIT;
            }
            break;
            
        case ST_ARWAIT:
            if (trigger) {
                if (mouseX >= 0 && mouseX < b12) area = 1;
                else if (mouseX >= b12 && mouseX < b23) area = 2;
                else if (mouseX >= b23 && mouseX < b34) area = 3;
                else if (mouseX >= b34 && mouseX < b45) area = 4;
                else if (mouseX >= b45 && mouseX < b56) area = 5;
                else if (mouseX >= b56 && mouseX < ofGetWidth()) area = 6;
                timeStamp = ofGetElapsedTimef();
                stat = ST_TOCIANIM;
                trigger = false;
            }
            break ;
            
        case ST_TOCIANIM:
            switch (area) {
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                default:
                    break;
            }
            if (tick > easeEnd) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_CIWAIT;
            }
            break;
            
        case ST_CIWAIT:
            switch (area) {
                case 1:
                    mapScale = 3.59756;
                    offsetX = 18.917;
                    offsetY = 82;
                    break;
                case 2:
                    mapScale = 2.83555556;
                    offsetX = 129.92946694;
                    offsetY = 62.31974915;
                    break;
                case 3:
                    mapScale = 2.19810508;
                    offsetX = 355.69161456;
                    offsetY = 190.73079964;
                    break;
                case 4:
                    mapScale = 2.07061688;
                    offsetX = 525.71462248;
                    offsetY = 257.82046293;
                    break;
                case 5:
                    mapScale = 2.83444444;
                    offsetX = 864.23206681;
                    offsetY = 58.90552731;
                    break;
                case 6:
                    mapScale = 1.75688705;
                    offsetX = 705.917;
                    offsetY = 72.84829585;
                    break;
                default:
                    break;
            }
            if (trigger) {
                cityIndex = getIndex(mouseX, mouseY);
                timeStamp = ofGetElapsedTimef();
                stat = ST_TODEANIM;
                trigger = false;
            }
            break;
            
        case ST_TODEANIM:
            if (tick > 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_DEWAIT;
            }
            break;
            
        case ST_DEWAIT:
            if (trigger) {
                ofxOscMessage m;
                m.setAddress("");
                m.addIntArg(cityIndex);
                sender.sendMessage(m);
                timeStamp = ofGetElapsedTimef();
                stat = ST_FADEOUT;
                trigger = false;
            }
            break;
            
        case ST_FADEOUT:
            if (value0 == 1 || trigger) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_TOARANIM;
                trigger = false;
            }
            else if (value0 == 2 || trigger) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
                trigger = false;
            }
            value0 = 10;
            if (tick > 3) {
                camPos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, -694);
                col1 = ofColor(240, 125, 138, 220);
                col2 = ofColor(255);
            }
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    tick = ofGetElapsedTimef() - timeStamp ;
    
    fbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(col2);
    
    cam.lookAt(node.getPosition());
    cam.begin();
    
    cam.setPosition(camPos);
//    ofDrawGrid(100, 20, true, true, true);
    
    
    switch(stat) {
        case ST_BLWAIT:
            camPos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, -694);
//            ofDrawBitmapStringHighlight("blank waiting OSC", ofGetWidth() / 2 - 50, 20);
            break ;
            
        case ST_TOARANIM:
//            ofDrawBitmapStringHighlight("animation time :" + ofToString(tick), ofGetWidth() / 2 - 50, ofGetHeight() / 2);
            ofSetColor(col2, 85 * tick);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            ofNoFill();
            ofDrawRectangle(0, 0, b12, ofGetHeight());
            ofDrawRectangle(b12, 0, b23 - b12, ofGetHeight());
            ofDrawRectangle(b23, 0, b34 - b23, ofGetHeight());
            ofDrawRectangle(b34, 0, b45 - b34, ofGetHeight());
            ofDrawRectangle(b45, 0, b56 - b45, ofGetHeight());
            ofDrawRectangle(b56, 0, ofGetWidth() - b56, ofGetHeight());
            ofFill();
            for (int i = 0; i < int(splitString.size() / 4); i++) {
                ofSetColor(col1, 85 * tick);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
            }

            break;
            
        case ST_ARWAIT:
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            ofSetColor(col2);
            ofNoFill();
            ofDrawRectangle(0, 0, b12, ofGetHeight());
            ofDrawRectangle(b12, 0, b23 - b12, ofGetHeight());
            ofDrawRectangle(b23, 0, b34 - b23, ofGetHeight());
            ofDrawRectangle(b34, 0, b45 - b34, ofGetHeight());
            ofDrawRectangle(b45, 0, b56 - b45, ofGetHeight());
            ofDrawRectangle(b56, 0, ofGetWidth() - b56, ofGetHeight());
            ofFill();
            for (int i = 0; i < int(splitString.size() / 4); i++) {
                ofSetColor(col1);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
            }
//            ofDrawBitmapStringHighlight("select area", ofGetWidth() / 2 - 50, 20);
            break ;
            
        case ST_TOCIANIM:
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            ofSetColor(col2);
            ofNoFill();
            ofDrawRectangle(0, 0, b12, ofGetHeight());
            ofDrawRectangle(b12, 0, b23 - b12, ofGetHeight());
            ofDrawRectangle(b23, 0, b34 - b23, ofGetHeight());
            ofDrawRectangle(b34, 0, b45 - b34, ofGetHeight());
            ofDrawRectangle(b45, 0, b56 - b45, ofGetHeight());
            ofDrawRectangle(b56, 0, ofGetWidth() - b56, ofGetHeight());
            ofFill();
            for (int i = 0; i < int(splitString.size() / 4); i++) {
                ofSetColor(col1);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
            }
            switch (area) {
                case 1:
                    camPos.x = easeInOutCubic(tick, ofGetWidth() / 2, - ofGetWidth() / 2 + b12, easeEnd);
                    camPos.y = easeInOutCubic(tick, ofGetHeight() / 2, - 200, easeEnd);
                    camPos.z = easeInOutCubic(tick, -694, 500, easeEnd);
//                    ofDrawBitmapStringHighlight("switching to area " + ofToString(area) + " : "
//+ ofToString(tick), ofGetWidth() / 2 - 50, 20);
                    break;
                case 2:
                    camPos.x = easeInOutCubic(tick, ofGetWidth() / 2, - ofGetWidth() / 2 + b23 - 40, easeEnd);
                    camPos.y = easeInOutCubic(tick, ofGetHeight() / 2, - 200, easeEnd);
                    camPos.z = easeInOutCubic(tick, -694, 450, easeEnd);
//                    ofDrawBitmapStringHighlight("switching to area " + ofToString(area) + " : "
//                                                + ofToString(tick), ofGetWidth() / 2 - 50, 20);
                    break;
                case 3:
                    camPos.x = easeInOutCubic(tick, ofGetWidth() / 2, - ofGetWidth() / 2 + b34 - 40, easeEnd);
                    camPos.y = easeInOutCubic(tick, ofGetHeight() / 2, - 30, easeEnd);
                    camPos.z = easeInOutCubic(tick, -694, 380, easeEnd);

//                    ofDrawBitmapStringHighlight("switching to area " + ofToString(area) + " : "
//                                                + ofToString(tick), ofGetWidth() / 2 - 50, 20);
                    break;
                case 4:
                    camPos.x = easeInOutCubic(tick, ofGetWidth() / 2, - ofGetWidth() / 2 + b45 - 40, easeEnd);
                    camPos.y = easeInOutCubic(tick, ofGetHeight() / 2, + 50, easeEnd);
                    camPos.z = easeInOutCubic(tick, -694, 360, easeEnd);

//                    ofDrawBitmapStringHighlight("switching to area " + ofToString(area) + " : "
//                                                + ofToString(tick), ofGetWidth() / 2 - 50, 20);
                    break;
                case 5:
                    camPos.x = easeInOutCubic(tick, ofGetWidth() / 2, - ofGetWidth() / 2 + b56 - 20, easeEnd);
                    camPos.y = easeInOutCubic(tick, ofGetHeight() / 2, - 200, easeEnd);
                    camPos.z = easeInOutCubic(tick, -694, 450, easeEnd);

//                    ofDrawBitmapStringHighlight("switching to area " + ofToString(area) + " : "
//                                                + ofToString(tick), ofGetWidth() / 2 - 50, 20);
                    break;
                case 6:
                    camPos.x = easeInOutCubic(tick, ofGetWidth() / 2, - ofGetWidth() / 2 + b56 - 40, easeEnd);
                    camPos.y = easeInOutCubic(tick, ofGetHeight() / 2, - 100, easeEnd);
                    camPos.z = easeInOutCubic(tick, -694, 300, easeEnd);

//                    ofDrawBitmapStringHighlight("switching to area " + ofToString(area) + " : "
//                                                + ofToString(tick), ofGetWidth() / 2 - 50, 20);
                    break;
                default:
                    break;
            }
            break;
            
        case ST_CIWAIT:
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
//            ofDrawBitmapStringHighlight("select the point of the city", ofGetWidth() / 2 - 50, 20);
            ofSetColor(col1);
            cout << ofToString(float((mouseX / mapScale) + offsetX)) << " , " << ofToString(float((mouseY / mapScale) + offsetY)) << '\n';
            switch (area) {
                case 1:
                    for (int i = 0; i < 3; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    
                    break;
                case 2:
                    for (int i = 3; i < 6; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 3:
                    for (int i = 6; i < 10; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 4:
                    for (int i = 10; i < 12; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 5:
                    for (int i = 12; i < 17; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 18; i < 19; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 6:
                    for (int i = 16; i < 22; i++) {
                        if (i != 18) {
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        }
                    }
                    break;
                default:
                    break;
            }
            break;
            
        case ST_TODEANIM:
            ofSetColor(col2);
//            ofDrawBitmapStringHighlight("animation time :" + ofToString(tick), ofGetWidth() / 2 - 50, ofGetHeight() / 2);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
//            ofDrawBitmapStringHighlight("select the point of the city", ofGetWidth() / 2 - 50, 20);
            ofSetColor(col1);
            ofFill();
            switch (area) {
                case 1:
                    for (int i = 0; i < 3; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 2:
                    for (int i = 3; i < 6; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 3:
                    for (int i = 6; i < 10; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 4:
                    for (int i = 10; i < 12; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 5:
                    for (int i = 12; i < 17; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 18; i < 19; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 6:
                    for (int i = 16; i < 22; i++) {
                        if (i != 18) {
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        }
                    }
                    break;
                default:
                    break;
            }

            break;
            
        case ST_DEWAIT:
            ofSetColor(col2 / 2);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
//            ofDrawBitmapStringHighlight("select the point of the city", ofGetWidth() / 2 - 50, 20);
            ofSetColor(col1);
            switch (area) {
                case 1:
                    for (int i = 0; i < 3; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 2:
                    for (int i = 3; i < 6; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 3:
                    for (int i = 6; i < 10; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 4:
                    for (int i = 10; i < 12; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 5:
                    for (int i = 12; i < 17; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 18; i < 19; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 6:
                    for (int i = 16; i < 22; i++) {
                        if (i != 18) {
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        }
                    }
                    break;
                default:
                    break;
            }

//            ofDrawRectangle(ofGetWidth() / (8 * mapScale) + offsetX, ofGetHeight() * 3 / (8 * mapScale) + offsetY, ofGetWidth() * 3 / (8 * mapScale) + offsetX, ofGetHeight() * 5 / (8 * mapScale) + offsetY);
//            ofSetColor(col1);
            break;
            
        case ST_FADEOUT:
            ofSetColor(col2 / 2 - tick * 42.5);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
//            ofDrawBitmapStringHighlight("select the point of the city", ofGetWidth() / 2 - 50, 20);
            break;
            
        default:
            break;
    }
    
    cam.end();
    fbo.end();
    
    fbo.draw(ofGetWidth(),ofGetHeight(), -ofGetWidth(), -ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    trigger = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
int ofApp::getIndex(int x, int y) {
    int index;
    for (int i = 0; i < int(splitString.size() / 4); i++) {
        if (abs(ofToFloat(splitString[i * 4 + 2]) - (float(x) / mapScale + offsetX)) < 20 && abs(ofToFloat(splitString[i * 4 + 3]) - (float(y) / mapScale + offsetY)) < 20) index = int(i + 1);
    }
    cout << ofToString(index);
    return index;
}

//--------------------------------------------------------------
float ofApp::easeInOutQuadratic(float t, float b, float c, float d) {
    t /= (d/2.0) ;
    ofLogNotice(ofToString(t)) ;
    if (t < 1) {
        return(c/2.0*t*t + b) ;
    } else {
        t-=1.0;
        return(-c/2.0 * (t*(t-2) - 1) + b) ;
    }
}

//--------------------------------------------------------------
float ofApp::easeInOutCubic(float t, float b, float c, float d) {
    t /= d/2.0 ;
    if (t < 1) {
        return (c/2.0*t*t*t + b) ;
    }
    t -= 2.0 ;
    return (c/2.0 * (t*t*t + 2) + b) ;
}

//--------------------------------------------------------------
float ofApp::easeOutCubic(float t, float b, float c, float d) {
    t /= d ;
    t-- ;
    return (c*(t*t*t + 1) + b) ;
}

//--------------------------------------------------------------
float ofApp::easeLinear(float t, float b, float c, float d) {
    return (c*t/d + b) ;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
