#pragma warning(disable: 4996)
#pragma once

#include "strcture.h"
#include <vector>

using namespace std;

class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();
	GLboolean ReadOBJ(char* filename);

	virtual void draw();
protected:

	GLvoid firstparse(FILE* file);
	GLvoid secondparse(FILE* file);
	GLMgroup* AddGroup(char* name);
	GLMgroup* FindGroup(char* name);
	GLuint FindMaterial(char* name);

	GLvoid ReadMTL(char* name);

	//Static 
	char*    pathname;			/* path to this model */
	char*    mtllibname;			/* name of the material library */

	GLuint   numvertices;			/* number of vertices in model */
	GLfloat* vertices;			/* array of vertices  */

	GLuint   numnormals;			/* number of normals in model */
	GLfloat* normals;			/* array of normals */

	GLuint   numtexcoords;		/* number of texcoords in model */
	GLfloat* texcoords;			/* array of texture coordinates */

	GLuint   numfacetnorms;		/* number of facetnorms in model */
	GLfloat* facetnorms;			/* array of facetnorms */

	GLuint       numtriangles;		/* number of triangles in model */
	GLMtriangle* triangles;		/* array of triangles */

	GLuint       nummaterials;		/* number of materials in model */
	vector<GLMmaterial> materials;

	GLuint       numgroups;		/* number of groups in model */
	GLMgroup*    groups;			/* linked list of groups */

	GLfloat position[3];			/* position of the model */
};
