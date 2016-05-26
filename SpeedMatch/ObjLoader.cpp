#include "ObjLoader.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T(x) triangles[(x)]

enum { X, Y, Z, W };

ObjLoader::ObjLoader()
{
	pathname = new char[50];
	mtllibname = new char[50];
	numvertices = 0;
	//vertices = NULL;
	numnormals = 0;
	//normals = NULL;
	numtexcoords = 0;
	//texcoords = NULL;
	numfacetnorms = 0;
	//facetnorms = NULL;
	numtriangles = 0;
	//triangles = NULL;
	nummaterials = 0;
//	materials = NULL;
	materials.clear();
	numgroups = 0;
	//groups = NULL;
	position[0] = 0.0;
	position[1] = 0.0;
	position[2] = 0.0;
}


ObjLoader::~ObjLoader()
{
	delete[] pathname;
	delete[] mtllibname;
}

GLMgroup* ObjLoader::FindGroup(char* name)
{
	GLMgroup* group0;

	group0 = groups;
	while (group0) {
		if (!strcmp(name, group0->name))
			break;
		group0 = group0->next;
	}

	return group0;
}

GLMgroup* ObjLoader::AddGroup(char* name)
{
	GLMgroup* group0;

	group0 = FindGroup(name);
	if (!group0) {
		group0 = (GLMgroup*)malloc(sizeof(GLMgroup));
		group0->name = strdup(name);
		group0->material = 0;
		group0->numtriangles = 0;
		group0->triangles = NULL;
		group0->next = groups;
		groups = group0;
		numgroups++;
	}

	return group0;
}

GLuint ObjLoader::FindMaterial(char* name)
{
	GLuint i;

	for (i = 0; i < nummaterials; i++) {
		if (!strcmp(materials[i].name, name))
			goto found;
	}

	/* didn't find the name, so set it as the default material */

	printf("_glmFindMaterial():  can't find material \"%s\".\n", name);
	i = 0;

found:
	return i;
}

