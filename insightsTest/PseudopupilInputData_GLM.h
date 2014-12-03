#pragma once
#include <glm/glm.hpp>

class PseudopupilInputData_GLM
{
public:
	PseudopupilInputData_GLM(void);
	~PseudopupilInputData_GLM(void);

	glm::vec3* points;
	glm::vec3* normals;
	glm::vec2* texCoords;
	glm::vec3* planeVec;

	void makeData(int numPolygon, glm::vec3* vertex_, glm::vec3* normal_, glm::vec2* texcoord_, float texResolution = 150.0 );

};

