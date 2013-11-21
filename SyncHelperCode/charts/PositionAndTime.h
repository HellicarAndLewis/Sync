//
//  PositionAndTime.h
//  BodyAsPen
//
//  Created by Joel Gethin Lewis on 05/10/2011.
//  Copyright 2011 Hellicar&Lewis. All rights reserved.
//

#ifndef BodyAsPen_PositionAndTime_h
#define BodyAsPen_PositionAndTime_h

struct PositionAndTime{
    ofVec2f position;
    float   time;
};

struct DualPositionAndTime{
    ofVec2f firstPosition;
    ofVec2f secondPosition;    
    float   time;
};

#endif
