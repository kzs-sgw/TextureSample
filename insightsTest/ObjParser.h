#pragma once

#include <glm\glm.hpp>
#include <vector>
#include <iostream>
#include <fstream>


using glm::vec2;
using glm::vec3;
using std::vector;

const int OBJ_BUFFER_LENGTH = 1024;
const int OBJ_NAME_LENGTH   = 256;


struct ObjFace
{
	bool			use_texture;
	bool			use_normal;
	int				element;
	vec3			f_normal;
	int*			vertex_id;
	int*			texcoord_id;
	int*			normal_id;
};



class ObjParser
{
private:
	char		objFileName[OBJ_NAME_LENGTH];
	bool		use_faceNorm;
	// input vertex data
	vector<vec3> raw_vertex;
	vector<vec2> raw_texcoord;
	vector<vec3> raw_normal;

	vector<ObjFace> face;
	
	
	
public:
	// array data ---to GPU---
	vector<vec3> vertex;
	vector<vec2> texcoord;
	vector<vec3> normal;

	ObjParser( bool _use_face_norm = false );
	~ObjParser(void);

	bool		ObjLoad( const char* filename );
	int			getNumPolygon();



};

