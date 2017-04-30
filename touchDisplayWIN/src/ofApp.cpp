#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	// text setting
	fontArea.load("fonts/NotoSansCJKjp-Regular.otf", 28);
	fontAreaNum.load("fonts/futura_book.otf", 60);
	fontSelectArea.load("fonts/NotoSansCJKjp-Regular.otf", 40);
	fontSelectCity.load("fonts/NotoSansCJKjp-Regular.otf", 40);
	fontCity.load("fonts/NotoSansCJKjp-Regular.otf", 30);
	fontDet.load("fonts/NotoSansCJKjp-Regular.otf", 30);
	// csv buffrering
	ofBuffer buffer = ofBufferFromFile("listHD.csv");
	string sp = buffer.getText();
	ofStringReplace(sp, "\r", ",");
	splitString = ofSplitString(sp, ",", true, true);

	//button image
	backButtonUI.load("buttons/backButton.png");
	goButtonUI.load("buttons/goButton.png");

	// camera
	cam.setPosition(camDefault);
	cam.setupPerspective();

	// background Image & mask
	fbo.allocate(W, H, GL_RGBA);
	maskFbo.allocate(4800, 2700, GL_RGBA);
	img.load("equirectangularHD.png");
	maskedImage.load("equirectangularHD.png");

	// OSC
	receiver.setup(RPORT);
	sender.setup(HOST, SPORT);
	value0 = 10;

	// city images
	for (int n = 0; n < 22; n++) {
		for (int i = 0; i < 2; i++) {
			cityImage[2 * n + i].load("cityImages/" + splitString[n * 7] + "/" + ofToString(i + 1) + ".jpg");
			ofLog() << "cityImage[" << 2 * n + i << "] : cityImages/" + splitString[n * 7] + "/" + ofToString(i + 1) + ".jpg";
		}
	}

	// OTHERS
	ofBackground(0);
	ofSetCircleResolution(60);
	ofDisableSmoothing();
}

