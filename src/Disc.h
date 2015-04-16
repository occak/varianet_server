//
//  Disc.h
//  multiDiscTest
//
//  Created by cemcakmak on 22/02/15.
//
//

#pragma once

#ifndef __multiDiscTest__Disc__
#define __multiDiscTest__Disc__

#include <stdio.h>
#include "ofMain.h"





class Disc{

    public:
    
    void setup();
    void update();
    void drawTexture();
    void selectDisc(int x, int y);
    
    //getter&setter functions
    
    int getDiscIndex() const;
    void setDiscIndex(int value);
    
    float getRadius(int index) const;
    void setRadius(int index, float size);
    
    float getThickness(int index) const;
    void setThickness(int index, float size);
    
    int getDensity(int index) const;
    void setDensity(int index, int newDensity);
    
    float getRotation(int index) const;
    void setRotation(int index, float newRotation);
    
    //for the groove object to draw
    float getRotationSpeed(int index) const;
    void setRotationSpeed(int index, float addSpeed);
    //for relative speeds
    float getNetRotationSpeed(int index) const;
    void setNetRotationSpeed(int index, float addSpeed);
    
    int getTexture(int index) const;
    void setTexture(int index, int type);
    
    float getPosition(int index) const;
    void setPosition(int index, float newPosition);
    
    float getPosOffset(int index) const;
    void setPosOffset(int index, float newOffset);
    
    float getLife() const;
    void setLife(float cost);
    
    ofColor getColor(int index) const;
    
    float getEnvelope(int index, int section) const;
    void setEnvelope(int index, int type);
    
    int isMute(int index) const;
    void toggleMute(int index);
    
    int isMoving(int index) const;
    void setMoving(int index, int moving);
    
    float origin = 10;
    int selected = -1;
    vector<int> resetPerlin;
    
    
private:
    
    int discIndex;
    float life;
    
    vector<int> texture;
    
    vector<float> rotation;
    vector<float> rotationSpeed;
    
    vector<int> discSelected;
    vector<float> radii;
    vector<int> density;
    
    vector<float> zPosition;
    vector<float> posOffset;
    
    vector<ofColor> playerColor;
    
    //adsr
    vector< vector <float> > envelope;
    
    //mute
    vector<int> mute;
    
    // z-motion
    vector<int> perlin;
    
    //we will use a different perlin noise so we can give seeds
    vector<unsigned int> seed;
    
};


#endif /* defined(__multiDiscTest__Disc__) */