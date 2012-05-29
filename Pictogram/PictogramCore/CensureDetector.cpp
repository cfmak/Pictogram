//
//  CCensureDetector.cpp
//  Pictogram
//
//  Created by Chat F Mak on 4/18/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

//#include <iostream>
#include "CensureDetector.hpp"

const int CensureDetector::mnInner[7][2] = 
{
    {3,0}, {3,1}, {3,2}, {5,2}, {5,3}, {5,4}, {5,5}
};
const int CensureDetector::mnOuter[7][2] =
{
    {5,2},{5,3},{7,3},{9,4},{9,7},{13,7},{15,10}
};

const float CensureDetector::scaleInner[7] = 
{
    0.000435729847494553f, 0.000186741363211951f,
    0.000105988341282459f, 5.68343279340722e-5f,
    4.04285425510410e-5f, 3.03997568019456e-5f,
    2.37670825906120e-5f
};

const float CensureDetector::scaleOuter[7] = 
{
    6.53594771241830e-5f, 5.15995872033024e-5f,
    3.63108206245461e-5f, 2.17864923747277e-5f,
    1.22549019607843e-5f, 8.03600128576021e-6f,
    4.66853408029879e-6f
};

const int CensureDetector::filterSize[7] =
{
    9,11,13,17,23,27,35
};

const int CensureDetector::extFilterSize[7] =
{
    17,19,25,29,41,49,65
};


CensureDetector::CensureDetector(CensureParam& p)
{
    param.imageSize = p.imageSize;
    param.nonMaxSuppThreshold=p.nonMaxSuppThreshold;
    param.harrisThreshold=p.harrisThreshold;
    
    bufx=bufy=param.imageSize;
    
    integralBoxImg = cv::Mat(bufy, bufx, cv::DataType<int>::type);
    integralSlant1Img = cv::Mat(bufy, bufx, cv::DataType<int>::type);
    integralSlant2Img = cv::Mat(bufy, bufx, cv::DataType<int>::type);
    for(int i=0;i<7;i++)
    {
        filterResponse[i] = new float[bufy*bufx];
    }
}

CensureDetector::~CensureDetector()
{
    for(int i=0;i<7;i++)
    {
        if(filterResponse[i]!=NULL)
        {
            delete [](filterResponse[i]);
            filterResponse[i]=0;
        }
    }
}

bool CensureDetector::SetImageBuffer(cv::Mat buffer)
{
    if(buffer.size().height != param.imageSize ||
       buffer.size().width != param.imageSize)
        return false;
    bufy = param.imageSize;
    bufx = param.imageSize;
    buf = buffer;
    IntegrateBoxImage();
    IntegrateSlantImage();
    return true;
}

void CensureDetector::IntegrateBoxImage()
{
    integralBoxImg.at<int>(0,0)=buf.at<unsigned char>(0,0);
    for(int i=1;i<bufy;i++)
    {
        integralBoxImg.at<int>(i,0) = integralBoxImg.at<int>(i-1,0) + (int)buf.at<unsigned char>(i,0);
    }
    for(int i=1;i<bufx;i++)
    {
        integralBoxImg.at<int>(0,i) = integralBoxImg.at<int>(0,i-1) + (int)buf.at<unsigned char>(0,i);
    }
    for(int i=1;i<bufy;i++)
    {
        for(int j=1;j<bufx;j++)
        {
            integralBoxImg.at<int>(i,j) = ((int)buf.at<unsigned char>(i,j))+integralBoxImg.at<int>(i,j-1)+
            integralBoxImg.at<int>(i-1,j) - integralBoxImg.at<int>(i-1,j-1);
        }
    }
}

cv::Mat CensureDetector::GetIntegralBoxImage()
{
    return integralBoxImg;
}

