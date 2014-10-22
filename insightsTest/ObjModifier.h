////////////////////////////////////////////////////////////////////////////
// 
// 独自形式[.objx]フィアルを作成する。
// 扱えるポリゴンの種類は三角ポリゴンのみ（2014/10/21現在）
// 
// 
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ObjParser.h"
#include <map>

using std::map;


struct VertexInfo // 頂点付加情報
{
	//vec3 position; // 不要。付加情報のみを扱う
	//int index_of_vertex; // mapのキーで代用する
	//int element;
	vector<int> neighbor_vertex_id;
	vector<int> neighbor_face_id;
};


class ObjModifier : public ObjParser
{
private:
	map<int, VertexInfo> raw_vertexinfo;

	bool ObjModifier::ObjLoadRawData( const char* filename );

public:
	ObjModifier(void);
	~ObjModifier(void);

	map<int/* vertex ID */, VertexInfo> vertex_info;

	bool ObjModifier::ObjLoadAndWrite( const char* filename );
	bool ObjModifier::ObjLoadAndMake( const char* filename );
	bool ObjModifier::ObjLoadModifiedData( const char* filename );

	int  ObjModifier::getStatus();
};

