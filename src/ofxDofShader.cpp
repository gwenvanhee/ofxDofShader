#define STRINGIFY(A) #A
#include "ofxDofShader.h"

//--------------------------------------------------------------
void ofxDofShader::setup(float width, float height) {
    
    // some defaults
    // there are 3 params to control
    // - passes : how many times the shader is applied
    // - blur   : gaussian blu amount
    // - focus  : focal point for the DOF
    passes = 1;
    blur = 5;
    focus = .3;
    
    // fbo settings
    ofFbo::Settings s;
    s.width  = width;
    s.height = height;
    
    // allocate ping-pong buffers
    // gaussian blur is applied in 2 runs
    // first run blurs vertically
    // second one blurs horizontally
    ping.allocate(s);
    pong.allocate(s);
    
    // create fragment shader
    string fragShader = STRINGIFY(
                                  
            uniform int pass;
            uniform float blur;
            uniform float focus;
                                  
            uniform sampler2DRect tex;
            uniform sampler2DRect depth;
                                  
            void main() {
                                      
                    // texture lookup
                    float x = gl_TexCoord[0].s;
                    float y = gl_TexCoord[0].t;
                    float dof = texture2DRect(depth, vec2(x, y)).r;
                
                    // calculate focus
                    dof = abs( pow(dof, 10.) - focus);
                                      
                    // gaussian blur offset & weight
                    float offset[5];
                    offset[0] = 0.;
                    offset[1] = 1.;
                    offset[2] = 2.;
                    offset[3] = 3.;
                    offset[4] = 4.;
                                      
                    float weight[5];
                    weight[0] = 0.2270270270;
                    weight[1] = 0.1945945946;
                    weight[2] = 0.1216216216;
                    weight[3] = 0.0540540541;
                    weight[4] = 0.0162162162;
                                      
                    // base pixel
                    vec4 color = texture2DRect(tex, vec2(x, y)) * weight[0];
                
                    // neighbours
                    for(int i=1; i<5; i++) {
                        
                            // pass 1 - horizontal blur
                            if(pass == 0) {
                                
                                color += texture2DRect(tex, vec2(x + offset[i]*blur*dof, y)) * weight[i];
                                color += texture2DRect(tex, vec2(x - offset[i]*blur*dof, y)) * weight[i];
                                
                            }
                                          
                            // pass 2 - vertical blur
                            if(pass == 1) {
                                
                                color += texture2DRect(tex, vec2(x, y + offset[i]*blur*dof)) * weight[i];
                                color += texture2DRect(tex, vec2(x, y - offset[i]*blur*dof)) * weight[i];
                                
                            }
                        
                    }
                
                    // set fragment color
                    // gl_FragColor = vec4(dof, dof, dof, 1.); 
                    gl_FragColor = vec4(color.rgb, 1.);
                                      
            }
    );
    
    // setup & link shader
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShader);
    shader.linkProgram();
    
}

//--------------------------------------------------------------
void ofxDofShader::render(ofFbo &scene) {
    
    // pass scene
    ping.begin();
    scene.draw(0, 0, ping.getWidth(), ping.getHeight());
    ping.end();
    
    // ping-pong blur
    for(int i=0; i<(int)passes; i++) {
        
        // horizontal blur
        pong.begin();
        ofClear(0, 0, 0, 0);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        shader.begin();
        shader.setUniform1i("pass", 0);
        shader.setUniform1f("blur", blur);
        shader.setUniform1f("focus", focus);
        shader.setUniformTexture("tex", ping.getTextureReference(), 0);
        shader.setUniformTexture("depth", scene.getDepthTexture(), 1);
        ping.draw(0,0);
        shader.end();
        pong.end();
        
        
        // vertical blur
        ping.begin();
        ofClear(0, 0, 0, 0);
        ofEnableBlendMode(OF_BLENDMODE_ALPHA);
        shader.begin();
        shader.setUniform1i("pass", 1);
        shader.setUniform1f("blur", blur);
        shader.setUniform1f("focus", focus);
        shader.setUniformTexture("tex", pong.getTextureReference(), 0);
        shader.setUniformTexture("depth", scene.getDepthTexture(), 1);
        pong.draw(0,0);
        shader.end();
        ping.end();
        
    }
    
    // draw result
    ping.draw(0, 0);
    
}
