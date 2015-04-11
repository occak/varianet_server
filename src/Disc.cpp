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
    
    //colors for players when they log in
    playerColor.push_back(ofColor(161,24,87));
    playerColor.push_back(ofColor(255,153,0));
    playerColor.push_back(ofColor(50,153,187));
    
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
        
        //seed random numbers to perlin function
        seed.push_back(static_cast<unsigned int>(ofRandom(256)));
        
        //z-motion is off
        perlin.push_back(0);
        resetPerlin.push_back(0);
        
        //sound
        vector<float> adsr;
        envelope.push_back(adsr);
        setEnvelope(i, getTexture(i));
        
        //muting, all initially false
        mute.push_back(0);
        
    }
    
    //    for (int i = 0; i <100; i++) {
    //        float n = Noise(i);
    //        cout<< n <<endl;
    //    }
    
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
            
            //        position += (perlinObject->PerlinNoise_1D(time*timeScale+timeOffset)) * displacementScale;
            
            
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

float Disc::getPosOffset(int index) const{
    
    return posOffset[index];
}
//----------------------------------

void Disc::setPosOffset (int index, float newOffset){
    
    posOffset[index] = newOffset;
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
ofColor Disc::getColor(int index) const{
    
    return playerColor[index];
    
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
void Disc::toggleMute(int index){
    
    if(mute[index] == 0) mute[index] = 1;
    else mute[index] = 0;
    
}

//----------------------------------
int Disc::isMoving(int index) const{
    
    return perlin[index];
    
}

//----------------------------------
void Disc::toggleMoving(int index){
    
    if(perlin[index] == 0) perlin[index] = 1;
    else perlin[index] = 0;
    
}










