//
//  CensureParam.h
//  Pictogram
//
//  Created by Chat F Mak on 4/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_CensureParam_h
#define Pictogram_CensureParam_h

class CensureParam
{
public:
    int imageSize;
    float nonMaxSuppThreshold;
    float harrisThreshold;
    
    CensureParam()
    {
        
    }
    
    CensureParam(int imgSize, float nonMaxSuppressionThreshold, float harrisLineTestThreshold)
    {
        imageSize = imgSize;
        nonMaxSuppThreshold = nonMaxSuppressionThreshold;
        harrisThreshold = harrisLineTestThreshold;
    }
};

#endif
