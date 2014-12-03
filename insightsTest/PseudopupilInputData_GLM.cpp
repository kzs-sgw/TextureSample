#include "PseudopupilInputData_GLM.h"


PseudopupilInputData_GLM::PseudopupilInputData_GLM(void)
{
}


PseudopupilInputData_GLM::~PseudopupilInputData_GLM(void)
{
	delete points;
	delete normals;
	delete texCoords;
	delete planeVec;
}


void PseudopupilInputData_GLM::makeData(int numPolygon, glm::vec3* vertex_, glm::vec3* normal_, glm::vec2* texcoord_, float texResolution )
{
	points		= new glm::vec3 [ 3*numPolygon ]; // numVertex * numPolygon
	normals		= new glm::vec3 [ 3*numPolygon ]; // numVertex * numPolygon
	texCoords	= new glm::vec2 [ 3*numPolygon ]; // numVertex * numPolygon

	for (int i = 0; i < numPolygon; i++)
	{
		points[i*3]   = vertex_[i*3];
		points[i*3+1] = vertex_[i*3+1];
		points[i*3+2] = vertex_[i*3+2];
	}
	for (int i = 0; i < numPolygon; i++)
	{
		normals[i*3]   = normal_[i*3];
		normals[i*3+1] = normal_[i*3+1];
		normals[i*3+2] = normal_[i*3+2];
	}
	for (int i = 0; i < numPolygon; i++)
	{
		texCoords[i*3]   = texcoord_[i*3]   * texResolution;
		texCoords[i*3+1] = texcoord_[i*3+1] * texResolution;
		texCoords[i*3+2] = texcoord_[i*3+2] * texResolution;
	}

	planeVec = new glm::vec3 [ 9*numPolygon ];

	for (int i = 0; i < numPolygon;/*polygons*/ i++)
	{
		glm::vec2 u0  = texCoords[i*3];
		glm::vec2 u10 = texCoords[i*3+1] - u0;
		glm::vec2 u20 = texCoords[i*3+2] - u0;

		float p,q,r,s;
		float det_inv;
		det_inv = 1.0 / (u10.x * u20.y - u20.x * u10.y);
		p =  u20.y * det_inv;
		q = -u10.y * det_inv;
		r = -u20.x * det_inv;
		s =  u10.x * det_inv;
		/*cout<<"p:"<<p<<endl;
		cout<<"q:"<<q<<endl;
		cout<<"r:"<<r<<endl;
		cout<<"s:"<<s<<endl;*/

		//vec2 check = p*u10 + q*u20;
		//cout<<"check:"<<check.x<<" "<<check.y<<endl;


		glm::vec3 v0  = points[i*3];
		glm::vec3 v10 = points[i*3+1] - v0;
		glm::vec3 v20 = points[i*3+2] - v0;

		glm::vec3 vu = p*v10 + q*v20;
		glm::vec3 vv = r*v10 + s*v20;
		glm::vec3 vc = v0 - texCoords[i*3].x * vu - texCoords[i*3].y * vv;

		//cout<<"vu: "<< vu.x <<" "<< vu.y <<" "<< vu.z <<endl;
		//cout<<"vv: "<< vv.x <<" "<< vv.y <<" "<< vv.z <<endl;

		// 3’¸“_‚É‘Î‚µ‚Ä
		for (int j = 0; j < 3; j++)
		{
			planeVec[i*9 + j*3]     = vc;
			planeVec[i*9 + j*3 + 1] = vu;
			planeVec[i*9 + j*3 + 2] = vv;
		}
	}
}