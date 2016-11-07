#include "ofApp.h"
#include <string>
//--------------------------------------------------------------
void ofApp::setup(){

    // Make the app full screen
    ofSetFullscreen(true);
    //need this to bind texture around sphere
    ofDisableArbTex();

    // Turn on depth testing so rendering happens according to z-depth rather
    // than draw order.
    ofEnableDepthTest();

    // Black background
    ofBackground(0);

    //setup solar system
    s.setup();

    //setup easyCam for zooming and moving perspective
    cam.setDistance(700);

    //setuo sound stream
    soundStream.setup(this, MY_CHANNELS, MY_CHANNELS, MY_SRATE, MY_BUFFERSIZE, MY_NBUFFERS);

    //resize and initialise left and right audio channels
    left.resize(MY_BUFFERSIZE, 0);
    right.resize(MY_BUFFERSIZE, 0);

    //setup the fft
    fft = ofxFft::create(MY_BUFFERSIZE, OF_FFT_WINDOW_HAMMING);


    //create snd object
    snd = new stk::FileWvIn[s.getNplanets()];

    //integer variable to keep track of which wave file is playing
    whichFile = -1;


    //load STK object with sound file
    const string fname[] = {"mercury.wav", "venus.wav", "earth.wav", "mars.wav",
                            "jupiter.wav", "saturn.wav",
                            "uranus.wav", "neptune.wav"};

    for(int i=0;i<s.getNplanets();i++){
        snd[i].setRate((float)MY_SRATE);
        snd[i].openFile(ofToDataPath(fname[i], true));

    }

}

//--------------------------------------------------------------
void ofApp::update(){

    //normalise left channel waveform
    float maxValue = 0;
    for(int i=0;i<left.size();i++){
        if(abs(left[i] > maxValue))
            maxValue = abs(left[i]);
    }

    for(int i = 0;i<left.size();i++)
        left[i] /= maxValue;

    //take fft of left channel
    fft->setSignal(&left[0]);

    //get magnitude and copy them to audio bins
    float* fftData = fft->getAmplitude();

    //normalise the FFT magnitude values
    maxValue = 0;
    for(int i = 0; i< fft->getBinSize(); i++){
        if(abs(fftData[i]) > maxValue)
            maxValue = abs(fftData[i]);
    }

    for(int i = 0; i < fft->getBinSize();i++)
        fftData[i] /= maxValue;

    //divide into 8 frequency bands and get average FFT value for each
    float* fbands = new float[s.getNplanets()];

    int nPointsPerBin = fft->getBinSize()/s.getNplanets();

    for(int i=0; i < s.getNplanets(); i++){
        for (int j = 0; j< nPointsPerBin; j++){
            fbands[i] += fftData[(nPointsPerBin * i) + j];
        }
        fbands[i] /= nPointsPerBin;

        //replace nan and negative values with 0
        if(fbands[i] != fbands[i] || fbands[i] < 0){fbands[i] = 0.0;}

        //print values for debugging
        std :: cout << "Orbit Speed for planet " << i << " is: " << fbands[i] << endl;
    }

    //speed of rotation of each planet depends on value of average intensity
    //in each frequency band
    s.update(fbands);

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofPushMatrix();
    ofTranslate(ofGetWindowWidth()*0.5, ofGetWindowHeight()*0.5);
    cam.begin();
    s.draw();
    cam.end();
    ofPopMatrix();

}

//--------------------------------------------------------------
// audioIn() -- deal with incoming audio buffer
//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    // Write incoming audio to buffer. Note: samples are interleaved.
    for (int i = 0; i < bufferSize; i++){
        left[i]		= input[i*2];
        right[i]	= input[i*2+1];
    }

}


//--------------------------------------------------------------

void ofApp::keyPressed(int key){

    //reset previous wave file
    if(whichFile > -1)
        snd[whichFile].reset();
    if(key >= '1' && key <= '8')
        whichFile = key - 49;
    else
        whichFile = -1;
}

//---------------------------------------------------------------
void ofApp::audioOut(float *output, int bufferSize, int nChannels){
    if(whichFile > -1){
        stk::StkFrames frames(bufferSize,2);
        snd[whichFile].tick(frames);

    for (int i = 0; i < bufferSize ; i++) {
        output[2*i] = frames(i,0);
        output[2*i+1] = frames(i,1);
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

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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

//destructor equivalent
void ofApp::exit(){

    //close all audio files
    for(int i=0; i < s.getNplanets(); i++){
        if(snd[i].isOpen())
            snd[i].closeFile();
    }
    //delete fft buffer
    delete [] fft;
    //delete sound buffer
    delete [] snd;

}
