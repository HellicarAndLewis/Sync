/*
 *  customDrawer.h
 *  ofxControlPanelDemo
 *
 *  Created by theo on 02/04/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "guiCustomImpl.h"
#include "guiBaseObject.h" //theo added for isInsideRect()
#include "ofxOpenNI.h"

class videoAndSkeletonCustomDrawer : public guiCustomImpl {
	
private:
	
    ofxOpenNI* theOpenNI;
	
public:
	
	bool mouseIsRelative;
	//ofRectangle boundingRect; //theo changed
	
	videoAndSkeletonCustomDrawer(){
	
	}
    
    void setTheOpenNI(ofxOpenNI* anOpenNI){
        theOpenNI = anOpenNI;
    }
	
	// -------------------------------------
	void saveSettings(string filename){
	}
	
	void reloadSettings(){
	
	}
	
	void saveSettings(){
	
	}
	
	void loadSettings(string filename) {
	
	}
	
	// -------------------------------------
	float getWidth() {
		return boundingRect.width;
	}
	float getHeight() {
		return boundingRect.height;
	}
	
	void draw(float x, float y, float w, float h) {
		
		//printf("x is %f y is %f w is %f h is %f\n", x, y, w, h);
		//printf("bounds x is %f y is %f w is %f h is %f\n", boundingRect.x, boundingRect.y, boundingRect.width, boundingRect.height);
		
		glPushMatrix();
		glTranslatef(x, y, 0); //theo changed
		
		//always check if you are diving by zero - or do  h/MAX(1, boundingRect.height)
		glScalef(w/boundingRect.width, h/boundingRect.height, 0);

		ofNoFill();
		ofSetColor(255, 255, 255);
        
        theOpenNI->drawImage(0, 0, w, h);
        
        theOpenNI->drawSkeletons(0,0,w,h);
				
		
		glPopMatrix();
		
		ofNoFill();
		ofSetColor(255, 33, 33);
		ofRect(x, y, w, h);
	}
	
	// -------------------------------------
	void mousePressed(float x, float y) {
	}

	//these values come in with 0,0 being the top left of your bounding rect 
	// -------------------------------------
	void mouseDragged(float x, float y){
		
		if( isInsideRect(x, y, ofRectangle(0, 0, boundingRect.width, boundingRect.height) ) ){
		}
	}
	
	void mouseReleased(){
	
	}
	
};