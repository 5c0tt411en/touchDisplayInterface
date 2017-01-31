#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetCircleResolution(60);
    ofDisableSmoothing();
    
    fontArea.load("NotoSansCJKjp-Regular.otf", 18);
    fontAreaNum.load("futura_book.otf", 70);
    fontSelectArea.load("NotoSansCJKjp-Regular.otf", 50);
    fontSelectCity.load("NotoSansCJKjp-Regular.otf", 50);
    fontBackButtonn.load("NotoSansCJKjp-Regular.otf", 50);
    value0 = 10;
    //    ofSetWindowPosition(2000, 0);
    //    ofSetFullscreen(TRUE);
    ofBackground(0);
    
    ofBuffer buffer = ofBufferFromFile("listHD.csv");
    string sp = buffer.getText();
    ofStringReplace(sp, "\r", ",");
    splitString = ofSplitString(sp, ",", true, true);
    
    cam.setPosition(ofGetWidth() / 2, ofGetHeight() / 2, -694);
    
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    maskFbo.allocate(5334, 3310, GL_RGBA);
    
    img.load("equirectangular.png");
    maskedImage.load("equirectangular.png");
    
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
            trigger = false;
            if (tick > 6) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_ARWAIT;
            }
            if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
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
            
            if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
            }
            break ;
            
        case ST_TOCIANIM:
            trigger = false;
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
            if (tick > easeEnd + 1) {
                timeStamp = ofGetElapsedTimef();
                timeStampDia = ofGetElapsedTimef();
                frameStamp = ofGetFrameNum();
                stat = ST_CIWAIT;
            }
            
            if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
            }
            break;
            
        case ST_CIWAIT:
            switch (area) {
                case 1:
                    mapScale = 3.57756;
                    offsetX = 18.117;
                    offsetY = 88;
                    break;
                case 2:
                    mapScale = 2.84555556;
                    offsetX = 129.12946694;
                    offsetY = 59.31974915;
                    break;
                case 3:
                    mapScale = 2.21410508;
                    offsetX = 355.59161456;
                    offsetY = 189.03079964;
                    break;
                case 4:
                    mapScale = 2.07661688;
                    offsetX = 524.91462248;
                    offsetY = 257.32046293;
                    break;
                case 5:
                    mapScale = 2.84444444;
                    offsetX = 865.33206681;
                    offsetY = 59.30552731;
                    break;
                case 6:
                    mapScale = 1.76088705;
                    offsetX = 960.917;
                    offsetY = 72.84829585;
                    break;
                default:
                    break;
            }
            if (trigger) {
                cityIndex = getIndex(mouseX, mouseY);
                if (backButtonPressed(ofGetWidth() / 8, ofGetHeight() * 7 / 8, mouseX, mouseY)) {
                    stat = ST_ARWAIT;
                    camPos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, -694);
                    timeStamp = ofGetElapsedTimef();
                    trigger = false;
                    break;
                }
                if (cityIndex == 100) {
                    trigger = false;
                    break;
                }
                timeStamp = ofGetElapsedTimef();
                stat = ST_TODEANIM;
                trigger = false;
                
            }
            
            if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
            }
            break;
            
        case ST_TODEANIM:
            trigger = false;
            if (tick > 1) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_DEWAIT;
            }
            break;
            
        case ST_DEWAIT:
            if (trigger) {
                if (backButtonPressed(ofGetWidth() / 4, ofGetHeight() * 4 / 5, mouseX, mouseY)) {
                    stat = ST_CIWAIT;
//                    camPos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, -694);
                    timeStamp = ofGetElapsedTimef();
                    trigger = false;
                    break;
                }
                if (goButtonPressed(ofGetWidth() * 3 / 4, ofGetHeight() * 4 / 5, mouseX, mouseY)){
                    timeStamp = ofGetElapsedTimef();
                    stat = ST_FADEOUT;
                    trigger = false;
                    if (cityIndex <= 22) {
                        ofxOscMessage m;
                        m.setAddress("/touch");
                        m.addIntArg(cityIndex);
                        sender.sendMessage(m);
                        
                    }
                }
            }
            
            if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
            }
            break;
            
        case ST_FADEOUT:
            trigger = false;
            if (value0 == 1) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_TOARANIM;
            }
            else if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
            }
            value0 = 10;
            if (tick > 3) {
                camPos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, -694);
                col1 = ofColor(240, 125, 138, 220);
                col2 = ofColor(255);
            }
            
            if (value0 == 2) {
                timeStamp = ofGetElapsedTimef();
                stat = ST_BLWAIT;
            }
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    tick = ofGetElapsedTimef() - timeStamp ;
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    fbo.begin();
    ofClear(0, 0, 0, 0);
    ofSetColor(col2);
    
    cam.lookAt(node.getPosition());
    cam.begin();
    
    cam.setPosition(camPos);
    
    switch(stat) {
        case ST_BLWAIT:
            camPos = ofVec3f(ofGetWidth() / 2, ofGetHeight() / 2, -694);
            break ;
            
        case ST_TOARANIM:
            path1.clear();
            path1.lineTo(tick * maskFbo.getWidth() / 3, 0);
            path1.lineTo(tick * maskFbo.getWidth() / 3, maskFbo.getHeight());
            path1.lineTo(100 + tick * maskFbo.getWidth() / 3, maskFbo.getHeight());
            path1.lineTo(100 + tick * maskFbo.getWidth() / 3, 0);
            path1.lineTo(tick * maskFbo.getWidth() / 3, 0);
            maskFbo.begin();
            ofClear(0, 0, 0, 0);
            path1.draw();
            maskFbo.end();
            maskedImage.getTexture().setAlphaMask(maskFbo.getTexture());
            ofSetColor(255);
            maskedImage.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            if (tick < 3) opCircle(50 + tick * ofGetWidth() / 3);
            ofSetColor(col2, 85 * (tick - 3));
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            
            areaText();
            
            ofNoFill();
            ofDrawRectangle(0, 0, b12, ofGetHeight());
            ofDrawRectangle(b12, 0, b23 - b12, ofGetHeight());
            ofDrawRectangle(b23, 0, b34 - b23, ofGetHeight());
            ofDrawRectangle(b34, 0, b45 - b34, ofGetHeight());
            ofDrawRectangle(b45, 0, b56 - b45, ofGetHeight());
            ofDrawRectangle(b56, 0, ofGetWidth() - b56, ofGetHeight());
            ofFill();
            for (int i = 0; i < int(splitString.size() / 4); i++) {
                if (tick >= 3) ofSetColor(col1);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
            }
            break;
            
        case ST_ARWAIT:
            ofSetColor(col2);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            
            ofSetColor(col2);
            areaText();
            
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
            break ;
            
        case ST_TOCIANIM:
            
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            
            if (tick >= 1) ofSetColor(col2, 255 - (tick - 1) * 255);
            else ofSetColor(col2);
            areaText();
            
            ofNoFill();
            flash(area, 1);
            ofDrawRectangle(0, 0, b12, ofGetHeight());
            ofNoFill();
             flash(area, 2);
            ofDrawRectangle(b12, 0, b23 - b12, ofGetHeight());
            ofNoFill();
             flash(area, 3);
            ofDrawRectangle(b23, 0, b34 - b23, ofGetHeight());
            ofNoFill();
             flash(area, 4);
            ofDrawRectangle(b34, 0, b45 - b34, ofGetHeight());
            ofNoFill();
             flash(area, 5);
            ofDrawRectangle(b45, 0, b56 - b45, ofGetHeight());
            ofNoFill();
             flash(area, 6);
            ofDrawRectangle(b56, 0, ofGetWidth() - b56, ofGetHeight());
            ofFill();
            for (int i = 0; i < int(splitString.size() / 4); i++) {
                ofSetColor(col1);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
            }
            if (tick >= 1) {
                switch (area) {
                    case 1:
                        camPos.x = easeInOutCubic(tick - 1, ofGetWidth() / 2, - ofGetWidth() / 2 + b12, easeEnd);
                        camPos.y = easeInOutCubic(tick - 1, ofGetHeight() / 2, - 200, easeEnd);
                        camPos.z = easeInOutCubic(tick - 1, -694, 500, easeEnd);
                        break;
                    case 2:
                        camPos.x = easeInOutCubic(tick - 1, ofGetWidth() / 2, - ofGetWidth() / 2 + b23 - 40, easeEnd);
                        camPos.y = easeInOutCubic(tick - 1, ofGetHeight() / 2, - 200, easeEnd);
                        camPos.z = easeInOutCubic(tick - 1, -694, 450, easeEnd);
                        break;
                    case 3:
                        camPos.x = easeInOutCubic(tick - 1, ofGetWidth() / 2, - ofGetWidth() / 2 + b34 - 40, easeEnd);
                        camPos.y = easeInOutCubic(tick - 1, ofGetHeight() / 2, - 30, easeEnd);
                        camPos.z = easeInOutCubic(tick - 1, -694, 380, easeEnd);
                        break;
                    case 4:
                        camPos.x = easeInOutCubic(tick - 1, ofGetWidth() / 2, - ofGetWidth() / 2 + b45 - 40, easeEnd);
                        camPos.y = easeInOutCubic(tick - 1, ofGetHeight() / 2, + 50, easeEnd);
                        camPos.z = easeInOutCubic(tick - 1, -694, 360, easeEnd);
                        break;
                    case 5:
                        camPos.x = easeInOutCubic(tick - 1, ofGetWidth() / 2, - ofGetWidth() / 2 + b56 - 20, easeEnd);
                        camPos.y = easeInOutCubic(tick - 1, ofGetHeight() / 2, - 200, easeEnd);
                        camPos.z = easeInOutCubic(tick - 1, -694, 450, easeEnd);
                        break;
                    case 6:
                        camPos.x = easeInOutCubic(tick - 1, ofGetWidth() / 2, - ofGetWidth() / 3 + b56, easeEnd);
                        camPos.y = easeInOutCubic(tick - 1, ofGetHeight() / 2, - 100, easeEnd);
                        camPos.z = easeInOutCubic(tick - 1, -694, 300, easeEnd);
                        break;
                    default:
                        break;
                }
            }
            
            break;
            
        case ST_CIWAIT:
//            debugScale();
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
//            cout << ofToString(float((mouseX / mapScale) + offsetX)) << " , " << ofToString(float((mouseY / mapScale) + offsetY)) << '\n';
            
            
            ofSetColor(col1, 220 - tick * 220);
            switch (area) {
                case 1:
                    for (int i = 0; i < 3; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    
                    break;
                case 2:
                    for (int i = 3; i < 6; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    break;
                case 3:
                    for (int i = 6; i < 10; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    break;
                case 4:
                    for (int i = 10; i < 12; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    break;
                case 5:
                    for (int i = 12; i < 16; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    for (int i = 18; i < 19; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    for (int i = 21; i < 22; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        annotation(i);
                        ofSetColor(col2, 0);
                        if (tick <= 1.0) ofSetColor(col2, tick * 255);
                        else if (tick > 1.0)ofSetColor(col2);
                        cityText(i);
                    }
                    break;
                case 6:
                    for (int i = 16; i < 21; i++) {
                        if (i != 18) {
                            ofSetColor(col1);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                            annotation(i);
                            ofSetColor(col2, 0);
                            if (tick <= 1.0) ofSetColor(col2, tick * 255);
                            else if (tick > 1.0)ofSetColor(col2);
                            cityText(i);
                        }
                    }
                    break;
                default:
                    break;
            }
            backButton(ofGetWidth() / 8, ofGetHeight() * 7 / 8);
            break;
            
        case ST_TODEANIM:
            ofSetColor(col2);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
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
                    for (int i = 12; i < 16; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 18; i < 19; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 21; i < 22; i++) {
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 6:
                    for (int i = 16; i < 21; i++) {
                        if (i != 18) {
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        }
                    }
                    break;
                default:
                    break;
            }
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(0, 123);
            ofPushMatrix();
            ofTranslate(offsetX, offsetY + easeInOutCubic(tick,
                                                          (-(ofGetHeight() / 2) / mapScale),
                                                          ((ofGetHeight() * 3 / 4) / mapScale), easeEnd));
            ofScale(1 / (mapScale), 1 / (mapScale));
            ofDrawRectangle(0, 0,
                            ofGetWidth(),
                            ofGetHeight() / 1.5);
            ofPopMatrix();
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            break;
            
        case ST_DEWAIT:
//            ofSetColor(30, 200);
//            ofDrawRectangle(0, - ofGetWidth() * 3 / (8 * mapScale) - offsetY,
//                            ofGetWidth(), -ofGetHeight() / (2  * mapScale));
            ofSetColor(col2);
            img.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
            //            ofDrawBitmapStringHighlight("select the point of the city", ofGetWidth() / 2 - 50, 20);
            switch (area) {
                case 1:
                    for (int i = 0; i < 3; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 2:
                    for (int i = 3; i < 6; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 3:
                    for (int i = 6; i < 10; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 4:
                    for (int i = 10; i < 12; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 5:
                    for (int i = 12; i < 16; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 18; i < 19; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    for (int i = 21; i < 22; i++) {
                        ofSetColor(col1);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                        ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                    }
                    break;
                case 6:
                    for (int i = 16; i < 21; i++) {
                        if (i != 18) {
                            ofSetColor(col1);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
                            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
                        }
                    }
                    break;
                default:
                    break;
            }
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            ofSetColor(0, 123);
            ofPushMatrix();
            ofTranslate(offsetX, offsetY + (ofGetHeight() / 4) / mapScale);
            ofScale(1 / (mapScale), 1 / (mapScale));
            ofDrawRectangle(0, 0,
                            ofGetWidth(),
                            ofGetHeight() / 1.5);
            ofPopMatrix();
            ofEnableBlendMode(OF_BLENDMODE_ADD);

            ofSetColor(col2);
            detText(cityIndex - 1);
            backButton(ofGetWidth() / 4, ofGetHeight() * 4 / 5);
                        goButton(ofGetWidth() * 3 / 4, ofGetHeight() * 4 / 5);
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
    if (key == 'f') ofToggleFullscreen();
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
    switch (area) {
        case 1:
            for (int i = 0; i < 3; i++) {
                float distanceX = abs(ofToFloat(splitString[i * 4 + 2])
                                      - (float(x) / mapScale + offsetX)),
                distanceY = abs(ofToFloat(splitString[i * 4 + 3])
                                - (float(y) / mapScale + offsetY));
                if (distanceX < 20 && distanceY < 20) {
                    index = int(i + 1);
                    break;
                }
                else index = 100;
            }
            break;
        case 2:
            for (int i = 3; i < 6; i++) {
                float distanceX = abs(ofToFloat(splitString[i * 4 + 2])
                                      - (float(x) / mapScale + offsetX)),
                distanceY = abs(ofToFloat(splitString[i * 4 + 3])
                                - (float(y) / mapScale + offsetY));
                if (distanceX < 20 && distanceY < 20) {
                    index = int(i + 1);
                    break;
                }
                else index = 100;
            }
            break;
        case 3:
            for (int i = 6; i < 10; i++) {
                float distanceX = abs(ofToFloat(splitString[i * 4 + 2])
                                      - (float(x) / mapScale + offsetX)),
                distanceY = abs(ofToFloat(splitString[i * 4 + 3])
                                - (float(y) / mapScale + offsetY));
                if (distanceX < 20 && distanceY < 20) {
                    index = int(i + 1);
                    break;
                }
                else index = 100;
            }
            break;
        case 4:
            for (int i = 10; i < 12; i++) {
                float distanceX = abs(ofToFloat(splitString[i * 4 + 2])
                                      - (float(x) / mapScale + offsetX)),
                distanceY = abs(ofToFloat(splitString[i * 4 + 3])
                                - (float(y) / mapScale + offsetY));
                if (distanceX < 20 && distanceY < 20) {
                    index = int(i + 1);
                    break;
                }
                else index = 100;
            }
            break;
        case 5:
            for (int i = 12; i < 22; i++) {
                if (i != 16 && i != 17 && i != 19 && i != 20) {
                    float distanceX = abs(ofToFloat(splitString[i * 4 + 2])
                                          - (float(x) / mapScale + offsetX)),
                    distanceY = abs(ofToFloat(splitString[i * 4 + 3])
                                    - (float(y) / mapScale + offsetY));
                    if (distanceX < 20 && distanceY < 20) {
                        index = int(i + 1);
                        break;
                    }
                    else index = 100;
                }
                
            }
            break;
        case 6:
            for (int i = 16; i < 21; i++) {
                if (i != 18) {
                    float distanceX = abs(ofToFloat(splitString[i * 4 + 2])
                                          - (float(x) / mapScale + offsetX)),
                    distanceY = abs(ofToFloat(splitString[i * 4 + 3])
                                    - (float(y) / mapScale + offsetY));
                    if (distanceX < 20 && distanceY < 20) {
                        index = int(i + 1);
                        break;
                    }
                    else index = 100;
                }
            }
            break;
        default:
            break;
    }
    
    cout << ofToString(index) << '\n';
    return index;
}

void ofApp::areaText() {
    ofPushMatrix();
    ofTranslate(ofGetWidth() / 2 - fontSelectArea.stringWidth("エリアを選択してください") / 2,  100);
    fontSelectArea.drawString("エリアを選択してください", 0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area1.x - fontArea.stringWidth("AREA") / 2, - area1.y);
    fontArea.drawString("AREA", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area1.x - fontAreaNum.stringWidth("1") / 2 - 15, - area1.y - numOffset);
    fontAreaNum.drawString("1", 0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area2.x - fontArea.stringWidth("AREA") / 2, - area2.y);
    fontArea.drawString("AREA", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area2.x - fontAreaNum.stringWidth("2") / 2, - area2.y - numOffset);
    fontAreaNum.drawString("2", 0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area3.x - fontArea.stringWidth("AREA") / 2, - area3.y);
    fontArea.drawString("AREA", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area3.x - fontAreaNum.stringWidth("3") / 2, - area3.y - numOffset);
    fontAreaNum.drawString("3", 0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area4.x - fontArea.stringWidth("AREA") / 2, - area4.y);
    fontArea.drawString("AREA", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area4.x - fontAreaNum.stringWidth("4") / 2, - area4.y - numOffset);
    fontAreaNum.drawString("4", 0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area5.x - fontArea.stringWidth("AREA") / 2, - area5.y);
    fontArea.drawString("AREA", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area5.x - fontAreaNum.stringWidth("5") / 2, - area5.y - numOffset);
    fontAreaNum.drawString("5", 0, 0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area6.x - fontArea.stringWidth("AREA") / 2, - area6.y);
    fontArea.drawString("AREA", 0, 0);
    ofPopMatrix();
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(area6.x - fontAreaNum.stringWidth("6") / 2, - area6.y - numOffset);
    fontAreaNum.drawString("6", 0, 0);
    ofPopMatrix();
}

void ofApp::cityText(int n) {
    ofPushMatrix();
    ofTranslate(ofGetWidth() / (mapScale * 2) + offsetX - fontSelectArea.stringWidth("都市を選択してください") / (mapScale * 2), ofGetHeight() * ( 1) / (mapScale * 8) + offsetY);
    ofScale(1 / mapScale, 1 / mapScale);
    fontSelectArea.drawString("都市を選択してください", 0, 0);
    ofPopMatrix();
    
    int p;
   /* if (n == 4) p = 5;
    else if (n == 5) p = 4;
    else if (n == 7) p = 8;
    else if (n == 8) p = 7;
    else if (n == 12) p = 14;
    else if (n == 14) p = 12;
    else */if (n == 13) p = 15;
    else if (n == 15) p = 13;
    else if (n == 16) p = 19;
    else if (n == 19) p = 16;
    else p = n;
    
    int pos = n;
    if (n >= 3 && n < 6) pos -= 3;
    else if (n >= 6 && n < 10) pos -= 6;
    else if (n >= 10 && n < 12) pos -= 10;
    else if (n >= 12 && n < 16) pos -= 12;
    else if (n == 18) pos -= 14;
    else if (n >= 16 && n < 18) pos -= 16;
    else if (n >= 19 && n < 21) pos -= 17;
    else if (n == 21) pos -= 16;
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(ofGetWidth() * 7.5 / (mapScale * 8) + offsetX - fontSelectCity.stringWidth(ofToString(splitString[p * 4]) + " (" + ofToString(splitString[p * 4 + 1]) + ")") / (mapScale * 2), -ofGetHeight() * (pos + 2) / (mapScale * 7) - offsetY + 10);
    ofScale(1 / (mapScale * 2), 1 / (mapScale * 2));
    fontSelectCity.drawString(ofToString(splitString[p * 4]) + " (" + ofToString(splitString[p * 4 + 1]) + ")", 0, 0);
    ofPopMatrix();
}

void ofApp::detText(int n) {
    ofPushMatrix();
    ofTranslate(ofGetWidth() / (mapScale * 2) + offsetX - fontSelectArea.stringWidth(splitString[n * 4] + "が選択されました。決定しますか？") / (mapScale * 2 * 1.4), ofGetHeight() * 2 / (mapScale * 5) + offsetY);
    ofScale(1 / (mapScale * 1.4), 1 / (mapScale * 1.4));
    fontSelectArea.drawString(splitString[n * 4] + "が選択されました。決定しますか？", 0, 0);
    ofPopMatrix();
}

void ofApp::backButton(int x, int y) {
    ofPushMatrix();
    ofTranslate((x) / (mapScale) + offsetX - fontSelectArea.stringWidth("＜戻る") / (mapScale * 2 * 1.4), (y) / (mapScale) + offsetY - fontSelectArea.stringHeight("＜戻る") / (mapScale * 2 * 1.4));
    ofScale(1 / (mapScale * 1.4), 1 / (mapScale * 1.4));
    fontSelectArea.drawString("＜戻る", 0, 0);
    ofPopMatrix();
}

bool ofApp::backButtonPressed(int x, int y, int mx, int my) {
    bool button;
    if (abs(x - mx)
        <
        fontSelectArea.stringWidth("＜戻る") / 1.9
        &&
        abs(y - my * 1.05)
        <
        fontSelectArea.stringHeight("＜戻る") / 1.9)
    {
        cout << ofToString(1);
        button = true;
    }
    else button = false;
    
    return button;
}

void ofApp::goButton(int x, int y) {
    ofPushMatrix();
    ofTranslate((x) / (mapScale) + offsetX - fontSelectArea.stringWidth("決定") / (mapScale * 2 * 1.4), (y) / (mapScale) + offsetY - fontSelectArea.stringHeight("決定") / (mapScale * 2 * 1.4));
    ofScale(1 / (mapScale * 1.4), 1 / (mapScale * 1.4));
    fontSelectArea.drawString("決定", 0, 0);
    ofPopMatrix();
}

bool ofApp::goButtonPressed(int x, int y, int mx, int my) {
    bool button;
    if (abs(x - mx)
        <
        fontSelectArea.stringWidth("決定") / 1.9
        &&
        abs(y - my * 1.05)
        <
        fontSelectArea.stringHeight("決定") / 1.9)
    {
        cout << ofToString(1);
        button = true;
    }
    else button = false;
    
    return button;
}
void ofApp::opCircle(int x) {
    for (int i = 0; i < int(splitString.size() / 4); i++) {
        if (ofToInt(splitString[i * 4 + 2]) <= x) {
            ofSetColor(col1, 250 - (x - ofToInt(splitString[i * 4 + 2])));
            ofFill();
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), (ofToInt(splitString[i * 4 + 2]) - x) / 30 + 10);
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), (ofToInt(splitString[i * 4 + 2]) - x) / 30 + 5);
            ofNoFill();
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), (x - ofToInt(splitString[i * 4 + 2])) / 20);
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), (x - ofToInt(splitString[i * 4 + 2])) / 3);
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), (x - ofToInt(splitString[i * 4 + 2])) / 10);
            
            ofSetColor(col1, (x - ofToInt(splitString[i * 4 + 2])));
            ofFill();
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter);
            ofDrawCircle(ofToInt(splitString[i * 4 + 2]), ofToInt(splitString[i * 4 + 3]), diameter / 2);
        }
    }
}

