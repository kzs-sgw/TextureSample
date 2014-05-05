#pragma once
#include "PxUserApp.h"

class TextureAppTest : public PxUserApp
{
private:
	kzsGLSL	shader;
	kzsGLSL simple;
	GLuint	vao[2];
	float	t;

public:
	TextureAppTest(void);
	~TextureAppTest(void);

	void initGL();
	void display();

	// for debug
	void initAxes();
	void drawAxes();
	void testLinePlaneCrossPoint(vec3 n, vec3 p, vec3 x0, vec3 v );
	void testTexCoordCalc();
	void testAffine();
};

