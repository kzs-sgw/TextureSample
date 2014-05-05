#pragma once

//#include "scene.h"
#include "kzsGLSL.h"
#include "GLCamera.h"
#include <glm\glm.hpp>

using namespace kzsGL;

class SceneTextureTest //: public Scene
{
private:

public:
	SceneTextureTest(void);
	~SceneTextureTest(void);

	kzsGLSL			shader;
	GLCamera		camera;

	void initScene();
	void update( float t );
	void render();
	void resize ( int, int );
};

