#pragma once

#include "strcture.h"


class Skybox
{
public:
	Skybox();
	~Skybox();
	/** ��ʼ�� */
	bool init();

	/** ���� */
	void LoadSkyBMP();

	/** ��Ⱦ */
	void render();

private:

	unsigned int	m_texture[5];   // ��պ����� */
	float			length;         // ���� */
	float			width;          // ��� */
	float			height;         // �߶� */
	float			yRot;           // ��Y����ת */
	GLuint			m_iList;		// ��պ���ʾ�б�� */

};

