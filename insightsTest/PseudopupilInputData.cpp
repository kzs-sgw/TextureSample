#include "PseudopupilInputData.h"
#include <iostream>

PseudopupilInputData::PseudopupilInputData(void)
{
}


PseudopupilInputData::~PseudopupilInputData(void)
{
	delete points;
	delete normals;
	delete texCoords;
	delete vc;
	delete vu;
	delete vv;
}

void PseudopupilInputData::makeData(int numPolygon, float vertex_[], float normal_[], float texcoord_[], float texResolution )
{
	points		= new float [ 3*3*numPolygon ]; // vec3 * numVertex * numPolygon
	normals		= new float [ 3*3*numPolygon ]; // vec3 * numVertex * numPolygon
	texCoords	= new float [ 2*3*numPolygon ]; // vec2 * numVertex * numPolygon

	for (int i = 0; i < numPolygon; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			// push back
			points [ i*9+j ] = vertex_[ i*9+j ];
			normals[ i*9+j ] = normal_[ i*9+j ];
		}
	}
	
	for (int i = 0; i < numPolygon; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			// push back
			texCoords[ i*6+j ] = texcoord_[ i*6+j ] * texResolution;
		}
	}

	vc = new float [ 3*3*numPolygon ]; // vec3 * numVertex * numPolygon
	vu = new float [ 3*3*numPolygon ]; // vec3 * numVertex * numPolygon
	vv = new float [ 3*3*numPolygon ]; // vec3 * numVertex * numPolygon

	for (int i = 0; i < numPolygon; i++)
	{
		float u0[2], u10[2], u20[2];
		u0 [0] = texcoord_[ i*6   ];
		u0 [1] = texcoord_[ i*6+1 ];
		u10[0] = texcoord_[ i*6+2 ] - u0[0];
		u10[1] = texcoord_[ i*6+3 ] - u0[1];
		u20[0] = texcoord_[ i*6+4 ] - u0[0];
		u20[1] = texcoord_[ i*6+5 ] - u0[1];

		//std::cout<<"CHECK u0\n";
		//std::cout<<u0[0]<<", "<<u0[1]<<std::endl;
		//std::cout<<u10[0]<<", "<<u10[1]<<std::endl;
		//std::cout<<u20[0]<<", "<<u20[1]<<std::endl;

		float p,q,r,s;
		float det_inv;
		det_inv = 1.0f/ (u10[0] * u20[1] - u20[0] * u10[1]);
		p =  u20[1] * det_inv;
		q = -u10[1] * det_inv;
		r = -u20[0] * det_inv;
		s =  u10[0] * det_inv;

		float v0[3], v10[3], v20[3];
		v0 [0] = points[ i*9   ];
		v0 [1] = points[ i*9+1 ];
		v0 [2] = points[ i*9+2 ];
		v10[0] = points[ i*9+3 ] - v0[0];
		v10[1] = points[ i*9+4 ] - v0[1];
		v10[2] = points[ i*9+5 ] - v0[2];
		v20[0] = points[ i*9+6 ] - v0[0];
		v20[1] = points[ i*9+7 ] - v0[1];
		v20[2] = points[ i*9+8 ] - v0[2];

		// push back
		// 3’¸“_‚Å“¯‚¶’l‚ð‘—‚é
		for (int j = 0; j < 3; j++)
		{
			//std::cout<<"vu,vv,vc\n";

			vu[i*9+j*3  ] = p*v10[0] + q*v20[0];
			vu[i*9+j*3+1] = p*v10[1] + q*v20[1];
			vu[i*9+j*3+2] = p*v10[2] + q*v20[2];
			
			//std::cout<<"vu: "<<vu[i*9+j*3]<<", "<<vu[i*9+j*3+1]<<", "<<vu[i*9+j*3+2]<<std::endl;

			vv[i*9+j*3  ] = r*v10[0] + s*v20[0];
			vv[i*9+j*3+1] = r*v10[1] + s*v20[1];
			vv[i*9+j*3+2] = r*v10[2] + s*v20[2];

			//std::cout<<"vv: "<<vv[i*9+j*3]<<", "<<vv[i*9+j*3+1]<<", "<<vv[i*9+j*3+2]<<std::endl;

			vc[i*9+j*3  ] = v0[0] - texCoords[i*6] * vu[i*9+j*3  ] - texCoords[i*6+1] * vv[i*9+j*3  ];
			vc[i*9+j*3+1] = v0[1] - texCoords[i*6] * vu[i*9+j*3+1] - texCoords[i*6+1] * vv[i*9+j*3+1];
			vc[i*9+j*3+2] = v0[2] - texCoords[i*6] * vu[i*9+j*3+2] - texCoords[i*6+1] * vv[i*9+j*3+2];
			
			//std::cout<<"vc: "<<vc[i*9+j*3]<<", "<<vc[i*9+j*3+1]<<", "<<vc[i*9+j*3+2]<<std::endl;
		}		
		
		
	}
}
