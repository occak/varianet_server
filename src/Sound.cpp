//
//  Sound.cpp
//  groove_snd
//
//  Created by cemcakmak on 13/03/15.
//
//

#include "Sound.h"

void Sound::setup(Disc* disc){
    
    this->disc = disc;
    
    ///////////////////////
    // Tonic Synth setup
    ///////////////////////
    
    
    vector<float> scale;
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    scale.push_back(ofRandom(13));
    
    for(int i = 0; i < disc->getDiscIndex(); i++){
        
        
        // synthesis network
        
        Generator groove;
        
        ControlGenerator bpm =  synth.addParameter("bpm"+ofToString(i), 0);
        
        float pitch = ofMap(disc->getDensity(i), 1, 30, 48, 1);
        
        ControlGenerator freq = synth.addParameter("freq"+ofToString(i), pitch).min(100).max(1200);
        
        ControlGenerator metronome = ControlMetro().bpm(bpm);
        
        
        float pulseRatio = ofMap(disc->getDensity(i), 1, 30, 0.001, 1);
        ControlGenerator pulseLength = synth.addParameter("pulseLength"+ofToString(i), pulseRatio);
        ControlGenerator pulse = ControlPulse().length(pulseLength).input(metronome);
        
        float envelopeCoeff = ofMap(disc->getDensity(i), 1, 30, 1, 5);
        ControlGenerator envelope = synth.addParameter("envelope"+ofToString(i), envelopeCoeff);
        ControlGenerator attack = synth.addParameter("attack"+ofToString(i),disc->getEnvelope(i, 0));
        ControlGenerator decay = synth.addParameter("decay"+ofToString(i),disc->getEnvelope(i, 1));
        ControlGenerator sustain = synth.addParameter("sustain"+ofToString(i),disc->getEnvelope(i, 2));
        ControlGenerator release = synth.addParameter("release"+ofToString(i),disc->getEnvelope(i, 3));
        
        Generator amplitude = ADSR().attack(attack*envelope).decay(decay*pulseLength*envelope).sustain(sustain*envelope).release(release*envelope).trigger(pulse);
        
        Generator snd = SawtoothWave();
        groove = snd * amplitude;
        
        float qTarget = ofMap(disc->getRadius(i)-disc->getRadius(i-1), 15, 100, 50, 10);
        float cutoffTarget = ofMap(disc->getRadius(i)-disc->getRadius(i-1), 15, 100, 0, 50);
        
        ControlGenerator cutoff = synth.addParameter("frequency"+ofToString(i), cutoffTarget);
        ControlSnapToScale scaleSnapper = ControlSnapToScale().setScale(scale);
        scaleSnapper.input(50 + cutoff);
        ControlGenerator filterFreq = ControlMidiToFreq().input(scaleSnapper);
        
        ControlGenerator q = synth.addParameter("q"+ofToString(i),qTarget).min(0).max(50);
        
        Generator filter = BPF24().input(groove).cutoff(filterFreq).Q(q);
        Generator limiter = Limiter().input(2*filter);
        
        master = master + limiter;
    }
    
    synth.setOutputGen(master);
}

void Sound::update(){
    
    if (disc->perlin) {
        
        for(int i = 0; i < disc->getDiscIndex(); i++){
            
            //            float newFreq = ofMap(abs(disc->getPosition(i)), 0, 200, 100, 1200);
            //            synth.setParameter("freq"+ofToString(i), newFreq);
            //
            //            float newCutoff = ofMap(disc->getPosition(i), 0, 200, 100, 10000);
            //            synth.setParameter("cutoff"+ofToString(i), newFreq);
            //
            //            float newQ = ofMap(disc->getDensity(i), 1, 16, 0, 50);
            //            synth.setParameter("q"+ofToString(i), newQ);
            
        }
        
    }
    
    
}

//vector<float> Sound::setEnvelope(int index) {
//
//    float attack, decay, sustain, release;
//    switch (disc->getTexture(index)) {
//        case 0:
//            attack = 0;
//            decay = 0;
//            sustain = 0;
//            release = 0;
//            break;
//
//        case 1:
//            attack = 0.01;
//            decay = 0;
//            sustain = 0;
//            release = 0.01; //texture 1
//            break;
//
//        case 2:
//            attack = 0.05;
//            decay = 0.1;
//            sustain = 0;
//            release = 0;  //texture 2&3
//            break;
//
//        case 3:
//            attack = 0.05;
//            decay = 0.1;
//            sustain = 0;
//            release = 0;  //texture 2&3
//            break;
//
//        case 4:
//            attack = 0.01;
//            decay = 0;
//            sustain = 0.05;
//            release = 0.001;    //texture 4
//            break;
//
//        default:
//            break;
//    }
//
//    envelopes[index].push_back(attack);
//    envelopes[index].push_back(decay);
//    envelopes[index].push_back(sustain);
//    envelopes[index].push_back(release);
//
//}