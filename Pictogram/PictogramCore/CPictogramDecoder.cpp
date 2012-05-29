#include "ImageCommon.h"
#include "CensureDetector.hpp"
#include "Brief.h"
#include "Sift.h"
#include "CPictogramDecoder.hpp"
#include "FastMatchTemplate.h"

CPictogramDecoder::CPictogramDecoder()
{
    corrected = false;
    corrected_width = corrected_height = 0;
}

//CPictogramDecoder::CPictogramDecoder(cv::Mat img, cv::Mat tlAnchor, cv::Mat trAnchor, cv::Mat blAnchor, cv::Mat brAnchor)
//{
//    //    tl = tlCorner;
//    //    tr = trCorner;
//    //    bl = blCorner;
//    //    br = brCorner;
//    anchorTemplate[0]=tlAnchor;
//    anchorTemplate[1]=trAnchor;
//    anchorTemplate[2]=blAnchor;
//    anchorTemplate[3]=brAnchor;
//    image = img;
//    lastTimestamp = timestamp = 0;
//    corrected = false;
//    corrected_width = corrected_height = 216;
//    outerPixWidth = (corrected_width-16) / 10;
//    outerPixHeight = (corrected_height-16) / 10;
//    innerPixWidth = outerPixWidth * 7 / 10-4;
//    innerPixHeight = outerPixHeight * 7 / 10-4;
//    innerOffsetX = (outerPixWidth - innerPixWidth)/2+2;
//    innerOffsetY = (outerPixHeight - innerPixHeight)/2+2;
//}

CPictogramDecoder::CPictogramDecoder(cv::Mat tlAnchor, cv::Mat trAnchor, cv::Mat blAnchor, cv::Mat brAnchor)
{
    //    tl = tlCorner;
    //    tr = trCorner;
    //    bl = blCorner;
    //    br = brCorner;
    anchorTemplate[0]=tlAnchor;
    anchorTemplate[1]=trAnchor;
    anchorTemplate[2]=blAnchor;
    anchorTemplate[3]=brAnchor;
    lastTimestamp = timestamp = 0;
    corrected = false;
//    anchorDim=16;
//    corrected_width = corrected_height = 216;
    anchorDim=20;
    corrected_width = corrected_height = 276;
    outerPixWidth = (corrected_width-anchorDim) / 10;
    outerPixHeight = (corrected_height-anchorDim) / 10;
    innerPixWidth = outerPixWidth * 5 / 10;
    innerPixHeight = outerPixHeight * 5 / 10;
    innerOffsetX = (outerPixWidth - innerPixWidth)/2;
    innerOffsetY = (outerPixHeight - innerPixHeight)/2;
}

CPictogramDecoder::~CPictogramDecoder()
{
    //if(bbox)
    //    cvFree(&bbox);
    //if(image_gray)
    //    cvReleaseImage(&image_gray);
}

void CPictogramDecoder::PushFrame()
{
    lastTimestamp = timestamp;
//    lastUndistorted = undistorted;
    memcpy(lastIn, in, sizeof(int)*100);
    memcpy(lastOut, out, sizeof(int)*100);
    memset(in, 0, sizeof(int)*100);
    memset(out, 0, sizeof(int)*100);
    corrected = false;
}