//--------------------------------------------------------------
void ofApp::update() {
	//OSC
	while (receiver.hasWaitingMessages()) {
		ofxOscMessage message;
		receiver.getNextMessage(&message);
		value0 = message.getArgAsInt32(0);
	}

	// camera
	pointToView.x = camPos.x;
	pointToView.y = camPos.y;
	pointToView.z = -1000;
	node.setPosition(pointToView);

	imageScale = mapScale / 2;

	// update
	switch (stat) {
	case ST_BLWAIT:
		if (value0 == 0) {
			timeStamp = ofGetElapsedTimef();
			trigger = false;
			stat = ST_TOARANIM;
		}
		value0 = 10;
		break;

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
			else if (mouseX >= b56 && mouseX < W) area = 6;
			timeStamp = ofGetElapsedTimef();
			stat = ST_TOCIANIM;
			trigger = false;
		}

		if (value0 == 2) {
			timeStamp = ofGetElapsedTimef();
			stat = ST_BLWAIT;
		}
		break;

	case ST_TOCIANIM:
		trigger = false;
		if (tick >= 1) {
			switch (area) {
			case 1:
				camPos.x = easeInOutCubic(tick - 1, W / 2, anim1.x, easeEnd);
				camPos.y = easeInOutCubic(tick - 1, H / 2, anim1.y, easeEnd);
				camPos.z = easeInOutCubic(tick - 1, camDefault.z, anim1.z, easeEnd);
				break;
			case 2:
				camPos.x = easeInOutCubic(tick - 1, W / 2, anim2.x, easeEnd);
				camPos.y = easeInOutCubic(tick - 1, H / 2, anim2.y, easeEnd);
				camPos.z = easeInOutCubic(tick - 1, camDefault.z, anim2.z, easeEnd);
				break;
			case 3:
				camPos.x = easeInOutCubic(tick - 1, W / 2, anim3.x, easeEnd);
				camPos.y = easeInOutCubic(tick - 1, H / 2, anim3.y, easeEnd);
				camPos.z = easeInOutCubic(tick - 1, camDefault.z, anim3.z, easeEnd);
				break;
			case 4:
				camPos.x = easeInOutCubic(tick - 1, W / 2, anim4.x, easeEnd);
				camPos.y = easeInOutCubic(tick - 1, H / 2, anim4.y, easeEnd);
				camPos.z = easeInOutCubic(tick - 1, camDefault.z, anim4.z, easeEnd);
				break;
			case 5:
				camPos.x = easeInOutCubic(tick - 1, W / 2, anim5.x, easeEnd);
				camPos.y = easeInOutCubic(tick - 1, H / 2, anim5.y, easeEnd);
				camPos.z = easeInOutCubic(tick - 1, camDefault.z, anim5.z, easeEnd);
				break;
			case 6:
				camPos.x = easeInOutCubic(tick - 1, W / 2, anim6.x, easeEnd);
				camPos.y = easeInOutCubic(tick - 1, H / 2, anim6.y, easeEnd);
				camPos.z = easeInOutCubic(tick - 1, camDefault.z, anim6.z, easeEnd);
				break;
			default:
				break;
			}
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
			mapScale = 2.37756;
			break;
		case 2:
			mapScale = 2.37756;
			break;
		case 3:
			mapScale = 1.92746;
			break;
		case 4:
			mapScale = 1.26499342;
			break;
		case 5:
			mapScale = 1.93499;
			break;
		case 6:
			mapScale = 1.65322;
			break;
		default:
			break;
		}
		if (trigger) {
			cityIndex = getIndex(mouseX, mouseY);
			if (backButtonPressed(W / 8, H * 7 / 8, mouseX, mouseY)) {
				stat = ST_ARWAIT;
				camPos = camDefault;
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
			if (backButtonPressed(W / 4, H * 4 / 5, mouseX, mouseY)) {
				stat = ST_CIWAIT;
				timeStamp = ofGetElapsedTimef();
				trigger = false;
				break;
			}
			if (goButtonPressed(W * 3 / 4, H * 4 / 5, mouseX, mouseY)) {
				timeStamp = ofGetElapsedTimef();
				stat = ST_FADEOUT;
				trigger = false;
				value0 = 10;
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
		if (tick > 3 && value0 == 10) {
			camPos = camDefault;
			col1 = ofColor(240, 125, 138, 220);
			col2 = ofColor(255);
			if (cityIndex <= 22) {
				ofxOscMessage m;
				m.setAddress("/touch");
				m.addIntArg(cityIndex);
				sender.sendMessage(m);
			}
			value0++;
		}
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (debug) ofDrawBitmapStringHighlight("Type 'f' to to run in full-screen", 20, 20);

	// time manegement
	tick = ofGetElapsedTimef() - timeStamp;
	ofEnableBlendMode(OF_BLENDMODE_ADD);

	// camera
	cam.lookAt(node.getPosition());
	cam.begin();
	cam.setPosition(camPos);

	// draw
	switch (stat) {
	case ST_BLWAIT:
		camPos = camDefault;
		cam.end();
		break;

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
		maskedImage.draw(0, 0, W, H);
		if (tick < 3) opCircle(50 + tick * W / 3);
		ofSetColor(col2, 85 * (tick - 3));
		img.draw(0, 0, W, H);

		areaText();

		ofNoFill();
		ofDrawRectangle(0, 0, b12, H);
		ofDrawRectangle(b12, 0, b23 - b12, H);
		ofDrawRectangle(b23, 0, b34 - b23, H);
		ofDrawRectangle(b34, 0, b45 - b34, H);
		ofDrawRectangle(b45, 0, b56 - b45, H);
		ofDrawRectangle(b56, 0, W - b56, H);
		ofFill();
		for (int i = 0; i < int(splitString.size() / 7); i++) {
			ofVec2f pos = ofVec2f(
				ofToFloat(splitString[i * 7 + 2]) * W,
				ofToFloat(splitString[i * 7 + 3]) * H
			);
			if (tick >= 3) ofSetColor(col1);
			ofDrawCircle(pos.x, pos.y, diameter);
			ofDrawCircle(pos.x, pos.y, diameter / 2);
		}
		cam.end();
		break;

	case ST_ARWAIT:
		ofSetColor(col2);
		img.draw(0, 0, W, H);

		ofSetColor(col2);
		areaText();

		ofNoFill();
		ofDrawRectangle(0, 0, b12, H);
		ofDrawRectangle(b12, 0, b23 - b12, H);
		ofDrawRectangle(b23, 0, b34 - b23, H);
		ofDrawRectangle(b34, 0, b45 - b34, H);
		ofDrawRectangle(b45, 0, b56 - b45, H);
		ofDrawRectangle(b56, 0, W - b56, H);
		ofFill();
		for (int i = 0; i < int(splitString.size() / 7); i++) {
			ofVec2f pos = ofVec2f(
				ofToFloat(splitString[i * 7 + 2]) * W,
				ofToFloat(splitString[i * 7 + 3]) * H
			);
			ofSetColor(col1);
			ofDrawCircle(pos.x, pos.y, diameter);
			ofDrawCircle(pos.x, pos.y, diameter / 2);
		}
		cam.end();
		break;

	case ST_TOCIANIM:
		ofSetColor(col2);
		img.draw(0, 0, W, H);
		img.draw(-W, 0, W, H);
		img.draw(W, 0, W, H);

		if (tick >= 1) ofSetColor(col2, 255 - (tick - 1) * 255);
		else ofSetColor(col2);
		areaText();

		ofNoFill();
		flash(area, 1);
		ofDrawRectangle(0, 0, b12, H);
		ofNoFill();
		flash(area, 2);
		ofDrawRectangle(b12, 0, b23 - b12, H);
		ofNoFill();
		flash(area, 3);
		ofDrawRectangle(b23, 0, b34 - b23, H);
		ofNoFill();
		flash(area, 4);
		ofDrawRectangle(b34, 0, b45 - b34, H);
		ofNoFill();
		flash(area, 5);
		ofDrawRectangle(b45, 0, b56 - b45, H);
		ofNoFill();
		flash(area, 6);
		ofDrawRectangle(b56, 0, W - b56, H);
		ofFill();
		for (int i = 0; i < int(splitString.size() / 7); i++) {
			ofVec2f pos = ofVec2f(
				ofToFloat(splitString[i * 7 + 2]) * W,
				ofToFloat(splitString[i * 7 + 3]) * H
			);
			ofSetColor(col1);
			ofDrawCircle(pos.x, pos.y, diameter);
			ofDrawCircle(pos.x, pos.y, diameter / 2);
		}
		cam.end();
		break;

	case ST_CIWAIT:
		ofSetColor(col2);
		img.draw(0, 0, W, H);
		img.draw(-W, 0, W, H);
		img.draw(W, 0, W, H);
		ofSetColor(col1, 220 - tick * 220);
		switch (area) {
		case 1:
			cityFrom = 0;
			cityTo = 3;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 2:
			cityFrom = 3;
			cityTo = 6;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 3:
			cityFrom = 6;
			cityTo = 10;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 4:
			cityFrom = 10;
			cityTo = 12;
			cityPointDraw(cityFrom, cityTo);
			cityPointDraw(14);
			break;
		case 5:
			cityFrom = 12;
			cityTo = 14;
			cityPointDraw(cityFrom, cityTo);
			cityPointDraw(18);
			cityPointDraw(21);
			break;
		case 6:
			cityFrom = 15;
			cityTo = 21;
			cityPointDraw(cityFrom, cityTo, 18);
			break;
		default:
			break;
		}
		cam.end();
		cityText();
		backButton(W / 8 - backButtonUI.getWidth() / 2, H * 7 / 8 - backButtonUI.getHeight() / 2);
		break;

	case ST_TODEANIM:
		ofSetColor(col2);
		img.draw(0, 0, W, H);
		img.draw(-W, 0, W, H);
		img.draw(W, 0, W, H);
		ofSetColor(col1);
		ofFill();

		switch (area) {
		case 1:
			cityFrom = 0;
			cityTo = 3;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 2:
			cityFrom = 3;
			cityTo = 6;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 3:
			cityFrom = 6;
			cityTo = 10;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 4:
			cityFrom = 10;
			cityTo = 12;
			cityPointDraw(cityFrom, cityTo);
			cityPointDraw(14);
			break;
		case 5:
			cityFrom = 12;
			cityTo = 14;
			cityPointDraw(cityFrom, cityTo);
			cityPointDraw(18);
			cityPointDraw(21);
			break;
		case 6:
			cityFrom = 15;
			cityTo = 21;
			cityPointDraw(cityFrom, cityTo, 18);
			break;
		default:
			break;
		}            cam.end();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(0, 123);
		ofDrawRectangle(0, easeInOutCubic(tick, -H / 2, H * 3 / 4, easeEnd), W, H / 1.5);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		break;

	case ST_DEWAIT:
		ofSetColor(col2);
		img.draw(0, 0, W, H);
		img.draw(-W, 0, W, H);
		img.draw(W, 0, W, H);

		switch (area) {
		case 1:
			cityFrom = 0;
			cityTo = 3;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 2:
			cityFrom = 3;
			cityTo = 6;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 3:
			cityFrom = 6;
			cityTo = 10;
			cityPointDraw(cityFrom, cityTo);
			break;
		case 4:
			cityFrom = 10;
			cityTo = 12;
			cityPointDraw(cityFrom, cityTo);
			cityPointDraw(14);
			break;
		case 5:
			cityFrom = 12;
			cityTo = 14;
			cityPointDraw(cityFrom, cityTo);
			cityPointDraw(18);
			cityPointDraw(21);
			break;
		case 6:
			cityFrom = 15;
			cityTo = 21;
			cityPointDraw(cityFrom, cityTo, 18);
			break;
		default:
			break;
		}
		cam.end();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(0, 123);
		ofDrawRectangle(0, H / 4, W, H / 1.5);
		ofEnableBlendMode(OF_BLENDMODE_ADD);

		ofSetColor(col2);
		detText(cityIndex - 1);
		backButton(W / 4 - backButtonUI.getWidth() / 2, H * 4 / 5 - backButtonUI.getHeight() / 2);
		goButton(W * 3 / 4 - goButtonUI.getWidth() / 2, H * 4 / 5 - goButtonUI.getHeight() / 2);

		break;

	case ST_FADEOUT:
		ofSetColor(col2 / 2 - tick * 42.5);
		img.draw(0, 0, W, H);
		img.draw(-W, 0, W, H);
		img.draw(W, 0, W, H);
		cam.end();
		break;

	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'f') {
		if (debug) {
			tick = 0;
			timeStamp = ofGetElapsedTimef();
			trigger = false;
			value0 = 10;
			camPos = camDefault;
			stat = ST_TOARANIM;
		}
		debug = !debug;
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	trigger = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
int ofApp::getIndex(int x, int y) {
	int index;
	switch (area) {
	case 1:
		index = getIndexft(x, y, cityFrom, cityTo);
		break;
	case 2:
		index = getIndexft(x, y, cityFrom, cityTo);
		break;
	case 3:
		index = getIndexft(x, y, cityFrom, cityTo);
		break;
	case 4:
		index = getIndexft(x, y, cityFrom, cityTo);
		if (index > 12) index = getIndexp(x, y, 14);
		break;
	case 5:
		index = getIndexft(x, y, cityFrom, cityTo);
		if (index > 22) index = getIndexp(x, y, 18);
		if (index > 22) index = getIndexp(x, y, 21);
		break;
	case 6:
		index = getIndexftex(x, y, cityFrom, cityTo, 18);
		break;
	default:
		break;
	}
	return index;
}

int ofApp::getIndexft(int x, int y, int f, int t) {
	int index;
	for (int i = f; i < t; i++) {
		ofVec2f pos = ofVec2f(
			ofToFloat(splitString[i * 7 + 4]) * W,
			ofToFloat(splitString[i * 7 + 5]) * H
		);
		ofVec2f pointPos = ofVec2f(
			ofToFloat(splitString[i * 7 + 2]) * W,
			ofToFloat(splitString[i * 7 + 3]) * H
		);
		ofVec3f imageScreen = cam.worldToScreen(ofVec2f(pos));
		ofVec3f posScreen = cam.worldToScreen(ofVec2f(pointPos));
		float distanceX = abs(imageScreen.x - x),
			distanceY = abs(imageScreen.y - H + y - 15);
		float distancePoint = ofDist(posScreen.x, posScreen.y, x, H - y);
		if ((distanceX < imageSize.x * scAdjust / 2 && distanceY < imageSize.y * scAdjust / 2 + 15)
			||
			distancePoint < 20) {
			index = int(i + 1);
			break;
		}
		else index = 100;
	}
	return index;
}

int ofApp::getIndexftex(int x, int y, int f, int t, int ex) {
	int index;
	for (int i = f; i < t; i++) {
		if (i != ex) {
			ofVec2f pos = ofVec2f(
				ofToFloat(splitString[i * 7 + 4]) * W,
				ofToFloat(splitString[i * 7 + 5]) * H
			);
			ofVec2f pointPos = ofVec2f(
				ofToFloat(splitString[i * 7 + 2]) * W,
				ofToFloat(splitString[i * 7 + 3]) * H
			);
			ofVec3f imageScreen = cam.worldToScreen(ofVec2f(pos));
			ofVec3f posScreen = cam.worldToScreen(ofVec2f(pointPos));
			float distanceX = abs(imageScreen.x - x),
				distanceY = abs(imageScreen.y - H + y - 15);
			float distancePoint = ofDist(posScreen.x, posScreen.y, x, H - y);
			if ((distanceX < imageSize.x * scAdjust / 2 && distanceY < imageSize.y * scAdjust / 2 + 15)
				||
				distancePoint < 20) {
				index = int(i + 1);
				break;
			}
			else index = 100;
		}
	}
	return index;
}

int ofApp::getIndexp(int x, int y, int p) {
	int index;
	ofVec2f pos = ofVec2f(
		ofToFloat(splitString[p * 7 + 4]) * W,
		ofToFloat(splitString[p * 7 + 5]) * H
	);
	ofVec2f pointPos = ofVec2f(
		ofToFloat(splitString[p * 7 + 2]) * W,
		ofToFloat(splitString[p * 7 + 3]) * H
	);
	ofVec3f imageScreen = cam.worldToScreen(ofVec2f(pos));
	ofVec3f posScreen = cam.worldToScreen(ofVec2f(pointPos));
	float distanceX = abs(imageScreen.x - x),
		distanceY = abs(imageScreen.y - H + y - 15);
	float distancePoint = ofDist(posScreen.x, posScreen.y, x, H - y);
	if ((distanceX < imageSize.x * scAdjust / 2 && distanceY < imageSize.y * scAdjust / 2 + 15)
		||
		distancePoint < 20)
		index = int(p + 1);
	else index = 100;
	return index;
}

void ofApp::areaText() {
	string s = "エリアを選択してください";
	fontSelectArea.drawString(s, W / 2 - fontSelectArea.stringWidth(s) / 2, 70 + fontSelectArea.stringHeight(s) / 2);
	fontArea.drawString("AREA", area1.x - fontArea.stringWidth("AREA") / 2, area1.y);
	fontAreaNum.drawString("1", area1.x - fontAreaNum.stringWidth("1") / 2 - 15, area1.y + numOffset);
	fontArea.drawString("AREA", area2.x - fontArea.stringWidth("AREA") / 2, area2.y);
	fontAreaNum.drawString("2", area2.x - fontAreaNum.stringWidth("2") / 2, area2.y + numOffset);
	fontArea.drawString("AREA", area3.x - fontArea.stringWidth("AREA") / 2, area3.y);
	fontAreaNum.drawString("3", area3.x - fontAreaNum.stringWidth("3") / 2, area3.y + numOffset);
	fontArea.drawString("AREA", area4.x - fontArea.stringWidth("AREA") / 2, area4.y);
	fontAreaNum.drawString("4", area4.x - fontAreaNum.stringWidth("4") / 2, area4.y + numOffset);
	fontArea.drawString("AREA", area5.x - fontArea.stringWidth("AREA") / 2, area5.y);
	fontAreaNum.drawString("5", area5.x - fontAreaNum.stringWidth("5") / 2, area5.y + numOffset);
	fontArea.drawString("AREA", area6.x - fontArea.stringWidth("AREA") / 2, area6.y);
	fontAreaNum.drawString("6", area6.x - fontAreaNum.stringWidth("6") / 2, area6.y + numOffset);
}

void ofApp::cityText() {
	string s = "都市を選択してください";
	fontSelectArea.drawString(s, W / 2 - fontSelectArea.stringWidth(s) / 2, 70 + fontSelectArea.stringHeight(s) / 2);
}

void ofApp::detText(int n) {
	string s = splitString[n * 7] + "が選択されました。決定しますか？";
	fontSelectArea.drawString(s, W / 2 - fontSelectArea.stringWidth(s) / 2, H * 2 / 5);
	ofPopMatrix();
}

void ofApp::backButton(int x, int y) {
	backButtonUI.draw(x, y);
}

bool ofApp::backButtonPressed(int x, int y, int mx, int my) {
	bool button;
	if (abs(x - mx)
		<
		backButtonUI.getWidth() / 2
		&&
		abs(y - my)
		<
		backButtonUI.getHeight() / 2)
	{
		cout << ofToString(1);
		button = true;
	}
	else button = false;

	return button;
}

void ofApp::goButton(int x, int y) {
	goButtonUI.draw(x, y);
}

bool ofApp::goButtonPressed(int x, int y, int mx, int my) {
	bool button;
	if (abs(x - mx)
		<
		backButtonUI.getWidth() / 2
		&&
		abs(y - my)
		<
		goButtonUI.getHeight() / 2)
	{
		cout << ofToString(1);
		button = true;
	}
	else button = false;

	return button;
}

void ofApp::opCircle(int x) {
	for (int i = 0; i < int(splitString.size() / 7); i++) {
		ofVec2f pos = ofVec2f(
			ofToFloat(splitString[i * 7 + 2]) * W,
			ofToFloat(splitString[i * 7 + 3]) * H
		);
		if (pos.x <= x) {
			ofSetColor(col1, 250 - (x - pos.x));
			ofFill();
			ofDrawCircle(pos.x, pos.y, (pos.x - x) / 30 + 10);
			ofDrawCircle(pos.x, pos.y, (pos.x - x) / 30 + 5);
			ofNoFill();
			ofDrawCircle(pos.x, pos.y, (x - pos.x) / 20);
			ofDrawCircle(pos.x, pos.y, (x - pos.x) / 3);
			ofDrawCircle(pos.x, pos.y, (x - pos.x) / 10);

			ofSetColor(col1, (x - pos.x));
			ofFill();
			ofDrawCircle(pos.x, pos.y, diameter);
			ofDrawCircle(pos.x, pos.y, diameter / 2);
		}
	}
}

void ofApp::flash(int area, int n) {
	if (area == n) {
		if (tick >= 0 && tick < 0.07) ofFill();
		else if (tick >= 0.07 && tick < 0.15) ofNoFill();
		else if (tick >= 0.15 && tick < 0.24) ofFill();
		else if (tick >= 0.24 && tick < 0.35) ofNoFill();
		else if (tick >= 0.35 && tick < 0.42) ofFill();
		else if (tick >= 0.42 && tick < 0.51) ofNoFill();
		else if (tick >= 0.51 && tick < 0.60) ofFill();
		else if (tick >= 0.60) ofNoFill();

	}
}

void ofApp::annotation(int n) {
	ofVec2f pos = ofVec2f(
		ofToFloat(splitString[n * 7 + 2]) * W,
		ofToFloat(splitString[n * 7 + 3]) * H
	);
	if (tick == 0) timeStampDia = ofGetElapsedTimef();
	if (circleAlpha >= 255) timeStampDia = ofGetElapsedTimef();
	circleAlpha = ofGetFrameNum() - frameStamp;
	if (circleAlpha >= 255) frameStamp = ofGetFrameNum();
	circleDia = ofGetElapsedTimef() - timeStampDia;
	ofSetColor(col1, 255 - circleAlpha);
	ofNoFill();
	ofDrawCircle(pos.x, pos.y, circleDia * 8.2 / mapScale);
	ofDrawCircle(pos.x, pos.y, circleDia * 6.3 / mapScale);
	ofFill();
	ofDrawCircle(pos.x, pos.y, circleDia * 10.2 / mapScale);
	ofDrawCircle(pos.x, pos.y, circleDia * 4.3 / mapScale);
	ofSetColor(col1, circleAlpha);
	ofNoFill();
	if (circleDia <= 3) ofDrawCircle(pos.x, pos.y, (circleDia - 3) * 5.4 / mapScale);
	if (circleDia <= 4)ofDrawCircle(pos.x, pos.y, (circleDia - 4) * 6.4 / mapScale);
	if (circleDia <= 5) ofDrawCircle(pos.x, pos.y, (circleDia - 5) * 6.9 / mapScale);
	if (circleDia <= 6) ofDrawCircle(pos.x, pos.y, (circleDia - 6) * 7.4 / mapScale);
	ofSetColor(col1, 255 - circleAlpha);
	ofFill();
	ofDrawCircle(pos.x, pos.y, diameter / mapScale);
	ofDrawCircle(pos.x, pos.y, diameter / 2 / mapScale);

	ofSetLineWidth(3);
	if (stat == ST_DEWAIT || stat == ST_TODEANIM) ofSetColor(col1);
	else if (tick < 1) ofSetColor(col1, tick * 220);
	else if (tick >= 1)  ofSetColor(col1);
	float x1 = ofToFloat(splitString[n * 7 + 2]) * W;
	float y1 = ofToFloat(splitString[n * 7 + 3]) * H;
	float x2 = ofToFloat(splitString[n * 7 + 4]) * W;
	float y2 = ofToFloat(splitString[n * 7 + 5]) * H;
	if (y1 < y2) {
		if (x1 > x2) {
			ofDrawLine(x1, y1, x1 - y2 + y1, y2);
			ofDrawLine(x1 - y2 + y1, y2, x2, y2);
		}
		else {
			ofDrawLine(x1, y1, x1 + y2 - y1, y2);
			ofDrawLine(x1 + y2 - y1, y2, x2, y2);
		}
	}
	else if (y1 == y2) {
		ofDrawLine(x1, y2, x2, y2);
	}
	else if (y1 > y2) {
		if (x1 > x2) {
			ofDrawLine(x1, y1, x1 + y2 - y1, y2);
			ofDrawLine(x1 + y2 - y1, y2, x2, y2);
		}
		else {
			ofDrawLine(x1, y1, x1 - y2 + y1, y2);
			ofDrawLine(x1 - y2 + y1, y2, x2, y2);

		}
	}
	ofSetLineWidth(1);
}

void ofApp::cityPointDraw(int p) {
	ofVec2f pos = ofVec2f(
		ofToFloat(splitString[p * 7 + 2]) * W,
		ofToFloat(splitString[p * 7 + 3]) * H
	);
	ofVec2f imagePos = ofVec2f(
		ofToFloat(splitString[p * 7 + 4]) * W,
		ofToFloat(splitString[p * 7 + 5]) * H
	);
	localTime = ofGetHours() + ofToInt(splitString[p * 7 + 6]);
	if (localTime >= 24) localTime -= 24;
	else if (localTime < 0) localTime += 24;
	int imageNum;
	if (localTime >= 6 && localTime < 18) imageNum = p * 2;  else imageNum = p * 2 + 1;
	ofLog() << localTime;
	ofSetColor(col1);
	ofDrawCircle(pos.x, pos.y, diameter);
	ofDrawCircle(pos.x, pos.y, diameter / 2);
	annotation(p);
	ofSetColor(col2, 0);
	if (stat == ST_DEWAIT || stat == ST_TODEANIM) ofSetColor(col2);
	else if (tick <= 1.0) ofSetColor(col2, tick * 255);
	else if (tick > 1.0)ofSetColor(col2);
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	ofPushMatrix();
	ofTranslate(imagePos.x, imagePos.y);
	ofScale(1 / imageScale, 1 / imageScale);
	cityImage[imageNum].draw(-imageSize.x / 2, -imageSize.y / 2, imageSize.x, imageSize.y);
	string s = splitString[p * 7] + "(" + splitString[p * 7 + 1] + ")";
	ofTranslate(
		-fontCity.stringWidth(s) / 8,
		imageSize.y / 2 + fontCity.stringHeight(s) / 4
	);
	ofScale(.25, .25);
	fontCity.drawString(s, 0, 0);
	ofPopMatrix();
	ofEnableBlendMode(OF_BLENDMODE_ADD);
}

void ofApp::cityPointDraw(int f, int t) {
	for (int i = f; i < t; i++) {
		ofVec2f pos = ofVec2f(
			ofToFloat(splitString[i * 7 + 2]) * W,
			ofToFloat(splitString[i * 7 + 3]) * H
		);
		ofVec2f imagePos = ofVec2f(
			ofToFloat(splitString[i * 7 + 4]) * W,
			ofToFloat(splitString[i * 7 + 5]) * H
		);
		localTime = ofGetHours() + ofToInt(splitString[i * 7 + 6]);
		if (localTime >= 24) localTime -= 24;
		else if (localTime < 0) localTime += 24;
		int imageNum;
		if (localTime >= 6 && localTime < 18) imageNum = i * 2;  else imageNum = i * 2 + 1;
		ofSetColor(col1);
		ofDrawCircle(pos.x, pos.y, diameter);
		ofDrawCircle(pos.x, pos.y, diameter / 2);
		annotation(i);
		ofSetColor(col2, 0);
		if (stat == ST_DEWAIT || stat == ST_TODEANIM) ofSetColor(col2);
		else if (tick <= 1.0) ofSetColor(col2, tick * 255);
		else if (tick > 1.0)ofSetColor(col2);
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofPushMatrix();
		ofTranslate(imagePos.x, imagePos.y);
		ofScale(1 / imageScale, 1 / imageScale);
		cityImage[imageNum].draw(-imageSize.x / 2, -imageSize.y / 2, imageSize.x, imageSize.y);
		string s = splitString[i * 7] + "(" + splitString[i * 7 + 1] + ")";
		ofTranslate(
			-fontCity.stringWidth(s) / 8,
			imageSize.y / 2 + fontCity.stringHeight(s) / 4
		);
		ofScale(.25, .25);
		fontCity.drawString(s, 0, 0);
		ofPopMatrix();
		ofEnableBlendMode(OF_BLENDMODE_ADD);
	}
}

void ofApp::cityPointDraw(int f, int t, int ex) {
	for (int i = f; i < t; i++) {
		if (i != ex) {
			ofVec2f pos = ofVec2f(
				ofToFloat(splitString[i * 7 + 2]) * W,
				ofToFloat(splitString[i * 7 + 3]) * H
			);
			ofVec2f imagePos = ofVec2f(
				ofToFloat(splitString[i * 7 + 4]) * W,
				ofToFloat(splitString[i * 7 + 5]) * H
			);
			localTime = ofGetHours() + ofToInt(splitString[i * 7 + 6]);
			if (localTime >= 24) localTime -= 24;
			else if (localTime < 0) localTime += 24;
			int imageNum;
			if (localTime >= 6 && localTime < 18) imageNum = i * 2;  else imageNum = i * 2 + 1;
			ofSetColor(col1);
			ofDrawCircle(pos.x, pos.y, diameter);
			ofDrawCircle(pos.x, pos.y, diameter / 2);
			annotation(i);
			ofSetColor(col2, 0);
			if (stat == ST_DEWAIT || stat == ST_TODEANIM) ofSetColor(col2);
			else if (tick <= 1.0) ofSetColor(col2, tick * 255);
			else if (tick > 1.0)ofSetColor(col2);
			ofEnableBlendMode(OF_BLENDMODE_ALPHA);
			ofPushMatrix();
			ofTranslate(imagePos.x, imagePos.y);
			ofScale(1 / imageScale, 1 / imageScale);
			cityImage[imageNum].draw(-imageSize.x / 2, -imageSize.y / 2, imageSize.x, imageSize.y);
			string s = splitString[i * 7] + "(" + splitString[i * 7 + 1] + ")";
			ofTranslate(
				-fontCity.stringWidth(s) / 8,
				imageSize.y / 2 + fontCity.stringHeight(s) / 4
			);
			ofScale(.25, .25);
			fontCity.drawString(s, 0, 0);
			ofPopMatrix();
			ofEnableBlendMode(OF_BLENDMODE_ADD);
		}
	}
}

//--------------------------------------------------------------
float ofApp::easeInOutQuadratic(float t, float b, float c, float d) {
	t /= (d / 2.0);
	ofLogNotice(ofToString(t));
	if (t < 1) {
		return(c / 2.0*t*t + b);
	}
	else {
		t -= 1.0;
		return(-c / 2.0 * (t*(t - 2) - 1) + b);
	}
}

//--------------------------------------------------------------
float ofApp::easeInOutCubic(float t, float b, float c, float d) {
	t /= d / 2.0;
	if (t < 1) {
		return (c / 2.0*t*t*t + b);
	}
	t -= 2.0;
	return (c / 2.0 * (t*t*t + 2) + b);
}

//--------------------------------------------------------------
float ofApp::easeOutCubic(float t, float b, float c, float d) {
	t /= d;
	t--;
	return (c*(t*t*t + 1) + b);
}

//--------------------------------------------------------------
float ofApp::easeLinear(float t, float b, float c, float d) {
	return (c*t / d + b);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
