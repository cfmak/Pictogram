//
//  Brief.h
//  Pictogram
//
//  Created by Chat F Mak on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_Brief_h
#define Pictogram_Brief_h

#import <OpenCV/opencv2/opencv.hpp>
#include "BriefDescriptor.h"
#include "BriefParam.h"
#include "Keypoint.h"

class Brief {
    //    private int[][] integralBoxImage;
    //    private int bufx;
    //    private int bufy;
private:
    static BriefParam param;
    
    //2D GRV ~ N(0,[1 0; 0 1])
    static const float samplePointsX[][2]; 
    
    //2D GRV ~ N(0,[1 0; 0 1]), samplePointsX[i] != samplePointsY[i]
    static const float samplePointsY[][2]; 
    
    static float SmoothePixel(int x, int y, int lv, cv::Mat& integralBoxImage);
    
    static int ComparePixels(int x1, int y1, 
                             int x2, int y2, 
                             int lv, 
                             cv::Mat&integralBoxImage);
    
public:    
    static void SetBriefParameters(BriefParam& p);
    
    //    public boolean SetIntegralBoxImage(int[][] buffer)
    //    {
    //        if(buffer == null || buffer[0] == null)
    //            return false;
    //        integralBoxImage = buffer;
    //        bufx = integralBoxImage[0].length;
    //        bufy = integralBoxImage.length;
    //        return true;
    //    }
    
    static void GetBriefDescriptor(BriefDescriptor& desc, Keypoint& kp, 
                                   cv::Mat& integralBoxImage);
};


#endif
