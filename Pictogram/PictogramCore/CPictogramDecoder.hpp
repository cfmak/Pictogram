#ifndef CPICTOGRAMDECODER_HPP
#define CPICTOGRAMDECODER_HPP

#import <OpenCV/opencv2/opencv.hpp>
#include <vector>
#include "PictoType.hpp"
#include "Sift.h"
#include "Rs.h"
#include "climits"

class CPictogramDecoder
{
public:
    static const int CODE_LENGTH=100;
    enum Checker{TLChecker, TRChecker, BLChecker, BRCherker};
    CPictogramDecoder();
//    CPictogramDecoder(cv::Mat img, cv::Mat tlCorner, cv::Mat trCorner, cv::Mat blCorner, cv::Mat brCorner);
    CPictogramDecoder(cv::Mat tlCorner, cv::Mat trCorner, cv::Mat blCorner, cv::Mat brCorner);
    ~CPictogramDecoder();
    
    void PushFrame();
    bool ProcessFrame(cv::Mat& img, SiftDescriptor* desc);
    cv::Mat GetImage();
    
    //cv::Mat LoadImage(const char* filepath);
    //void ShowImage();
    
//    bool FindBBoxFlann(int* ret);
    bool FindBBox(float corrThreshold, int* ret);
    bool FindNearestPoint(cv::Point ref, std::vector<cv::Point>*  foundPointsList, 
                          std::vector<double>*  confidencesList, cv::Point* pt, double* conf);
    bool FastMatchTemplate(cv::Mat subImage, cv::Mat target, float corrThreshold, 
                           std::vector<cv::Point>*  foundPointsList,
                           std::vector<double>* confidencesList);
    bool CorrectPerspective();
    bool ProcessRuler(cv::Mat& rulerRoi, std::vector<int>& output);
    
    cv::Mat GetUndistorted();
    bool Decode(int* expcode);
    int InnerPixel(int X, int Y);
    int OuterPixel(int X, int Y);
    float Kmean(int* data, float* kcenter, int* cluster, float error);
    
    //debug only
    //std::vector<cv::Point>  cornerList;
    std::vector<double> confList;
    
    void GetInnerPixelTable(int output[][10]);
    void GetOuterPixelTable(int output[][10]);
    
protected:
    
    //temporary storage
    cv::Mat image; //captured frame to be processed
    cv::Point2f abox[4];
    cv::Point2f bbox[4];
    
    
    //last time frame
    double lastTimestamp;    
//    cv::Mat lastUndistorted;
    int lastIn[10][10];
    int lastOut[10][10];
    
    //this time frame
    double timestamp;
    cv::Mat undistorted;
    std::vector<int> rulerT, rulerB, rulerL, rulerR;
    int in[10][10];
    int out[10][10];
    int data[CODE_LENGTH];
    float kcenter[4];
    
    //template images for 4 corners
//    cv::Mat tl;
//    cv::Mat tr;
//    cv::Mat bl;
//    cv::Mat br;
    cv::Mat anchorTemplate[4];
    
    //IplImage* image;
    //IplImage* image_gray;
    bool corrected;
    int corrected_height;
    int corrected_width;
    
    int anchorDim;
    int outerPixWidth;
    int outerPixHeight;
    int innerPixWidth;
    int innerPixHeight;
    int innerOffsetX;
    int innerOffsetY;
    
    Rs rsdecoder;
};

#endif