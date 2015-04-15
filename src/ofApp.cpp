#include "ofApp.h"

/*
 texture patterns can be randomized
 
 colors for users:
 161,24,87 - purple
 255,153,0 - orange
 50,153,187 - blue
 
 */


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    //    ofBackground(255);
    
    //set up network
    server.setup(10002);
    server.setMessageDelimiter("varianet");
    
    // set up values of objects
    disc.setup();
    //    groove.setup(&disc);
    
    //set up gui
    //    for(int i = 0; i < disc.getDiscIndex(); i++){
    //
    //        ofxUICanvas *_ui;
    //
    //        _ui = new ofxUICanvas("Groove " + ofToString(i+1));
    //        _ui->addSlider("rotation" + ofToString(i+1), -10, 10, disc.getRotationSpeed(i));
    //        _ui->addSlider("radius" + ofToString(i+1), 15, 100, disc.getRadius(i)-disc.getRadius(i-1));
    //        _ui->addBiLabelSlider("density" + ofToString(i+1), "sparse", "dense", 30, 1, disc.getDensity(i));
    //
    //        if(disc.getTexture(i)==0) _ui->addToggle("blank", true);
    //        else _ui->addToggle("blank", false);
    //        if(disc.getTexture(i)==1) _ui->addToggle("line", true);
    //        else _ui->addToggle("line", false);
    //        if(disc.getTexture(i)==2) _ui->addToggle("tri", true);
    //        else _ui->addToggle("tri", false);
    //        if(disc.getTexture(i)==3) _ui->addToggle("saw", true);
    //        else _ui->addToggle("saw", false);
    //        if(disc.getTexture(i)==4) _ui->addToggle("rect", true);
    //        else _ui->addToggle("rect", false);
    //
    //        _ui->autoSizeToFitWidgets();
    //        _ui->setVisible(false);
    //        ofAddListener(_ui->newGUIEvent, this, &ofApp::guiEvent);
    //
    //        ui.push_back(_ui);
    //
    //    }
    
    
    //set up audio stream & synth network
    //    phase = 0;
    //    volume = 0;
    //    ofSoundStreamSetup(2, 0); // 2 out, 0 in
    
        sound.setup(&disc);
    
    // set up game costs
    costRadius = 1;
    costDensity = 1;
    costTexture = 1;
    costRotation = 1;
    costMute = 5;
}
//--------------------------------------------------------------
void ofApp::exit(){
    
    for(int i = 0; i < players.size(); i++){
        delete players[i];
    }
    
}
//--------------------------------------------------------------
//void ofApp::guiEvent(ofxUIEventArgs &e)
//{
//        for(int i = 0; i < disc.getDiscIndex(); i++){
//            if(e.getName() == "rotation" + ofToString(i+1)){
//                ofxUISlider *slider = e.getSlider();
//    
//                if(disc.getLife()>0){
//                    rotationChanged = true;
//                    float newRotation = slider->getScaledValue()-disc.getNetRotationSpeed(i);
//                    disc.setRotationSpeed(i, newRotation);
//    
//                    //change sound
//                    float netSpeed = abs(disc.getNetRotationSpeed(i));
//                    float beat = ofMap(netSpeed, 0, 10, 0, 1000);
//                    soundChange("bpm", i, beat);
//    
//                    //send to all clients
//                    string change = "rotationSpeed//"+ ofToString(i)+": "+ ofToString(newRotation);
//                    server.sendToAll(change);
//                }
//            }
//            else if(e.getName() == "radius" + ofToString(i+1)){
//                ofxUISlider *slider = e.getSlider();
//    
//                if(disc.getLife() > 0) {
//                    radiusChanged = true;
//                    disc.setThickness(i, slider->getScaledValue());
//    
//                    //change sound
//                    float q = ofMap(disc.getRadius(i)-disc.getRadius(i-1), 15, 100, 10, 0);
//                    soundChange("q", i, q);
//    
//                    //send to all clients
//                    string change = "radius//"+ofToString(i)+": "+ofToString(slider->getScaledValue());
//                    server.sendToAll(change);
//                }
//            }
//            else if(e.getName() == "density" + ofToString(i+1)){
//                ofxUISlider *slider = e.getSlider();
//                if(disc.getLife() > 0) {
//                    densityChanged = true;
//                    disc.setDensity(i, slider->getScaledValue());
//    
//                    //change sound
//                    float envelopeCoeff = ofMap(disc.getDensity(i), 1, 30, 1, 5);
//                    float pulseRatio = ofMap(disc.getDensity(i), 1, 30, 0.001, 1);
//                    soundChange("envelopeWidth", i, envelopeCoeff);
//                    soundChange("pulseLength", i, pulseRatio);
//    
//                    //send to all clients
//                    string change = "density//"+ ofToString(i)+": "+ ofToString(disc.getDensity(i));
//                    server.sendToAll(change);
//    
//                }
//            }
//            else if(e.getName() == "blank"){
//                ofxUIToggle *toggle = e.getToggle();
//                int texture = disc.getTexture(disc.selected);
//                if(disc.getLife() > 0 && texture != 0) {
//                    textureChanged = true;
//                    disc.setTexture(disc.selected, 0);
//                    ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
//                    ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
//                    ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
//                    ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
//                    ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
//                    toggle1->setValue(false);
//                    toggle2->setValue(false);
//                    toggle3->setValue(false);
//                    toggle4->setValue(false);
//                }
//                else toggle->setValue(true);
//    
//                //when texture is set to blank, rotation stops
//                disc.setRotationSpeed(disc.selected, 0);
//    
//                //sound
//                soundChange("envelope", i, 0);
//                soundChange("bpm", i, 0);
//                //update ui
//                ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[i]);
//                ofxUISlider *slider = static_cast <ofxUISlider*> (canvas->getWidget("rotation"+ofToString(i+1)));
//                slider->setValue(0);
//    
//                //send to all clients
//                string change = "texture//"+ ofToString(i)+": "+ ofToString(disc.getTexture(i));
//                server.sendToAll(change);
//            }
//            else if(e.getName() == "line"){
//                ofxUIToggle *toggle = e.getToggle();
//                int texture = disc.getTexture(disc.selected);
//                if(disc.getLife() > 0 && texture != 1) {
//                    textureChanged = true;
//                    disc.setTexture(disc.selected, 1);
//                    ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
//                    ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
//                    ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
//                    ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
//                    ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
//                    toggle0->setValue(false);
//                    toggle2->setValue(false);
//                    toggle3->setValue(false);
//                    toggle4->setValue(false);
//                }
//                else toggle->setValue(true);
//    
//                //sound
//                soundChange("envelope", i, 1);
//    
//                //send to all clients
//                string change = "texture//"+ ofToString(i)+": "+ ofToString(disc.getTexture(i));
//                server.sendToAll(change);
//    
//            }
//            else if(e.getName() == "tri"){
//                ofxUIToggle *toggle = e.getToggle();
//                int texture = disc.getTexture(disc.selected);
//                if(disc.getLife() > 0 && texture != 2) {
//                    textureChanged = true;
//                    disc.setTexture(disc.selected, 2);
//                    ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
//                    ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
//                    ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
//                    ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
//                    ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
//                    toggle0->setValue(false);
//                    toggle1->setValue(false);
//                    toggle3->setValue(false);
//                    toggle4->setValue(false);
//                }
//                else toggle->setValue(true);
//    
//                //sound
//                soundChange("envelope", i, 2);
//    
//                //send to all clients
//                string change = "texture//"+ ofToString(i)+": "+ ofToString(disc.getTexture(i));
//                server.sendToAll(change);
//            }
//            else if(e.getName() == "saw"){
//                ofxUIToggle *toggle = e.getToggle();
//                int texture = disc.getTexture(disc.selected);
//                if(disc.getLife() > 0 && texture != 3) {
//                    textureChanged = true;
//                    disc.setTexture(disc.selected, 3);
//                    ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
//                    ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
//                    ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
//                    ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
//                    ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
//                    toggle0->setValue(false);
//                    toggle1->setValue(false);
//                    toggle2->setValue(false);
//                    toggle4->setValue(false);
//                }
//                else toggle->setValue(true);
//    
//                //sound
//                soundChange("envelope", i, 3);
//    
//                //send to all clients
//                string change = "texture//"+ ofToString(i)+": "+ ofToString(disc.getTexture(i));
//                server.sendToAll(change);
//            }
//            else if(e.getName() == "rect"){
//                ofxUIToggle *toggle = e.getToggle();
//                int texture = disc.getTexture(disc.selected);
//                if(disc.getLife() > 0 && texture != 4) {
//                    textureChanged = true;
//                    disc.setTexture(disc.selected, 4);
//                    ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
//                    ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
//                    ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
//                    ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
//                    ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
//                    toggle0->setValue(false);
//                    toggle1->setValue(false);
//                    toggle2->setValue(false);
//                    toggle3->setValue(false);
//                }
//                else toggle->setValue(true);
//    
//                //sound
//                soundChange("envelope", i, 4);
//    
//                //send to all clients
//                string change = "texture//"+ ofToString(i)+": "+ ofToString(disc.getTexture(i));
//                server.sendToAll(change);
//            }
//        }
//}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    disc.update();
    
    
    //    groove.update();
    
    //    for(int i = 0; i< disc.getDiscIndex(); i++){
    //
    //        float amountFreq = ofMap(abs(disc.getRotationSpeed(i)), 0, 10, 0, 5000);
    //        float amountMod = ofMap(abs(disc.getPosition(i)), 0, 50, 0, 5000);
    //        soundChange("amountFreq", i, amountFreq);
    //        soundChange("amountMod", i, amountMod);
    //
    //        if (disc.isMoving(i)==1) {
    //            string change = "zPosition//"+ofToString(i)+": "+ofToString(disc.getPosition(i));
    //            sender.Send(change.c_str(), change.size());
    //            cout<< change <<endl;
    //        }
    //    }
    
    for ( int i = 0; i < server.getLastID(); i++ ) {
        if(server.isClientConnected(i)) {
            string str = server.receive(i);
            if(str.length()>0){
                received = ofSplitString(str, "//");
                title = received[0];
                
                /////////////// initialize /////////////////
                if (title == "hello") {
                    
                    Player* _player = new Player();
                    string thisIP = server.getClientIP(i);
                    _player->setIP(thisIP);
                    bool rejoin = false;
                    for(int i = 0; i < players.size(); i++){
                        if(_player->getIP() == players[i]->getIP()) {
                            _player = players[i];
                            rejoin = true;
                            break;
                        }
                        else continue;
                    }
                    if(rejoin == false) {
                        players.push_back(_player);
                        _player->setLife(100);
                        _player->setDiscIndex(-1);
                        _player->setColorByIndex(players.size());
                    }
                    _player->setConnection(true);
                    
                    string playerInfo;
                    playerInfo += "playerInfo//";
                    playerInfo += "IP: " + ofToString(_player->getIP()) + "//";
                    playerInfo += "color: " + ofToString(_player->getColor()) + "//";
                    playerInfo += "life: " + ofToString(_player->getLife()) + "//";
                    playerInfo += "index: " + ofToString(_player->getDiscIndex()) + "//";
                    server.send(i, playerInfo);
                    
                    string newPlayerInfo;
                    newPlayerInfo += "otherPlayers//";
                    newPlayerInfo += "IP: " + ofToString(_player->getIP()) + "//";
                    newPlayerInfo += "color: " + ofToString(_player->getColor()) + "//";
                    newPlayerInfo += "life: " + ofToString(_player->getLife()) + "//";
                    newPlayerInfo += "index: " + ofToString(_player->getDiscIndex()) + "//";
                    for (int j = 0; j < server.getLastID(); j++) {
                        if(j != i) server.send(j, newPlayerInfo);
                        else continue;
                    }
                    
                    if(players.size() > 1){
                        for(int j = 0; j < players.size() - 1; j++){
                            if( players[j] != _player){
                                string otherPlayers = "otherPlayers//";
                                otherPlayers += "IP: " + ofToString(players[j]->getIP()) + "//";
                                otherPlayers += "color: " + ofToString(players[j]->getColor()) + "//";
                                otherPlayers += "life: " + ofToString(players[j]->getLife()) + "//";
                                otherPlayers += "index: " + ofToString(players[j]->getDiscIndex()) + "//";
                                server.send(i, otherPlayers);
                                cout<< otherPlayers <<endl;
                            }
                        }
                    }
                    
                    string scale;
                    scale += "scale//";
                    for(int j = 0; j < sound.scale.size(); j++){
                        if( j == sound.scale.size()-1 ) scale += ofToString(sound.scale[j]);
                        else scale += ofToString(sound.scale[j])+": ";
                    }
                    server.send(i, scale);
                    
                    
                    string state;   //prepare to send the current state of the server
                    state += "state//";
                    state += "discIndex: " + ofToString(disc.getDiscIndex()) + "//";
                    for(int j = 0; j < disc.getDiscIndex(); j++){
                        state += "radius"+ofToString(j)+": " + ofToString(disc.getRadius(j)) + "//";
                        state += "density"+ofToString(j)+": " + ofToString(disc.getDensity(j)) + "//";
                        state += "rotation"+ofToString(j)+": " + ofToString(disc.getRotation(j)) + "//";
                        state += "rotationSpeed"+ofToString(j)+": " + ofToString(disc.getNetRotationSpeed(j)) + "//";
                        state += "texture"+ofToString(j)+": " + ofToString(disc.getTexture(j)) + "//";
                        state += "zPosition"+ofToString(j)+": " + ofToString(disc.getPosition(j)) + "//";
                        state += "posOffset"+ofToString(j)+": " + ofToString(disc.getPosOffset(j)) + "//";
                        state += "mute"+ofToString(j)+": " + ofToString(disc.isMute(j)) + "//";
                        state += "perlin"+ofToString(j)+": " + ofToString(disc.isMoving(j)) + "//";
                    }
                    
                    server.send(i, state);  //send current state to new client
                    cout<< "sent to "+ofToString(server.getClientIP(i)) <<endl;
                }
                /////////////////////////////////////////////
                
                /////////////single changes//////////////////
                else if (title == "rotationSpeed"){
                    vector<string> nameValue;
                    nameValue = ofSplitString(received[1], ": ");
                    int index = ofToInt(nameValue[0]);
                    disc.setRotationSpeed(index, ofToFloat(nameValue[1]));
                    
                    //change sound
//                    float netSpeed = abs(disc.getNetRotationSpeed(index));
//                    float beat = ofMap(netSpeed, 0, 10, 0, 1000);
//                    soundChange("bpm", index, beat);
                    
                    //update ui
                    //                    ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[index]);
                    //                    ofxUISlider *slider = static_cast <ofxUISlider*> (canvas->getWidget("rotation"+ofToString(index+1)));
                    //                    slider->setValue(disc.getNetRotationSpeed(index));
                }
                
                else if (title == "radius"){
                    vector<string> nameValue;
                    nameValue = ofSplitString(received[1], ": ");
                    int index = ofToInt(nameValue[0]);
                    disc.setThickness(index, ofToFloat(nameValue[1]));
                    
                    //change sound
//                    float q = ofMap(disc.getRadius(index)-disc.getRadius(index-1), 15, 100, 10, 0);
//                    soundChange("q", index, q);
                    
                    //update ui
                    //                    ofxUICanvas *canvas = static_cast<ofxUICanvas*>(ui[index]);
                    //                    ofxUISlider *slider = static_cast<ofxUISlider*>(canvas->getWidget("radius"+ofToString(index+1)));
                    //                    slider->setValue(disc.getThickness(index));
                }
                
                else if (title == "density"){
                    vector<string> nameValue;
                    nameValue = ofSplitString(received[1], ": ");
                    int index = ofToInt(nameValue[0]);
                    disc.setDensity(index, ofToFloat(nameValue[1]));
                    
                    //change sound
//                    float envelopeCoeff = ofMap(disc.getDensity(index), 1, 30, 1, 5);
//                    float pulseRatio = ofMap(disc.getDensity(index), 1, 30, 0.001, 1);
//                    soundChange("envelopeWidth", index, envelopeCoeff);
//                    soundChange("pulseLength", index, pulseRatio);
                    
                    //update ui
                    //                    ofxUICanvas *canvas = static_cast<ofxUICanvas*>(ui[index]);
                    //                    ofxUISlider *slider = static_cast<ofxUISlider*>(canvas->getWidget("density"+ofToString(index+1)));
                    //                    slider->setValue(disc.getDensity(index));
                }
                
                else if (title == "texture"){
                    vector<string> nameValue;
                    nameValue = ofSplitString(received[1], ": ");
                    int index = ofToInt(nameValue[0]);
                    disc.setTexture(index, ofToInt(nameValue[1]));
                    
                    //change sound
//                    soundChange("envelope", index, ofToInt(nameValue[1]));
                    
                    //update ui
                    //                    ofxUICanvas *canvas = static_cast<ofxUICanvas*>(ui[index]);
                    //                    ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
                    //                    ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
                    //                    ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
                    //                    ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
                    //                    ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
                    //                    if(disc.getTexture(index) == 0) toggle0->setValue(true);
                    //                    else toggle0->setValue(false);
                    //                    if(disc.getTexture(index) == 1) toggle1->setValue(true);
                    //                    else toggle1->setValue(false);
                    //                    if(disc.getTexture(index) == 2) toggle2->setValue(true);
                    //                    else toggle2->setValue(false);
                    //                    if(disc.getTexture(index) == 3) toggle3->setValue(true);
                    //                    else toggle3->setValue(false);
                    //                    if(disc.getTexture(index) == 4) toggle4->setValue(true);
                    //                    else toggle4->setValue(false);
                }
                
                else if (title == "mute"){
                    int thisDisc = ofToInt(received[1]);
                    disc.toggleMute(thisDisc);
//                    if(disc.isMute(thisDisc) == 0) soundChange("envelope", thisDisc, disc.getTexture(thisDisc));
//                    else soundChange("envelope", thisDisc, 0);
                }
                
                else if (title == "perlin"){
                    int thisDisc = ofToInt(received[1]);
                    disc.toggleMoving(thisDisc);
                }
                
                else if (title == "resetPerlin"){
                    vector<string> nameValue;
                    nameValue = ofSplitString(received[1], ": ");
                    disc.resetPerlin[ofToInt(nameValue[0])] = ofToInt(nameValue[1]);
                }
                
                else if (title == "life"){
                    for (int j = 0; j < players.size(); j++) {
                        if(server.getClientIP(i) == players[j]->getIP()) {
                            players[j]->setLife(ofToFloat(received[1]));
                            break;
                        }
                        else continue;
                    }
                    
                }
                /////////////////////////////////////////////
                
                ////////////send to all other clients////////
                for (int j = 0; j < server.getLastID(); j++) {
                    if(j != i && title != "hello") server.send(j, str);
                    else continue;
                }
            }
        
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackgroundGradient(255, 233);
    
    ofSetColor(0);
    ofDrawBitmapString("Server Running", 10, 20);
    ofDrawBitmapString("Number of Players: "+ofToString(players.size()), 10, 40);
    
    //    glEnable(GL_DEPTH_TEST);
    //
    //    ofPushMatrix();
    //
    //    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    ////
    ////    cam.begin();
    ////    groove.draw();
    ////    cam.end();
    //
    //    //    ofSetColor(ofColor::gray);
    //    //    ofNoFill();
    //    //    ofRectRounded(groove.lifeBarFrame, 5);
    //
    //    ofSetColor(disc.getColor(0));
    //    ofFill();
    //    ofRect(groove.lifeBar);
    //
    //    ofPopMatrix();
    //
    //    glDisable(GL_DEPTH_TEST);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' ') groove.turn = !groove.turn;
    if(key == 'p') {
        disc.toggleMoving(disc.selected);
        
        string change = "perlin//"+ofToString(disc.selected)+": "+ofToString(disc.isMoving(disc.selected));
        server.sendToAll(change);
        
    }
    if(key == 'o') {
        disc.resetPerlin[disc.selected] = 1;
        string change = "resetPerlin//"+ofToString(disc.selected)+": "+ofToString(disc.resetPerlin[disc.selected]);
        server.sendToAll(change);
        
    }
    if(key == '[') {
        for(int i = 0; i<disc.getDiscIndex(); i++){
            if (disc.isMoving(i) == 1) continue;
            else disc.toggleMoving(i);
        }
    }
    if(key == ']'){
        for(int i = 0; i<disc.getDiscIndex(); i++){
            disc.resetPerlin[i] = 1;
        }
    }
    
    if(key == 'a' && disc.selected != -1) {
        
        if(disc.getLife() > 0) {
            disc.setLife(costRotation);     // reduce life
            disc.setRotationSpeed(disc.selected, +0.05);
            
            //update ui
            ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
            ofxUISlider *slider = static_cast <ofxUISlider*> (canvas->getWidget("rotation"+ofToString(disc.selected+1)));
            slider->setValue(disc.getNetRotationSpeed(disc.selected));
            
            //change sound
            float netSpeed = abs(disc.getNetRotationSpeed(disc.selected));
            float beat = ofMap(netSpeed, 0, 10, 0, 1000);
            soundChange("bpm", disc.selected, beat);
        }
    }
    
    if(key == 'd' && disc.selected != -1 ) {
        
        if(disc.getLife() > 0) {
            disc.setLife(costRotation);     // reduce life
            disc.setRotationSpeed(disc.selected, -0.05);
            
            //update ui (w/ c++ style casting)
            ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
            ofxUISlider *slider = static_cast <ofxUISlider*> (canvas->getWidget("rotation"+ofToString(disc.selected+1)));
            slider->setValue(disc.getNetRotationSpeed(disc.selected));
            
            //change sound
            float netSpeed = abs(disc.getNetRotationSpeed(disc.selected));
            float beat = ofMap(netSpeed, 0, 10, 0, 1000);
            soundChange("bpm", disc.selected, beat);
        }
    }
    
    if(key == 'w'){
        
        if(disc.selected + 1 < disc.getDiscIndex()){
            
            disc.selected++;
            for(int i = 0; i < disc.getDiscIndex(); i++){
                ui[i]->setVisible(false);
            }
            ui[disc.selected]->toggleVisible();
        }
        else {
            disc.selected = 0;
            for(int i = 0; i < disc.getDiscIndex(); i++){
                ui[i]->setVisible(false);
            }
            ui[disc.selected]->toggleVisible();
        }
    }
    
    if(key == 's'){
        if(disc.selected - 1 > -1){
            disc.selected--;
            for(int i = 0; i < disc.getDiscIndex(); i++){
                ui[i]->setVisible(false);
            }
            ui[disc.selected]->toggleVisible();
        }
        else {
            disc.selected = disc.getDiscIndex()-1;
            for(int i = 0; i < disc.getDiscIndex(); i++){
                ui[i]->setVisible(false);
            }
            ui[disc.selected]->toggleVisible();
        }
    }
    
    if(key == OF_KEY_BACKSPACE) {
        disc.selected = -1;
        for(int i = 0; i < disc.getDiscIndex(); i++){
            ui[i]->setVisible(false);
        }
    }
    
    //    if(key == '1') {
    //        if(disc.getLife() > 0){
    //            disc.setLife(costTexture);
    //            disc.setTexture(disc.selected, 1);
    //            soundChange("envelope", disc.selected, 1);
    //
    //        }
    //    }
    //    if(key == '2') {
    //        if(disc.getLife() > 0){
    //            disc.setLife(costTexture);
    //            disc.setTexture(disc.selected, 2);
    //            soundChange("envelope", disc.selected, 2);
    //        }
    //    }
    //    if(key == '3') {
    //        if(disc.getLife() > 0){
    //            disc.setLife(costTexture);
    //            disc.setTexture(disc.selected, 3);
    //            soundChange("envelope", disc.selected, 3);
    //        }
    //    }
    //    if(key == '4') {
    //        if(disc.getLife() > 0){
    //            disc.setLife(costTexture);
    //            disc.setTexture(disc.selected, 4);
    //            soundChange("envelope", disc.selected, 0);
    //        }
    //    }
    //    if(key == '0') {
    //        if(disc.getLife() > 0){
    //            disc.setLife(costTexture);
    //            disc.setTexture(disc.selected, 0);
    //            soundChange("envelope", disc.selected, 0);
    //
    //            // when texture is set to blank, rotation stops
    //            disc.setRotationSpeed(disc.selected, -(disc.getRotationSpeed(disc.selected)+disc.getRotationSpeed(disc.selected-1)));
    //
    //            //            disc.setRotation(disc.selected, -disc.getRotationSpeed(disc.selected));
    //            soundChange("bpm", disc.selected, 0);
    //        }
    //    }
    if(key == 'f') {
        fullScreen = !fullScreen;
        ofSetFullscreen(fullScreen);
    }
    if(key == 'q' && disc.selected != -1 ) {
        disc.setPosition(disc.selected, disc.getPosition(disc.selected)+1);
        
    }
    if(key == 'e' && disc.selected != -1 ) {
        disc.setPosition(disc.selected, disc.getPosition(disc.selected)-1);
        
    }
    if(key == 'm' && disc.selected != -1 ) {
        if(disc.getLife() > 0){
            disc.setLife(costMute);
            if(disc.isMute(disc.selected) == 0) {
                disc.toggleMute(disc.selected); //mute on
                soundChange("envelope", disc.selected, 0);
            }
            else{
                disc.toggleMute(disc.selected); //mute off
                soundChange("envelope", disc.selected, disc.getTexture(disc.selected));
            }
            string change = "mute//"+ofToString(disc.selected);
            server.sendToAll(change);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if(radiusChanged) {
        radiusChanged = false;
        disc.setLife(costRadius);
    }
    else if(densityChanged){
        densityChanged = false;
        disc.setLife(costDensity);
    }
    else if(textureChanged){
        textureChanged = false;
        disc.setLife(costTexture);
    }
    else if(rotationChanged){
        rotationChanged = false;
        disc.setLife(costRotation);
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
float ofApp::triangleWave(float frequency){
    
    float phaseStep = frequency/44100;
    phase += phaseStep;
    return asin(sin(TWO_PI*phase))/PI;
    
}

//--------------------------------------------------------------
float ofApp::squareWave(float frequency){
    
    float phaseStep = frequency/44100.;
    phase += phaseStep;
    if(phase > (1)) phase = phase - (1);
    float y;
    if(phase < .5) y = 1;
    else y = -1;
    return y;
    
}

//--------------------------------------------------------------
float ofApp::sawWave(float frequency){
    
    float phaseStep = frequency/44100;
    phase += phaseStep;
    if(phase > (1)) phase = phase - (1);
    return 1 - ( phase/.5 );
    
}

//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {
    
    //    sound.synth.fillBufferOfFloats(output, bufferSize, nChannels);
    
    
    //    for(int i = 0; i < bufferSize; i++){
    //        float value = 0.1 *sawWave(220);
    //        output[2*i] = value;
    //        output[2*i+1] = value;
    //    }
    
}

//--------------------------------------------------------------
void ofApp::soundChange(string name, int index, float value) {
    
    //    if(name == "envelope"){
    //        if(disc.isMute(index) == 0){
    //            disc.setEnvelope(index, value);
    //            sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(index, 0));
    //            sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(index, 1));
    //            sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(index, 2));
    //            sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(index, 3));
    //        }
    //        else{
    //            disc.setEnvelope(index, 0);
    //            sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(index, 0));
    //            sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(index, 1));
    //            sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(index, 2));
    //            sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(index, 3));
    //        }
    //    }
    //    
    //    else sound.synth.setParameter(name+ofToString(index), value);
    
}