bool CPictogramDecoder::ProcessFrame(cv::Mat& img, SiftDescriptor* desc)
{
    image = img;
    int ret = 0;
    corrected = false;
    double t = (double)cv::getTickCount();
    if(FindBBox(70,&ret) == true)
    {
        
        //        self.tlfound.text = [NSString stringWithFormat:@"%lf", picto->confList[0]];
        //        self.trfound.text = [NSString stringWithFormat:@"%lf", picto->confList[1]];
        //        self.blfound.text = [NSString stringWithFormat:@"%lf", picto->confList[2]];
        //        self.brfound.text = [NSString stringWithFormat:@"%lf", picto->confList[3]];
        if(CorrectPerspective())
        {
            timestamp = t;
            cv::Mat roi = undistorted(cv::Rect(anchorDim/2,anchorDim/2,256,256));
            
//            CensureParam param(corrected_height, 0.2, 10);
//            CensureDetector censure(param);
//            censure.SetImageBuffer(roi);
//                                   
//            cv::Mat intImg = censure.GetIntegralBoxImage();
            
            cv::Mat intImg = ImageCommon::IntegrateBoxImage(roi);
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<3; j++)
                {
                    Keypoint kp(j*85+42, i*85+42, 7, 1);
                    Sift::GetDescriptor(kp, intImg, &desc[i*3+j]);
                    
for(int k=0;k<128;k++)
    printf("%f, ", desc[i*3+j].desc[k]);
printf("\n");
                    
                }
            }
            return true;
            
            //            found = true;
            //            self.imageView.image = [UIImage imageWithCVMat: picto->GetUndistorted()];
            //            U32 code = 0;
            //            if(picto->Decode(code))
            //            {
            //                self.decodedLabel.text = [NSString stringWithFormat:@"%x", code];
            //            }
            //            else
            //            {
            //                self.decodedLabel.text = [NSString stringWithFormat:@"decode fail"];
            //            }
        }        
    }
    else
    {
        //        self.tlfound.text = @"0";
        //        self.trfound.text = @"0";
        //        self.blfound.text = @"0";
        //        self.brfound.text = @"0";
    }
    return false;
}



cv::Mat CPictogramDecoder::GetImage()
{
    return image;
}

//cv::Mat CPictogram::LoadImage(const char* filepath)
//{
//    image = imread(filepath);
//    //image_gray= 0;
//    corrected = false;
//    
//    return image;
//}