void ofApp::flash(int area, int n) {
    if (area == n) {
        if(tick >= 0 && tick < 0.07) ofFill();
        else if(tick >= 0.07 && tick < 0.15) ofNoFill();
        else if(tick >= 0.15 && tick < 0.24) ofFill();
        else if(tick >= 0.24 && tick < 0.35) ofNoFill();
        else if(tick >= 0.35 && tick < 0.42) ofFill();
        else if(tick >= 0.42 && tick < 0.51) ofNoFill();
        else if(tick >= 0.51 && tick < 0.60) ofFill();
        else if(tick >= 0.60) ofNoFill();
        
    }
}

void ofApp::debugScale() {
    for (int i = 0; i < 11; i++) {
        ofSetColor(col2);
        ofPushMatrix();
        ofTranslate(offsetX, offsetY);
        ofScale(1 / mapScale, 1 / mapScale);
        ofDrawLine(i * ofGetWidth() / 10, 0, i * ofGetWidth() / 10, ofGetHeight());
        ofDrawLine(0, i * ofGetHeight() / 10, ofGetWidth(), i * ofGetHeight() / 10);
        ofPopMatrix();
    }
}

void ofApp::annotation(int n) {
    if (tick == 0) timeStampDia = ofGetElapsedTimef();
    if (circleAlpha >= 255) timeStampDia = ofGetElapsedTimef();
    circleAlpha = ofGetFrameNum() - frameStamp;
    if (circleAlpha >= 255) frameStamp = ofGetFrameNum();
    circleDia = ofGetElapsedTimef() - timeStampDia;
    ofSetColor(col1, 255 - circleAlpha);
    ofNoFill();
    ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), circleDia * 8.2 / mapScale);
    ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), circleDia * 6.3 / mapScale);
    ofFill();
    ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), circleDia * 10.2 / mapScale);
    ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), circleDia * 4.3 / mapScale);
    ofSetColor(col1,circleAlpha);
    ofNoFill();
    if (circleDia <= 3) ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), (circleDia - 3) * 5.4 / mapScale);
    if (circleDia <= 4)ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), (circleDia - 4) * 6.4 / mapScale);
    if (circleDia <= 5) ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), (circleDia - 5) * 6.9 / mapScale);
    if (circleDia <= 6) ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), (circleDia - 6) * 7.4 / mapScale);
    ofSetColor(col1, 255 - circleAlpha);
    ofFill();
    ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), diameter / mapScale);
    ofDrawCircle(ofToInt(splitString[n * 4 + 2]), ofToInt(splitString[n * 4 + 3]), diameter / 2 / mapScale);
