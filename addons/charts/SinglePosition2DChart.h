//
//  SinglePosition2DChart.h
//  BodyAsPen
//
//  Created by Joel Gethin Lewis on 04/10/2011.
//  Copyright 2011 Hellicar&Lewis. All rights reserved.
//

#ifndef BodyAsPen_SinglePosition2DChart_h
#define BodyAsPen_SinglePosition2DChart_h

#include "ofMain.h"
#include "ofxControlPanel.h"
#include "PositionAndTime.h"

class SinglePosition2DChart{
public:
    SinglePosition2DChart(){
        minPosition = ofVec2f(0,0);
        maxPosition = ofVec2f(1.f,1.f);
        
        dimensions = ofVec2f(640.f,480.f);
        
        name = "Unamed Position Chart";
        
        gui = NULL;
    }
    
    ~SinglePosition2DChart(){};
    
    void update(ofVec2f newPosition, ofxControlPanel* _gui){
        
        gui = _gui;
        
        int historyLengthFromGUI = gui->getValueI("HISTORY_LENGTH");
        
        while(positions.size() > historyLengthFromGUI){ //let it grow, only shrink if it gets too big, rather than resize
            positions.erase(positions.begin());
        }
        
        ofVec2f range = maxPosition - minPosition;
        
        float relativeXPosition = newPosition.x / range.x;
        float relativeYPosition = newPosition.y / range.y;  
        
        ofVec2f relativeNewPosition = ofVec2f(relativeXPosition,relativeYPosition);
        
        float time = ofGetElapsedTimef();
        
        PositionAndTime latest;
        
        latest.position = relativeNewPosition;
        latest.time = time;
        
        positions.push_back(latest);
    }
	void drawRibbon(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
//not clearing the background
//        ofFill();
//        ofSetColor(0, 0, 0);
//        ofRect(0,0,w,h); //draw a black box in the background
//        ofNoFill();
//        ofSetColor(64,64,64);
//        ofRect(0,0,w,h); //draw a dark grey outline
        //ofSetColor(0, 0, 255);
        //ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        
        //http://en.wikipedia.org/wiki/Triangle_strip
        /*
         glBegin(GL_TRIANGLE_STRIP);      
         glVertex3f( 0.0f, 0.0f, 0.0f ); //vertex 1
         glVertex3f( 0.0f, 1.0f, 0.0f ); //vertex 2
         glVertex3f( 1.0f, 0.0f, 0.0f ); //vertex 3
         glVertex3f( 1.5f, 1.0f, 0.0f ); //vertex 4
         glEnd();
         */
        
        if(positions.size() > 1){ //need at least two elements to be able to work out velocity between points...
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            ofSetColor(0, 0, 0, lineAlpha);
            
            glBegin(GL_TRIANGLE_STRIP);
            
            for(int i=1; i < positions.size(); i++){
                if(gui->getValueB("DO_HISTORY_FADE")){
                    float distanceThroughHistory = ((float)i/(float)positions.size());
                    float finalAlpha = distanceThroughHistory*lineAlpha;
                    
                    ofSetColor(0, 0, 0, finalAlpha);
                }
                
                ofVec2f distanceTravelled = positions[i].position - positions[i-1].position;
                
                float differenceInTime = positions[i].time - positions[i].time;
                
                ofVec2f velocity = distanceTravelled / differenceInTime;
                
                float magnitude = velocity.length();
                
                magnitude *= 1000.f; //scale it up a bit
                
                float scaledX = positions[i].position.x * w;
                float scaledY = positions[i].position.y * h;
                
                glVertex3f(scaledX + magnitude/2.f, scaledY + magnitude/2.f, 0.f);
                glVertex3f(scaledX - magnitude/2.f, scaledY - magnitude/2.f, 0.f);
            }
            
            glEnd();
        }
        
        ofDisableAlphaBlending();
        ofDisableSmoothing();
        ofPopMatrix();
        ofPopStyle();
    }
	void drawRibbon(const ofPoint & point){
		drawRibbon( point.x, point.y);
	}
	void drawRibbon(const ofRectangle & rect){
		drawRibbon(rect.x, rect.y, rect.width, rect.height); 
	}
    void drawRibbon(float x,float y){
        drawRibbon(x,y,dimensions.x, dimensions.y);
    }    
    void drawRibbon(){
        drawRibbon(0,0,dimensions.x, dimensions.y);
    }        
	void drawCircles(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
//not clearing the background
//        ofFill();
//        ofSetColor(0, 0, 0);
//        ofRect(0,0,w,h); //draw a black box in the background
//        ofNoFill();
//        ofSetColor(64,64,64);
//        ofRect(0,0,w,h); //draw a dark grey outline
        //ofSetColor(0, 0, 255);
        //ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        
        if(positions.size() > 1){ //need at least two elements to be able to work out velocity between points...
            
            float lineScale = gui->getValueF("LINE_SCALE");
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            float lineWidth = gui->getValueF("LINE_WIDTH");
            
            ofSetColor(0, 0, 0, lineAlpha);
            ofSetLineWidth(lineWidth); //works for circles?
            
            for(int i=1; i < positions.size(); i++){
                if(gui->getValueB("DO_HISTORY_FADE")){
                    float distanceThroughHistory = ((float)i/(float)positions.size());
                    float finalAlpha = distanceThroughHistory*lineAlpha;
                    
                    ofSetColor(0, 0, 0, finalAlpha);
                }
                
                ofVec2f distanceTravelled = positions[i].position - positions[i-1].position;
                
                float differenceInTime = positions[i].time - positions[i].time;
                
                ofVec2f velocity = distanceTravelled / differenceInTime;
                
                float magnitude = velocity.length();
                
                magnitude *= 100.f; //scale it up a bit
                
                magnitude *= lineScale;
                
                float scaledX = positions[i].position.x * w;
                float scaledY = positions[i].position.y * h;
                
                ofNoFill(); //just outline?
                ofCircle(scaledX, scaledY, magnitude);
                //ofNoFill();
            }
        }
        
        ofDisableAlphaBlending();
        ofDisableSmoothing();
        ofPopMatrix();
        ofPopStyle();
    }
	void drawCircles(const ofPoint & point){
		drawCircles( point.x, point.y);
	}
	void drawCircles(const ofRectangle & rect){
		drawCircles(rect.x, rect.y, rect.width, rect.height); 
	}
    void drawCircles(float x,float y){
        drawCircles(x,y,dimensions.x, dimensions.y);
    }    
    void drawCircles(){
        drawCircles(0,0,dimensions.x, dimensions.y);
    }    
	void drawHorizontalLines(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
        //not clearing the background
        //        ofFill();
        //        ofSetColor(0, 0, 0);
        //        ofRect(0,0,w,h); //draw a black box in the background
        //        ofNoFill();
        //        ofSetColor(64,64,64);
        //        ofRect(0,0,w,h); //draw a dark grey outline
        //ofSetColor(0, 0, 0);
        //ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        
        if(positions.size() > 1){ //need at least two elements to be able to work out velocity between points...
            
            float lineScale = gui->getValueF("LINE_SCALE");
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            float lineWidth = gui->getValueF("LINE_WIDTH");
            
            ofSetColor(0, 0, 0, lineAlpha);
            ofSetLineWidth(lineWidth);
            
            for(int i=1; i < positions.size(); i++){
                if(gui->getValueB("DO_HISTORY_FADE")){
                    float distanceThroughHistory = ((float)i/(float)positions.size());
                    float finalAlpha = distanceThroughHistory*lineAlpha;
                    
                    ofSetColor(0, 0, 0, finalAlpha);
                }
                
                ofVec2f distanceTravelled = positions[i].position - positions[i-1].position;
                
                float differenceInTime = positions[i].time - positions[i].time;
                
                ofVec2f velocity = distanceTravelled / differenceInTime;
                
                float magnitude = velocity.length();
                
                magnitude *= 1000.f; //scale it up a bit
                
                magnitude *= lineScale;
                
                float scaledX = positions[i].position.x * w;
                float scaledY = positions[i].position.y * h;
                
                ofLine(scaledX - (magnitude / 2.f), scaledY, scaledX + (magnitude / 2.f), scaledY);
            }
        }
        
        ofDisableAlphaBlending();
        ofDisableSmoothing();
        ofPopMatrix();
        ofPopStyle();
    }
	void drawHorizontalLines(const ofPoint & point){
		drawHorizontalLines( point.x, point.y);
	}
	void drawHorizontalLines(const ofRectangle & rect){
		drawHorizontalLines(rect.x, rect.y, rect.width, rect.height); 
	}
    void drawHorizontalLines(){
        drawHorizontalLines(0,0,dimensions.x, dimensions.y);
    }  
    void drawHorizontalLines(float x,float y){
        drawHorizontalLines(x,y,dimensions.x, dimensions.y);
    }    
	void drawVerticalLines(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
        //not clearing the background
        //        ofFill();
        //        ofSetColor(0, 0, 0);
        //        ofRect(0,0,w,h); //draw a black box in the background
        //        ofNoFill();
        //        ofSetColor(64,64,64);
        //        ofRect(0,0,w,h); //draw a dark grey outline
        //ofSetColor(0, 0, 255);
        //ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        
        if(positions.size() > 1){ //need at least two elements to be able to work out velocity between points...
            
            float lineScale = gui->getValueF("LINE_SCALE");
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            float lineWidth = gui->getValueF("LINE_WIDTH");
            
            ofSetColor(0, 0, 0, lineAlpha);
            ofSetLineWidth(lineWidth);
            
            for(int i=1; i < positions.size(); i++){
                if(gui->getValueB("DO_HISTORY_FADE")){
                    float distanceThroughHistory = ((float)i/(float)positions.size());
                    float finalAlpha = distanceThroughHistory*lineAlpha;
                    
                    ofSetColor(0, 0, 0, finalAlpha);
                }
                
                ofVec2f distanceTravelled = positions[i].position - positions[i-1].position;
                
                float differenceInTime = positions[i].time - positions[i].time;
                
                ofVec2f velocity = distanceTravelled / differenceInTime;
                
                float magnitude = velocity.length();
                
                magnitude *= 1000.f; //scale it up a bit
                
                magnitude *= lineScale;
                
                float scaledX = positions[i].position.x * w;
                float scaledY = positions[i].position.y * h;
                
                ofLine(scaledX, scaledY - (magnitude / 2.f), scaledX, scaledY + (magnitude / 2.f));
            }
        }
        
        ofDisableAlphaBlending();
        ofDisableSmoothing();
        ofPopMatrix();
        ofPopStyle();
    }
	void drawVerticalLines(const ofPoint & point){
		drawVerticalLines( point.x, point.y);
	}
	void drawVerticalLines(const ofRectangle & rect){
		drawVerticalLines(rect.x, rect.y, rect.width, rect.height); 
	}
    void drawVerticalLines(float x,float y){
        drawVerticalLines(x,y,dimensions.x, dimensions.y);
    }    
    void drawVerticalLines(){
        drawVerticalLines(0,0,dimensions.x, dimensions.y);
    }    
	void draw(float x,float y,float w, float h){
        ofPushStyle();
        ofPushMatrix();
        
        ofTranslate(x,y);
        //not clearing the background
        //        ofFill();
        //        ofSetColor(0, 0, 0);
        //        ofRect(0,0,w,h); //draw a black box in the background
        //        ofNoFill();
        //        ofSetColor(64,64,64);
        //        ofRect(0,0,w,h); //draw a dark grey outline
        //ofSetColor(0, 0, 255);
        //ofDrawBitmapString(name, 20.f, 20.f); //and the name
        
        ofEnableSmoothing();
        ofEnableAlphaBlending();
        ofNoFill();
        
        if(positions.size() > 0){
            float lineScale = gui->getValueF("LINE_SCALE");
            int lineAlpha = gui->getValueI("LINE_ALPHA");
            float lineWidth = gui->getValueF("LINE_WIDTH");
            
            ofSetColor(0, 0, 0, lineAlpha);
            ofSetLineWidth(lineWidth);
            
            for(int i=0; i < positions.size()-1; i++){
                ofBeginShape(); //doing the begin and end shape inside the for loop to get the alpha fade working nicely
                if(gui->getValueB("DO_HISTORY_FADE")){
                    float distanceThroughHistory = ((float)i/(float)positions.size());
                    float finalAlpha = distanceThroughHistory*(float)lineAlpha;
                    
                    ofSetColor(0, 0, 0, finalAlpha);
                }
                //ofCurveVertex(positions[i].position.x*w, positions[i].position.y*h); //black curve of points.....
                //ofCurveVertex(positions[i+1].position.x*w, positions[i+1].position.y*h); 
                ofVertex(positions[i].position.x*w, positions[i].position.y*h); //black line of points.....
                ofVertex(positions[i+1].position.x*w, positions[i+1].position.y*h);
                ofEndShape(false);
            }
            
            
            float dotRadius = 8.f;
            float scaledXPosition = positions.back().position.x * w;
            float scaledYPosition = positions.back().position.y * h;
            

            //no more red...
            //            ofSetColor(255, 0, 0); //the position circle is red
//            ofSetLineWidth(1.f);
//            
//            ofCircle(scaledXPosition, scaledYPosition, dotRadius);
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
    vector<PositionAndTime> positions;
    ofxControlPanel* gui;
    
};

#endif
