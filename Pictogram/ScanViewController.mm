//
//  OpenCVClientViewController.m
//  OpenCVClient
//
//  Created by Robin Summerhill on 02/09/2011.
//  Copyright 2011 Aptogo Limited. All rights reserved.
//
//  Permission is given to use this source code file without charge in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.
//

// UIImage extensions for converting between UIImage and cv::Mat
#import "UIImage+OpenCV.h"
#import "CPictogramDecoder.hpp"
//#import "ScanViewController.h"
#import "PGScanner.h"

// Aperture value to use for the Canny edge detection
//const int kCannyAperture = 7;

//@interface ScanViewController ()
//- (void)processFrame;
//@end

@implementation ScanViewController
@synthesize imageView = _imageView;
@synthesize elapsedTimeLabel = _elapsedTimeLabel;
//@synthesize tlfound = _tlfound;
//@synthesize trfound = _trfound;
//@synthesize blfound = _blfound;
//@synthesize brfound = _brfound;
//@synthesize decodedLabel = _decodedLabel;
//

- (id)initWithNibName:(NSString *)nibName bundle:(NSBundle *)nibBundle jsInterface:(PGScanner*) pgscanner
{
    pgScanner = pgscanner;
    _videoCapture = new cv::VideoCapture();
    mutex = [[NSLock alloc] init];
    
    UIImage *tl = [UIImage imageNamed:@"crossCorner12.jpg"];
    UIImage *tr = [UIImage imageNamed:@"crossCorner12.jpg"];
    UIImage *bl = [UIImage imageNamed:@"crossCorner12.jpg"];
    UIImage *br = [UIImage imageNamed:@"crossCorner12.jpg"];
    
    tlcorner = [tl CVMat];
    cvtColor( tlcorner, tlcorner, CV_RGB2GRAY );
    trcorner = [tr CVMat];
    cvtColor( trcorner, trcorner, CV_RGB2GRAY );
    blcorner = [bl CVMat];
    cvtColor( blcorner, blcorner, CV_RGB2GRAY );
    brcorner = [br CVMat];
    cvtColor( brcorner, brcorner, CV_RGB2GRAY );
    decoder = new CPictogramDecoder(tlcorner, trcorner, blcorner, brcorner);

    return [self initWithNibName:nibName bundle:nibBundle];
}

-(void)dealloc {
    if(mutex)
        [mutex release];
    if(decoder)
        delete decoder;
    [super dealloc];
}

//
//- (IplImage *)CreateIplImageFromUIImage:(UIImage *)image {
//    // Getting CGImage from UIImage
//    CGImageRef imageRef = image.CGImage;
//    
//    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//    // Creating temporal IplImage for drawing
//    IplImage *iplimage = cvCreateImage(
//                                       cvSize(image.size.width,image.size.height), IPL_DEPTH_8U, 4
//                                       );
//    // Creating CGContext for temporal IplImage
//    CGContextRef contextRef = CGBitmapContextCreate(
//                                                    iplimage->imageData, iplimage->width, iplimage->height,
//                                                    iplimage->depth, iplimage->widthStep,
//                                                    colorSpace, kCGImageAlphaPremultipliedLast|kCGBitmapByteOrderDefault
//                                                    );
//    // Drawing CGImage to CGContext
//    CGContextDrawImage(
//                       contextRef,
//                       CGRectMake(0, 0, image.size.width, image.size.height),
//                       imageRef
//                       );
//    CGContextRelease(contextRef);
//    CGColorSpaceRelease(colorSpace);
//    
//    // Creating result IplImage
//    IplImage *ret = cvCreateImage(cvGetSize(iplimage), IPL_DEPTH_8U, 3);
//    cvCvtColor(iplimage, ret, CV_RGBA2BGR);
//    cvReleaseImage(&iplimage);
//    
//    return ret;
//}
//
//// NOTE You should convert color mode as RGB before passing to this function
//- (UIImage *)UIImageFromIplImage:(IplImage *)image {
//    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
//    // Allocating the buffer for CGImage
//    NSData *data =
//    [NSData dataWithBytes:image->imageData length:image->imageSize];
//    CGDataProviderRef provider =
//    CGDataProviderCreateWithCFData((CFDataRef)data);
//    // Creating CGImage from chunk of IplImage
//    CGImageRef imageRef = CGImageCreate(
//                                        image->width, image->height,
//                                        image->depth, image->depth * image->nChannels, image->widthStep,
//                                        colorSpace, kCGImageAlphaNone|kCGBitmapByteOrderDefault,
//                                        provider, NULL, false, kCGRenderingIntentDefault
//                                        );
//    // Getting UIImage from CGImage
//    UIImage *ret = [UIImage imageWithCGImage:imageRef];
//    CGImageRelease(imageRef);
//    CGDataProviderRelease(provider);
//    CGColorSpaceRelease(colorSpace);
//    return ret;
//}
//
- (void)viewDidLoad
{
    [super viewDidLoad];
       
//    UIImage *tl = [UIImage imageNamed:@"checkerboardCorner12.bmp"];
//    UIImage *tr = [UIImage imageNamed:@"checkerboardCorner12.bmp"];
//    UIImage *bl = [UIImage imageNamed:@"checkerboardCorner12.bmp"];
//    UIImage *br = [UIImage imageNamed:@"checkerboardCorner12.bmp"];
    found = false;
}

