#ifndef MATHHELP_H_
#define MATHHELP_H_

#include "../Helper/HelperStructures.h"
#include "../../../Particle/Particle.h"
#include "Particles.h"
#include <IL/il.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "assimp/Importer.hpp"  //OO version Header!
#include "assimp/postprocess.h"
#include "assimp/scene.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <string>
#include <sstream>
#include <exception>
#include <fstream>
#include <map>
#include <cmath>
#include <iostream>
#include <vector>
#include <cmath>

#define aisgl_min(x,y) (x<y?x:y)
#define aisgl_max(x,y) (y>x?y:x)

extern GLuint matricesUniBuffer;
#define MatricesUniBufferSize sizeof(float) * 16 * 3
#define ProjMatrixOffset 0
#define ViewMatrixOffset sizeof(float) * 16
#define ModelMatrixOffset sizeof(float) * 16 * 2
#define MatrixSize sizeof(float) * 16

extern GLuint matricesUniLoc;
extern GLuint materialUniLoc;


// Vertex Attribute Locations
extern GLuint vertexLoc;
extern GLuint normalLoc;
extern GLuint texCoordLoc;

// Program and Shader Identifiers
extern GLuint program;
extern GLuint program1;
extern GLuint vertexShader;
extern GLuint fragmentShader;
extern char *vertexfile1;
extern char *fragmentfile1;
extern GLuint texUnit;

// the global Assimp scene object
extern const aiScene* scene;

// scale factor for the model to fit in the window
extern float scaleFactor;

// Shader Names
extern char *vertexfile;
extern char *fragmentfile;

extern std::map<std::string, GLuint> textureIdMap;

extern std::vector<struct Helper::MyMesh> myMeshes;

// For push and pop matrix
extern std::vector<float *> matrixStack;

// Camera Position
extern float camera[3];

// mesh tranlation
extern float translation[3];

// Model Matrix (part of the OpenGL Model View Matrix)
extern float modelMatrix[16];

// Frame counting and FPS computation
extern long time1;
extern long timebase;
extern long frame;

extern char s[32];

extern std::vector<std::vector<float> > perspectiveList;
extern int currentPerspective;
extern int perspectiveCount;
extern bool snapshot;
extern int imageNum;
extern float theta;
extern std::string generatedImagesDirectory;
extern std::string generatedInputDirectory;
extern std::string fileExt;
extern std::string modelname;
extern std::string modelDirectoryName;
extern std::string pathToModelDataDirectory;

extern std::vector<MCL::Particle> pList;
extern std::vector<MyParticle> particles;
 

extern int main_window;


extern int width;
extern int height;


namespace MathHelp
{

    float  DegToRad(float degrees);

    void color4_to_float4(const aiColor4D *c, float f[4]);

    void set_float4(float f[4], float a, float b, float c, float d);

    // res = a cross b;
    void crossProduct( float *a, float *b, float *res);


    // Normalize a vec3
    void normalize(float *a);

    // sets the square matrix mat to the identity matrix,
    // size refers to the number of rows (or columns)
    void setIdentityMatrix( float *mat, int size);

    // a = a * b;
    void multMatrix(float *a, float *b);


    // Defines a transformation matrix mat with a translation
    void setTranslationMatrix(float *mat, float x, float y, float z);

    // Defines a transformation matrix mat with a scale
    void setScaleMatrix(float *mat, float sx, float sy, float sz);

    // Defines a transformation matrix mat with a rotation 
    // angle alpha and a rotation axis (x,y,z)
    void setRotationMatrix(float *mat, float angle, float x, float y, float z);

} // end namespace


#endif