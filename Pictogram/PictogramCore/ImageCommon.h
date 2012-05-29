//
//  ImageCommon.h
//  Pictogram
//
//  Created by Chat F Mak on 5/28/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_ImageCommon_h
#define Pictogram_ImageCommon_h

class ImageCommon {
public:    
    static cv::Mat IntegrateBoxImage(cv::Mat buf)
    {
        int bufy = buf.size().height;
        int bufx = buf.size().width;
        cv::Mat integralBoxImg = cv::Mat(bufy, bufx, cv::DataType<int>::type);
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
        
//        for(int i=0;i<bufy;i++)
//        {
//            for(int j=0;j<bufx;j++)
//            {
//                printf("%d,",integralBoxImg.at<int>(i,j));
//            }
//            printf("\n");
//        }
        return integralBoxImg;
    }
};


#endif
