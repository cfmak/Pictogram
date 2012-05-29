#import <Foundation/Foundation.h>

@interface Base64 : NSObject

+ (NSString *)encodeBase64WithString:(NSString *)strData;
+ (NSString *)encodeBase64WithData:(NSData *)objData;
+ (void) modify:(char*)strCode length:(int)len;
@end