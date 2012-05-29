#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <PhoneGap/PGPlugin.h>
//#import "ScanViewController.h"
#import "MainViewController.h"
#import "CPictogramDecoder.hpp"
#import "PictoKey.h"

@interface PGScanner : PGPlugin {
    NSString* callbackID; 
    UIViewController* scanView;
}

@property (nonatomic, copy) NSString* callbackID;

//Instance Method  
- (void) scan:(NSMutableArray*)arguments withDict:(NSMutableDictionary*)options;
-(void)queryServer:(PictoKey*) pk;
-(void)cancelScan;
@end

@interface ScanViewController : UIViewController
{
    cv::VideoCapture *_videoCapture;
    cv::Mat _lastFrame;
    
    cv::Mat tlcorner;
    cv::Mat trcorner;
    cv::Mat blcorner;
    cv::Mat brcorner;
    
    bool found;
    NSLock *mutex;
    
    CPictogramDecoder* decoder;
    PGScanner* pgScanner;
    NSTimer *timer;
    
    //truth code
    PictoKey* pkTruth;
}
//
@property (nonatomic, retain) IBOutlet UIImageView *imageView;

@property (nonatomic, retain) IBOutlet UILabel *elapsedTimeLabel;
//
//@property (nonatomic, retain) IBOutlet UILabel *tlfound;
//@property (nonatomic, retain) IBOutlet UILabel *trfound;
//@property (nonatomic, retain) IBOutlet UILabel *blfound;
//@property (nonatomic, retain) IBOutlet UILabel *brfound;
//@property (nonatomic, retain) IBOutlet UILabel *decodedLabel;
//
//
//- (IplImage *)CreateIplImageFromUIImage:(UIImage *)image;
//- (UIImage *)UIImageFromIplImage:(IplImage *)image;
- (id)initWithNibName:(NSString *)nibName bundle:(NSBundle *)nibBundle jsInterface:(PGScanner*) pgscanner;
- (IBAction)cancel:(id)sender;
-(void) scheduleDetectTimer;
-(void) stopDetectTimer;
-(void)detectLoop;
- (void)processFrame;
-(bool) openVideoCapture;
-(void) closeVideoCapture;
-(void) closeVideoCaptureThread;

@end