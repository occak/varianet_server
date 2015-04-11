//
//  Groove.h
//  multiDiscTest
//
//  Created by cemcakmak on 26/02/15.
//
//

#pragma once

#ifndef __multiDiscTest__Groove__
#define __multiDiscTest__Groove__

#include <stdio.h>
#include "ofMain.h"
#include "Disc.h"
#include "Player.h"


class Groove{
    
public:
    
    void setup(Disc* disc);
    void update();
    void draw();
    
    ofRectangle lifeBarFrame;
    ofRectangle lifeBar;
    
    bool turn = false;
    Disc* disc;
    Player player;
    
    vector<ofMesh> mesh;
    
    ofColor playerColor;
    
};


#endif /* defined(__multiDiscTest__Groove__) */