#pragma once

#include "strcture.h"


class Skybox
{
public:
	Skybox();
	~Skybox();
	/** 初始化 */
	bool init();

	/** 加载 */
	void LoadSkyBMP();

	/** 渲染 */
	void render();

private:

	unsigned int	m_texture[5];   // 天空盒纹理 */
	float			length;         // 长度 */
	float			width;          // 宽度 */
	float			height;         // 高度 */
	float			yRot;           // 绕Y轴旋转 */
	GLuint			m_iList;		// 天空盒显示列表号 */

};