GLvoid ObjLoader::firstparse(FILE* file){
	GLuint    numvertices0;		/* number of vertices in model */
	GLuint    numnormals0;			/* number of normals in model */
	GLuint    numtexcoords0;		/* number of texcoords in model */
	GLuint    numtriangles0;		/* number of triangles in model */
	GLMgroup* group;			/* current group */
	unsigned  v, n, t;
	char      buf[128];

	/* make a default group */
	group = AddGroup("default");

	numvertices0 = numnormals0 = numtexcoords0 = numtriangles0 = 0;
	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'v':				/* v, vn, vt */
			switch (buf[1]) {
			case '\0':			/* vertex */
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				numvertices0++;
				break;
			case 'n':				/* normal */
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				numnormals0++;
				break;
			case 't':				/* texcoord */
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				numtexcoords0++;
				break;
			default:
				printf("FirstPass(): Unknown token \"%s\".\n", buf);
				exit(1);
				break;
			}
			break;
		case 'm':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);
			mtllibname = strdup(buf);
			ReadMTL(buf);
			break;
		case 'u':
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'g':				/* group */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s", buf);
			group = AddGroup(buf);
			break;
		case 'f':				/* face */
			v = n = t = 0;
			fscanf(file, "%s", buf);
			/* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
			if (strstr(buf, "//")) {
				/* v//n */
				sscanf(buf, "%d//%d", &v, &n);
				fscanf(file, "%d//%d", &v, &n);
				fscanf(file, "%d//%d", &v, &n);
				numtriangles0++;
				group->numtriangles++;
				while (fscanf(file, "%d//%d", &v, &n) > 0) {
					numtriangles0++;
					group->numtriangles++;
				}
			}
			else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
				/* v/t/n */
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				numtriangles0++;
				group->numtriangles++;
				while (fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
					numtriangles0++;
					group->numtriangles++;
				}
			}
			else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
				/* v/t */
				fscanf(file, "%d/%d", &v, &t);
				fscanf(file, "%d/%d", &v, &t);
				numtriangles0++;
				group->numtriangles++;
				while (fscanf(file, "%d/%d", &v, &t) > 0) {
					numtriangles0++;
					group->numtriangles++;
				}
			}
			else {
				/* v */
				fscanf(file, "%d", &v);
				fscanf(file, "%d", &v);
				numtriangles0++;
				group->numtriangles++;
				while (fscanf(file, "%d", &v) > 0) {
					numtriangles0++;
					group->numtriangles++;
				}
			}
			break;

		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	/* set the stats in the model structure */
	numvertices = numvertices0;
	numnormals = numnormals0;
	numtexcoords = numtexcoords0;
	numtriangles = numtriangles0;

	/* allocate memory for the triangles in each group */
	group = groups;
	while (group) {
		group->triangles = (GLuint*)malloc(sizeof(GLuint)* group->numtriangles);
		group->numtriangles = 0;
		group = group->next;
	}
}
GLvoid ObjLoader::secondparse(FILE* file){
	GLuint    numvertices0;		/* number of vertices in model */
	GLuint    numnormals0;			/* number of normals in model */
	GLuint    numtexcoords0;		/* number of texcoords in model */
	GLuint    numtriangles0;		/* number of triangles in model */

	GLfloat*  vertice = vertices;			/* array of vertices  */
	GLfloat*  normal = normals;			/* array of normals */
	GLfloat*  texcoord = texcoords;			/* array of texture coordinates */
	GLMgroup* group = groups;			/* current group pointer */

	GLuint    material0;			/* current material */
	GLuint    v, n, t;
	char      buf[128];

	/* on the second pass through the file, read all the data into the
	allocated arrays */
	numvertices0 = numnormals0 = numtexcoords0 = 1;
	numtriangles0 = 0;
	material0 = 0;
	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'v':				/* v, vn, vt */
			switch (buf[1]) {
			case '\0':			/* vertex */
				fscanf(file, "%f %f %f",
					&vertice[3 * numvertices0 + X],
					&vertice[3 * numvertices0 + Y],
					&vertice[3 * numvertices0 + Z]);
				numvertices0++;
				break;
			case 'n':				/* normal */
				fscanf(file, "%f %f %f",
					&normal[3 * numnormals0 + X],
					&normal[3 * numnormals0 + Y],
					&normal[3 * numnormals0 + Z]);
				numnormals0++;
				break;
			case 't':				/* texcoord */
				fscanf(file, "%f %f",
					&texcoord[2 * numtexcoords0 + X],
					&texcoord[2 * numtexcoords0 + Y]);
				numtexcoords0++;
				break;
			}
			break;
		case 'u':
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);
			group->material = material0 = FindMaterial(buf);
			break;
		case 'g':				/* group */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s", buf);
			group = FindGroup(buf);
			group->material = material0;
			break;
		case 'f':				/* face */
			v = n = t = 0;
			fscanf(file, "%s", buf);
			/* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
			if (strstr(buf, "//")) {
				/* v//n */
				sscanf(buf, "%d//%d", &v, &n);
				T(numtriangles0).vindices[0] = v;
				T(numtriangles0).nindices[0] = n;
				fscanf(file, "%d//%d", &v, &n);
				T(numtriangles0).vindices[1] = v;
				T(numtriangles0).nindices[1] = n;
				fscanf(file, "%d//%d", &v, &n);
				T(numtriangles0).vindices[2] = v;
				T(numtriangles0).nindices[2] = n;
				group->triangles[group->numtriangles++] = numtriangles0;
				numtriangles0++;
				while (fscanf(file, "%d//%d", &v, &n) > 0) {
					T(numtriangles0).vindices[0] = T(numtriangles0 - 1).vindices[0];
					T(numtriangles0).nindices[0] = T(numtriangles0 - 1).nindices[0];
					T(numtriangles0).vindices[1] = T(numtriangles0 - 1).vindices[2];
					T(numtriangles0).nindices[1] = T(numtriangles0 - 1).nindices[2];
					T(numtriangles0).vindices[2] = v;
					T(numtriangles0).nindices[2] = n;
					group->triangles[group->numtriangles++] = numtriangles0;
					numtriangles0++;
				}
			}
			else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
				/* v/t/n */
				T(numtriangles0).vindices[0] = v;
				T(numtriangles0).tindices[0] = t;
				T(numtriangles0).nindices[0] = n;
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				T(numtriangles0).vindices[1] = v;
				T(numtriangles0).tindices[1] = t;
				T(numtriangles0).nindices[1] = n;
				fscanf(file, "%d/%d/%d", &v, &t, &n);
				T(numtriangles0).vindices[2] = v;
				T(numtriangles0).tindices[2] = t;
				T(numtriangles0).nindices[2] = n;
				group->triangles[group->numtriangles++] = numtriangles0;
				numtriangles0++;
				while (fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
					T(numtriangles0).vindices[0] = T(numtriangles0 - 1).vindices[0];
					T(numtriangles0).tindices[0] = T(numtriangles0 - 1).tindices[0];
					T(numtriangles0).nindices[0] = T(numtriangles0 - 1).nindices[0];
					T(numtriangles0).vindices[1] = T(numtriangles0 - 1).vindices[2];
					T(numtriangles0).tindices[1] = T(numtriangles0 - 1).tindices[2];
					T(numtriangles0).nindices[1] = T(numtriangles0 - 1).nindices[2];
					T(numtriangles0).vindices[2] = v;
					T(numtriangles0).tindices[2] = t;
					T(numtriangles0).nindices[2] = n;
					group->triangles[group->numtriangles++] = numtriangles0;
					numtriangles0++;
				}
			}
			else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
				/* v/t */
				T(numtriangles0).vindices[0] = v;
				T(numtriangles0).tindices[0] = t;
				fscanf(file, "%d/%d", &v, &t);
				T(numtriangles0).vindices[1] = v;
				T(numtriangles0).tindices[1] = t;
				fscanf(file, "%d/%d", &v, &t);
				T(numtriangles0).vindices[2] = v;
				T(numtriangles0).tindices[2] = t;
				group->triangles[group->numtriangles++] = numtriangles0;
				numtriangles0++;
				while (fscanf(file, "%d/%d", &v, &t) > 0) {
					T(numtriangles0).vindices[0] = T(numtriangles0 - 1).vindices[0];
					T(numtriangles0).tindices[0] = T(numtriangles0 - 1).tindices[0];
					T(numtriangles0).vindices[1] = T(numtriangles0 - 1).vindices[2];
					T(numtriangles0).tindices[1] = T(numtriangles0 - 1).tindices[2];
					T(numtriangles0).vindices[2] = v;
					T(numtriangles0).tindices[2] = t;
					group->triangles[group->numtriangles++] = numtriangles0;
					numtriangles0++;
				}
			}
			else {
				/* v */
				sscanf(buf, "%d", &v);
				T(numtriangles0).vindices[0] = v;
				fscanf(file, "%d", &v);
				T(numtriangles0).vindices[1] = v;
				fscanf(file, "%d", &v);
				T(numtriangles0).vindices[2] = v;
				group->triangles[group->numtriangles++] = numtriangles0;
				numtriangles0++;
				while (fscanf(file, "%d", &v) > 0) {
					T(numtriangles0).vindices[0] = T(numtriangles0 - 1).vindices[0];
					T(numtriangles0).vindices[1] = T(numtriangles0 - 1).vindices[2];
					T(numtriangles0).vindices[2] = v;
					group->triangles[group->numtriangles++] = numtriangles0;
					numtriangles0++;
				}
			}
			break;

		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

}


