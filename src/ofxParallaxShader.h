//
//  ofxParallaxShader.h
//  emptyExample
//
//  Created by Daniel Rosser on 10/04/2014.
//
//

#ifndef __ofxParallaxShader_h
#define __ofxParallaxShader_h

#define STRINGIFY(x) #x

static string blurVertShaderES2 = STRINGIFY
(
 uniform mat4 modelViewProjectionMatrix;
 
 attribute vec4 position;
 attribute vec2 texcoord;
 
 varying vec2 texCoordVarying;
 
 void main()
{
    texCoordVarying = texcoord;
	gl_Position = modelViewProjectionMatrix * position;
}
 );

static string blurFragShaderES2 = STRINGIFY
(
     precision mediump float;
     uniform sampler2D tex0;
     uniform float fade;
     uniform float textureWidth;
    //uniform float blurSize;
     varying vec2 texCoordVarying;
 
 
 
 // gray
// void main(void){
//     vec4 textureColor = texture2D(tex0, texCoordVarying);
//     float luminance = dot(vec3 (0.2125, 0.7154, 0.0721), vec3(textureColor));
//     
//     gl_FragColor = vec4(luminance, luminance, luminance, textureColor.a);
// 
//     
 
     //}
 
//     float kernel[9];
//     vec2 offset[9];
//
    void main()
    {

 
//            vec2  st = texCoordVarying;
//            vec4 sum = vec4(0.0);
//
//            offset[0] = vec2(-1.0, -1.0);
//            offset[1] = vec2(0.0, -1.0);
//            offset[2] = vec2(1.0, -1.0);
//            
//            offset[3] = vec2(-1.0, 0.0);
//            offset[4] = vec2(0.0, 0.0);
//            offset[5] = vec2(1.0, 0.0);
//            
//            offset[6] = vec2(-1.0, 1.0);
//            offset[7] = vec2(0.0, 1.0);
//            offset[8] = vec2(1.0, 1.0);
//            
//            kernel[0] = 1.0/16.0;   kernel[1] = 2.0/16.0;   kernel[2] = 1.0/16.0;
//            kernel[3] = 2.0/16.0;   kernel[4] = 4.0/16.0;   kernel[5] = 2.0/16.0;
//            kernel[6] = 1.0/16.0;   kernel[7] = 2.0/16.0;   kernel[8] = 1.0/16.0;
//            
//            int i = 0;
//            for (i = 0; i < 9; i++){
//                vec4 tmp = texture2D(tex0, st + offset[i]);
//                sum += tmp * kernel[i];
//            }
//            
//            vec4 previousValue = texture2D(tex0, st);
//            gl_FragColor = previousValue +  vec4(sum.rgb, previousValue.a);
        
//        gl_FragColor = texture2D(tex0, texCoordVarying);
        
        // bloom
//         vec4 blurSample = vec4(0.0);
//         vec4 tmpPix;
//         vec4 offPix;
//         vec2 uv = texCoordVarying;
//         for (float i = -4.0; i < 5.0; i++)
//         {
//         tmpPix = texture2D(tex0, uv + vec2(i*0.003, 0));
//         offPix = -0.3+tmpPix;
//         offPix = offPix * 5.0;
//         if ( (offPix.r + offPix.g + offPix.b) > 0.0 )
//         {
//         blurSample = blurSample + offPix;
//         }
//         }
//         
//         for (float i = -4.0; i < 5.0; i++)
//         {
//         tmpPix = texture2D(tex0, uv + vec2(0, i*0.003));
//         offPix = -0.3+tmpPix;
//         offPix = offPix * 5.0;
//         if ((offPix.r+offPix.g+offPix.b) > 0.0)
//         {
//         blurSample += offPix;
//         }
//         
//         }
//         
//         blurSample = blurSample / 64.0;
//         gl_FragColor = blurSample*1.2;
        
        
        
//        // Blur
//        vec4 sum = vec4(0.0);
//        float blurSize = 1.0/textureWidth;
//        sum += texture2D(tex0, vec2(texCoordVarying.x - 4.0 * blurSize, texCoordVarying.y)) * 0.05;
//        sum += texture2D(tex0, vec2(texCoordVarying.x - 3.0 * blurSize, texCoordVarying.y)) * 0.09;
//        sum += texture2D(tex0, vec2(texCoordVarying.x - 2.0 * blurSize, texCoordVarying.y)) * 0.12;
//        sum += texture2D(tex0, vec2(texCoordVarying.x - 1.0 * blurSize, texCoordVarying.y)) * 0.15;
//        sum += texture2D(tex0, vec2(texCoordVarying.x                 , texCoordVarying.y)) * 0.16;
//        sum += texture2D(tex0, vec2(texCoordVarying.x + 1.0 * blurSize, texCoordVarying.y)) * 0.15;
//        sum += texture2D(tex0, vec2(texCoordVarying.x + 2.0 * blurSize, texCoordVarying.y)) * 0.12;
//        sum += texture2D(tex0, vec2(texCoordVarying.x + 3.0 * blurSize, texCoordVarying.y)) * 0.09;
//        sum += texture2D(tex0, vec2(texCoordVarying.x + 4.0 * blurSize, texCoordVarying.y)) * 0.05;
//
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y - 4.0 * blurSize)) * 0.05;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y - 3.0 * blurSize)) * 0.09;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y - 2.0 * blurSize)) * 0.12;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y - 1.0 * blurSize)) * 0.15;
//        sum += texture2D(tex0, vec2(texCoordVarying.x                 , texCoordVarying.y)) * 0.16;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y + 1.0 * blurSize)) * 0.15;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y + 2.0 * blurSize)) * 0.12;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y + 3.0 * blurSize)) * 0.09;
//        sum += texture2D(tex0, vec2(texCoordVarying.x, texCoordVarying.y + 4.0 * blurSize)) * 0.05;
//
//        gl_FragColor = sum/2.0;
        
        // Blur #2
        vec4 sum = vec4(0.0);
         float blurSize = 1.0/textureWidth;
         float i;
        float j;
         for (i = -2.0; i <= 2.0; i++)
         {
            for (j = -2.0; j <= 2.0; j++)
            {
             sum += texture2D(tex0, vec2(texCoordVarying.x + i*blurSize, texCoordVarying.y + j*blurSize));
            }
         }
         gl_FragColor = sum/25.0;
        
        
        

   }
);



#endif
