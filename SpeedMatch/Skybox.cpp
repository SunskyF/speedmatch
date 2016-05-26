#include "Skybox.h"
#include "GLAUX.H"
#include <iostream>

using namespace std;

Skybox::Skybox() :length(10000.0f), width(10000.0f), height(5000.0f), yRot(360.0f)
{
}


Skybox::~Skybox()
{
	/** 删除纹理对象及其占用的内存 */
	for (int i = 0; i< 5; i++)
	{
		glDeleteTextures(1, &m_texture[i]);
	}
}

bool Skybox::init()
{
	char filename[128];                                         // 用来保存文件名 */
	char *bmpName[] = { "back", "front", "top", "left", "right" };
	for (int i = 0; i< 5; i++)
	{
		AUX_RGBImageRec *pBitmap = NULL;
		sprintf(filename, "sky/%s", bmpName[i]);
		strcat(filename, ".bmp");
		pBitmap = auxDIBImageLoad(filename);	// 装入位图，并保存数据

		glGenTextures(1, &m_texture[i]);                        // 生成一个纹理对象名称 */	 
		glBindTexture(GL_TEXTURE_2D, m_texture[i]);             // 创建纹理对象 */
		/** 控制滤波 */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		/** 创建纹理 */
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

		free(pBitmap->data);                  // 释放位图数据占据的内存资源
		free(pBitmap);
	}

	/** 加载天空盒的显示列表 */
	LoadSkyBMP();

	return true;
}


void Skybox::LoadSkyBMP()
{
	/* 创建天空盒的显示列表*/
	m_iList = glGenLists(1);

	glNewList(m_iList, GL_COMPILE);
	glPushAttrib(GL_LIGHTING);	 // 保存场景中光照状态 */

	glDisable(GL_LIGHTING);      // 关闭光照 */
	glEnable(GL_TEXTURE_2D);

	/** 绘制背面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);
	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, -length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, -length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-width, height, -length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-width, -height, -length);

	glEnd();



	/** 绘制前面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, -height, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, length);

	glEnd();



	/** 绘制顶面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);
	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, height, length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-width, height, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-width, height, -length);

	glEnd();



	/** 绘制左面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);
	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(1.0f, 1.0f);  glVertex3f(-width, height, -length);
	glTexCoord2f(0.0f, 1.0f);  glVertex3f(-width, height, length);
	glTexCoord2f(0.0f, 0.0f);  glVertex3f(-width, -height, length);
	glTexCoord2f(1.0f, 0.0f);  glVertex3f(-width, -height, -length);

	glEnd();

	/** 绘制右面 */
	glBindTexture(GL_TEXTURE_2D, m_texture[4]);
	glBegin(GL_QUADS);

	/** 指定纹理坐标和顶点坐标 */
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width, -height, -length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(width, -height, length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(width, height, length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width, height, -length);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glPopAttrib();		/** 恢复光照状态 */
	glEndList();
}


void Skybox::render()
{
	/** 开始绘制 */
	glPushMatrix();
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	glCallList(m_iList);
	glPopMatrix();				/** 绘制结束 */

	yRot -= 0.01f;
	if (yRot < 0.0f)
		yRot = 360.0f;
}
