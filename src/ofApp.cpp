#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    
    ofSetFrameRate(60);
    ofBackground(0, 0, 0);
    
    sound.listDevices();
    sound.setDeviceID(0);
    
    int bufferSize = 256;
    
    left.assign(bufferSize, 0.0);
    right.assign(bufferSize, 0.0);
    volHistory.assign(ofGetWidth(), 0.0);
    
    drawCounter = 0;
    bufferCounter	= 0;
    drawCounter		= 0;
    smoothedVol     = 0.0;
    scaledVol		= 0.0;
    
    sound.setup(this, 0, 2, 44100, bufferSize, 4);
    
    
    
    image.loadImage("Richard D. James Album.jpg");
    image.resize(image.getWidth()/4,image.getHeight()/4);

    mesh.setMode(OF_PRIMITIVE_LINES);

    
    float intensityLowerThreshold = 0;
    float intensityUpperThreshold = 225;
    int w = image.getWidth();
    int h = image.getHeight();
    
    for(int x = 0; x < w; x++){
        
        for(int y=0; y<h; y++){
            ofColor c = image.getColor(x, y);
            float intensity = c.getLightness();
            if(intensity <= intensityUpperThreshold && intensity>= intensityLowerThreshold){
                float saturation = c.getSaturation();
                float z = ofMap(saturation, 0, 255, -100, 100);
                ofVec3f pos(x*5,y*5,z);
                mesh.addVertex(pos);
                mesh.addColor(c);
                offsets.push_back(ofVec3f(ofRandom(0,100000), ofRandom(0,100000),ofRandom(0,100000)));
            }
                
        }
    }
    cout << mesh.getNumVertices() << endl;
    
    float connectionDistance = 15;
    int numVerts = mesh.getNumVertices();
    for (int a=0; a<numVerts; ++a) {
        ofVec3f verta = mesh.getVertex(a);
        for (int b=a+1; b<numVerts; ++b) {
            ofVec3f vertb = mesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= connectionDistance) {
                mesh.addIndex(a);
                mesh.addIndex(b);
            }
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    scaledVol = ofMap(smoothedVol, 0.0, 0.17, 0.0, 1.0, true);
    volHistory.push_back( scaledVol );
    
    if( volHistory.size() >= ofGetWidth() ){
        volHistory.erase(volHistory.begin(), volHistory.begin()+1);
    }

    
    int numVerts = mesh.getNumVertices();
    
    if(scaledVol> .75){
    for (int i=0; i<numVerts; ++i) {
        ofVec3f vert = mesh.getVertex(i);
        
        ofVec3f timeOffsets = offsets[i];
       
        vert.z = ofRandom(0,200);
        mesh.setVertex(i, vert);
        }
    }
    else if (scaledVol> .5){
        for (int i=0; i<numVerts; ++i) {
            ofVec3f vert = mesh.getVertex(i);
            
            ofVec3f timeOffsets = offsets[i];
            
            vert.z = ofRandom(0,100);
            mesh.setVertex(i, vert);
        }
    }
    else if (scaledVol> .25){
        for (int i=0; i<numVerts; ++i) {
            ofVec3f vert = mesh.getVertex(i);
            
            ofVec3f timeOffsets = offsets[i];
            
            vert.z = ofRandom(0,50);
            mesh.setVertex(i, vert);
        }
    }
    else{
        for (int i=0; i<numVerts; ++i) {
            ofVec3f vert = mesh.getVertex(i);
            
            ofVec3f timeOffsets = offsets[i];
            
            vert.z = ofRandom(0,5);
            mesh.setVertex(i, vert);
        }
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofColor centerColor = ofColor(65,58,48);
    ofColor edgeColor(0,0,0);
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);

    ofBackground(255);
    easyCam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    mesh.draw();
    ofPopMatrix();
    easyCam.end();
    
    
    
    ofPushStyle();
    ofPushMatrix();
    
    ofSetColor(0,0);
    ofDrawBitmapString("Scaled average vol (0-100): " + ofToString(scaledVol * 100.0, 0), 4, ofGetHeight()-118);
    ofRect(0, ofGetHeight()-100, ofGetWidth(), 100);
    
    ofSetColor(135, 58, 245);
    ofFill();
    
    //lets draw the volume history as a graph
    ofBeginShape();
    for (unsigned int i = 0; i < volHistory.size(); i++){
        if( i == 0 ) ofVertex(i, ofGetHeight());
        
        ofVertex(i, ofGetHeight() - volHistory[i] * 70);
        
        if( i == volHistory.size() -1 ) ofVertex(i, ofGetHeight());
    }
    ofEndShape(false);
    ofPopMatrix();
    ofPopStyle();
    drawCounter++;

}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){

    currentVol = 0.0;
    
    //interleaved samples
    int numCounted = 0;
    
    //calculate volume via RMS
    for(int i = 0; i < bufferSize; i++){
        left[i] = input[i*2];
        right[i] = input[i*2+1];
        
        currentVol += left[i]*left[i];
        currentVol += right[i]*right[i];
        
        numCounted += 2;
    }
    
    //this is how we get the mean of rms :)
    currentVol /= (float)numCounted;
    
    // this is how we get the root of rms :)
    currentVol = sqrt( currentVol );
    
    
    
    smoothedVol *= 0.93;
    smoothedVol += 0.07 * currentVol;
   
    
    bufferCounter++;
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if( key == ' ' && soundOn == false){
        sound.start();
        soundOn = true;
        cout << "stream is on" << endl;

     }
     else if( key == ' ' && soundOn == true){
         sound.stop();
         soundOn = false;
         cout << "stream is off" << endl;

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
