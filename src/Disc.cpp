//
//  Disc.cpp
//  multiDiscTest
//
//  Created by cemcakmak on 22/02/15.
//
//

#include "Disc.h"

void Disc::setup(){
    
    discIndex = 10;    // 10 discs
    
    for(int i = 0; i < discIndex; i++){
        
        // generate radius information of discs
        if (i == 0) radii.push_back(15. + origin);
        else radii.push_back(radii[i-1] + 15.);
        
        // generate texture densities, rule out non-symmetrics
        float thisDensity = 30;
//        while ((int) thisDensity % 360 != 0) {
//            thisDensity = ofRandom(30) + 1;
//            //            cout << "recalculating" << endl;
//        }
        density.push_back(thisDensity);
        //        cout << (int) thisDensity << endl;
        
        // determine their initial rotation angle and speed
        rotation.push_back(0);
        rotationSpeed.push_back(0);
        
        // determine texture type
        texture.push_back(0);
        
        // set depths, all zero by default
        zPosition.push_back(0.);
        
        counter.push_back(0.);
        
        //z-motion is off
        perlin.push_back(0);
        resetPerlin.push_back(0);
        
        //sound
        vector<float> adsr;
        envelope.push_back(adsr);
        setEnvelope(i, getTexture(i));
        
        //muting, all initially false
        mute.push_back(0);
     
        //seeds for perlin generators
        seed.push_back((int) ofRandom(10000));
    }
}

void Disc::update(){
    
    
    for(int i = 0; i < discIndex; i++){
        
        
        if(resetPerlin[i] == 1){
            setPosition(i, 0);
            perlin[i] = 0;
            resetPerlin[i] = 0;
        }
        
        if(perlin[i] == 1){
        
            
//            position += (sin((time*timeScale)) * displacementScale) - (position/2);
           
//            position += (perlinz.PerlinNoise_1D(time*timeScale+timeOffset, 3)) * displacementScale;

        
            //update groove position
//            setPosition(i, position);
            
        }
        
    }
    
}
//----------------------------------

void Disc::selectDisc(int x, int y){
    
}
//----------------------------------

void Disc::drawTexture(){
    
    
}
//----------------------------------

int Disc::getTexture(int index) const{
    
    return texture[index];
}
//----------------------------------

void Disc::setTexture(int index, int type) {
    
    texture[index] = type;
}
//----------------------------------

int Disc::getDiscIndex() const{
    
    return discIndex;
}
//----------------------------------

void Disc::setDiscIndex(int value){
    
    discIndex = value;
}
//----------------------------------

float Disc::getRadius(int index) const{
    
    if ( index == -1) return origin;
    else return radii[index];
}
//----------------------------------

void Disc::setRadius(int index, float size){
    
    if ( index != -1) radii[index] = size;
}
//----------------------------------

float Disc::getThickness(int index) const{
    
    return radii[index]-radii[index-1];
    
}

void Disc::setThickness(int index, float size){
    
    float change = size - (radii[index]-radii[index-1]); // change in the difference of size between the inner circle
    for (int i = index; i < getDiscIndex(); i++) {
        radii[i] = radii[i] + change;
    }
}
//----------------------------------
int Disc::getDensity(int index) const{
    
    return density[index];
}
//----------------------------------

void Disc::setDensity(int index, int newDensity) {
    
    density[index] = newDensity;
}
//----------------------------------

float Disc::getRotation(int index) const{
    
    return rotation[index];
}
//----------------------------------

void Disc::setRotation(int index, float newRotation){
    
    // we need to increase rotate values by an amount of rotationSpeed to draw properly in groove.draw()
    rotation[index] += newRotation;
    if (rotation[index] > 360.) rotation[index] -= 360;
}
//----------------------------------

float Disc::getRotationSpeed(int index) const{
    
    return rotationSpeed[index];
    
}
//----------------------------------

void Disc::setRotationSpeed(int index, float addSpeed){
    
    rotationSpeed[index+1] -= addSpeed; //outer disc rotates relative to the inner disc
    return rotationSpeed[index] += addSpeed;
    
}
//----------------------------------

float Disc::getNetRotationSpeed(int index) const{
    
    if(index != 0){
        float allBelow = 0;
        for(int i = 0; i < index; i++){
            allBelow += rotationSpeed[i];
        }
        return rotationSpeed[index]+allBelow;
    }
    else return rotationSpeed[0];
}
//----------------------------------

void Disc::setNetRotationSpeed(int index, float newSpeed){
    
    if(index != 0){
        float allBelow = 0;
        for(int i = 0; i < index; i++){
            allBelow += rotationSpeed[i];
        }
        rotationSpeed[index] = newSpeed - allBelow;
        rotationSpeed[index+1] -= newSpeed - allBelow; //outer disc rotates relative to the inner disc
    }
    else rotationSpeed[0] = newSpeed;
    
}
//----------------------------------

float Disc::getPosition(int index) const{
    
    return zPosition[index];
}
//----------------------------------

void Disc::setPosition(int index, float newPosition){
    
    zPosition[index] = newPosition;
}

//----------------------------------
float Disc::getLife() const{
    
    return life;
}

//----------------------------------
void Disc::setLife(float cost){
    
    life -= cost;
}

//----------------------------------
float Disc::getEnvelope(int index, int section) const{
    
    return envelope[index][section];
    
}

//----------------------------------
void Disc::setEnvelope(int index, int type) {
    
    vector <float> adsr;
    float attack, decay, sustain, release;
    switch (type) {
        case 0:         // no sound
            attack = 0;
            decay = 0;
            sustain = 0;
            release = 0;
            break;
            
        case 1:         // texture 1 - click
            attack = 0.005;
            decay = 0.005;
            sustain = 0;
            release = 0;
            break;
            
        case 2:         // texture 2 - triangle
            attack = 0.05;
            decay = 0.05;
            sustain = 0;
            release = 0.05;
            break;
            
        case 3:         // texture 3 - note
            attack = 0.005;
            decay = 0.5;
            sustain = 0;
            release = 0;
            break;
            
        case 4:         // texture 4 - rect
            attack = 0.001;
            decay = 0;
            sustain = 0.05;
            release = 0.001;
            break;
            
        default:
            break;
    }
    
    adsr.push_back(attack);
    adsr.push_back(decay);
    adsr.push_back(sustain);
    adsr.push_back(release);
    
    envelope[index] = adsr;
    
}

//----------------------------------
int Disc::isMute(int index) const{
    
    return mute[index];
    
}

//----------------------------------
void Disc::setMute(int index, int onOff){
    
    mute[index] = onOff;
    
}

//----------------------------------
int Disc::isMoving(int index) const{
    
    return perlin[index];
}

//----------------------------------
void Disc::setMoving(int index, int moving){
    
    perlin[index] = moving;
}

int Disc::getCounter(int index) const{
    
    return counter[index];
}

void Disc::setCounter(int index, int value){
    
    counter[index] = value;
}

int Disc::getSeed(int index) const{
    
    return seed[index];
}

void Disc::setSeed(int index, int value){
    
    seed[index] = value;
}
//
//float Disc::getScale(int index) const{
//    
//    return scale[index];
//}
//
//float Disc::getScaleSize() const{
//    
//    return scale.size();
//}
//
//void Disc::setScale(int index, float value){
//    
//    scale[index] = value;
//}
