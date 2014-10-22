////////////////////////////////////////////////////////////////////////////
// 
// �Ǝ��`��[.objx]�t�B�A�����쐬����B
// ������|���S���̎�ނ͎O�p�|���S���̂݁i2014/10/21���݁j
// 
// 
////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ObjParser.h"
#include <map>

using std::map;


struct VertexInfo // ���_�t�����
{
	//vec3 position; // �s�v�B�t�����݂̂�����
	//int index_of_vertex; // map�̃L�[�ő�p����
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

