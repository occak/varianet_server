#include "ofApp.h"

/*
 texture patterns can be randomized
 
 what property has what audible consequence?
 UI?
 
 send all disc values to client when initialized
 
 */


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    ofBackground(255);
    
    //set up network
    server.setup(10002);
    server.setMessageDelimiter("varianet");
    
    // set up values of objects
    disc.setup();
    groove.setup(&disc);
    
    //set up gui
    for(int i = 0; i < disc.getDiscIndex(); i++){
        
        ofxUICanvas *_ui;
        
        _ui = new ofxUICanvas("Groove " + ofToString(i+1));
        _ui->addSlider("rotation" + ofToString(i+1), -10, 10, disc.getRotationSpeed(i));
        _ui->addSlider("radius" + ofToString(i+1), 15, 100, disc.getRadius(i)-disc.getRadius(i-1));
        _ui->addBiLabelSlider("density" + ofToString(i+1), "sparse", "dense", 30, 1, disc.getDensity(i));
        
        if(disc.getTexture(i)==0) _ui->addToggle("blank", true);
        else _ui->addToggle("blank", false);
        if(disc.getTexture(i)==1) _ui->addToggle("line", true);
        else _ui->addToggle("line", false);
        if(disc.getTexture(i)==2) _ui->addToggle("tri", true);
        else _ui->addToggle("tri", false);
        if(disc.getTexture(i)==3) _ui->addToggle("saw", true);
        else _ui->addToggle("saw", false);
        if(disc.getTexture(i)==4) _ui->addToggle("rect", true);
        else _ui->addToggle("rect", false);
        
        _ui->autoSizeToFitWidgets();
        _ui->setVisible(false);
        ofAddListener(_ui->newGUIEvent, this, &ofApp::guiEvent);
        
        ui.push_back(_ui);
        
    }
    
    
    //set up audio stream & synth network
    phase = 0;
    volume = 0;
    ofSoundStreamSetup(2, 0); // 2 out, 0 in
    
    sound.setup(&disc);
    
    // set up game costs
    costRadius = .1;
    costDensity = .1;
    costTexture = 1;
    costRotation = .1;
    costMute = .5;
}
//--------------------------------------------------------------
void ofApp::exit(){
    
    for(int i = 0; i < disc.getDiscIndex(); i++){
        delete ui[i];
    }
    
}
//--------------------------------------------------------------
void ofApp::guiEvent(ofxUIEventArgs &e)
{
    for(int i = 0; i < disc.getDiscIndex(); i++){
        if(e.getName() == "rotation" + ofToString(i+1)){
            ofxUISlider *slider = e.getSlider();
            
            if(disc.getLife()>0){
                rotationChanged = true;
                disc.setRotationSpeed(i, (slider->getScaledValue()-(disc.getRotationSpeed(i)+disc.getRotationSpeed(i-1))));
                //change sound
                float netSpeed = abs(abs(disc.getRotationSpeed(disc.selected))-abs(disc.getRotationSpeed(disc.selected-1)));
                float beat = ofMap(netSpeed, 0, 10, 0, 1000);
                sound.synth.setParameter("bpm"+ofToString(disc.selected), beat);
            }
        }
        else if(e.getName() == "radius" + ofToString(i+1)){
            ofxUISlider *slider = e.getSlider();
            
            if(disc.getLife() > 0) {
                radiusChanged = true;
                disc.setRadius(i, slider->getScaledValue());
                float q = ofMap(disc.getRadius(disc.selected)-disc.getRadius(disc.selected-1), 15, 100, 10, 0);
                sound.synth.setParameter("q"+ofToString(disc.selected), q);
            }
        }
        else if(e.getName() == "density" + ofToString(i+1)){
            ofxUISlider *slider = e.getSlider();
            if(disc.getLife() > 0) {
                densityChanged = true;
                disc.setDensity(i, slider->getScaledValue());
                float envelopeCoeff = ofMap(disc.getDensity(i), 1, 30, 1, 5);
                sound.synth.setParameter("envelope"+ofToString(i),envelopeCoeff);
                float pulseRatio = ofMap(disc.getDensity(i), 1, 30, 0.001, 1);
                sound.synth.setParameter("pulseLength"+ofToString(i), pulseRatio);
                
            }
        }
        else if(e.getName() == "blank"){
            ofxUIToggle *toggle = e.getToggle();
            int texture = disc.getTexture(disc.selected);
            if(disc.getLife() > 0 && texture != 0) {
                textureChanged = true;
                disc.setTexture(disc.selected, 0);
                ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
                ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
                ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
                ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
                ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
                toggle1->setValue(false);
                toggle2->setValue(false);
                toggle3->setValue(false);
                toggle4->setValue(false);
            }
            else toggle->setValue(true);
        }
        else if(e.getName() == "line"){
            ofxUIToggle *toggle = e.getToggle();
            int texture = disc.getTexture(disc.selected);
            if(disc.getLife() > 0 && texture != 1) {
                textureChanged = true;
                disc.setTexture(disc.selected, 1);
                ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
                ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
                ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
                ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
                ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
                toggle0->setValue(false);
                toggle2->setValue(false);
                toggle3->setValue(false);
                toggle4->setValue(false);
            }
            else toggle->setValue(true);
            
            //sound
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 1);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            
        }
        else if(e.getName() == "tri"){
            ofxUIToggle *toggle = e.getToggle();
            int texture = disc.getTexture(disc.selected);
            if(disc.getLife() > 0 && texture != 2) {
                textureChanged = true;
                disc.setTexture(disc.selected, 2);
                ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
                ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
                ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
                ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
                ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
                toggle0->setValue(false);
                toggle1->setValue(false);
                toggle3->setValue(false);
                toggle4->setValue(false);
            }
            else toggle->setValue(true);
            
            //sound
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 2);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
        }
        else if(e.getName() == "saw"){
            ofxUIToggle *toggle = e.getToggle();
            int texture = disc.getTexture(disc.selected);
            if(disc.getLife() > 0 && texture != 3) {
                textureChanged = true;
                disc.setTexture(disc.selected, 3);
                ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
                ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
                ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
                ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
                ofxUIToggle *toggle4 = static_cast <ofxUIToggle*> (canvas->getWidget("rect"));
                toggle0->setValue(false);
                toggle1->setValue(false);
                toggle2->setValue(false);
                toggle4->setValue(false);
            }
            else toggle->setValue(true);
            
            //sound
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 3);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
        }
        else if(e.getName() == "rect"){
            ofxUIToggle *toggle = e.getToggle();
            int texture = disc.getTexture(disc.selected);
            if(disc.getLife() > 0 && texture != 4) {
                textureChanged = true;
                disc.setTexture(disc.selected, 4);
                ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
                ofxUIToggle *toggle0 = static_cast <ofxUIToggle*> (canvas->getWidget("blank"));
                ofxUIToggle *toggle1 = static_cast <ofxUIToggle*> (canvas->getWidget("line"));
                ofxUIToggle *toggle2 = static_cast <ofxUIToggle*> (canvas->getWidget("tri"));
                ofxUIToggle *toggle3 = static_cast <ofxUIToggle*> (canvas->getWidget("saw"));
                toggle0->setValue(false);
                toggle1->setValue(false);
                toggle2->setValue(false);
                toggle3->setValue(false);
            }
            else toggle->setValue(true);
            
            //sound
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 4);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    disc.update();
    groove.update();
    
    for(int i = 0; i< disc.getDiscIndex(); i++){
        float amountFreq = ofMap(abs(disc.getRotationSpeed(i)), 0, 10, 0, 5000);
        sound.synth.setParameter("amountFreq"+ofToString(i), amountFreq);
        float amountMod = ofMap(abs(disc.getPosition(i)), 0, 50, 0, 5000);
        sound.synth.setParameter("amountMod"+ofToString(i), amountMod);
    }
    
    sound.update();
    
    for ( int i = 0; i < server.getLastID(); i++ ) {
        if(server.isClientConnected(i)) {
            string str = server.receive(i);
            if(str.length()>0){
                received = ofSplitString(str, ": ");
                title = received[0];
            }
        }
    }
    
    
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    
    glEnable(GL_DEPTH_TEST);
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    cam.begin();
    groove.draw();
    cam.end();
    
    ofSetColor(ofColor::gray);
    ofNoFill();
    ofRectRounded(groove.lifeBarFrame, 5);
    
    ofSetColor(ofColor::darkGray);
    ofFill();
    ofRectRounded(groove.lifeBar, 3);
    
    ofPopMatrix();
    
    glDisable(GL_DEPTH_TEST);
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == ' ') groove.turn = !groove.turn;
    if(key == 'p') disc.toggleMoving(disc.selected);
    if(key == 'o') disc.resetPerlin[disc.selected] = 1;
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
    
    if(key == 'j' && disc.selected != -1) {
        
        if(disc.getLife() > 0) {
            disc.setLife(costRotation);     // reduce life
            disc.setRotationSpeed(disc.selected, +0.05);
            
            //update ui
            ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
            ofxUISlider *slider = static_cast <ofxUISlider*> (canvas->getWidget("rotation"+ofToString(disc.selected+1)));
            slider->setValue(disc.getRotationSpeed(disc.selected));
            
            //change sound
            float netSpeed = abs(abs(disc.getRotationSpeed(disc.selected))-abs(disc.getRotationSpeed(disc.selected-1)));
            float beat = ofMap(netSpeed, 0, 10, 0, 1000);
            sound.synth.setParameter("bpm"+ofToString(disc.selected), beat);
        }
    }
    
    if(key == 'l' && disc.selected != -1 ) {
        
        if(disc.getLife() > 0) {
            disc.setLife(costRotation);     // reduce life
            disc.setRotationSpeed(disc.selected, -0.05);
            
            //update ui (w/ c++ style casting)
            ofxUICanvas *canvas = static_cast <ofxUICanvas*> (ui[disc.selected]);
            ofxUISlider *slider = static_cast <ofxUISlider*> (canvas->getWidget("rotation"+ofToString(disc.selected+1)));
            slider->setValue(disc.getRotationSpeed(disc.selected));
            
            //change sound
            float netSpeed = abs(abs(disc.getRotationSpeed(disc.selected))-abs(disc.getRotationSpeed(disc.selected-1)));
            float beat = ofMap(netSpeed, 0, 10, 0, 1000);
            sound.synth.setParameter("bpm"+ofToString(disc.selected), beat);
        }
    }
    
    if(key == 'i'){
        
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
    
    if(key == 'k'){
        if(disc.selected - 1 > -1){
            disc.selected--;
            for(int i = 0; i < disc.getDiscIndex(); i++){
                ui[i]->setVisible(false);
            }
            ui[disc.selected]->toggleVisible();
        }
        else {
            disc.selected = 9;
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
    
    if(key == '1') {
        if(disc.getLife() > 0){
            disc.setLife(costTexture);
            disc.setTexture(disc.selected, 1);
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 1);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            
        }
    }
    if(key == '2') {
        if(disc.getLife() > 0){
            disc.setLife(costTexture);
            disc.setTexture(disc.selected, 2);
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 2);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
        }
    }
    if(key == '3') {
        if(disc.getLife() > 0){
            disc.setLife(costTexture);
            disc.setTexture(disc.selected, 3);
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 3);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
        }
    }
    if(key == '4') {
        if(disc.getLife() > 0){
            disc.setLife(costTexture);
            disc.setTexture(disc.selected, 4);
            if(disc.isMute(disc.selected) == 0){
                disc.setEnvelope(disc.selected, 4);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            }
        }
    }
    if(key == '0') {
        if(disc.getLife() > 0){
            disc.setLife(costTexture);
            disc.setTexture(disc.selected, 0);
            disc.setEnvelope(disc.selected, 0);
            sound.synth.setParameter("attack"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 0));
            sound.synth.setParameter("decay"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 1));
            sound.synth.setParameter("sustain"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 2));
            sound.synth.setParameter("release"+ofToString(disc.selected),disc.getEnvelope(disc.selected, 3));
            
            // when texture is set to blank, rotation stops
            disc.setRotationSpeed(disc.selected, -(disc.getRotationSpeed(disc.selected)+disc.getRotationSpeed(disc.selected-1)));
            
            //            disc.setRotation(disc.selected, -disc.getRotationSpeed(disc.selected));
            sound.synth.setParameter("bpm"+ofToString(disc.selected), 0);
        }
    }
    if(key == 'f') {
        fullScreen = !fullScreen;
        ofSetFullscreen(fullScreen);
    }
    if(key == 'w' && disc.selected != -1 ) {
        disc.setPosition(disc.selected, disc.getPosition(disc.selected)+1);
        
    }
    if(key == 's' && disc.selected != -1 ) {
        disc.setPosition(disc.selected, disc.getPosition(disc.selected)-1);
        
    }
    if(key == 'm' && disc.selected != -1 ) {
        if(disc.getLife() > 0){
            disc.setLife(costMute);
            if(disc.isMute(disc.selected) == 0) {
                disc.toggleMute(disc.selected);
                disc.setEnvelope(disc.selected, 0);
                sound.synth.setParameter("attack"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 3));
            }
            else{
                disc.toggleMute(disc.selected);
                disc.setEnvelope(disc.selected, disc.getTexture(disc.selected));
                sound.synth.setParameter("attack"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 0));
                sound.synth.setParameter("decay"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 1));
                sound.synth.setParameter("sustain"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 2));
                sound.synth.setParameter("release"+ofToString(disc.selected), disc.getEnvelope(disc.selected, 3));
            }
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
    
    sound.synth.fillBufferOfFloats(output, bufferSize, nChannels);
    
    
    //    for(int i = 0; i < bufferSize; i++){
    //        float value = 0.1 *sawWave(220);
    //        output[2*i] = value;
    //        output[2*i+1] = value;
    //    }
    
}
