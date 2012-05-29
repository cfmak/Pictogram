//
//  PictoKey.h
//  Pictogram
//
//  Created by Chat F Mak on 3/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "Sift.h"

@interface PictoKey : NSObject <NSCopying>
{
    NSData* key;
}

- (id)initWithBytes:(const void *)bytes length:(NSUInteger)length;
- (id) initWithSiftDescriptorArray:(SiftDescriptor*) desc nDescriptors:(NSUInteger) length;
//- (id)initWithExpandedCode:(int*)expcode;
- (id)copyWithZone:(NSZone *)zone;

- (float)correctness:(PictoKey*)truth errorTable:(int*)output;

-(NSString*) base64encode;
//-(void) expandedCode:(int*)output;

@end
