#import "PictoKey.h"
#import "Base64.h"
//#import "b64/encode.h"
#include <sstream>
#include <string>

@implementation PictoKey

- (id)initWithBytes:(const void *)bytes length:(NSUInteger)length
{
    self = [super init];
    if (self) {
        key = [[NSData alloc] initWithBytes:bytes length:length];
    }
    return self;    
}

- (id) initWithSiftDescriptorArray:(SiftDescriptor*) desc nDescriptors:(NSUInteger) length
{
    self = [super init];
    if (self)
    {
        int mult = 128*4;
        char* code = new char[length*sizeof(float)*128];
        for(int j=0;j<length;j++)
        {
            memcpy(&code[j*mult], &desc[j].desc, sizeof(float)*128);
            //flip endian
            
            for(int i=0;i<mult;i+=4)
            {
                char tmp = code[j*mult+i];
                code[j*mult+i] = code[j*mult+i+3];
                code[j*mult+i+3] = tmp;
                tmp = code[j*mult+i+1];
                code[j*mult+i+1] = code[j*mult+i+2];
                code[j*mult+i+2] = tmp;
            }
        }
        key = [[NSData alloc] initWithBytes:code length:4608];
        delete []code;
        code = 0;
    }
    return self;
}

//- (id)initWithExpandedCode:(int*)expcode
//{
//    self = [super init];
//    if (self) {
//        char code[25];
//        memset(code, 0, 25);
//        int count = 0;
//        for(int i=0; i<100; i++)
//        {
//            code[count] = code[count]<<2;
//            code[count] |= expcode[i];
//            if((i+1)%4==0)
//                count++;
//        }
//        key = [[NSData alloc] initWithBytes:code length:25];
//    }
//    return self;
//}

- (id)copyWithZone:(NSZone *)zone
{
    PictoKey *copy = [super copy];
    
    copy->key=[key copyWithZone:zone];
    
    return copy;
}


//truth - input truth pictokey
//output - array which matches the expanded code showing where errors are occuring 
//      - if output[i] != 0 => expcode[i] has error
//- (float)correctness:(PictoKey*)truth errorTable:(int*)output
//{
//
//    int error = 0;
//    int expcode[100];
//    [self expandedCode:expcode];
//    int exptruth[100];
//    [truth expandedCode:exptruth];
//    for(int i=0;i<100;i++)
//    {
//        output[i] = (exptruth[i] ^ expcode[i]) & 3;
//        error += (output[i]&1)+((output[i]>>1)&1);
//    }
//    return 1.0f-(error/200.0f);
//}

-(NSString*) base64encode
{
//    base64::encoder encoder(12000);
//    int length = [key length];
//    char* data = (char*)[key bytes];
//    char code[10000];
//    
//    encoder.encode(data, length, code);
//    std::cout<<code;
//    
//    return [NSString stringWithCString:code encoding:NSASCIIStringEncoding];
    return [Base64 encodeBase64WithData:key];
}

//-(void) expandedCode:(int*)output
//{
//    char* code = (char*)[key bytes];
//    int length = [key length];
//    
//    int count = 0;
//    for(int i=0;i<length;i++)
//    {
//        for(int j=6;j>=0;j-=2)
//        {
//            int c = (code[i]>>j) &3;
//            output[count] = c;
//            count++;
//        }
//    }
//}

@end