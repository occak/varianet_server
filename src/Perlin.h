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

#include <stdio.h>
#include "ofApp.h"

class Perlin{
    
public:
    
    float Noise(int x);
    float SmoothedNoise(float x);
    float InterpolatedNoise(float x);
    float PerlinNoise_1D(float x);
    
private:
    
    int Number_Of_Octaves = 5;
    int persistence = 10;
    
};

#endif /* defined(__varianet_server__Perlin__) */
