//
//  CCensureDetector.h
//  Pictogram
//
//  Created by Chat F Mak on 4/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_CCensureDetector_h
#define Pictogram_CCensureDetector_h

#include <OpenCV/opencv2/opencv.hpp>
#include "CensureParam.h"
#include "Keypoint.h"
#include <vector>

class CensureDetector{
public:
    //Octagon dimension
    static const int mnInner[7][2];
    static const int mnOuter[7][2];
    static const float scaleInner[7];
    static const float scaleOuter[7];
    static const int filterSize[7];
    static const int extFilterSize[7];
    
private:
    CensureParam param;

    cv::Mat integralBoxImg;
    cv::Mat integralSlant1Img; // slant /
    cv::Mat integralSlant2Img; // slant \

    float* filterResponse[7];

    cv::Mat buf;
    int bufx;
    int bufy;

    float CensureFilter(int x, int y, int lv);
    int OctagonFilter(int x, int y, int m, int n);
    
    //find local extrema from filterResponse, do harris test
    void FindKeypoints(std::vector<Keypoint>& v);
    
    //find out if filterResponse[lv][y][x] is a local max or min
    //locality is defined as the closest 3x3x3 block in the lv-x-y domain.    
    bool IsLocalExtremum(int x, int y, int lv, float threshold);
    bool HarrisTest(int x, int y, int lv, float threshold);
public:
    CensureDetector(CensureParam& p);
    ~CensureDetector();
    
    bool SetImageBuffer(cv::Mat buffer);

    void IntegrateBoxImage();
    cv::Mat GetIntegralBoxImage();
    void IntegrateSlantImage();
    
    void GetKeypoints(std::vector<Keypoint>& v);
//    void FindMaximum(int lv);
};

#endif
