#pragma once

#include "ofxOpenNI.h"
#include "ofMain.h"

#include "ofxControlPanel.h"
#include "SinglePosition2DChart.h"
#include "DualPosition2DChart.h"
#include "videoAndSkeletonCustomDrawer.h"

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
        void exit();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
	ofxOpenNI openNIRecorder;
    
    ofTrueTypeFont verdana;
    
    void userEvent(ofxOpenNIUserEvent & event);
    void gestureEvent(ofxOpenNIGestureEvent & event);    
    
    void setupGUI();
    
    bool bRecording;
    
    ofxControlPanel gui;
    
    float elapsedTime;
    int appFrameCount;
    float appFrameRate;
    
    bool bShowMouse;
    
    vector<SinglePosition2DChart> bodyCharts;
    
    ofxOpenNI* theOpenNI;
    
    DualPosition2DChart hipChart;
    DualPosition2DChart shoulderChart;
    DualPosition2DChart handsChart;
    
    videoAndSkeletonCustomDrawer skeletonDrawer;
};
