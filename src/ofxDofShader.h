#pragma once

#include "ofMain.h"
// include GUI to control
// some params (OF8.0)
#include "ofxGui.h"

class ofxDofShader : public ofNode {
public:
    
    // GUI params
    // if GUI is not included
    // use basic ints & floats (< OF8.0
    ofxIntSlider passes;
    ofxFloatSlider blur;
    ofxFloatSlider focus;
    // int passes;
    // float blur;
    // float focus
    
    void setup(float width = ofGetWidth(), float height = ofGetHeight());
    void render(ofFbo &scene);
    
private:
    
    ofFbo ping;
    ofFbo pong;
    ofShader shader;
    
};