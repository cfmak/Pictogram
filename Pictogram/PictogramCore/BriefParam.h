//
//  BriefParam.h
//  Pictogram
//
//  Created by Chat F Mak on 4/21/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_BriefParam_h
#define Pictogram_BriefParam_h

class BriefParam {
public:
    int nPairs;
    int kernelSize[7];
    int kpSize[7];
    
    BriefParam(int npairs)
    {
        nPairs = npairs;
        
        kpSize[0] = 9;
        kpSize[1] = 11;
        kpSize[2] = 13;
        kpSize[3] = 17;
        kpSize[4] = 23;
        kpSize[5] = 27;
        kpSize[6] = 35;
        
        kernelSize[0] = 3;
        kernelSize[1] = 3;
        kernelSize[2] = 5;
        kernelSize[3] = 5;
        kernelSize[4] = 7;
        kernelSize[5] = 9;
        kernelSize[6] = 12;
    }
};

#endif
