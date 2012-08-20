//
//  DualPosition2DChart.h
//  BodyAsPen
//
//  Created by Joel Gethin Lewis on 04/10/2011.
//  Copyright 2011 Hellicar&Lewis. All rights reserved.
//

#ifndef BodyAsPen_DualPosition2DChart_h
#define BodyAsPen_DualPosition2DChart_h

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "PositionAndTime.h"
#include "LineEquation.h"

class DualPosition2DChart{
public:
    DualPosition2DChart(){
        minPosition = ofVec2f(0.f,0.f);
        maxPosition = ofVec2f(1.f,1.f);
        
        dimensions = ofVec2f(320.f,240.f);
        
        name = "Unamed DualPosition2DChart";
        
        gui = NULL;
    }
    
    ~DualPosition2DChart(){};
    
    void update(ofVec2f newFirstPosition, ofVec2f newSecondPosition, ofxControlPanel* _gui){
        
        gui = _gui;
        
        int historyLengthFromGUI = gui->getValueI("HISTORY_LENGTH");
        
        while(dualPositions.size() > historyLengthFromGUI){ //let it grow, only shrink if it gets too big, rather than resize
            dualPositions.erase(dualPositions.begin());
        }
        
        float time = ofGetElapsedTimef();
        
        DualPositionAndTime latest;
        
        latest.firstPosition = scalePosition(newFirstPosition);
        latest.secondPosition = scalePosition(newSecondPosition);
        latest.time = time;
        
        dualPositions.push_back(latest);
    }
    
    ofVec2f scalePosition(ofVec2f positionToScale){
        ofVec2f scaledPosition;
        
        ofVec2f range = maxPosition - minPosition;
        
        scaledPosition.x = positionToScale.x / range.x;
        scaledPosition.y = positionToScale.y / range.y;
        
        return scaledPosition;
    }
    
    void drawToEdgeOfScreen(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
        ofSetColor(0, 0, 255);
        ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        
        if(dualPositions.size() > 0){
            //float lineScale = gui->getValueF("LINE_SCALE"); not using at the moment...
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            float lineWidth = gui->getValueF("LINE_WIDTH");
            
            ofSetColor(0, 0, 0, lineAlpha);
            ofSetLineWidth(lineWidth);
            
            ofVec2f intersectionOnLeft;
            ofVec2f intersectionOnRight;
            
            LineEquation equationFormed;
            
            equationFormed.setFrom2Points(dualPositions.back().firstPosition, dualPositions.back().secondPosition);
            
            intersectionOnLeft.x = 0.f;
            intersectionOnLeft.y = equationFormed.getY(intersectionOnLeft.x);
            
            intersectionOnRight.x = 1.f;
            intersectionOnRight.y = equationFormed.getY(intersectionOnRight.x);
            
            float scaledFirstXPosition = intersectionOnLeft.x * w;
            float scaledFirstYPosition = intersectionOnLeft.y * h;
            float scaledSecondXPosition = intersectionOnRight.x * w;
            float scaledSecondYPosition = intersectionOnRight.y * h;  
            
            ofLine(scaledFirstXPosition, scaledFirstYPosition, scaledSecondXPosition, scaledSecondYPosition);
            
            float dotRadius = 4.f;        
            
            ofSetColor(255, 0, 0, 255); //the position circle is red, full alpha
            
            scaledFirstXPosition = dualPositions.back().firstPosition.x * w;
            scaledFirstYPosition = dualPositions.back().firstPosition.y * h;
            scaledSecondXPosition = dualPositions.back().secondPosition.x * w;
            scaledSecondYPosition = dualPositions.back().secondPosition.y * h;              
            
            ofCircle(scaledFirstXPosition, scaledFirstYPosition, dotRadius);
            ofCircle(scaledSecondXPosition, scaledSecondYPosition, dotRadius);
        }
        
        ofDisableAlphaBlending();
        ofDisableSmoothing();
        ofPopMatrix();
        ofPopStyle();
    }
    
	void drawToEdgeOfScreen(const ofPoint & point){
		drawToEdgeOfScreen( point.x, point.y);
	}
    
	void drawToEdgeOfScreen(const ofRectangle & rect){
		drawToEdgeOfScreen(rect.x, rect.y, rect.width, rect.height); 
	}
    
    void drawToEdgeOfScreen(float x,float y){
        drawToEdgeOfScreen(x,y,dimensions.x, dimensions.y);
    } 
    
    void drawToEdgeOfScreen(){
        drawToEdgeOfScreen(0,0,dimensions.x, dimensions.y);
    }
    
	void draw(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
        ofSetColor(0, 0, 255);
        //ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        
        if(dualPositions.size() > 0){
            //float lineScale = gui->getValueF("LINE_SCALE"); not using at the moment...
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            float lineWidth = gui->getValueF("LINE_WIDTH");
            
            ofSetColor(0, 0, 0, lineAlpha);
            ofSetLineWidth(lineWidth);
            
            float scaledFirstXPosition;
            float scaledFirstYPosition;
            float scaledSecondXPosition;
            float scaledSecondYPosition;  
            
            for(int i=0; i < dualPositions.size(); i++){
                if(gui->getValueB("DO_HISTORY_FADE")){
                    float distanceThroughHistory = ((float)i/(float)dualPositions.size());
                    float finalAlpha = distanceThroughHistory*lineAlpha;
                    
                    ofSetColor(0, 0, 0, finalAlpha);
                }
                
                scaledFirstXPosition = dualPositions[i].firstPosition.x * w;
                scaledFirstYPosition = dualPositions[i].firstPosition.y * h;
                scaledSecondXPosition = dualPositions[i].secondPosition.x * w;
                scaledSecondYPosition = dualPositions[i].secondPosition.y * h;  
                
                ofLine(scaledFirstXPosition, scaledFirstYPosition, scaledSecondXPosition, scaledSecondYPosition); //alpha'd white line of pair of points
            }

            //no dots
//            float dotRadius = 4.f;        
//            
//            ofSetColor(255, 0, 0, 255); //the position circle is red, full alpha
//            
//            scaledFirstXPosition = dualPositions.back().firstPosition.x * w;
//            scaledFirstYPosition = dualPositions.back().firstPosition.y * h;
//            scaledSecondXPosition = dualPositions.back().secondPosition.x * w;
//            scaledSecondYPosition = dualPositions.back().secondPosition.y * h;              
//            
//            ofCircle(scaledFirstXPosition, scaledFirstYPosition, dotRadius);
//            ofCircle(scaledSecondXPosition, scaledSecondYPosition, dotRadius);
        }
        
        ofDisableAlphaBlending();
        ofDisableSmoothing();
        ofPopMatrix();
        ofPopStyle();
    }
    
	void draw(const ofPoint & point){
		draw( point.x, point.y);
	}
    
	void draw(const ofRectangle & rect){
		draw(rect.x, rect.y, rect.width, rect.height); 
	}
    
    void draw(float x,float y){
        draw(x,y,dimensions.x, dimensions.y);
    } 
    
    void draw(){
        draw(0,0,dimensions.x, dimensions.y);
        
        
    }
    
	float getHeight(){
        return dimensions.y;
    }
    
	float getWidth(){
        return dimensions.x;
    }
    
	void setAnchorPercent(float xPct, float yPct){};
    void setAnchorPoint(float x, float y){};
	void resetAnchor(){};
    
    ofVec2f minPosition;
    ofVec2f maxPosition;
    ofVec2f dimensions;
    string name;
private:
    vector<DualPositionAndTime> dualPositions;
    ofxControlPanel* gui;
};

#endif