GLboolean ObjLoader::ReadOBJ(char* filename){
	pathname = strdup(filename);
	FILE* file;

	/* open the file */
	file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "glmReadOBJ() failed: can't open data file \"%s\".\n",
			filename);
		getchar();
		return GL_FALSE;
	}

	/* allocate a new model */
	
	

	/* make a first pass through the file to get a count of the number
	of vertices, normals, texcoords & triangles */
	firstparse(file);

	/* allocate memory */
	vertices = (GLfloat*)malloc(sizeof(GLfloat)*
		3 * (numvertices + 1));
	triangles = (GLMtriangle*)malloc(sizeof(GLMtriangle)*
		numtriangles);
	if (numnormals) {
		normals = (GLfloat*)malloc(sizeof(GLfloat)*
			3 * (numnormals + 1));
	}
	if (numtexcoords) {
		texcoords = (GLfloat*)malloc(sizeof(GLfloat)*
			2 * (numtexcoords + 1));
	}

	/* rewind to beginning of file and read in the data this pass */
	rewind(file);

	secondparse(file);

	/* close the file */
	fclose(file);

	return GL_TRUE;
}

GLvoid ObjLoader::ReadMTL(char* name)
{
	FILE* file;

	char  buf[128];
	GLuint nummaterials0, i;


	/* open the file */
	file = fopen(name, "r");
	if (!file) {
		fprintf(stderr, "_glmReadMTL() failed: can't open material file \"%s\".\n",
			name);
		exit(1);
	}

	/* count the number of materials in the file */
	nummaterials0 = 1;
	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'n':				/* newmtl */
			fgets(buf, sizeof(buf), file);
			nummaterials0++;
			sscanf(buf, "%s %s", buf, buf);
			break;
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}

	rewind(file);

	/* allocate memory for the materials */
	materials.resize(nummaterials0);
	nummaterials = nummaterials0;

	/* set the default material */
	for (i = 0; i < nummaterials; i++) {
		materials[i].name = " ";
		materials[i].shininess = 0;
		materials[i].diffuse[0] = 0.8;
		materials[i].diffuse[1] = 0.8;
		materials[i].diffuse[2] = 0.8;
		materials[i].diffuse[3] = 1.0;
		materials[i].ambient[0] = 0.2;
		materials[i].ambient[1] = 0.2;
		materials[i].ambient[2] = 0.2;
		materials[i].ambient[3] = 1.0;
		materials[i].specular[0] = 0.0;
		materials[i].specular[1] = 0.0;
		materials[i].specular[2] = 0.0;
		materials[i].specular[3] = 1.0;
	}
	materials[0].name = strdup("default");

	/* now, read in the data */
	nummaterials0 = 0;
	while (fscanf(file, "%s", buf) != EOF) {
		switch (buf[0]) {
		case '#':				/* comment */
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		case 'n':				/* newmtl */
			fgets(buf, sizeof(buf), file);
			sscanf(buf, "%s %s", buf, buf);
			nummaterials0++;
			materials[nummaterials0].name = strdup(buf);
			break;
		case 'N':
			fscanf(file, "%f", &materials[nummaterials0].shininess);
			/* wavefront shininess is from [0, 1000], so scale for OpenGL */
			materials[nummaterials0].shininess /= 1000.0;
			materials[nummaterials0].shininess *= 128.0;
			break;
		case 'K':
			switch (buf[1]) {
			case 'd':
				fscanf(file, "%f %f %f",
					&materials[nummaterials0].diffuse[0],
					&materials[nummaterials0].diffuse[1],
					&materials[nummaterials0].diffuse[2]);
				break;
			case 's':
				fscanf(file, "%f %f %f",
					&materials[nummaterials0].specular[0],
					&materials[nummaterials0].specular[1],
					&materials[nummaterials0].specular[2]);
				break;
			case 'a':
				fscanf(file, "%f %f %f",
					&materials[nummaterials0].ambient[0],
					&materials[nummaterials0].ambient[1],
					&materials[nummaterials0].ambient[2]);
				break;
			default:
				/* eat up rest of line */
				fgets(buf, sizeof(buf), file);
				break;
			}
			break;
		default:
			/* eat up rest of line */
			fgets(buf, sizeof(buf), file);
			break;
		}
	}
}
void ObjLoader::draw(){
	GLuint i;
	GLMgroup* group;

	/*
	if (mode & GLM_COLOR)
	glEnable(GL_COLOR_MATERIAL);
	if (mode & GLM_MATERIAL)
	glDisable(GL_COLOR_MATERIAL);
	*/
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslatef(position[0], position[1], position[2]);

	glBegin(GL_TRIANGLES);
	group = groups;
	while (group) {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
			materials[group->material].ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
			materials[group->material].diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
			materials[group->material].specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS,
			materials[group->material].shininess);

		glColor3fv(materials[group->material].diffuse);

		for (i = 0; i < group->numtriangles; i++) {

			glNormal3fv(&normals[3 * T(group->triangles[i]).nindices[0]]);

			glVertex3fv(&vertices[3 * T(group->triangles[i]).vindices[0]]);

			glNormal3fv(&normals[3 * T(group->triangles[i]).nindices[1]]);
			glVertex3fv(&vertices[3 * T(group->triangles[i]).vindices[1]]);

			glNormal3fv(&normals[3 * T(group->triangles[i]).nindices[2]]);
			glVertex3fv(&vertices[3 * T(group->triangles[i]).vindices[2]]);
		}

		group = group->next;
	}
	glEnd();

	glPopMatrix();
}