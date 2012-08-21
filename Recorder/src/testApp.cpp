#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetFrameRate(60);
    
    openNIRecorder.setup();
    openNIRecorder.addDepthGenerator();
    openNIRecorder.addImageGenerator();
    openNIRecorder.setRegister(true);
    openNIRecorder.setMirror(true);
    openNIRecorder.addUserGenerator();
    openNIRecorder.setMaxNumUsers(1); //1 for this one....
    
    // uncomment these if you want to ensure a clean exit from the application!
    openNIRecorder.setSafeThreading(true);
    
    //start with the live image from the kinect to initialise the gui live image preview elements
    
    theOpenNI = &openNIRecorder;
    
    setupGUI();
    
    for(int i=0; i<16; i++){ //16 is hardcoded for the number of limbs
        SinglePosition2DChart new2DChart;
        new2DChart.maxPosition = ofVec2f(700.f,500.f);
        bodyCharts.push_back(new2DChart);
    }
    
    hipChart.maxPosition = ofVec2f(700.f,500.f);
    shoulderChart.maxPosition = ofVec2f(700.f,500.f);
    handsChart.maxPosition = ofVec2f(700.f,500.f);
}

//--------------------------------------------------------------
void testApp::update(){
    
    elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();
    
    int numberOfTrackedUsers = 0;
    
    skeletonDrawer.setTheOpenNI(theOpenNI); //making sure the GUI has the right version...
    
    theOpenNI->update();
    numberOfTrackedUsers = theOpenNI->getNumTrackedUsers();
    
    //cout << "Number of tracked users is " << numberOfTrackedUsers << endl;
    
    if( numberOfTrackedUsers > 0){
        
        ofxOpenNIUser user = theOpenNI->getTrackedUser(0); //get the 0th
        
        int numberOfLimbs = user.getNumLimbs();
        
        //cout << "Number of limbs is " << numberOfLimbs << endl;
        
        for(int i = 0; i < numberOfLimbs; i++){
            ofxOpenNILimb limb = user.limbs[i];
            ofxOpenNIJoint joint = limb.getEndJoint();
            
            ofPoint point = joint.getProjectivePosition();
            
            ofVec2f newPosition = ofVec2f(point.x, point.y);
            
            bodyCharts[i].update(newPosition, &gui);
        }
        
        //now update the hip,shoulder and hands charts
        
        //hip first
        
        ofxOpenNIJoint leftHip = user.getJoint(JOINT_LEFT_HIP);
        ofPoint leftHipPoint = leftHip.getProjectivePosition();
        ofVec2f newLeftHipPosition = ofVec2f(leftHipPoint.x, leftHipPoint.y);
        
        ofxOpenNIJoint rightHip = user.getJoint(JOINT_RIGHT_HIP);
        ofPoint rightHipPoint = rightHip.getProjectivePosition();
        ofVec2f newRightHipPosition = ofVec2f(rightHipPoint.x, rightHipPoint.y);
        
        hipChart.update(newLeftHipPosition, newRightHipPosition, &gui);
        
        //shoulder first
        
        ofxOpenNIJoint leftShoulder = user.getJoint(JOINT_LEFT_SHOULDER);
        ofPoint leftShoulderPoint = leftShoulder.getProjectivePosition();
        ofVec2f newLeftShoulderPosition = ofVec2f(leftShoulderPoint.x, leftShoulderPoint.y);
        
        ofxOpenNIJoint rightShoulder = user.getJoint(JOINT_RIGHT_SHOULDER);
        ofPoint rightShoulderPoint = rightShoulder.getProjectivePosition();
        ofVec2f newRightShoulderPosition = ofVec2f(rightShoulderPoint.x, rightShoulderPoint.y);
        
        shoulderChart.update(newLeftShoulderPosition, newRightShoulderPosition, &gui);
        
        //then hands
        
        ofxOpenNIJoint leftHand = user.getJoint(JOINT_LEFT_HAND);
        ofPoint leftHandPoint = leftHand.getProjectivePosition();
        ofVec2f newLeftHandPosition = ofVec2f(leftHandPoint.x, leftHandPoint.y);
        
        ofxOpenNIJoint rightHand = user.getJoint(JOINT_RIGHT_HAND);
        ofPoint rightHandPoint = rightHand.getProjectivePosition();
        ofVec2f newRightHandPosition = ofVec2f(rightHandPoint.x, rightHandPoint.y);
        
        handsChart.update(newLeftHandPosition, newRightHandPosition, &gui);
    }
    
    gui.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    bool shouldBeLive = gui.getValueB("SHOULD_BE_LIVE");
    
    ofBackground(255,255,255);
    
    float windowWidth = ofGetWidth();
    float windowHeight = ofGetHeight();
    float ratioBetweenWindowWidthAndCapture = windowWidth/640.f;
    float ratioBetweenWindowHeightAndCapture = windowHeight/480.f;
    
    ofSetColor(0, 0, 0);
    
    if(gui.getValueB("DISPLAY_ALL_POINTS")){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].draw(0,0,windowWidth,windowHeight);
        } 
    }
    
    if(gui.getValueB("DRAW_VERTICAL_LINES")){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawVerticalLines(0,0,windowWidth,windowHeight);
        }
    }
    
    if(gui.getValueB("DRAW_HORIZONTAL_LINES")){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawHorizontalLines(0,0,windowWidth,windowHeight);
        }
    }
    
    if(gui.getValueB("DRAW_CIRCLES")){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawCircles(0,0,windowWidth,windowHeight);
        }
    } 
    
    if(gui.getValueB("DRAW_RIBBON")){
        for(int i=0; i< (int) bodyCharts.size(); i++){
            bodyCharts[i].drawRibbon(0,0,windowWidth,windowHeight);
        }
    }
    
    if(gui.getValueB("HIP_LINE")){
        hipChart.draw(0,0,windowWidth,windowHeight);
    }
    
    if(gui.getValueB("SHOULDER_LINE")){
        shoulderChart.draw(0,0,windowWidth,windowHeight);
    }
    
    if(gui.getValueB("HANDS_LINE")){
        handsChart.draw(0,0,windowWidth,windowHeight);
    }
    
    if(gui.getValueB("HIP_LINE_EDGE")){
        hipChart.drawToEdgeOfScreen(0,0,windowWidth,windowHeight);
    }
    
    if(gui.getValueB("SHOULDER_LINE_EDGE")){
        shoulderChart.drawToEdgeOfScreen(0,0,windowWidth,windowHeight);
    }
    
    if(gui.getValueB("HANDS_LINE_EDGE")){
        handsChart.drawToEdgeOfScreen(0,0,windowWidth,windowHeight);
    }
    
    ofSetColor(255, 255, 255);
    
    gui.draw();    
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
    
    int cloudRes = -1;
    
    bool control_panel_ate_key = gui.keyPressed( key );
    
    if ( !control_panel_ate_key ){   
        switch (key) {
            case 'f':{
                ofToggleFullscreen();
                break;				
            }
            case 'h':{
                gui.toggleView();	
                break;
            }
            case 'm':{
                if(bShowMouse){
                    ofHideCursor();
                    bShowMouse = false;
                }else {
                    ofShowCursor();
                    bShowMouse = true;
                } 
            }
                break;
            case ' ':
                if(!openNIRecorder.isRecording()){
                    string theNewFileName;
                    
                    theNewFileName = ofGetTimestampString();
                    
                    theNewFileName.append(".oni");
                    
                    openNIRecorder.startRecording(ofToDataPath(theNewFileName));
                }else{
                    openNIRecorder.stopRecording();
                }
                break;
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
	if(gui.mouseDragged(x, y, button)){
        //then its within the gui area
    }else{
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	if(gui.mousePressed(x, y, button)){
        //then its within the gui area
    }else{
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	gui.mouseReleased();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

void testApp::setupGUI(){
    ofxControlPanel::setBackgroundColor(simpleColor(30, 30, 30, 200));
	ofxControlPanel::setTextColor(simpleColor(240, 240, 240, 255));
    
    int guiWidth = 800;
    int guiHeight = 800;
    
    gui.setup("Sync", 0, 0, guiWidth, guiHeight, true); // string controlPanelName, float panelX, float panelY, float width, float height, bool doSaveRestore
	gui.addPanel("Line", 3, false); // name, number of columns, locked
	gui.addPanel("Tracking", 3, false); // name, number of columns, locked    
    gui.addPanel("App", 3, false); //just doing playback/recording on keyboard for now.....
    
	//--------- PANEL 0, Line Variables
	gui.setWhichPanel("Line");
	gui.setWhichColumn(0);
    gui.addSlider("Length of History", "HISTORY_LENGTH", 100, 2, 200, true);
    gui.addToggle("Fade History?", "DO_HISTORY_FADE", 1);
    gui.addSlider("Line scale", "LINE_SCALE", 1.f, 1.f, 1000.f, false);
    gui.addSlider("Line width", "LINE_WIDTH", 1.f, 1.f, 20.f, false);
    gui.addSlider("Line alpha", "LINE_ALPHA", 128, 1, 255, true); 
    
	//--------- PANEL 1, Tracking Variables
	gui.setWhichPanel("Tracking");
	gui.setWhichColumn(0);
    
    skeletonDrawer.setTheOpenNI(theOpenNI);
    //gui.addDrawableRect("Video Image", &theOpenNI->getimageTextureReference(), 320, 240);
    gui.addCustomRect("Video and skeleton", &skeletonDrawer, 320, 240);
    gui.addDrawableRect("Depth Image", &theOpenNI->getDepthTextureReference(), 320, 240);
    gui.setWhichColumn(2);
    gui.addToggle("All points tracking?", "DISPLAY_ALL_POINTS", 1);
    gui.addToggle("Hip Line?", "HIP_LINE", 0);
    gui.addToggle("Shoulder Line?", "SHOULDER_LINE", 0);
    gui.addToggle("Hands Line", "HANDS_LINE", 0);  
    gui.addToggle("Hip Line to edge?", "HIP_LINE_EDGE", 0);
    gui.addToggle("Shoulder Line to edge?", "SHOULDER_LINE_EDGE", 0);
    gui.addToggle("Hands Line to edge", "HANDS_LINE_EDGE", 0);  
    gui.addToggle("All points: vertical lines", "DRAW_VERTICAL_LINES", 0);
    gui.addToggle("All points: horizontal lines", "DRAW_HORIZONTAL_LINES", 0); 
    gui.addToggle("All points: circles", "DRAW_CIRCLES", 0); 
    gui.addToggle("All points: ribbon ", "DRAW_RIBBON", 0);  
  
    
    //--------- PANEL 2, App Variables
	gui.setWhichPanel("App");
	gui.setWhichColumn(0);
//	gui.addToggle("Live?", "SHOULD_BE_LIVE", 1);
//    gui.addToggle("Record?", "SHOULD_BE_RECORDING", 0);    
	
	//some dummy vars we will update to show the variable lister object
	elapsedTime		= ofGetElapsedTimef();
	appFrameCount	= ofGetFrameNum();	
	appFrameRate	= ofGetFrameRate();
    
	vector <guiVariablePointer> vars;
	vars.push_back( guiVariablePointer("elapsed time", &elapsedTime, GUI_VAR_FLOAT, 2) );
	vars.push_back( guiVariablePointer("elapsed frames", &appFrameCount, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("app fps", &appFrameRate, GUI_VAR_FLOAT, 2) );
    
	vars.push_back( guiVariablePointer("mouse x", &mouseX, GUI_VAR_INT) );
	vars.push_back( guiVariablePointer("mouse y", &mouseY, GUI_VAR_INT) );
    
	gui.addVariableLister("app vars", vars);
	
	gui.addChartPlotter("App FPS", guiStatVarPointer("app fps", &appFrameRate, GUI_VAR_FLOAT, true, 2), 200, 100, 200, 10, 50);
	
	//SETTINGS AND EVENTS
    
	//load from xml!
	gui.loadSettings("controlPanelSettings.xml");
    
	//if you want to use events call this after you have added all your gui elements
	gui.setupEvents();
	gui.enableEvents();
    
    bShowMouse = true;
    
    //gui.toggleView(); //show it by default
}