//        }
//    }
    
    ofSetLineWidth(3);
    int p;
   /* if (n == 4) p = 5;
    else if (n == 5) p = 4;
    else if (n == 7) p = 8;
    else if (n == 8) p = 7;
    else if (n == 12) p = 14;
    else if (n == 14) p = 12;
    else */if (n == 13) p = 15;
    else if (n == 15) p = 13;
    else if (n == 16) p = 19;
    else if (n == 19) p = 16;
    else p = n;
    
    int pos = n;
    if (n >= 3 && n < 6) pos -= 3;
    else if (n >= 6 && n < 10) pos -= 6;
    else if (n >= 10 && n < 12) pos -= 10;
    else if (n >= 12 && n < 16) pos -= 12;
    else if (n == 18) pos -= 14;
    else if (n >= 16 && n < 18) pos -= 16;
    else if (n >= 19 && n < 21) pos -= 17;
    else if (n == 21) pos -= 16;

    if (tick < 1 ) ofSetColor(col1, tick * 220);
    else if (tick >= 1)  ofSetColor(col1);
                float x1 = ofToFloat(splitString[p * 4 + 2]);
                float y1 = ofToFloat(splitString[p * 4 + 3]);
                float x2 = ofGetWidth() * 7.5 / (mapScale * 8) + offsetX - fontSelectCity.stringWidth(ofToString(splitString[p * 4]) + " (" + ofToString(splitString[p * 4 + 1]) + ")") / (mapScale * 2);
                float y2 = ofGetHeight() * (pos + 2) / (mapScale * 7) + offsetY - 13;
    if (y1 < y2) {
        ofDrawLine(x1, y1, x1 + y2 - y1, y2);
        ofDrawLine(x1 + y2 - y1, y2, x2, y2);
    }
    else if (y1 == y2) {
        ofDrawLine(x1, y2, x2, y2);
    }
    else if (y1 > y2) {
        ofDrawLine(x1, y1, x1 - y2 + y1, y2);
        ofDrawLine(x1 - y2 + y1, y2, x2, y2);
    }
    
    ofSetLineWidth(1);
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