void CensureDetector::IntegrateSlantImage()
{
    integralSlant1Img.at<int>(0,0) = (int)buf.at<unsigned char>(0,0);
    integralSlant2Img.at<int>(0,0) = (int)buf.at<unsigned char>(0,0);
    for(int i=1;i<bufx;i++)
    {
        integralSlant1Img.at<int>(0,i) = integralSlant1Img.at<int>(0,i-1) + (int)buf.at<unsigned char>(0,i);
        integralSlant2Img.at<int>(0,i) = integralSlant2Img.at<int>(0,i-1) + (int)buf.at<unsigned char>(0,i);
    }
    int rowIntegral = 0;
    for(int i=1;i<bufy;i++)
    {
        rowIntegral = 0;
        for(int j=0;j<bufx;j++)
        {
            rowIntegral += (int)buf.at<unsigned char>(i,j);
            integralSlant1Img.at<int>(i,j) = 
                integralSlant1Img.at<int>(i-1,j+1<bufx?j+1:bufx-1)+rowIntegral;
            if(j-1>=0)
                integralSlant2Img.at<int>(i,j) = integralSlant2Img.at<int>(i-1,j-1)+rowIntegral;
            else
                integralSlant2Img.at<int>(i,j) = rowIntegral;
        }
    }
}

int CensureDetector::OctagonFilter(int x, int y, int m, int n)
{
//    if(mn==NULL) 
//        return -1;
    
//    int m = mn[0];
//    int n = mn[1];
    
    int outUp = 0;
    int outBot = 0;
    if(n>0)
    {
        outUp = (integralSlant2Img.at<int>(y-m/2-1,x+n+m/2-1) - 
                 integralSlant2Img.at<int>(y-m/2-1-n,x+n+m/2-1-n)) - 
                (integralSlant1Img.at<int>(y-m/2-1,x-n-m/2) - 
                 integralSlant1Img.at<int>(y-m/2-1-n,x-n-m/2+n));
        
        outBot = (integralSlant1Img.at<int>(y+m/2+n,x+m/2) - 
                  integralSlant1Img.at<int>(y+m/2,x+n+m/2)) - 
                (integralSlant2Img.at<int>(y+m/2+n,x-m/2-1) - 
                 integralSlant2Img.at<int>(y+m/2,x-n-m/2-1));
    }
    int outCenter = integralBoxImg.at<int>(y+m/2,x+n+m/2)
                    - integralBoxImg.at<int>(y-m/2-1,x+n+m/2)
                    - integralBoxImg.at<int>(y+m/2,x-n-m/2-1)
                    + integralBoxImg.at<int>(y-m/2-1,x-n-m/2-1);
    return outUp+outBot+outCenter;
}

float CensureDetector::CensureFilter(int x, int y, int lv)
{
    float out = OctagonFilter(x,y,mnOuter[lv][0],mnOuter[lv][1]);
    float in = OctagonFilter(x,y,mnInner[lv][0], mnInner[lv][1]);
//    float v = scaleOuter[lv]*(out-in) - scaleInner[lv]*in;
    return scaleOuter[lv]*(out-in) - scaleInner[lv]*in;
}

void CensureDetector::GetKeypoints(std::vector<Keypoint>& v)
{
    for(int lv=0;lv<7;lv++)
    {
//        std::cout<<"lv="<<lv<<std::endl;
        int margin = extFilterSize[lv+1>6?6:lv+1]/2;
        for(int i=margin;i<bufy-margin;i++)
        {
//            std::cout<<"[";
            for(int j=margin;j<bufx-margin;j++)
            {
                filterResponse[lv][i*bufx+j] = CensureFilter(j,i,lv);
//                std::cout<<filterResponse[lv].at<float>(i,j)<<", ";
            }
//            std::cout<<"],"<<std::endl;
        }
//        FindMaximum(lv);
    }
    FindKeypoints(v);
}

void CensureDetector::FindKeypoints(std::vector<Keypoint>& v)
{
    for(int lv=0;lv<7;lv++)
    {
        int margin = extFilterSize[lv+1>6?6:lv+1]/2;
        for(int i=margin;i<bufy-margin;i++)
        {
            for(int j=margin;j<bufx-margin;j++)
            {
                if(IsLocalExtremum(j,i,lv,param.nonMaxSuppThreshold))
                {
//                    if(HarrisTest(j, i, lv, param.harrisThreshold))
//                    {
                        v.push_back(Keypoint(j,i,lv, filterResponse[lv][i*bufx+j]));
//                    }
                }
            }
        }
    }
}

