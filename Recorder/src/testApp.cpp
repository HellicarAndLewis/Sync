#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    openNIRecorder.setup();
    openNIRecorder.addDepthGenerator();
    openNIRecorder.addImageGenerator();
    openNIRecorder.setRegister(true);
    openNIRecorder.addUserGenerator();
    openNIRecorder.setMaxNumUsers(1);
    openNIRecorder.start();
    
    ofSetVerticalSync(true);
	ofEnableSmoothing();
    
    //set up the charts to draw the motion on screen...
    for(int i=0; i<16; i++){ //16 is hardcoded for the number of limbs
        SinglePosition2DChart new2DChart;
        new2DChart.maxPosition = ofVec2f(700.f,500.f);
        bodyCharts.push_back(new2DChart);
    }
    
    hipChart.maxPosition = ofVec2f(700.f,500.f);
    shoulderChart.maxPosition = ofVec2f(700.f,500.f);
    handsChart.maxPosition = ofVec2f(700.f,500.f);
    
    //new official GUI from oF dev branch
    gui.setup("Sync"); // most of the time you don't need a name
    gui.add(foreground.setup("foreground", ofColor(0,0,0),ofColor(0,0),ofColor(255,255)));
    gui.add(background.setup("background", ofColor(255,255,255),ofColor(0,0),ofColor(255,255)));
    gui.add(lengthOfHistory.setup("length of history", 100,2,200));
    gui.add(fadeHistory.setup("fade history?", false));
    gui.add(lineScale.setup("line scale", 1.f, 1.f, 1000.f));
    gui.add(lineWidth.setup("line width", 1.f, 1.f ,20.f));
    gui.add(lineAlpha.setup("line alpha", 255.f,0.f,255.f));
    gui.add(allPointsTracking.setup("All points", true));
    gui.add(hipLine.setup("Hip line", false));
    gui.add(shoulderLine.setup("Shoulder line", false));
    gui.add(handsLine.setup("Hand line", false));
    gui.add(hipLineToEdge.setup("Hip line to edge", false));
    gui.add(shoulderLineToEdge.setup("Shoulder line to edge", false));
    gui.add(handsLineToEdge.setup("Hand line to edge", false));
    gui.add(allPointsVerticalLines.setup("Vertical lines", false));
    gui.add(allPointsHorizontalLines.setup("Horizontal lines", false));
    gui.add(allPointsCircles.setup("Circles", false));
    gui.add(allPointsRibbon.setup("Ribbon", false));
    gui.add(drawVideo.setup("Draw video", false));
    gui.add(drawDepth.setup("Draw depth", false));
    
    bHide = false;
}

