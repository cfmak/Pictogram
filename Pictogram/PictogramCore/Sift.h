//
//  Sift.h
//  Pictogram
//
//  Created by Chat F Mak on 5/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_Sift_h
#define Pictogram_Sift_h

#include "SiftDescriptor.h"
#include "Keypoint.h"

class Sift {
protected:    
    //Gaussian weighting scheme, sigma=8
    static float weight[][16];
    
    static int DescriptorSize[8];
    
public:
    Sift();    
    static void GetDescriptor(Keypoint kp, cv::Mat integralBoxImage, SiftDescriptor* desc);
    
protected:
    static float SmoothePixel(int tlx, int tly, int lv, cv::Mat integralBoxImage);
};

#endif