//void CensureDetector::FindMaximum(int lv)
//{
//    float max = -100;
//    for(int i=0; i<=bufy; i++)
//    {
//        for(int j=0; j<=bufx; j++)
//        {
//            float response = filterResponse[lv][i*bufx+j];
////            std::cout<<response<<std::endl;
//            if(fabs(response)-max > 0)
//            {
//                max = fabs(response);
//            }
//        }
//    }
//    if(max > -100)
//        printf("max=%f\n", max);
//}

//find out if filterResponse[lv][y][x] is a local max or min
//locality is defined as the closest 3x3x3 block in the lv-x-y domain.    
bool CensureDetector::IsLocalExtremum(int x, int y, int lv, float threshold)
{        
    if(fabs(filterResponse[lv][y*bufx+x])<threshold)
        return false;
//    if(x==214 && y==143 && lv==5)
//        System.out.println();
    bool max = true;
    bool min = true;
    
    //first ensure "array out of bounds" won't happen to the Brief descriptor
    int lvstart = lv-1<0?0:lv-1;
    while(lvstart<7 && (x<extFilterSize[lvstart]/2 || y<extFilterSize[lvstart]/2))
    {
        lvstart++;
    }
    if(lvstart>lv+1 || lvstart>=7)
        return false;
    
    int lvend = lv+1>=7?6:lv+1;
    while(lvend>=0 && (x>param.imageSize-extFilterSize[lvend] || y>param.imageSize-extFilterSize[lvend]))
    {
        lvend--;
    }
    if(lvend<lv || lvend<lvstart)
        return false;
    
    for(int l=lvstart; l<=lvend; l++)
    {
        for(int i=y-1; i<=y+1; i++)
        {
            for(int j=x-1; j<=x+1; j++)
            {
                if(i==y && j==x &&l==lv)
                    continue;
                if(filterResponse[lv][y*bufx+x] < filterResponse[l][i*bufx+j])
                {
                    max = false;
                }
                else if(filterResponse[lv][y*bufx+x] > filterResponse[l][i*bufx+j])
                {
                    min = false;
                }
                if(!max && !min)
                    return false;
            }
        }
    }
    
    if(max || min)
        return true;
    return false;
}

bool CensureDetector::HarrisTest(int x, int y, int lv, float threshold)
{
//    int filterLength = filterSize[lv];
//    float[] Lx = new float[filterLength-1];
//    float[] Ly = new float[filterLength-1];
//    
//    int jj=0;
//    for(int j=x-filterLength/2+1;j<=x+filterLength/2;j++)
//    {
//        Lx[jj++] = filterResponse[lv].at<float>(y,j) - filterResponse[lv].at<float>(y,j-1);
//    }
//    jj=0;
//    for(int j=y-filterLength/2+1;j<=y+filterLength/2;j++)
//    {
//        Ly[jj++] = filterResponse[lv].at<float>(j,x) - filterResponse[lv].at<float>(j-1,x);
//    }
//    
//    float[][] h = new float[2][2];
//    for(int i=0;i<filterLength-1;i++)
//    {
//        h[0][0] += Lx[i]*Lx[i];
//        h[0][1] += Lx[i]*Ly[i];
//        h[1][1] += Ly[i]*Ly[i];
//    }
//    //        h[2][1] = h[1][2];
//    
//    float trace = h[0][0] + h[1][1];
//    float det = h[0][0]*h[1][1] - h[0][1]*h[0][1];
//    
//    float a = trace*trace*0.25f-det;
//    if(a<0)
//    {
//        return false;
//    }
//    
//    float b = (float) Math.sqrt(a);
//    float lambda1 = (float) (trace/2 + Math.sqrt(a));
//    float lambda2 = (float) (trace/2 - Math.sqrt(a));
//    float ratio = Math.abs(lambda1)/Math.abs(lambda2);
//    if(ratio < 10f && ratio > 0.1f)
//        return true;
//    return false;
    return true;
}