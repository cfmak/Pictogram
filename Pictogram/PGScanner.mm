#import "PGScanner.h"
#import "ScanViewController.h"

@implementation PGScanner

@synthesize callbackID;

-(void)scan:(NSMutableArray*)arguments withDict:(NSMutableDictionary*)options  
{
    NSLog(@"PGScanner.scan()");
    //The first argument in the arguments parameter is the callbackID.
    //We use this to send data back to the successCallback or failureCallback
    //through PluginResult.   
    self.callbackID = [arguments pop];
    
    //Get the string that javascript sent us 
    NSString *stringObtainedFromJavascript = [arguments objectAtIndex:0];
    
    scanView = [[ScanViewController alloc] 
                                    initWithNibName:@"ScanView" 
                                    bundle:[NSBundle mainBundle]
                                    jsInterface:self];
    
    [self.viewController presentModalViewController:scanView animated:YES]; //this call is non blocking
    
    //[self.parentViewController dismissModalViewControllerAnimated: YES];
    
//    //Create the Message that we wish to send to the Javascript
//    NSMutableString *stringToReturn = [NSMutableString stringWithString: @"StringReceived:"];
//    //Append the received string to the string we plan to send out        
//    [stringToReturn appendString: stringObtainedFromJavascript];
//    //Create Plugin Result
//    PluginResult* pluginResult = [PluginResult resultWithStatus:PGCommandStatus_OK messageAsString:                        [stringToReturn stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
//    
//    //Checking if the string received is HelloWorld or not
//    if([stringObtainedFromJavascript isEqualToString:@"HelloWorld"]==YES)
//    {
//        //Call  the Success Javascript function
//        [self writeJavascript: [pluginResult toSuccessCallbackString:self.callbackID]];
//        
//    }else
//    {    
//        //Call  the Failure Javascript function
//        [self writeJavascript: [pluginResult toErrorCallbackString:self.callbackID]];
//        
//    }
    
}

-(void)queryServer:(PictoKey*) pk
{
    NSLog(@"PGScanner.queryServer");
    if(scanView)
    {
        [scanView dismissModalViewControllerAnimated: YES];
        [scanView release];
        scanView = nil;
    }
    
    NSString* s = [pk base64encode];
//    NSLog(@"%@",s);
    
    NSMutableDictionary* resultDict = [[[NSMutableDictionary alloc] init] autorelease];
    [resultDict setObject:s  forKey:@"query"];
    [resultDict setObject:@"query" forKey:@"action"];
    PluginResult* result = [PluginResult
                            resultWithStatus: PGCommandStatus_OK
                            messageAsDictionary: resultDict
                            ];
    
    NSString* js = [result toSuccessCallbackString:callbackID];
    [self writeJavascript:js];
}

-(void)cancelScan
{
    NSLog(@"PGScanner.cancelScan");
    if(scanView)
    {
        [scanView dismissModalViewControllerAnimated: YES];
        [scanView release];
        scanView = nil;
    }
    
//    char bytes[26]= "1234567890123456789012345";
//    PictoKey* pk = [[PictoKey alloc] initWithBytes:bytes length:25];
    
    NSMutableDictionary* resultDict = [[[NSMutableDictionary alloc] init] autorelease];
    [resultDict setObject:@"cancelled" forKey:@"action"];
//    NSString* s = [pk base64];
    
//    int expandedCode[100];
//    [pk expandedCode:expandedCode];
//    for(int i=0;i<100;i++)
//    {
//        NSLog(@"%d",expandedCode[i]);
//    }
    
//    [resultDict setObject:s  forKey:@"query"];
    PluginResult* result = [PluginResult
                            resultWithStatus: PGCommandStatus_OK
                            messageAsDictionary: resultDict];
    
    NSString* js = [result toSuccessCallbackString:callbackID];
    [self writeJavascript:js];
}

-(void)dealloc {
    if(scanView)
    {
        [scanView release];
    }
    [super dealloc];
}

@end