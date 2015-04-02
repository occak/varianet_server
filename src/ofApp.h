#pragma once

#include "ofMain.h"
#include "ofxUI.h"
#include "Groove.h"
#include "Disc.h"
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
    
    //ui
    void exit();
    void guiEvent(ofxUIEventArgs &e);
    
    //game
    float costRadius, costTexture, costDensity, costRotation, costMute;
		
    
    ofEasyCam cam;
    
    //TCP
    ofxTCPServer server;
    vector<string> received;
    string title;
    
    ofTextField chat;
    
private:
    
    bool fullScreen;
    
    Groove groove;
    Disc disc;
    Sound sound;
    
    vector<ofxUICanvas *> ui;


};

