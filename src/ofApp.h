#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Groove.h"
#include "Disc.h"
#include "Player.h"
#include "Sound.h"
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
    
    float triangleWave(float frequency);
    float squareWave(float frequency);
    float sawWave(float frequency);
    
    //audio
    void audioOut( float * output, int bufferSize, int nChannels );
    double phase;
    float volume;
    
    void soundChange(string name, int index, float value);
    
    //ui
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    //game
    float costRadius, costTexture, costDensity, costRotation, costMute;
    bool radiusChanged, textureChanged, densityChanged, rotationChanged, positionChanged = false;
		
    
    ofEasyCam cam;
    
    //TCP & udp
    ofxUDPManager sender;
    ofxTCPServer server;
    vector<string> received;
    string title;
    
    ofTextField chat;
    
private:
    
    bool fullScreen;
    int playerNum;
    
    Groove groove;
    Disc disc;
    vector<Player *> players;
    Sound sound;
    
    vector<ofxUICanvas *> ui;


};

