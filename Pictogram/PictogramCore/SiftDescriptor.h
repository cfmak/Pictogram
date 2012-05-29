//
//  SiftDescriptor.h
//  Pictogram
//
//  Created by Chat F Mak on 5/16/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Pictogram_SiftDescriptor_h
#define Pictogram_SiftDescriptor_h

class SiftDescriptor {        
public:
    float desc[128];
    
    SiftDescriptor()
    {
        memset(desc, 0, sizeof(float)*128);
//        desc = new float[128];
//        hamming = new int[2];
    }
    
//    SiftDescriptor(float[] arr)
//    {
//        desc = arr.clone();
//        
//        hamming = new int[2];
//        HammingEmbedding();
//    }
    
    //blockx - 0 to 16
    //blocky - 0 to 16
    //bin - 0 to 7
    void HistogramAccumulate(int blockx, int blocky, int bin, float weight)
    {
        int idx = ((blocky/4)*4+blockx/4)*8+bin;
        //        System.out.println(idx);
        desc[idx] = desc[idx]+weight;
    }
    
    
    void Normalize()
    {
        bool needRenormalize = false;
        float norm = 0;
        for(int i=0;i<128;i++)
            norm += desc[i]*desc[i];
        norm = (float)sqrt(norm);
        
        if(norm==0)
            return;
        
        for(int i=0;i<128;i++)
        {
            desc[i] /= norm;
            
            //cap at 0.2
            if(desc[i]>0.2f)
            {
                desc[i] = 0.2f;
                needRenormalize = true;
            }
        }
        
        //renormalize
        if(needRenormalize)
        {
            norm = 0;
            for(int i=0;i<128;i++)
                norm += desc[i]*desc[i];
            norm = (float)sqrt(norm);
            for(int i=0;i<128;i++)
            {
                desc[i] /= norm;
            }
        }
    }
    
    float Distance(SiftDescriptor other)
    {
        return (float) sqrt(DistanceSquared(other));
    }
    
    float DistanceSquared(SiftDescriptor other)
    {
        float d = 0;
        for(int i=0;i<128;i++)
        {
            float delta = desc[i]-other.desc[i];
            d+= delta*delta; //L2 distance
        }
        return d;
    }
    
//    int HammingDistance(SiftDescriptor other)
//    {
//        return Integer.bitCount(hamming[0]^other.hamming[0]) + 
//                Integer.bitCount(hamming[1]^other.hamming[1]);
//    }
//    
//    void GetHammingEmbedding(int* output)
//    {
//        output[0] = hamming[0];
//        output[1] = hamming[1];
//    }
    
//    void HammingEmbedding()
//    {
//        Vector<Float> z = new Vector<Float>();
//        for(int i=0; i<P.length; i++)
//        {
//            float s = 0;
//            for(int j=0;j<P[0].length;j++)
//            {
//                s += P[i][j]*desc[j];
//            }
//            z.add(new Float(s));
//        }
//        Collections.sort(z);
//        
//        float median= z.elementAt(P.length/2);
//        if(P.length%2==0)
//            median = 0.5f*(z.elementAt(P.length/2-1).floatValue()+median);
//        
//        int b=0;
//        hamming[0] = hamming[1] = 0;
//        for(int i=0; i<P.length; i++)
//        {
//            b = 0;
//            if(z.elementAt(i).floatValue()>median)
//                b=1;
//            hamming[i<32?0:1] |= (b<<(i%32));
//        }
//    }
    
//    public String toString()
//    {
//        String s="";
//        for(int i=desc.length-1;i>=0;i--)
//        {
//            s = desc[i]+", "+s;
//        }
//        return s;
//    }
    
//    public Object clone()
//    {
//        SiftDescriptor tmp = new SiftDescriptor();
//        for(int i=0;i<desc.length;i++)
//        {
//            tmp.desc[i] = desc[i];
//        }
//        return tmp;
//    }
};

#endif
