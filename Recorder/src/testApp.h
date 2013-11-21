#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxGui.h"
#include "SinglePosition2DChart.h"
#include "DualPosition2DChart.h"

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

	ofxOpenNI openNIRecorder;
    
    ofTrueTypeFont verdana;
    
    void userEvent(ofxOpenNIUserEvent & event);
    void gestureEvent(ofxOpenNIGestureEvent & event);
    
    ofxColorSlider foreground;
    ofxColorSlider background;
    
    //from old Sync player
    vector<SinglePosition2DChart> bodyCharts;
    
    DualPosition2DChart hipChart;
    DualPosition2DChart shoulderChart;
    DualPosition2DChart handsChart;
    
    //values to bind to GUI
    ofxIntSlider lengthOfHistory; //int 2-200
    ofxToggle fadeHistory;
    ofxFloatSlider lineScale; //float 1-1000
    ofxFloatSlider lineWidth; //float 1-20
    ofxFloatSlider lineAlpha; //float 0-255
    
    ofxToggle allPointsTracking;
    ofxToggle hipLine;
    ofxToggle shoulderLine;
    ofxToggle handsLine;
    ofxToggle hipLineToEdge;
    ofxToggle shoulderLineToEdge;
    ofxToggle handsLineToEdge;
    ofxToggle allPointsVerticalLines;
    ofxToggle allPointsHorizontalLines;
    ofxToggle allPointsCircles;
    ofxToggle allPointsRibbon;
    
    ofxToggle drawVideo;
    ofxToggle drawDepth;
    
    ofxPanel gui;
    
	bool bHide;
    
};

#endif
