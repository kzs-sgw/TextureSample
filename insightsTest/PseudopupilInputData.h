/////////////////////////////////////////////////////////////////////////
// �Ή��f�[�^�͎O�p�|���S���̂�
// ���͒l::�|���S�����A���_���W�A�@���A�e�N�X�`�����W
// 
// ���ꂼ��̔z���GPU�ɂԂ�����ŉ������B
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
	float*		vc; // (vec3) 3������ԏ�̃e�N�X�`�����W���_
	float*		vu; // (vec3) 3������ԏ�̃e�N�X�`�����W��U�P�ʃx�N�g��
	float*		vv; // (vec3) 3������ԏ�̃e�N�X�`�����W��V�P�ʃx�N�g��

	void		makeData(int numPolygon, float vertex_[], float normal_[], float texcoord_[], float texResolution = 150.0 );
};

