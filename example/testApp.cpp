#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    // setup & allocate scene
    ofFbo::Settings s;
    
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.useDepth = true;
    s.depthStencilAsTexture = true;
    
    scene.allocate(s);
    
    // setup & allocate dof
    ofFbo::Settings sdof;
    sdof.width  = ofGetWidth();
    sdof.height = ofGetHeight();
    
    dof.allocate(sdof);
    
    // setup shader
    shader.setup();
    
    // gui
    gui.setup();
    gui.add(shader.passes.setup("dof.passes", 1, 0, 5));
    gui.add(shader.blur.setup("dof.blur", 2.6, 0, 10));
    gui.add(shader.focus.setup("dof.focus", 0.45, 0, 1));

    
}

//--------------------------------------------------------------
void testApp::update(){
                  
}

//--------------------------------------------------------------
void testApp::draw(){
    
    // start drawing to
    // the offscreen buffer
    scene.begin();
    ofClear(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    
        // some awesome 3D stuff

    glDisable(GL_DEPTH_TEST);
    scene.end();

    
    // start post-processing
    // the offscreen buffer
    dof.begin();
    ofClear(0, 0, 0, 0);
    shader.render(scene);
    dof.end();

    
    // debug output
    // dof.draw(0, 0);
    // scene.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    // draw the depth texture
    scene.getDepthTexture().draw(ofGetWidth()*.75, ofGetHeight()*.75, ofGetWidth()*.25, ofGetHeight()*.25);
    
    // gui
    gui.draw();

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
        
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
