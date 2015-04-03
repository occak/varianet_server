//
//  Disc.cpp
//  multiDiscTest
//
//  Created by cemcakmak on 22/02/15.
//
//

#include "Disc.h"

void Disc::setup(){

    life = 100;         // initial life value
    
    discIndex = 10;    // 10 discs
    

    for(int i = 0; i < discIndex; i++){
        
        // generate radius information of discs
        if (i == 0) radii.push_back(ofRandom(50) + 15.);
        else radii.push_back(radii[i-1] + ofRandom(50) + 15.);
        
        // generate texture densities, rule out non-symmetrics
        float thisDensity = ofRandom(15.) + 1;
        while ((int) thisDensity == 7 || (int) thisDensity == 11 || (int) thisDensity == 13) {
            thisDensity = ofRandom(15.) + 1;
//            cout << "recalculating" << endl;
        }
        density.push_back(thisDensity);
//        cout << (int) thisDensity << endl;
        
        // determine their initial rotation angle and speed
        rotation.push_back(0);
        rotationSpeed.push_back(0);
    
        // determine texture type
        texture.push_back((int) ofRandom(5));
        
        // set depths, all zero by default
        zPosition.push_back(0.);
        
        posOffset.push_back(ofRandom(0,100000));
        
        //sound
        vector<float> adsr;
        envelope.push_back(adsr);
        setEnvelope(i, getTexture(i));
        
        //muting, all initially false
        mute.push_back(0);
        
        //z-motion is off
        perlin.push_back(0);
        resetPerlin.push_back(0);
    }
   
}

void Disc::update(){
    
    for(int i = 0; i < discIndex; i++){
        
        
    if(resetPerlin[i] == 1){
        setPosition(i, 0);
        perlin[i] = 0;
        }
        
    if(perlin[i] == 1){
        
        float position = getPosition(i);
        
        float time = ofGetElapsedTimef();
        float timeScale = .1;
        float displacementScale = 10;
        float timeOffset = getPosOffset(i);
        
        // A typical design pattern for using Perlin noise uses a couple parameters:
        // ofSignedNoise(time*timeScale+timeOffset)*displacementScale
        //     ofSignedNoise(time) gives us noise values that change smoothly over time
        //     ofSignedNoise(time*timeScale) allows us to control the smoothness of our noise (smaller timeScale, smoother values)
        //     ofSignedNoise(time+timeOffset) allows us to use the same Perlin noise function to control multiple things and have them look as if they are moving independently
        //     ofSignedNoise(time)*displacementScale allows us to change the bounds of the noise from [-1, 1] to whatever we want
        // Combine all of those parameters together, and you've got some nice control over your noise
        
        position += (ofSignedNoise(time*timeScale+timeOffset)) * displacementScale;
        

        //update groove position
        setPosition(i, position);
        
        }
        resetPerlin[i] = 0;
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

int Disc::setTexture(int index, int type) {
    
    texture[index] = type;
}
//----------------------------------

int Disc::getDiscIndex() const{
    
    return discIndex;
}
//----------------------------------

float Disc::getRadius(int index) const{
    
    if ( index == -1) return 10; // '0 point'
    return radii[index];
}
//----------------------------------

float Disc::setRadius(int index, float size){
    
    float change = size - (radii[index]-radii[index-1]); // change in the difference of size between the inner circle
    for (int i = index; i < getDiscIndex(); i++) {
        radii[i] = radii[i] + change;
    }
    return radii[index];
}
//----------------------------------

int Disc::getDensity(int index) const{
    
    return density[index];
}
//----------------------------------

int Disc::setDensity(int index, int newDensity) {
    
    return density[index] = newDensity;
}
//----------------------------------

float Disc::getRotation(int index) const{
    
    return rotation[index];
}
//----------------------------------

float Disc::setRotation(int index, float newRotation){
    
    // we need to increase rotate values by an amount of rotationSpeed to draw properly in groove.draw()
    rotation[index] += newRotation;
    if (rotation[index] > 360.) rotation[index] -= 360;
    return rotation[index];
}
//----------------------------------

float Disc::getRotationSpeed(int index) const{
    
    return rotationSpeed[index];
}
//----------------------------------

float Disc::setRotationSpeed(int index, float newSpeed){

    rotationSpeed[index+1] -= newSpeed; //outer disc rotates relative to the inner disc
    return rotationSpeed[index] += newSpeed;

}
//----------------------------------

float Disc::getPosition(int index) const{
    
    return zPosition[index];
}
//----------------------------------

float Disc::setPosition(int index, float newPosition){
    
    return zPosition[index] = newPosition;
}

//----------------------------------

float Disc::getPosOffset(int index) const{
    
    return posOffset[index];
}
//----------------------------------

float Disc::setPosOffset (int index, float newOffset){
    
    return posOffset[index] = newOffset;
}

//----------------------------------
float Disc::getLife() const{
    
    return life;
}

//----------------------------------
float Disc::setLife(float cost){
    
    return life -= cost;
}

//----------------------------------
float Disc::getEnvelope(int index, int section) const{
    
    return envelope[index][section];

}

//----------------------------------
float Disc::setEnvelope(int index, int type) {

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
            attack = 0.001;
            decay = 0.001;
            sustain = 0;
            release = 0;
            break;
            
        case 2:         // texture 2 - triangle
            attack = 0.05;
            decay = 0;
            sustain = 0;
            release = 0.05;
            break;
            
        case 3:         // texture 3 - note
            attack = 0.001;
            decay = 0.1;
            sustain = 0;
            release = 0;
            break;
            
        case 4:         // texture 4 - rect
            attack = 0;
            decay = 0;
            sustain = 0.05;
            release = 0;
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
int Disc::toggleMute(int index){
    
    if(mute[index] == 0) mute[index] = 1;
    else mute[index] = 0;
    return mute[index];
    
}

//----------------------------------
int Disc::isMoving(int index) const{
    
    return perlin[index];
    
}

//----------------------------------
int Disc::toggleMoving(int index){
    
    if(perlin[index] == 0) perlin[index] = 1;
    else perlin[index] = 0;
    return perlin[index];
    
}










