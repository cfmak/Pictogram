//
//  BriefDescriptor.h
//  Pictogram
//
//  Created by Chat F Mak on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_BriefDescriptor_h
#define Pictogram_BriefDescriptor_h

#include "BriefParam.h"
#include <vector>

class BriefDescriptor
{ 
public:
    std::vector<int> desc;
    
    BriefDescriptor(BriefParam& param)
    {
        desc.resize(param.nPairs/32);
    }
    
//    ~BriefDescriptor()
//    {
//        
//    }
    int length()
    {
        return desc.size();
    }
    
    void SetBit(int x, int pos)
    {
        desc[pos/32] |= (x<<(pos%32));
    }
    
    int NumberOfSetBits(int i)
    {
        i = i - ((i >> 1) & 0x55555555);
        i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
        return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
    }
    
    int Distance(BriefDescriptor& other)
    {
        if(desc.size()!=other.desc.size())
            return desc.size()*32;
            
        int d = 0;
        for(int i=0;i<desc.size();i++)
        {
            d+= NumberOfSetBits(desc[i]^other.desc[i]); //hamming distance
        }
        return d;
    }
};

#endif