- (void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:animated];
    [self scheduleDetectTimer];
}

-(void) viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self stopDetectTimer];
}

-(bool) openVideoCapture
{
#if TARGET_IPHONE_SIMULATOR
    NSLog(@"Video capture is not supported in the simulator");
    return NO;
#else
    if(_videoCapture)
        [self closeVideoCapture];
    _videoCapture = new cv::VideoCapture();
    if (_videoCapture && !_videoCapture->open(CV_CAP_AVFOUNDATION))
    {
//        NSLog(@"Failed to open video camera");
        return NO;
    }
    return YES;
#endif
}

//When we need to closeVideoCapture, we need to ensure detectLoop is not running.
//Because if it is, it is using videoCapture, so we cannot free it.
//The only way to free it safely is to use a mutex lock.
//It requires the use of a thread: closeVideoCaptureThread
//this thread will check if the mutex is locked before freeing.
//if the mutex is locked, the thread will suspend until mutex is unlocked.
-(void) closeVideoCapture
{
    [NSThread detachNewThreadSelector:@selector(closeVideoCaptureThread)
                             toTarget:self
                           withObject:nil];
}

-(void) closeVideoCaptureThread
{
    NSAutoreleasePool *pool;
    pool = [[NSAutoreleasePool alloc] init];
    [mutex lock];
    if(_videoCapture && _videoCapture->isOpened())
    {
        _videoCapture->release();
        delete _videoCapture;
        _videoCapture = nil;
    }
    [mutex unlock];
    [pool release];
}

-(void) stopDetectTimer
{
    if(timer)
    {
        [timer invalidate];
        [timer release];
        timer = nil;
        [self closeVideoCapture];
    }
}
//
-(void) scheduleDetectTimer
{
    if(!timer)
    {
        // Initialise video capture - only supported on iOS device NOT simulator
#if TARGET_IPHONE_SIMULATOR
        NSLog(@"Video capture is not supported in the simulator");
#else
        [self openVideoCapture];
#endif
        timer = [[NSTimer scheduledTimerWithTimeInterval:0.1f
                                                       target:self
                                                     selector:@selector( detectLoop )
                                                     userInfo:nil
                                                      repeats:YES]
                    retain];
       
    }
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    [self stopDetectTimer];
    self.imageView = nil;//    self.elapsedTimeLabel = nil;
}
//
//- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
//{
//    // Return YES for supported orientations
//    return (interfaceOrientation == UIInterfaceOrientationPortrait);
//}
//
// Called when the user taps the Capture button. Grab a frame and process it
- (IBAction)cancel:(id)sender
{
    [pgScanner cancelScan];
}

-(void)detectLoop
{
    //if timer is not running, detectLoop is not supposed to run. Check for safety.
    if(!timer)
        return;
//    if(!_videoCapture || !_videoCapture->isOpened())
//    {
//        [self openVideoCapture];
//        //do detect in the next loop
//        return;
//    }
    [mutex lock];
    if (_videoCapture && _videoCapture->isOpened() && _videoCapture->grab())
    {
        cv::Mat bgrFrame;
        if(_videoCapture && _videoCapture->read(bgrFrame))
        {
            cvtColor(bgrFrame, _lastFrame, CV_BGR2GRAY);
        //NSLog(@"dimension = %d %d\n", _lastFrame.size().width, _lastFrame.size().height); //360*480
            [self processFrame];
        }
    }
    [mutex unlock];
    
}
//
//// Perform image processing on the last captured frame and display the results
- (void)processFrame
{
    //pkTruth = [[PictoKey alloc] initWithBytes:"1234567890123456789012345" length:25];
    
    if(found)
        return;
    
    double t = (double)cv::getTickCount();
    //pyrUp(_lastFrame, _lastFrame);
    
    cv::Mat roi = _lastFrame(cv::Rect(0,
                                      _lastFrame.size().height - _lastFrame.size().width,
                                      _lastFrame.size().width, 
                                      _lastFrame.size().width));
    decoder->PushFrame();
    
    SiftDescriptor desc[9];
//    if(decoder->ProcessFrame(roi, expcode))
//    printf("\n");
    if(decoder->ProcessFrame(roi, desc))
    {
//        PictoKey* pk = [[PictoKey alloc] initWithExpandedCode:expcode];
        PictoKey* pk = [[PictoKey alloc] initWithSiftDescriptorArray:desc nDescriptors:9];
        [pgScanner queryServer:pk];
        [pk release];
        self.imageView.image = [UIImage imageWithCVMat: decoder->GetUndistorted()];
//        self.imageView.image = [UIImage imageWithCVMat: decoder->GetImage()];
//        NSLog(@"success");
    }
    else
    {
        self.imageView.image = [UIImage imageWithCVMat: decoder->GetImage()];
    }
    [pkTruth release];
    t = 1000 * ((double)cv::getTickCount() - t) / cv::getTickFrequency();
}

@end
