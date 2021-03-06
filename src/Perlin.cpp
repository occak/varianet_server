//
//  Perlin.cpp
//  varianet_server
//
//  Created by cemcakmak on 10/04/15.
//
//

#include "Perlin.h"
#include "ofApp.h"

float Perlin::Noise(int x, int seed){
    
    int n = x * seed;
//    n = (n<<13) ^ n;
    
    
    return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589)
                    & 0x7fffffff) / 1073741824.0);
    
}

float Perlin::SmoothedNoise(float x, int seed){
    
    return Noise(x,seed)/2  +  Noise(x-1,seed)/4  +  Noise(x+1,seed)/4;
    
}


float Perlin::InterpolatedNoise(float x, int seed){
    
    int integer_X = static_cast<int>(x);
    float fractional_X = x - integer_X;
    
    float v1 = SmoothedNoise(integer_X, seed);
    float v2 = SmoothedNoise(integer_X + 1, seed);
    
    return ofInterpolateCosine(v1, v2, fractional_X);
    
}

float Perlin::PerlinNoise_1D(float x, int seed){
    
    float total = 0;
    float p = persistence;
    int n = Number_Of_Octaves - 1;
    
    for(int i = 0; i < n; i++){
        float frequency = pow(2,(float)i);
        float amplitude = pow(p,(float)i);
        
        total = total + InterpolatedNoise(x * frequency , seed) * amplitude;
        
    }
    
    return total;
}