bool CPictogramDecoder::FindBBox(float corrThreshold, int* ret)
{
    // get timestamp first
    double t = (double)cv::getTickCount();
    
    //if frame is already undistorted, return
    if(corrected) return true;
    
//    Point tlf, trf, blf, brf;
    Point anchorPos[4];
    cv::Mat eqlize = image;
//    cv::equalizeHist(image, eqlize);
//    cv::threshold(eqlize, eqlize, 30, 255, THRESH_BINARY);
    
    bool anchorFound = false;
    vector<Point>  foundList;
    vector<double> confidencesList;
    //last corners were identified >0.3s ago
    //go for slow path: template match the whole image for 4 anchors
    
//    printf("t = %f\n", (t - lastTimestamp)/cv::getTickFrequency());
    if((t - lastTimestamp)/cv::getTickFrequency() > 1) 
    {
        if(CPictogramDecoder::FastMatchTemplate(eqlize, anchorTemplate[0], corrThreshold, &foundList, &confidencesList))
        {
//printf("slow path %.2lf", (t - lastTimestamp)/cv::getTickFrequency());
            double conf=0;
            confList.resize(4);
            FindNearestPoint(cv::Point(0,0), &foundList, &confidencesList, &anchorPos[0], &conf);
            confList[0] = conf;
            FindNearestPoint(cv::Point(image.size().width,0), &foundList, &confidencesList, &anchorPos[1], &conf);
            confList[1] = conf;
            FindNearestPoint(cv::Point(0,image.size().height), &foundList, &confidencesList, &anchorPos[2], &conf);
            confList[2] = conf;
            FindNearestPoint(cv::Point(image.size().width,image.size().height), &foundList, &confidencesList, &anchorPos[3], &conf);
            confList[3] = conf;
            
            //check the four corners are four distinct points
            if(anchorPos[0].x==anchorPos[1].x && anchorPos[0].y==anchorPos[1].y) return false;
            if(anchorPos[0].x==anchorPos[2].x && anchorPos[0].y==anchorPos[2].y) return false;
            if(anchorPos[1].x==anchorPos[3].x && anchorPos[1].y==anchorPos[3].y) return false;
            if(anchorPos[2].x==anchorPos[3].x && anchorPos[2].y==anchorPos[3].y) return false;
            if(anchorPos[0].x==anchorPos[3].x && anchorPos[0].y==anchorPos[3].y) return false;
            if(anchorPos[1].x==anchorPos[2].x && anchorPos[1].y==anchorPos[2].y) return false;
            
            anchorFound = true;
            abox[0]=anchorPos[0];//cv::circle(image, abox[0], 10, Scalar::all(0));
            abox[1]=anchorPos[1];//cv::circle(image, abox[1], 10, Scalar::all(0));
            abox[2]=anchorPos[2];//cv::circle(image, abox[2], 10, Scalar::all(0));
            abox[3]=anchorPos[3];//cv::circle(image, abox[3], 10, Scalar::all(0));
//            printf(" found\n");
        }
        if(!anchorFound)
        {
            //printf("!anchorFound");
            return false;
        }
    }
    else //last corners were identified <0.3s ago
    {
//printf("fast path %.2lf ", (t - lastTimestamp)/cv::getTickFrequency());
        double conf=0;
        confList.resize(4);
        int r = 20; //search radius
        
        for(int i=0;i<4;i++)
        {
            int x = abox[i].x-r>0?abox[i].x-r:0;
            int y = abox[i].y-r>0?abox[i].y-r:0;
            int w = abox[i].x+r<=eqlize.size().width-1? abox[i].x+r-x+1: eqlize.size().width-x;
            int h = abox[i].y+r<=eqlize.size().height-1? abox[i].y+r-y+1: eqlize.size().height-y;
            cv::Mat roi = eqlize(cv::Rect(x,y,w,h));
            CPictogramDecoder::FastMatchTemplate(roi, anchorTemplate[i], corrThreshold, &foundList, &confidencesList);
            if(foundList.size()>=1)
            {
//                printf(" %d",i);
                for(int j=0;j<foundList.size();j++)
                {
                    foundList[j].x+=x;
                    foundList[j].y+=y;
                }
                if(i==0)
                    FindNearestPoint(cv::Point(0,0), &foundList, &confidencesList, &anchorPos[i], &conf);
                else if(i==1)
                    FindNearestPoint(cv::Point(image.size().width,0), &foundList, &confidencesList, &anchorPos[i], &conf);
                else if(i==2)
                    FindNearestPoint(cv::Point(0,image.size().height), &foundList, &confidencesList, &anchorPos[i], &conf);
                else
                    FindNearestPoint(cv::Point(0,image.size().height), &foundList, &confidencesList, &anchorPos[i], &conf);
                
                confList[i] = conf;
            }
            else
            {
//                printf("\n");
//                image = eqlize;
                //printf("!anchorFound");
                return false;
            }
        }
//        printf("\n");
   
        
        //check the four corners are four distinct points
        if(anchorPos[0].x==anchorPos[1].x && anchorPos[0].y==anchorPos[1].y) return false;
        if(anchorPos[0].x==anchorPos[2].x && anchorPos[0].y==anchorPos[2].y) return false;
        if(anchorPos[1].x==anchorPos[3].x && anchorPos[1].y==anchorPos[3].y) return false;
        if(anchorPos[2].x==anchorPos[3].x && anchorPos[2].y==anchorPos[3].y) return false;
        if(anchorPos[0].x==anchorPos[3].x && anchorPos[0].y==anchorPos[3].y) return false;
        if(anchorPos[1].x==anchorPos[2].x && anchorPos[1].y==anchorPos[2].y) return false;
        
        anchorFound = true;
        abox[0]=anchorPos[0];//cv::circle(image, abox[0], 10, Scalar::all(0));
        abox[1]=anchorPos[1];//cv::circle(image, abox[1], 10, Scalar::all(0));
        abox[2]=anchorPos[2];//cv::circle(image, abox[2], 10, Scalar::all(0));
        abox[3]=anchorPos[3];//cv::circle(image, abox[3], 10, Scalar::all(0));
    }
    
    //return false;
//    if(!cornerTempFound)
//    {
//        return false;
//    }
    
//    vector<cv::Point> cornerRoiStart; //Construct ROI to search for corner. This specify the TL of the ROI.

    int max_corner_count = 1;
    
    
    
    float cornerThres = 0.5f;
    
    for(int i=0;i<4;i++)
    {
        cv::Point cornerRoiStart(anchorPos[i].x-3>0?anchorPos[i].x-3:0,
                                 anchorPos[1].y-3>0?anchorPos[i].y-3:0);
        vector<cv::Point> corners; //This will store the possible position of the corner
        cv::Mat roi = eqlize(cv::Rect(cornerRoiStart.x, 
                                      cornerRoiStart.y, 
                                      cornerRoiStart.x+7<=eqlize.size().width? 
                                        7 : eqlize.size().width-cornerRoiStart.x, 
                                      cornerRoiStart.y+7<=eqlize.size().height? 
                                        7 : eqlize.size().height-cornerRoiStart.y));
        
        //find anchor centers
        cv::goodFeaturesToTrack(roi, corners, max_corner_count, cornerThres, 3);

        if(corners.size()>0)
        {
            corners.push_back(anchorPos[i]);
            bbox[i].x=corners[0].x+cornerRoiStart.x;
            bbox[i].y=corners[0].y+cornerRoiStart.y;
            //cv::circle(image, bbox[i], 5, Scalar::all(0));
        }
        else
            return false;
    }
//    image = eqlize;
    return true;
}

