#pragma once
#include "PxUserApp.h"
#include "ObjParser.h"
#include "TextureManager.h"
#include "Timer.h"
#include <string>
#include <iostream>
#include <fstream>
#pragma comment(lib, "FreeImage.lib")

using glm::normalize;

class TextureAppTest : public PxUserApp
{
private:
	
	ofstream	ofs;
	ObjParser	mesh;
	kzsGLSL		shader;
	kzsGLSL		simple;
	GLuint		vao[3];
	float		t;
	float		meshRadius;
	int			num_screenshot;
	bool		canWriteText;

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
	void keyboard( unsigned char key, int x, int y );
	// for debug
	void initAxes();
	void drawAxes();
	void testLinePlaneCrossPoint(vec3 n, vec3 p, vec3 x0, vec3 v );
	void testTexCoordCalc();
	void testAffine();

	// rurikakesu(bird)
	void initBird();
	void drawBird();

	void CaptureScreen( const char* save_filename)
	{
		int width = glutGet(GLUT_WINDOW_WIDTH);
		int height = glutGet(GLUT_WINDOW_HEIGHT);

		// Make the BYTE array, factor of 3 because it's RBG.
		BYTE* pixels = new BYTE[ 3 * width * height];

		glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, pixels);

		// Convert to FreeImage format & save to file
		FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
		FreeImage_Save(FIF_BMP, image, save_filename, 0);

		// Free resources
		FreeImage_Unload(image);
		delete [] pixels;
	}

};

