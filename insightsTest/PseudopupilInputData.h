/////////////////////////////////////////////////////////////////////////
// 対応データは三角ポリゴンのみ
// 入力値::ポリゴン数、頂点座標、法線、テクスチャ座標
// 
// それぞれの配列をGPUにぶち込んで下さい。
// 
// 
/////////////////////////////////////////////////////////////////////////


#pragma once

class PseudopupilInputData
{
private:

public:
	PseudopupilInputData(void);
	~PseudopupilInputData(void);

	float*		points;
	float*		normals;
	float*		texCoords;
	float*		vc; // (vec3) 3次元空間上のテクスチャ座標原点
	float*		vu; // (vec3) 3次元空間上のテクスチャ座標軸U単位ベクトル
	float*		vv; // (vec3) 3次元空間上のテクスチャ座標軸V単位ベクトル

	void		makeData(int numPolygon, float vertex_[], float normal_[], float texcoord_[], float texResolution = 150.0 );
};