bool CPictogramDecoder::FindNearestPoint(cv::Point ref, std::vector<cv::Point>*  foundPointsList, 
                                  std::vector<double>*  confidencesList, cv::Point* pt, double* conf)
{
    if(foundPointsList->size() == 0)
        return false;
    *pt = (*foundPointsList)[0];
    if(confidencesList && conf)
        *conf = (*confidencesList)[0];
    float dx = pt->x-ref.x;
    float dy = pt->y-ref.y;
    float norm2 = dx*dx + dy*dy;
    //printf("ref %d %d\n", ref.x, ref.y);
    //printf("px %d, py %d, norm2 %f\n", (*foundPointsList)[0].x, (*foundPointsList)[0].y, norm2);
    for(int i=1;i<foundPointsList->size(); i++)
    {
        dx = (*foundPointsList)[i].x-ref.x;
        dy = (*foundPointsList)[i].y-ref.y;
        float currentNorm2 = dx*dx + dy*dy;
        //printf("px %d, py %d, currentnorm2 %f\n", (*foundPointsList)[i].x, (*foundPointsList)[i].y, currentNorm2);
        if(currentNorm2 < norm2)
        {
            *pt = (*foundPointsList)[i];
            if(confidencesList && conf)
                *conf = (*confidencesList)[i];
            norm2 = currentNorm2;
        }
    }
    return true;
}

bool CPictogramDecoder::FastMatchTemplate(cv::Mat subImage, cv::Mat target, float corrThreshold, 
                                   std::vector<cv::Point>*  foundPointsList,
                                   std::vector<double>* confidencesList)
{
    foundPointsList->clear();
    confidencesList->clear();
    Mat source = subImage;
    
    // start the timer
//    ( const Mat&      source,
//     const Mat&      target,
//     vector<Point>*  foundPointsList,
//     vector<double>* confidencesList,
//     int             matchPercentage,
//     bool            findMultipleTargets,
//     int             numMaxima,
//     int             numDownPyrs,
//     int             searchExpansion )
    if(!CFastMatchTemplate::FastMatchTemplate(source,
                                              target,
                                              foundPointsList,
                                              confidencesList,
                                              corrThreshold,
                                              true,
                                              5,
                                              1))
    {
//printf("\nERROR: Fast match template failed.\n");
        return false;
    }
//printf("number of matches = %lu\n", confidencesList->size());
//    for(int i=0;i<confidencesList->size();i++)
//    {
//        cv::circle(source, (*foundPointsList)[i], 15, Scalar::all(128));
//        //printf("conf=%f\n", confidencesList[i]);
//    }
    return (foundPointsList->size() >= 4);
}



