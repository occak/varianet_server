//
//  Player.h
//  varianet_server
//
//  Created by cemcakmak on 10/04/15.
//
//

#ifndef __varianet_server__Player__
#define __varianet_server__Player__

#include <stdio.h>
#include "ofMain.h"

class Player{
    
public:
    
    string getIP() const;
    string setIP(string IP);
    
    ofColor getColor() const;
    ofColor setColor(ofColor color);
    
    float getLife();
    float setLife(float life);
    
    
    
private:
    
    string IP;
    ofColor color;
    
    float life;
    
    
    
};


#endif /* defined(__varianet_server__Player__) */

