//
//  ofxParallax.h
//
//  Created by Daniel Rosser on 10/04/2014.
//
//

#ifndef __ofxParallax__
#define __ofxParallax__

#include "ofMain.h"
#include "ofxParallaxShader.h"


struct ofxParallaxLayer {
    ofxParallaxLayer()
        : image(NULL), pos(0,0) { }
    ~ofxParallaxLayer() {
        if(image != NULL) {
            delete image;
            image = NULL;
        }
    }
    
    void setup(string path, ofPoint position) {
        if(image != NULL) {
            delete image;
            image = NULL;
        }
        image = new ofImage();
        bool completed = image->loadImage(path);
        if(!completed) {
            ofLog(OF_LOG_WARNING, "ofxParallaxLayer did not load image: " + path);
        }
        
        pos = position;
    }
    
    void draw(ofPoint offset) {
        image->draw(pos.x+offset.x, pos.y+offset.y);
    }
    
    // Variables
    ofImage * image;
    ofPoint pos;
};

struct ofxParallaxLayers {

    ofxParallaxLayers() : shader(NULL), fboLayer(NULL), offset(0,0), isBlurred(false), hasRenderedTexture(false), blurAmount(1), lastFrameCollision(false), collision(0,0) {
        
    }
    ~ofxParallaxLayers() {
        if(layer.size() != 0) {
            int size = layer.size()-1;
            for(int i=size; i>=0; i--){
                ofxParallaxLayer* toRemove = layer[i];
                delete toRemove;
                toRemove = NULL;
                layer.pop_back();
            }
        }
        layer.clear();
    }
    
    void exit() {
        
    }
    
    void addToLayers(string path, ofPoint pos) {
        ofxParallaxLayer * pnew =  new ofxParallaxLayer();
        pnew->setup(path, pos);
        layer.push_back(pnew);
        
    }
    
    void setup() {
        
        
        
        fboLayer = new ofFbo();
        
        if(size.x < 1 || size.y < 1) {
            ofLog(OF_LOG_ERROR, "ofxParallax::Adding a layer. Size is not valid");
            size = ofVec2f(64,64);
        }
        fboLayer->allocate(size.x, size.y, GL_RGBA);
        
        if(ofIsGLProgrammableRenderer() == true) {
#ifdef TARGET_OPENGLES
            shader = new ofShader();
            shader->setupShaderFromSource(GL_VERTEX_SHADER, blurVertShaderES2);
            shader->setupShaderFromSource(GL_FRAGMENT_SHADER, blurFragShaderES2);
#else
            shader = new ofShader();
            shader->setupShaderFromSource(GL_VERTEX_SHADER, blurXvertShaderGL3);
            shader->setupShaderFromSource(GL_FRAGMENT_SHADER, blurXfragShaderGL3);
#endif
            initShader();
        }  else {
            ofLogError("Application must be using ofGLProgrammableRenderer");
        }
    }
    
    void update(float impulse) {
//        int number = layer.size()-1;
        if(layer.size() != 0) {
            lastFrameCollision = false;
            if(impulse != 0.0) {
                offset.x = offset.x - (impulse / (speed * ofGetLastFrameTime()));
            }
           
            if(offset.x <= collision.x) {
                offset.x = collision.x;
                lastFrameCollision = true;
            } else if(offset.x >= collision.y) {
                offset.x = collision.y;
                lastFrameCollision = true;
            }
            
//            for(int i = 0; i<=number;i++){
//                ofxParallaxLayer* theLayer = layer[i];
//                theLayer->update();
//            }
        }
    }
    
    void draw() {
        if(layer.size() != 0) {
            if(hasRenderedTexture == true) {
                fboLayer->draw(offset);
            } else {
            if(isBlurred && hasRenderedTexture == false) {
                beginShader();
            }
            int number = layer.size()-1;
            for(int i = 0; i<=number;i++) {
                ofxParallaxLayer* theLayer = layer[i];
                theLayer->draw(offset);
            }
            
            if(isBlurred && hasRenderedTexture == false) {
                endShader();
                if(hasRenderedTexture == true) {
                    fboLayer->draw(offset);
                }
            }
            }
        }
    }
    
    void beginShader() {
        
        fboLayer->begin();
        ofClear(255, 0);
        shader->begin();
        shader->setUniform1f("fade", 1.0);
        shader->setUniform1f("blurAmnt", blurAmount);
        shader->setUniform1f("textureWidth", ofGetWidth());
        shader->setUniform1f("bloom", isBlurred);
        ofPushMatrix();
        ofTranslate(offset.x * -1, 0);
    }
    