bool CPictogramDecoder::CorrectPerspective()
{
    if(corrected || !bbox) return false;
    
    //corrected_width = abs(bbox[1].x - bbox[0].x);
    //corrected_height = abs(bbox[2].y - bbox[0].y);
    
    //if(corrected_width<50 || corrected_height<50) return false;
//    corrected_width = corrected_height = 100;
    if(abs(bbox[3].x - bbox[2].x)<100 || abs(bbox[1].y - bbox[3].y) < 100) return false;
    
    Point2f original[4];
    original[0].x = 0;                  original[0].y = 0;
    original[1].x = corrected_width;    original[1].y = 0;
    original[2].x = 0;                  original[2].y = corrected_height;
    original[3].x = corrected_width;    original[3].y = corrected_height;
    
    cv::Mat perspective = cv::getPerspectiveTransform(original, bbox);
//    cout << "P = "<< endl << " "  << perspective << endl;
    
    //take out the xy-mapping part, and check SVD, expect it to be "almost" an ortho matrix (plus some skewing etc)
    cv::Mat rot = perspective(Rect(0,0,2,2));
    //cout << "R = "<<endl<<" " <<rot <<endl<<endl;
    
    cv::SVD rotsvd(rot);
//    cout << "sigma = "<<endl<<" " <<rotsvd.w <<endl<<endl;
    if(rotsvd.w.at<double>(0, 0)>1.2 || rotsvd.w.at<double>(0, 1) < 0.8)
    {
//        cout<<"too skewed"<<endl<<endl;
//        return false; //too skewed
    }
    //cout << "U = "<<endl<<" " << rotsvd.u <<endl<<endl;
    //cout << "V = "<<endl<<" " << rotsvd.vt <<endl<<endl;
    double forwardr = asin(rotsvd.u.at<double>(0,1));
    double backwardr = asin(rotsvd.vt.at<double>(0,1));
    //cout << "forwardr = "<<forwardr<<endl<<"backwardr = "<<backwardr<<endl<<endl;
    if(abs(forwardr)>=M_PI_2 || abs(backwardr)>=M_PI_2) 
    {
//        cout << "too much individual rotation"<<endl<<endl;
        return false; //too much rotation
    }
    if(abs(forwardr + backwardr) > 0.2) 
    {
//        cout << "too much net rotation" <<endl<<endl;
        return false; //too much net rotation
    }
    
    //passed the SVD test, now correct
    cv::Mat perspectiveInv = perspective.inv(DECOMP_SVD); //pseudo inverse
//    cv::Mat tmpUndistorted;
    cv::warpPerspective(image, undistorted, perspectiveInv, Size(corrected_width, corrected_height)); //apply matrix to undistort with resampling
    
    //Process the rulers
//    cv::Mat roiT, roiB, roiL, roiR;
//    
//    roiT = undistorted(cv::Rect(0,anchorDim/3, 216, 1));
//    roiB = undistorted(cv::Rect(0,215-anchorDim/3, 216, 1));
//    roiL = undistorted(cv::Rect(anchorDim/3, 0, 1, 216));
//    roiR = undistorted(cv::Rect(215-anchorDim/3, 0, 1, 216));
//    if(!ProcessRuler(roiT, rulerT))
//    {
////        printf("!rulerT found\n"); 
//        return false;
//    }
//    if(!ProcessRuler(roiB, rulerB))
//    {
////        printf("!rulerB found\n"); 
//        return false;
//    }
//    if(!ProcessRuler(roiL, rulerL))
//    {
////        printf("!rulerL found\n"); 
//        return false;
//    }
//    if(!ProcessRuler(roiR, rulerR))
//    {
////        printf("!rulerR found\n"); 
//        return false;
//    }
        
    
    corrected = true;
    return true;
}

