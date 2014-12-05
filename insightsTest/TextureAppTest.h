#pragma once
#include "PxUserApp.h"
#include "ObjParser.h"
#include "TextureManager.h"
#pragma comment(lib, "FreeImage.lib")

using glm::normalize;

class TextureAppTest : public PxUserApp
{
private:

	ObjParser	mesh;
	kzsGLSL		shader;
	kzsGLSL		simple;
	GLuint		vao[3];
	float		t;
	float		meshRadius;

	//TextureManager_beta texManager;

	// rurikakesu(bird)
	ObjParser	bird;
	kzsGLSL		simple4bird;
	//GLuint		vaoBird;

public:
	TextureAppTest(void);
	~TextureAppTest(void);

	void initGL();
	void display();
	void idle();

	// for debug
	void initAxes();
	void drawAxes();
	void testLinePlaneCrossPoint(vec3 n, vec3 p, vec3 x0, vec3 v );
	void testTexCoordCalc();
	void testAffine();

	// rurikakesu(bird)
	void initBird();
	void drawBird();



};