//--------------------------------------------------------------
void testApp::update(){
    openNIRecorder.update();
    
    int numberOfTrackedUsers = openNIRecorder.getNumTrackedUsers();
    
    if( numberOfTrackedUsers > 0){
        
        ofxOpenNIUser user = openNIRecorder.getTrackedUser(0); //get the 0th
        
        int numberOfLimbs = user.getNumLimbs();
        
        //cout << "Number of limbs is " << numberOfLimbs << endl;
        
        for(int i = 0; i < numberOfLimbs; i++){
            ofxOpenNILimb limb = user.limbs[i];
            ofxOpenNIJoint joint = limb.getEndJoint();
            
            ofPoint point = joint.getProjectivePosition();
            
            ofVec2f newPosition = ofVec2f(point.x, point.y);
            
            bodyCharts[i].update(newPosition, lengthOfHistory);
        }
        
        //now update the hip,shoulder and hands charts
        
        //hip first
        
        ofxOpenNIJoint leftHip = user.getJoint(JOINT_LEFT_HIP);
        ofPoint leftHipPoint = leftHip.getProjectivePosition();
        ofVec2f newLeftHipPosition = ofVec2f(leftHipPoint.x, leftHipPoint.y);
        
        ofxOpenNIJoint rightHip = user.getJoint(JOINT_RIGHT_HIP);
        ofPoint rightHipPoint = rightHip.getProjectivePosition();
        ofVec2f newRightHipPosition = ofVec2f(rightHipPoint.x, rightHipPoint.y);
        
        hipChart.update(newLeftHipPosition, newRightHipPosition, lengthOfHistory);
        
        //shoulder first
        
        ofxOpenNIJoint leftShoulder = user.getJoint(JOINT_LEFT_SHOULDER);
        ofPoint leftShoulderPoint = leftShoulder.getProjectivePosition();
        ofVec2f newLeftShoulderPosition = ofVec2f(leftShoulderPoint.x, leftShoulderPoint.y);
        
        ofxOpenNIJoint rightShoulder = user.getJoint(JOINT_RIGHT_SHOULDER);
        ofPoint rightShoulderPoint = rightShoulder.getProjectivePosition();
        ofVec2f newRightShoulderPosition = ofVec2f(rightShoulderPoint.x, rightShoulderPoint.y);
        
        shoulderChart.update(newLeftShoulderPosition, newRightShoulderPosition, lengthOfHistory);
        
        //then hands
        
        ofxOpenNIJoint leftHand = user.getJoint(JOINT_LEFT_HAND);
        ofPoint leftHandPoint = leftHand.getProjectivePosition();
        ofVec2f newLeftHandPosition = ofVec2f(leftHandPoint.x, leftHandPoint.y);
        
        ofxOpenNIJoint rightHand = user.getJoint(JOINT_RIGHT_HAND);
        ofPoint rightHandPoint = rightHand.getProjectivePosition();
        ofVec2f newRightHandPosition = ofVec2f(rightHandPoint.x, rightHandPoint.y);
        
        handsChart.update(newLeftHandPosition, newRightHandPosition, lengthOfHistory);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(background);
    float windowWidth = ofGetWidth();
    float windowHeight = ofGetHeight();
    
    //draw the playback over the top for debugging for now
    ofSetColor(ofColor::white);
    
    ofPushMatrix();
    
    //openNIRecorder(0, 0);
    
    if(drawVideo){
        openNIRecorder.drawImage(0, 0, windowWidth, windowHeight);
    }
    
    if(drawDepth){
        openNIRecorder.drawDepth(0, 0, windowWidth, windowHeight);
    }
    
    ofPopMatrix();
    
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    ofSetColor(foreground);
    
    ofColor theColourToUse = foreground;
    
    ofFill();
    
    if(allPointsTracking){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].draw(0,0,windowWidth,windowHeight, lineScale, lineAlpha, lineWidth, fadeHistory, theColourToUse);
        }
    }
    
    if(allPointsVerticalLines){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawVerticalLines(0,0,windowWidth,windowHeight, lineScale, lineAlpha, lineWidth, fadeHistory, theColourToUse);
        }
    }
    
    if(allPointsHorizontalLines){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawHorizontalLines(0, 0, windowWidth, windowHeight, lineScale, lineAlpha, lineWidth, fadeHistory, theColourToUse);
        }
    }
    
    if(allPointsCircles){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawCircles(0, 0, windowWidth, windowHeight, lineScale, lineAlpha, lineWidth,fadeHistory, theColourToUse);
        }
    }
    
    if(allPointsRibbon){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawRibbon(0, 0, windowWidth, windowHeight, lineAlpha, fadeHistory, theColourToUse);
        }
    }
    
    if(hipLine){
        hipChart.draw(0, 0, windowWidth, windowHeight, lineAlpha, lineWidth, fadeHistory, theColourToUse);
    }
    
    if(shoulderLine){
        shoulderChart.draw(0, 0, windowWidth, windowHeight, lineAlpha, lineWidth, fadeHistory, theColourToUse);
    }
    
    if(handsLine){
        handsChart.draw(0, 0, windowWidth, windowHeight, lineAlpha, lineWidth, fadeHistory, theColourToUse);
    }
    
    if(hipLineToEdge){
        hipChart.drawToEdgeOfScreen(0, 0, windowWidth, windowHeight, lineAlpha, lineWidth, theColourToUse);
    }
    
    if(shoulderLineToEdge){
        shoulderChart.drawToEdgeOfScreen(0, 0, windowWidth, windowHeight, lineAlpha, lineWidth, theColourToUse);
    }
    
    if(handsLineToEdge){
        handsChart.drawToEdgeOfScreen(0, 0, windowWidth, windowHeight, lineAlpha, lineWidth, theColourToUse);
    }
    
	// auto draw?
	// should the gui control hiding?
	if( !bHide){
		gui.draw();
	}
}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::gestureEvent(ofxOpenNIGestureEvent & event){
    ofLogNotice() << event.gestureName << getGestureStatusAsString(event.gestureStatus) << "from device" << event.deviceID << "at" << event.timestampMillis;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIRecorder.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    switch (key) {
        case ' ':
            {
                string newFilename = ofGetTimestampString();
                newFilename = newFilename+".oni";
                
//                if(openNIRecorder.isPlaying()){
//                    //lets stop it
//                    openNIRecorder.stop();
//                    //then start it again
//                    openNIRecorder.setup();
//                    openNIRecorder.addDepthGenerator();
//                    openNIRecorder.addImageGenerator();
//                    openNIRecorder.setRegister(true);
//                    openNIRecorder.setMirror(true);
//                    openNIRecorder.addUserGenerator();
//                    openNIRecorder.setMaxNumUsers(1);
//                    openNIRecorder.start();
//                }
                
                
                if(!openNIRecorder.isRecording()){
                    openNIRecorder.startRecording(ofToDataPath(newFilename));
                }else{
                    openNIRecorder.stopRecording();
                }
                break;
            }
        case 'l':
            {
                ofFileDialogResult results = ofSystemLoadDialog();
                
                openNIRecorder.setupFromONI(results.filePath);
                
                openNIRecorder.addDepthGenerator();
                openNIRecorder.addImageGenerator();
                openNIRecorder.setRegister(true);
                openNIRecorder.setMirror(true);
                openNIRecorder.addUserGenerator();
                openNIRecorder.setMaxNumUsers(1); //1 for this one....

                break;
            }
        case 'h':
        {
            bHide = !bHide;
        }
            break;
        case 's':
        {
            gui.saveToFile("settings.xml");
        }
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    
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