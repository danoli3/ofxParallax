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
        : texture(NULL), pos(0,0) { }
    ~ofxParallaxLayer() {
        if(texture != NULL) {
            delete texture;
            texture = NULL;
        }
    }
    
    void setup(string path, ofPoint position) {
        if(texture != NULL) {
            delete texture;
            texture = NULL;
        }
        ofImage * imageLoader = new ofImage();
        bool completed = imageLoader->loadImage(path);
        if(!completed) {
            ofLog(OF_LOG_WARNING, "ofxParallaxLayer did not load image: " + path);
        } else {
            texture = new ofTexture();
            texture->allocate(imageLoader->getPixelsRef());
            texture->loadData(imageLoader->getPixelsRef());
        }
        
        pos = position;
    }
    
    void setup(ofTexture * tex, ofPoint position) {
        if(texture != NULL) {
            delete texture;
            texture = NULL;
        }
        texture = tex;
        pos = position;
    }
    
    void draw(ofPoint offset) {
        transformation.makeIdentityMatrix();
        ofPushMatrix();
        transformation.preMultTranslate(pos);
        texture->draw(0, 0);
        ofPopMatrix();
    }
    
    // Variables
    ofTexture * texture;
    ofPoint pos;
    ofMatrix4x4 transformation;

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
    
    void setupLayersWithTexture(ofTexture * tex, ofPoint pos) {
        ofxParallaxLayer * pnew =  new ofxParallaxLayer();
        pnew->setup(tex, pos);
        layer.push_back(pnew);
    }
    
    void addToLayers(string path, ofPoint pos) {
        ofxParallaxLayer * pnew =  new ofxParallaxLayer();
        pnew->setup(path, pos);
        layer.push_back(pnew);
    }
    
    void setup() {
        
        scale = ofVec3f(0.5f,0.5f,0.5f);
        dynamicScale = ofVec3f(1.0f,1.0f,1.0f);
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
        
        transformation.makeIdentityMatrix();
        transformation.preMultTranslate(offset);
        if(zoomTopLeft != ofVec2f::zero()) {
            transformation.preMultTranslate(zoomTopLeft);
        }
        transformation.preMultScale(scale * dynamicScale);
        
    }
    
    void draw() {
        ofPushMatrix();
        ofMultMatrix(transformation);
        if(layer.size() != 0) {
            if(hasRenderedTexture == true) {
                fboLayer->draw(ofVec2f(0,0));
            } else {
            if(isBlurred && hasRenderedTexture == false) {
                beginShader();
            }
            int number = layer.size()-1;
            for(int i = 0; i<=number;i++) {
                ofxParallaxLayer* theLayer = layer[i];
                theLayer->draw(ofVec2f(0,0));
            }
            
            if(isBlurred && hasRenderedTexture == false) {
                endShader();
                if(hasRenderedTexture == true) {
                    fboLayer->draw(ofVec2f(0,0));
                }
            }
            }
        }
        ofPopMatrix();
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
    
    void setZoom(ofVec3f newScale, ofVec2f zoomOrigin) {
        dynamicScale = newScale;
        zoomTopLeft = zoomOrigin;
    }
    
    //-----------------
    vector<ofxParallaxLayer*> layer;
    ofFbo* fboLayer;
    ofShader * shader;
    ofVec2f offset;
    ofVec2f size;
    ofPoint collision;
    ofMatrix4x4 transformation;
    ofVec3f scale;
    ofVec3f dynamicScale;
    ofVec2f zoomTopLeft;
    
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
        removeLayers();
    };
    
    void setup() {
        
    }
    
    void removeLayers() {
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
    }
    
    bool loadDirectory(string directory) {
        
        vector<string> imageSequencePaths;
        ofDirectory dir(directory);
        int numOfFiles = dir.listDir();
        for(int i=0; i<numOfFiles; i++) {
            ofLog(OF_LOG_NOTICE, "loading... " + dir.getPath(i));
            imageSequencePaths.push_back(dir.getPath(i));
        }
        
        bool bLoaded = imageSequencePaths.size() > 0;
        if(bLoaded == false) {
            return false;
        }
        
        if(layers.size() != 0) {
            removeLayers();
        }
        
        for(int i=0; i<imageSequencePaths.size(); i++) {
            
            ofImage * imageLoader = new ofImage();
            ofTexture * texture = NULL;
            bool completed = imageLoader->loadImage(imageSequencePaths[i]);
            if(!completed) {
                ofLog(OF_LOG_WARNING, "ofxParallaxLayer did not load image: " + imageSequencePaths[i]);
            } else {
                texture = new ofTexture();
                texture->allocate(imageLoader->getPixelsRef());
                texture->loadData(imageLoader->getPixelsRef());
                
                if(!texture->isAllocated()) {
                    completed = false;
                }
                
                imageLoader->clear();
                delete imageLoader;
                imageLoader = NULL;
            }
            
            if(completed) {
                addNewLayer(i,                              // layer index
                            ofVec2f(0, 0),                  // origin point (topleft)
                            ofVec2f(texture->getWidth(), texture->getHeight()),         // size
                            getLayerSpeed(i),                         // index speed;
                            ofVec2f(-(texture->getWidth()/2), +(texture->getWidth()/2))   // collision left and collision right
                            );
                layers[i]->setupLayersWithTexture(texture, ofPoint(0,0));
            }
        }
        return bLoaded;
    }
    
    void addNewLayer(int layer, ofVec2f origin, ofVec2f size, float speed, ofVec2f collision = ofVec2f::zero()) {
        if(layers.size()-1 == layer) {
            ofLog(OF_LOG_WARNING, "ofxParallax::addNewLayer : Layer already created.");
        } else if (layers.size() != layer) {
            ofLog(OF_LOG_WARNING, "ofxParallax::addNewLayer : Layed one step to many");
        } else {

            ofxParallaxLayers* theLayer = new ofxParallaxLayers();
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
                layers[i]->setBlur(false);
            }
        }
    }
    
    void blurAll() {
        if(layers.size() != 0) {
            int number = layers.size()-1;
            for(int i = 0; i<=number;i++) {
                layers[i]->setBlur(true);
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
    
    void blurAllExceptLayer(int layerID) {
        if(layers.size() != 0) {
            blurAll();
            int number = layers.size()-1;
            if(layerID <= number){
                layers[layerID]->setBlur(false);
                
            }
        }
    }
    
    int getTotalLayersSize() {
        return layers.size();
    }
    
    void zoomAll(ofVec3f zoom, ofVec2f zoomOrigin) {
        if(layers.size() != 0) {
            int number = layers.size()-1;
            for(int i = 0; i<=number;i++) {
                layers[i]->setZoom(zoom, zoomOrigin);
            }
        }
    }
    
    virtual const float getLayerSpeed(int layerIndex) {
        // with 6 layers.
        float layerSpeed = 0.0f;
        switch (layerIndex) {
            case 0:
                layerSpeed = 5 * 360;
                break;
            case 1:
                layerSpeed = 5 * 330;
                break;
            case 2:
                layerSpeed = 5 * 300;
                break;
            case 3:
                layerSpeed = 5 * 130;
                break;
            case 4:
                layerSpeed = 5 * 90;
                break;
            case 5:
                layerSpeed = 5 * 80;
                break;
            case 6:
                layerSpeed = 5 * 50;
                break;
            default:
                layerSpeed = 0.0f;
                ofLog(OF_LOG_WARNING, "ofxParallax:: getLayerSpeed: Outside of default set speeds. Please implement your own.");
                break;
        }
        return layerSpeed;
    }

    
    
protected:
    int numberOflayers;
    vector<ofxParallaxLayers*> layers;
    
};

#endif /* defined(__ofxParallax__) */
