#include "Skybox.h"
#include "GLAUX.H"
#include <iostream>

using namespace std;

Skybox::Skybox() :length(10000.0f), width(10000.0f), height(5000.0f), yRot(360.0f)
{
}


Skybox::~Skybox()
{
	/** ɾ�����������ռ�õ��ڴ� */
	for (int i = 0; i< 5; i++)
	{
		glDeleteTextures(1, &m_texture[i]);
	}
}

bool Skybox::init()
{
	char filename[128];                                         // ���������ļ��� */
	char *bmpName[] = { "back", "front", "top", "left", "right" };
	for (int i = 0; i< 5; i++)
	{
		AUX_RGBImageRec *pBitmap = NULL;
		sprintf(filename, "sky/%s", bmpName[i]);
		strcat(filename, ".bmp");
		pBitmap = auxDIBImageLoad(filename);	// װ��λͼ������������

		glGenTextures(1, &m_texture[i]);                        // ����һ������������� */	 
		glBindTexture(GL_TEXTURE_2D, m_texture[i]);             // ����������� */
		/** �����˲� */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/** �������� */
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

		free(pBitmap->data);                  // �ͷ�λͼ����ռ�ݵ��ڴ���Դ
		free(pBitmap);
	}

	/** ������պе���ʾ�б� */
	LoadSkyBMP();

	return true;
}


void Skybox::LoadSkyBMP()
{
	/* ������պе���ʾ�б�*/
	m_iList = glGenLists(1);

	glNewList(m_iList, GL_COMPILE);
	glPushAttrib(GL_LIGHTING);	 // ���泡���й���״̬ */

	glDisable(GL_LIGHTING);      // �رչ��� */
	glEnable(GL_TEXTURE_2D);

	/** ���Ʊ��� */
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	glBegin(GL_QUADS);

	/** ָ����������Ͷ������� */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -length);

	glEnd();



	/** ����ǰ�� */
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	glBegin(GL_QUADS);

	/** ָ����������Ͷ������� */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, length);

	glEnd();



	/** ���ƶ��� */
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);
	glBegin(GL_QUADS);

	/** ָ����������Ͷ������� */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);

	glEnd();



	/** �������� */
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);
	glBegin(GL_QUADS);

	/** ָ����������Ͷ������� */
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height, length);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height, -length);

	glEnd();

	/** �������� */
	glBindTexture(GL_TEXTURE_2D, m_texture[4]);
	glBegin(GL_QUADS);

	/** ָ����������Ͷ������� */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, -length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopAttrib();		/** �ָ�����״̬ */
	glEndList();
}


void Skybox::render()
{
	/** ��ʼ���� */
	glPushMatrix();
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glCallList(m_iList);
	glPopMatrix();				/** ���ƽ��� */

	yRot -= 0.01f;
	if (yRot < 0.0f)
		yRot = 360.0f;
}