//roi is a one pixel wide ruler array, either vertical or horizontal will do
//output is a set of pixel position which marks the transition from black->white or white->black
bool CPictogramDecoder::ProcessRuler(cv::Mat& rulerRoi, vector<int>& output)
{
    int orientation = 0; //0 for horizontal ruler, 1 for vertical ruler
    int sum = 0;
    int mean = 0;
    vector<int> tmp;
    output.clear();
    
    cv::Mat roi = rulerRoi.clone();
    if(roi.rows==1)
    {
        orientation = 1; 
        roi=roi.t();
    }
    for(int x=0;x<80;x++)
        sum += roi.at<unsigned char>(0, x);
    
    for(int i=0;i<216;i++)
    {
        if(i>=40 && i<216-40)
        {
            sum -= roi.at<unsigned char>(0, i-40);
            sum += roi.at<unsigned char>(0, i+40);
        }
        mean = sum/80;
        if(roi.at<unsigned char>(0, i)<mean)
            roi.at<unsigned char>(0, i)=0;
        else
            roi.at<unsigned char>(0, i)=255;
        if(i>0 && abs(roi.at<unsigned char>(0, i)-roi.at<unsigned char>(0, i-1))>0)
        {
            tmp.push_back(i);
        }
    }
    if(tmp.size() <8)
    {
        return false;
    }
    
//cout<<"tmp: "<<endl;
//for(int i=0;i<tmp.size();i++)
//    cout<<tmp[i]<<", ";
//cout<<endl;
       
    bool* truth = new bool[tmp.size()];
    for(int i=0;i<tmp.size();i++)
        truth[i] = false;
    
    if((tmp[0]>=7 && tmp[0]<=10) ||
       (tmp[0]>=26 && tmp[0]<=30))
        truth[0] = true;
    for(int i=1;i<tmp.size();i++)
    {
        //non-center points are allowed to have more errors
        //than center points
        if((tmp[i]-tmp[i-1]>=18 && tmp[i]-tmp[i-1]<=22) || 
           (tmp[i]-tmp[i-1]>=38 && tmp[i]-tmp[i-1]<=42))
        {
            if(tmp[i]>=7)
                truth[i] = true;
            if(tmp[i-1]>=7)
                truth[i-1]=true;
        }
        else
        {
            //either tmp[i] or tmp[i+1] is wrong, (or both?)
            //hard decision:
            //if i+1 is not the last one, i is false
            if(i+1<tmp.size()-1)
            {
                truth[i-1] = false;
            }
            //if i+1 is the last one, i+1 is false
            //i is left unchanged
            else
            {
                truth[i] = false;
            }
        }
    }
    
    //get all the true points in one vector tmp2
    vector<int> tmp2;
    for(int i=0;i<tmp.size();i++)
    {
        if(truth[i])
            tmp2.push_back(tmp[i]);
    }
//cout<<"tmp2: "<<endl;
//for(int i=0;i<tmp2.size();i++)
//    cout<<tmp2[i]<<", ";
//cout<<endl;
    
    if(tmp2.size() <8)
    {
        delete []truth;
        return false;
    }
    //now tmp2 contains a subset of all the 11 points (hopefully 8), but no wrong points
    //guestimate missing points in tmp2
    if(tmp2[0]>=7 && tmp2[0]<=10) //first point is at the acceptable location
        output.push_back(tmp2[0]);
    else if(tmp2[0]>=26 && tmp2[0]<=30) //first point is missing
    {
        output.push_back(tmp2[0]-20);
        output.push_back(tmp2[0]);
    }
    else //first and seconds point are missing...?
    {
        delete []truth;
        return false;
    }
        
    for(int i=1;i<tmp2.size();i++)
    {
        if(tmp2[i]-tmp2[i-1]>=18 && tmp2[i]-tmp2[i-1]<=22)
            output.push_back(tmp2[i]);
        else if(tmp2[i]-tmp2[i-1]>=38 && tmp2[i]-tmp2[i-1]<=42) //in the middle of the picture, it is supposed to be missing
        {
            output.push_back(tmp2[i-1]+20);
            output.push_back(tmp2[i]);
        }
    }
    
//cout<<"output: "<<endl;
//for(int i=0;i<output.size();i++)
//    cout<<output[i]<<", ";
//cout<<endl;
    
    //lastly check the # elements... very weird if we get here
    if(output[output.size()-1]<186 || output.size() < 10)
    {
        delete []truth;
        return false;
    }
    
    //free truth array
    delete []truth;
    return true;
        
}

cv::Mat CPictogramDecoder::GetUndistorted()
{
    return undistorted;
}


