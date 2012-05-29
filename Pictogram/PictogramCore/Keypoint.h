//
//  Keypoint.h
//  Pictogram
//
//  Created by Chat F Mak on 4/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_Keypoint_h
#define Pictogram_Keypoint_h

class Keypoint {
public:    
    int x;
    int y;
    int lv;
    float v;
    
    Keypoint(int posx, int posy)
    {
        x=posx;
        y=posy;
        lv = -1;
    }
    
    Keypoint(int posx, int posy, int level, float value)
    {
        x=posx;
        y=posy;
        lv=level;
        v = value;
    }
};
#endif