    void endShader() {
        ofPopMatrix();
        shader->end();
        fboLayer->end();
        hasRenderedTexture = true;
        
    }
    
    
    void initShader() {
        if(shader != NULL) {
            shader->bindDefaults();
            shader->linkProgram();
        }
    }
    
    bool hasCollided() {
        return lastFrameCollision;
    }
    
    void setBlur(bool isOn) {
        if(isOn) {
            hasRenderedTexture = false;
            isBlurred = true;
        } else {
            hasRenderedTexture = false;
            isBlurred = false;
        }
    }
    
    //-----------------
    vector<ofxParallaxLayer*> layer;
    ofFbo* fboLayer;
    ofShader * shader;
    ofVec2f offset;
    ofVec2f size;
    ofPoint collision;
    
    bool isBlurred;
    bool hasRenderedTexture;
    float speed;
    float blurAmount;
    bool lastFrameCollision;
};


class ofxParallax {
public:
    ofxParallax() : numberOflayers(0) {
        
    }
    ~ofxParallax() {
        if(layers.size() != 0) {
            int size = layers.size()-1;
            for(int i=size; i>=0; i--){
                ofxParallaxLayers* toRemove = layers[i];
                delete toRemove;
                toRemove = NULL;
                layers.pop_back();
            }
        }
        layers.clear();
    };
    
    void setup() {
        
    }
    
//    
//    void setupLayers(int number, ofVec2f origin, float speed) {
//        for(int i = 0; i<= number-1; i++) {
//            int hardcoded = 1024;
//            ofxParallaxLayers* theLayer = new ofxParallaxLayers();
//            ofPoint pos = ofPoint(-((hardcoded/2 + ofGetWidth()/2)/2), 0);
////            ofPoint collision = ofPoint(-(hardcoded - ofGetWidth()), 0);
//            ofPoint collision = ofPoint(-hardcoded, 0);
//            theLayer->collision = collision;
//            theLayer->offset = pos;
//            theLayer->speed = speed;
//            theLayer->setup();
//            
//            layers.push_back(theLayer);
//        }
//    }
    
    void addNewLayer(int layer, ofVec2f origin, ofVec2f size, float speed, ofVec2f collision = ofVec2f::zero()) {
        if(layers.size()-1 == layer) {
            ofLog(OF_LOG_WARNING, "ofxParallax::addNewLayer : Layer already created.");
        } else if (layers.size() != layer) {
            ofLog(OF_LOG_WARNING, "ofxParallax::addNewLayer : Layed one step to many");
        } else {

            ofxParallaxLayers* theLayer = new ofxParallaxLayers();
            //            ofPoint collision = ofPoint(-(hardcoded - ofGetWidth()), 0);
            if(collision == ofVec2f::zero()) {
                collision = ofPoint(-(origin.x - size.x/2), origin.x + size.x);
            }
            theLayer->collision = collision;
            theLayer->offset = origin;
            theLayer->size = size;
            theLayer->speed = speed;
            theLayer->setup();
            
            layers.push_back(theLayer);
        }
    }
    
    void addImageToLayer(int layer, string imagePath, ofPoint pos) {
        if(layer <= layers.size()-1) {
            // all good
            layers[layer]->addToLayers(imagePath, pos);
        }
    }
    
    void exit() {
        
    }
    
    void update(float impulse) {
        if(layers.size() != 0) {
            int number = layers.size()-1;
            bool aboveHasStopped = false;
            for(int i = number; i>=0;i--) {
                if(aboveHasStopped == false) {
                    ofxParallaxLayers* theLayer = layers[i];
                    theLayer->update(impulse);
                    aboveHasStopped = theLayer->hasCollided();
                }
            }
        }
    }
    void draw() {
        if(layers.size() != 0) {
            int number = layers.size()-1;
            for(int i = 0; i<=number;i++) {
                ofxParallaxLayers* theLayer = layers[i];
                theLayer->draw();
            }
        }
    }
    
    void unblurAll() {
        if(layers.size() != 0) {
            int number = layers.size()-1;
            for(int i = 0; i<=number;i++) {
                ofxParallaxLayers* theLayer = layers[i];
                layers[i]->setBlur(false);
            }
        }
    }
    
    void blurLayer(int layerID) {
        if(layers.size() != 0) {
            int number = layers.size()-1;
            if(layerID <= number){
                layers[layerID]->setBlur(true);
                
            }
        }
    }
    
    int getTotalLayersSize() {
        return layers.size();
    }
    
    
protected:
    int numberOflayers;
    vector<ofxParallaxLayers*> layers;
    
};

#endif /* defined(__ofxParallax__) */
