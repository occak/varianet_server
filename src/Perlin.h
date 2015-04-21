//
//  Perlin.h
//  varianet_server
//
//  Created by cemcakmak on 10/04/15.
//
//

#pragma once

#ifndef __varianet_server__Perlin__
#define __varianet_server__Perlin__

#include <cmath>

class Perlin{
    
public:
    
    float Noise(int x, int seed);
    float SmoothedNoise(float x, int seed);
    float InterpolatedNoise(float x, int seed);
    float PerlinNoise_1D(float x, int seed);
    
private:
    
    int Number_Of_Octaves = 2;
    int persistence = 1;
    
};

#endif /* defined(__varianet_server__Perlin__) */