//expcode - output expanded code int[100]
bool CPictogramDecoder::Decode(int* expcode)
{
    if(!corrected) return false;
    
    memset(data, 0, 10*10*sizeof(int));
    
//    printf("data:\n");
    for(int y=0; y<10; y++)
    {
        for(int x=0;x<10;x++)
        {
            in[y][x] = InnerPixel(x,y);
            out[y][x] = OuterPixel(x,y);
            data[y*10+x]= lastIn[y][x]+in[y][x] - lastOut[y][x]-out[y][x];
//            printf("%d, %d, %d\n", in, out, data[y*10+x]);
        }
    }
    int negmin = 0;
    int negmax = INT_MIN;
    int posmax = 0;
    int posmin = INT_MAX;
    
    for(int i=0; i<CODE_LENGTH; i++)
    {
        if(data[i]<0 && data[i]<negmin)
            negmin = data[i];
        if(data[i]<0 && data[i]>negmax)
            negmax = data[i];
        if(data[i]>0 && data[i]<posmin)
            posmin = data[i];
        if(data[i]>0 && data[i]>posmax)
            posmax = data[i];
    }
    kcenter[0] = negmin;
    kcenter[1] = posmax;
    kcenter[2] = negmax;
    kcenter[3] = posmin;
    
    float error = Kmean(data, kcenter, expcode, FLT_MAX);
//    for(int i=0;i<100;i++)
//        printf("%d\n", cluster[i]);
//    return cluster;
    return true;
}

void CPictogramDecoder::GetInnerPixelTable(int output[][10])
{
    memcpy(output, in, sizeof(int)*100);
}

void CPictogramDecoder::GetOuterPixelTable(int output[][10])
{
    memcpy(output, out, sizeof(int)*100);
}

//int CPictogramDecoder::OuterPixel(int X, int Y)
//{
//    int outerOffsetX;// = X*outerPixWidth+8; //+8 to skip the margin (created by the anchor)
//    int outerOffsetY;// = Y*outerPixHeight+8;//+8 to skip the margin (created by the anchor)
//    
//    outerOffsetX = X<5?rulerT[X]:rulerB[X];
//    outerOffsetY = Y<5?rulerL[Y]:rulerR[Y];
//    
//    int outsum = 0;
//    
//    for(int i=0;i<(outerPixHeight-innerPixHeight)/2;i++)
//    {
//        for(int j=0;j<outerPixWidth;j++)
//        {
//            outsum += undistorted.at<unsigned char>(i+outerOffsetY, j+outerOffsetX);
//            undistorted.at<unsigned char>(i+outerOffsetY, j+outerOffsetX)=255;
//        }
//    }
//    for(int i=0;i<innerPixHeight;i++)
//    {
//        for(int j=0;j<(outerPixWidth-innerPixWidth)/2;j++)
//        {
//            outsum += undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY, j+outerOffsetX);
//            undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY, j+outerOffsetX)=255;
//            
//            outsum += undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY,
//                                                    (outerPixWidth-1-j)+outerOffsetX);
//            undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY,
//                                          (outerPixWidth-1-j)+outerOffsetX)=255;
//        }
//    }
//    for(int i=0;i<(outerPixHeight-innerPixHeight)/2;i++)
//    {
//        for(int j=0;j<outerPixWidth;j++)
//        {
//            outsum += undistorted.at<unsigned char>(i+outerOffsetY+innerPixHeight+
//                                                        (outerPixHeight-innerPixHeight)/2,
//                                                    j+outerOffsetX);
//            undistorted.at<unsigned char>(i+outerOffsetY+innerPixHeight+
//                                          (outerPixHeight-innerPixHeight)/2,
//                                          j+outerOffsetX)=255;
//        }
//    }
//    int area = (outerPixWidth*outerPixHeight - innerPixWidth*innerPixHeight);
//    return (int)(outsum / area);
//}

