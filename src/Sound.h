//
//  Sound.h
//  groove_snd
//
//  Created by cemcakmak on 13/03/15.
//
//

#pragma once

#ifndef __groove_snd__Sound__
#define __groove_snd__Sound__

#include <stdio.h>
#include "ofMain.h"
#include "ofxTonic.h"
#include "Disc.h"

using namespace Tonic;

class Sound{
    
public:
    
    void setup(Disc* disc);
    void update();
    float getScale(int index) const;
    void setScale(int index, float value);
    vector<float> scale;
    
    Disc* disc;
    
    ofxTonicSynth synth;
    Generator master;
    
private:
    

    
};

#endif /* defined(__groove_snd__Sound__) */
