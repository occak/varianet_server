#pragma once

#include "ofMain.h"
#include "Disc.h"
#include "Player.h"
#include "ofxNetwork.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    //ui
    void exit();
    
    //game
    float costRadius, costTexture, costDensity, costRotation, costMute, costMove;
    
    //TCP
    ofxTCPServer server;
    vector<string> received;
    string title;
    
private:
    
    bool fullScreen;
    int playerNum;
    
//    Groove groove;
    Disc disc;
    vector<Player *> players;
    
    


};