int CPictogramDecoder::OuterPixel(int X, int Y)
{
    int outerOffsetX;// = X*outerPixWidth+8; //+8 to skip the margin (created by the anchor)
    int outerOffsetY;// = Y*outerPixHeight+8;//+8 to skip the margin (created by the anchor)
    
    outerOffsetX = X<5?rulerT[X]:rulerB[X];
    outerOffsetY = Y<5?rulerL[Y]:rulerR[Y];
    
    int outsum = 0;
    
    for(int i=0;i<(outerPixHeight-innerPixHeight-2)/2;i++)
    {
        for(int j=0;j<outerPixWidth;j++)
        {
            outsum += undistorted.at<unsigned char>(i+outerOffsetY, j+outerOffsetX);
            undistorted.at<unsigned char>(i+outerOffsetY, j+outerOffsetX)=255;
        }
    }
    for(int i=0;i<innerPixHeight+2;i++)
    {
        for(int j=0;j<(outerPixWidth-innerPixWidth-2)/2;j++)
        {
            outsum += undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY-1, j+outerOffsetX);
            undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY-1, j+outerOffsetX)=255;
            
            outsum += undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY-1,
                                                    (outerPixWidth-1-j)+outerOffsetX);
            undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY-1,
                                          (outerPixWidth-j-1)+outerOffsetX)=255;
        }
    }
    for(int i=0;i<(outerPixHeight-innerPixHeight-2)/2;i++)
    {
        for(int j=0;j<outerPixWidth;j++)
        {
            outsum += undistorted.at<unsigned char>(i+outerOffsetY+innerPixHeight+
                                                    (outerPixHeight-innerPixHeight)/2+1,
                                                    j+outerOffsetX);
            undistorted.at<unsigned char>(i+outerOffsetY+innerPixHeight+
                                          (outerPixHeight-innerPixHeight)/2+1,
                                          j+outerOffsetX)=255;
        }
    }
    int area = (outerPixWidth*outerPixHeight - (innerPixWidth+2)*(innerPixHeight+2));
    return (int)(outsum / area);
}

int CPictogramDecoder::InnerPixel(int X, int Y)
{
    int outerOffsetX;// = X*outerPixWidth+8; //+8 to skip the margin (created by the anchor)
    int outerOffsetY;// = Y*outerPixHeight+8;//+8 to skip the margin (created by the anchor)
    
    outerOffsetX = X<5?rulerT[X]:rulerB[X];
    outerOffsetY = Y<5?rulerL[Y]:rulerR[Y];
    
    int insum = 0;
    
    for(int i=0;i<innerPixHeight-2;i++)
    {
        for(int j=0;j<innerPixWidth-2;j++)
        {
            insum += undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY+1,
                                                   j+outerOffsetX+innerOffsetX+1);
//            undistorted.at<unsigned char>(i+outerOffsetY+innerOffsetY,
//                                          j+outerOffsetX+innerOffsetX)=255;
        }
    }
    int area = (innerPixHeight-2)*(innerPixWidth-2);
    return (int)(insum/area);
}

//data - (innerPixel - outerPixel) array
//kcenter - 4 starting cluster centers
//cluster - which cluster does each data element belongs to
//error - summed error for the clusters
float CPictogramDecoder::Kmean(int* data, float* kcenter, int* cluster, float error)
{
    float newError = 0;
    for(int i=0;i<CODE_LENGTH;i++)
    {
        float mindist = FLT_MAX;
        cluster[i] = 0;
        for(int j=0; j<4;j++)
        {
            if(data[i]*kcenter[j]>0 && abs(data[i] - kcenter[j])<mindist)
            {
                cluster[i] = j;
                mindist = abs(data[i] - kcenter[j]);
            }
        }
        newError += abs(data[i] - kcenter[cluster[i]]);
    }
    for(int j=0; j<4;j++)
    {
        kcenter[j]=0;
        int nodeCount=0;
        for(int i=0;i<CODE_LENGTH;i++)
        {
            if(cluster[i]==j)
            {
                kcenter[j]+=data[i];
                nodeCount++;
            }
        }
        if(nodeCount>0)
            kcenter[j] /= nodeCount;
    }
    if(abs(newError - error)<50)
        return newError;
    return Kmean(data, kcenter, cluster, newError);
}
