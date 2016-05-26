#pragma once
#define FREEGLUT_STATIC
#include <GL\freeglut.h>
#pragma comment(lib, "freeglut_staticd.lib")

#ifndef M_PI
#define M_PI 3.14159265
#define GL_CLAMP_TO_EDGE	0x812F
#endif

/* defines */
#define GLM_NONE     (0)		/* render with only vertices */
#define GLM_FLAT     (1 << 0)		/* render with facet normals */
#define GLM_SMOOTH   (1 << 1)		/* render with vertex normals */
#define GLM_TEXTURE  (1 << 2)		/* render with texture coords */
#define GLM_COLOR    (1 << 3)		/* render with colors */
#define GLM_MATERIAL (1 << 4)		/* render with materials */

typedef struct _GLMmaterial
{
	char* name;				/* name of material */
	GLfloat diffuse[4];			/* diffuse component */
	GLfloat ambient[4];			/* ambient component */
	GLfloat specular[4];			/* specular component */
	GLfloat emmissive[4];			/* emmissive component */
	GLfloat shininess;			/* specular exponent */
} GLMmaterial;

/* GLMtriangle: Structure that defines a triangle in a model.
*/
typedef struct {
	GLuint vindices[3];			/* array of triangle vertex indices */
	GLuint nindices[3];			/* array of triangle normal indices */
	GLuint tindices[3];			/* array of triangle texcoord indices*/
	GLuint findex;			/* index of triangle facet normal */
} GLMtriangle;

/* GLMgroup: Structure that defines a group in a model.
*/
typedef struct _GLMgroup {
	char*             name;		/* name of this group */
	GLuint            numtriangles;	/* number of triangles in this group */
	GLuint*           triangles;		/* array of triangle indices */
	GLuint            material;           /* index to material for group */
	struct _GLMgroup* next;		/* pointer to next group in model */
} GLMgroup;

