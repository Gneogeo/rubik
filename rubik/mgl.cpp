
#include <math.h>
#include <QTime>

#include "mgl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

float R=0.2,S=0.16;
float dist=7;
#define ORTHOFRUSTRUM glFrustum (-.3, .3, -.3, .3, 4, 10);

float DT=500.;
int way=1;

class MoveBuffer {
	char *movBuffer;
	int movBufferLen;
	int movBufferMem;
public:
	
	MoveBuffer() {movBuffer=0; movBufferLen=0; movBufferMem=0;}
	~MoveBuffer() {free(movBuffer);}

	void pushBuffer(char mov);
	char popBuffer();
};

MoveBuffer Buffer;

void MoveBuffer::pushBuffer(char mov)
{
	if (movBufferLen>0 &&
		movBuffer[movBufferLen-1]==-mov) {
			movBufferLen--;
			return;
	}
	if (movBufferLen>1 &&
		movBuffer[movBufferLen-1]==mov &&
		movBuffer[movBufferLen-2]==mov) {
			movBuffer[movBufferLen-2]=-mov;
			movBufferLen--;
			return;
	}


	if (movBufferLen==movBufferMem) {
		movBufferMem+=10;
		movBuffer=(char *)realloc(movBuffer,movBufferMem*sizeof(char));
	}
	movBuffer[movBufferLen]=mov;
	movBufferLen++;
}

char MoveBuffer::popBuffer()
{
	if (movBufferLen>0) {
		movBufferLen--;
		return movBuffer[movBufferLen];
	}
	return 0;
}


float col[][3]={{0.1,0.1,0.1},{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1}};

int map[6][3][3]=
{{{1,1,1},{1,1,1},{1,1,1}},
	{{2,2,2},{2,2,2},{2,2,2}},
	{{3,3,3},{3,3,3},{3,3,3}},
	{{4,4,4},{4,4,4},{4,4,4}},
	{{5,5,5},{5,5,5},{5,5,5}},
	{{6,6,6},{6,6,6},{6,6,6}}};



class Cube 
 {
 public:
	 float coords[8][3];
	 float norm[6][3];
	 int colors[6];
	 Cube(float x0,float y0,float z0,float b,int r1,int r2,int r3,int r4,int r5,int r6);
	 void move(float dx,float dy,float dz);
	 void drawCube();
	 void drawWireCube();
	 void rotX(int angle);
	 void rotY(int angle);
	 void rotZ(int angle);
 };

Cube::Cube(float x0,float y0,float z0,float a,int r1,int r2,int r3,int r4,int r5,int r6)
{
	colors[0]=r1; colors[1]=r2; colors[2]=r3;
	colors[3]=r4; colors[4]=r5; colors[5]=r6;
	coords[0][0]=x0-a*.5; coords[0][1]=y0-a*.5; coords[0][2]=z0-a*.5;
	coords[1][0]=x0+a*.5; coords[1][1]=y0-a*.5; coords[1][2]=z0-a*.5;
	coords[2][0]=x0-a*.5; coords[2][1]=y0+a*.5; coords[2][2]=z0-a*.5;
	coords[3][0]=x0+a*.5; coords[3][1]=y0+a*.5; coords[3][2]=z0-a*.5;
	coords[4][0]=x0-a*.5; coords[4][1]=y0-a*.5; coords[4][2]=z0+a*.5;
	coords[5][0]=x0+a*.5; coords[5][1]=y0-a*.5; coords[5][2]=z0+a*.5;
	coords[6][0]=x0-a*.5; coords[6][1]=y0+a*.5; coords[6][2]=z0+a*.5;
	coords[7][0]=x0+a*.5; coords[7][1]=y0+a*.5; coords[7][2]=z0+a*.5;

	norm[0][0]= 0; norm[0][1]=-1; norm[0][2]= 0;
	norm[1][0]=-1; norm[1][1]= 0; norm[1][2]= 0;
	norm[2][0]= 0; norm[2][1]= 0; norm[2][2]= 1;
	norm[3][0]= 0; norm[3][1]= 0; norm[3][2]=-1;
	norm[4][0]= 1; norm[4][1]= 0; norm[4][2]= 0;
	norm[5][0]= 0; norm[5][1]= 1; norm[5][2]= 0;
}

void Cube::move(float dx,float dy,float dz)
{
	int k;
	for (k=0; k<8; k++) {
		coords[k][0]+=dx;
		coords[k][1]+=dy;
		coords[k][2]+=dz;
	}
}

void Cube::rotX(int angle)
{
	float f=angle*3.14159/180.;
	float cosf=cos(f),sinf=sin(f);
	float b,c;
	int k;
	for (k=0; k<8; k++) {
		b=cosf*coords[k][1]-sinf*coords[k][2];
		c=sinf*coords[k][1]+cosf*coords[k][2];
		coords[k][1]=b; coords[k][2]=c;		
	}
	for (k=0; k<6; k++) {
		b=cosf*norm[k][1]-sinf*norm[k][2];
		c=sinf*norm[k][1]+cosf*norm[k][2];
		norm[k][1]=b; norm[k][2]=c;		
	}
}

void Cube::rotY(int angle)
{
	float f=angle*3.14159/180.;
	float cosf=cos(f),sinf=sin(f);
	float b,c;
	int k;
	for (k=0; k<8; k++) {
		b=cosf*coords[k][2]-sinf*coords[k][0];
		c=sinf*coords[k][2]+cosf*coords[k][0];
		coords[k][2]=b; coords[k][0]=c;		
	}
	for (k=0; k<6; k++) {
		b=cosf*norm[k][2]-sinf*norm[k][0];
		c=sinf*norm[k][2]+cosf*norm[k][0];
		norm[k][2]=b; norm[k][0]=c;		
	}
}


void Cube::rotZ(int angle)
{
	float f=angle*3.14159/180.;
	float cosf=cos(f),sinf=sin(f);
	float b,c;
	int k;
	for (k=0; k<8; k++) {
		b=cosf*coords[k][0]-sinf*coords[k][1];
		c=sinf*coords[k][0]+cosf*coords[k][1];
		coords[k][0]=b; coords[k][1]=c;		
	}
	for (k=0; k<6; k++) {
		b=cosf*norm[k][0]-sinf*norm[k][1];
		c=sinf*norm[k][0]+cosf*norm[k][1];
		norm[k][0]=b; norm[k][1]=c;		
	}
}

void Cube::drawCube()
{
	glBegin(GL_QUADS);
	glColor3fv(col[colors[0]]);
	glNormal3fv(norm[0]);
	glVertex3fv(coords[0]); 
	glVertex3fv(coords[1]);
	glVertex3fv(coords[5]);
	glVertex3fv(coords[4]);

	glColor3fv(col[colors[1]]);
	glNormal3fv(norm[1]);
	glVertex3fv(coords[0]); 
	glVertex3fv(coords[4]);
	glVertex3fv(coords[6]);
	glVertex3fv(coords[2]);

	glColor3fv(col[colors[2]]);
	glNormal3fv(norm[2]);
	glVertex3fv(coords[4]); 
	glVertex3fv(coords[5]);
	glVertex3fv(coords[7]);
	glVertex3fv(coords[6]);

	glColor3fv(col[colors[3]]);
	glNormal3fv(norm[3]);
	glVertex3fv(coords[0]); 
	glVertex3fv(coords[2]);
	glVertex3fv(coords[3]);
	glVertex3fv(coords[1]);

	glColor3fv(col[colors[4]]);
	glNormal3fv(norm[4]);
	glVertex3fv(coords[1]); 
	glVertex3fv(coords[3]);
	glVertex3fv(coords[7]);
	glVertex3fv(coords[5]);

	glColor3fv(col[colors[5]]);
	glNormal3fv(norm[5]);
	glVertex3fv(coords[2]); 
	glVertex3fv(coords[6]);
	glVertex3fv(coords[7]);
	glVertex3fv(coords[3]);


	glEnd();
}


void Cube::drawWireCube()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_FLOAT,0,coords);

	
	static unsigned int P[]={0,1,3,2,0,4,5,7,6,4};

	glDrawElements(GL_LINE_STRIP,10,GL_UNSIGNED_INT,(void *)P);
	/*
	glBegin(GL_LINE_STRIP);
	glVertex3fv(coords[0]);
	glVertex3fv(coords[1]);
	glVertex3fv(coords[3]);
	glVertex3fv(coords[2]);
	glVertex3fv(coords[0]);
	glVertex3fv(coords[4]);
	glVertex3fv(coords[5]);
	glVertex3fv(coords[7]);
	glVertex3fv(coords[6]);
	glVertex3fv(coords[4]);
	glEnd();
	*/
	static unsigned int P1[]={1,5,2,6,3,7};
	glDrawElements(GL_LINES,6,GL_UNSIGNED_INT,(void *)P1);
	/*
	glBegin(GL_LINES);
	glVertex3fv(coords[1]); glVertex3fv(coords[5]);
	glVertex3fv(coords[2]); glVertex3fv(coords[6]);
	glVertex3fv(coords[3]); glVertex3fv(coords[7]);
	glEnd();
	*/

}

Cube *A[27]={0};


void resetCube()
{	
	int k;
	for (k=0; k<27; k++) {
		 delete A[k];
	}
	int k1,k2,k3;
	 k=0;
	 int c1,c2,c3,c4,c5,c6;
	 
	 
	A[ 0]=new Cube(-R,-R,-R,S,		map[0][0][0],map[1][0][2],           0,		map[3][0][0],           0,           0);
	A[ 1]=new Cube( 0,-R,-R,S,		map[0][0][1],           0,           0,		map[3][0][1],           0,           0);
	A[ 2]=new Cube( R,-R,-R,S,		map[0][0][2],           0,           0,		map[3][0][2],map[4][0][2],           0);
	A[ 3]=new Cube(-R, 0,-R,S,		           0,map[1][0][1],           0,		map[3][1][0],           0,           0);
	A[ 4]=new Cube( 0, 0,-R,S,		           0,           0,           0,		map[3][1][1],           0,           0);
	A[ 5]=new Cube( R, 0,-R,S,		           0,           0,           0,		map[3][1][2],map[4][0][1],           0);
	A[ 6]=new Cube(-R, R,-R,S,		           0,map[1][0][0],           0,		map[3][2][0],           0,map[5][0][0]);
	A[ 7]=new Cube( 0, R,-R,S,		           0,           0,           0,		map[3][2][1],           0,map[5][0][1]);
	A[ 8]=new Cube( R, R,-R,S,		           0,           0,           0,		map[3][2][2],map[4][0][0],map[5][0][2]);

	A[ 9]=new Cube(-R,-R, 0,S,		map[0][1][0],map[1][1][2],           0,		           0,           0,           0);
	A[10]=new Cube( 0,-R, 0,S,		map[0][1][1],           0,           0,		           0,           0,           0);
	A[11]=new Cube( R,-R, 0,S,		map[0][1][2],           0,           0,		           0,map[4][1][2],           0);
	A[12]=new Cube(-R, 0, 0,S,		           0,map[1][1][1],           0,		           0,           0,           0);
	A[13]=new Cube( 0, 0, 0,S,		           0,           0,           0,		           0,           0,           0);
	A[14]=new Cube( R, 0, 0,S,		           0,           0,           0,		           0,map[4][1][1],           0);
	A[15]=new Cube(-R, R, 0,S,		           0,map[1][1][0],           0,		           0,           0,map[5][1][0]);
	A[16]=new Cube( 0, R, 0,S,		           0,           0,           0,		           0,           0,map[5][1][1]);
	A[17]=new Cube( R, R, 0,S,		           0,           0,           0,		           0,map[4][1][0],map[5][1][2]);
	
	A[18]=new Cube(-R,-R, R,S,		map[0][2][0],map[1][2][2],map[2][0][0],		           0,           0,           0);
	A[19]=new Cube( 0,-R, R,S,		map[0][2][1],           0,map[2][0][1],		           0,           0,           0);
	A[20]=new Cube( R,-R, R,S,		map[0][2][2],           0,map[2][0][2], 	           0,map[4][2][2],           0);
	A[21]=new Cube(-R, 0, R,S,		           0,map[1][2][1],map[2][1][0],		           0,           0,           0);
	A[22]=new Cube( 0, 0, R,S,		           0,           0,map[2][1][1],		           0,           0,           0);
	A[23]=new Cube( R, 0, R,S,		           0,           0,map[2][1][2],		           0,map[4][2][1],           0);
	A[24]=new Cube(-R, R, R,S,		           0,map[1][2][0],map[2][2][0],		           0,           0,map[5][2][0]);
	A[25]=new Cube( 0, R, R,S,		           0,           0,map[2][2][1],		           0,           0,map[5][2][1]);
	A[26]=new Cube( R, R, R,S,		           0,           0,map[2][2][2],		           0,map[4][2][0],map[5][2][2]);

}




 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(parent)
 {
     xRot = 0;
     yRot = 0;
     zRot = 0;

	 xRotOld=0;
	 yRotOld=0;

		
	 resetCube();
	 
}

 GLWidget::~GLWidget()
 {
	 int k;
	 for (k=0; k<27; k++) {
		 delete A[k];
	 }
  }

#define SHIFT_FACE(a_)\
tmp[0]=map[a_][2][2]; tmp[1]=map[a_][1][2];\
tmp[2]=map[a_][0][2]; tmp[3]=map[a_][0][1];\
tmp[4]=map[a_][0][0]; tmp[5]=map[a_][1][0];\
tmp[6]=map[a_][2][0]; tmp[7]=map[a_][2][1];\
map[a_][2][2]=tmp[6]; map[a_][1][2]=tmp[7];\
map[a_][0][2]=tmp[0]; map[a_][0][1]=tmp[1];\
map[a_][0][0]=tmp[2]; map[a_][1][0]=tmp[3];\
map[a_][2][0]=tmp[4]; map[a_][2][1]=tmp[5];


#define DESHIFT_FACE(a_)\
tmp[6]=map[a_][2][2]; tmp[7]=map[a_][1][2];\
tmp[0]=map[a_][0][2]; tmp[1]=map[a_][0][1];\
tmp[2]=map[a_][0][0]; tmp[3]=map[a_][1][0];\
tmp[4]=map[a_][2][0]; tmp[5]=map[a_][2][1];\
map[a_][2][2]=tmp[0]; map[a_][1][2]=tmp[1];\
map[a_][0][2]=tmp[2]; map[a_][0][1]=tmp[3];\
map[a_][0][0]=tmp[4]; map[a_][1][0]=tmp[5];\
map[a_][2][0]=tmp[6]; map[a_][2][1]=tmp[7];

void  GLWidget::rot1()
{
	int tmp[8];

	if (way==1) {    
	    SHIFT_FACE(0);
		tmp[0]=map[1][2][2]; tmp[1]=map[1][1][2]; tmp[2]=map[1][0][2];
		map[1][2][2]=map[3][0][0]; map[1][1][2]=map[3][0][1]; map[1][0][2]=map[3][0][2];
		map[3][0][0]=map[4][0][2]; map[3][0][1]=map[4][1][2]; map[3][0][2]=map[4][2][2];
		map[4][0][2]=map[2][0][2]; map[4][1][2]=map[2][0][1]; map[4][2][2]=map[2][0][0];
		map[2][0][2]=tmp[0]; map[2][0][1]=tmp[1]; map[2][0][0]=tmp[2];
	} else {
		DESHIFT_FACE(0);
		tmp[0]=map[2][0][2]; tmp[1]=map[2][0][1]; tmp[2]=map[2][0][0];
		map[2][0][2]=map[4][0][2]; map[2][0][1]=map[4][1][2]; map[2][0][0]=map[4][2][2];
		map[4][0][2]=map[3][0][0]; map[4][1][2]=map[3][0][1]; map[4][2][2]=map[3][0][2];
		map[3][0][0]=map[1][2][2]; map[3][0][1]=map[1][1][2]; map[3][0][2]=map[1][0][2];
		map[1][2][2]=tmp[0]; map[1][1][2]=tmp[1]; map[1][0][2]=tmp[2];
	}
	
	int k;
	int r;
	float fr;
	QTime time;

	k=0;
	fr=0;
	while (k<90) {
		time.start();
		updateGL();
		fr+=(time.elapsed()*90.)/DT;
		r=fr; fr-=r;
		if (k+r>90) r=90-k;
		k=k+r;
		if (way==0) r=-r;
		A[0]->rotY(r); A[1]->rotY(r); A[2]->rotY(r);
		A[9]->rotY(r); A[10]->rotY(r); A[11]->rotY(r);
		A[18]->rotY(r); A[19]->rotY(r); A[20]->rotY(r);
		
	}
	
	resetCube();
	
	updateGL();
}



void  GLWidget::rot2()
{
	int tmp[8];
	if (way==1) {
		SHIFT_FACE(1);
		tmp[0]=map[5][2][0]; tmp[1]=map[5][1][0]; tmp[2]=map[5][0][0];
		map[5][2][0]=map[3][2][0]; map[5][1][0]=map[3][1][0]; map[5][0][0]=map[3][0][0];
		map[3][2][0]=map[0][0][0]; map[3][1][0]=map[0][1][0]; map[3][0][0]=map[0][2][0];
		map[0][0][0]=map[2][0][0]; map[0][1][0]=map[2][1][0]; map[0][2][0]=map[2][2][0];
		map[2][0][0]=tmp[0]; map[2][1][0]=tmp[1]; map[2][2][0]=tmp[2];
	} else {
		DESHIFT_FACE(1);
		tmp[0]=map[2][0][0]; tmp[1]=map[2][1][0]; tmp[2]=map[2][2][0];
		map[2][0][0]=map[0][0][0]; map[2][1][0]=map[0][1][0]; map[2][2][0]=map[0][2][0];
		map[0][0][0]=map[3][2][0]; map[0][1][0]=map[3][1][0]; map[0][2][0]=map[3][0][0];
		map[3][2][0]=map[5][2][0]; map[3][1][0]=map[5][1][0]; map[3][0][0]=map[5][0][0];
		map[5][2][0]=tmp[0]; map[5][1][0]=tmp[1]; map[5][0][0]=tmp[2];
	}

	int k;
	int r;
	float fr;
	QTime time;

	k=0;
	fr=0;
	while (k<90) {
		time.start();
		updateGL();
		fr+=(time.elapsed()*90.)/DT;
		r=fr; fr-=r;
		if (k+r>90) r=90-k;
		k=k+r;
		if (way==0) r=-r;
		A[0]->rotX(r); A[3]->rotX(r); A[6]->rotX(r);
		A[9]->rotX(r); A[12]->rotX(r); A[15]->rotX(r);
		A[18]->rotX(r); A[21]->rotX(r); A[24]->rotX(r);
		
	}
	
	updateGL();
	resetCube();
}

void  GLWidget::rot3()
{
	int tmp[8];
	if (way==1) {
		SHIFT_FACE(2);
		tmp[0]=map[1][2][0]; tmp[1]=map[1][2][1]; tmp[2]=map[1][2][2];
		map[1][2][0]=map[0][2][0]; map[1][2][1]=map[0][2][1]; map[1][2][2]=map[0][2][2];
		map[0][2][0]=map[4][2][2]; map[0][2][1]=map[4][2][1]; map[0][2][2]=map[4][2][0];
		map[4][2][2]=map[5][2][2]; map[4][2][1]=map[5][2][1]; map[4][2][0]=map[5][2][0];
		map[5][2][2]=tmp[0]; map[5][2][1]=tmp[1]; map[5][2][0]=tmp[2];
	} else {
		DESHIFT_FACE(2);
		tmp[0]=map[5][2][2]; tmp[1]=map[5][2][1]; tmp[2]=map[5][2][0];
		map[5][2][2]=map[4][2][2]; map[5][2][1]=map[4][2][1]; map[5][2][0]=map[4][2][0];
		map[4][2][2]=map[0][2][0]; map[4][2][1]=map[0][2][1]; map[4][2][0]=map[0][2][2];
		map[0][2][0]=map[1][2][0]; map[0][2][1]=map[1][2][1]; map[0][2][2]=map[1][2][2];
		map[1][2][0]=tmp[0]; map[1][2][1]=tmp[1]; map[1][2][2]=tmp[2];
	}

	int k;
	int r;
	float fr;
	QTime time;

	k=0;
	fr=0;
	while (k<90) {
		time.start();
		updateGL();
		fr+=(time.elapsed()*90.)/DT;
		r=fr; fr-=r;
		if (k+r>90) r=90-k;
		k=k+r;
		if (way==0) r=-r;
		A[18]->rotZ(-r); A[19]->rotZ(-r); A[20]->rotZ(-r);
		A[21]->rotZ(-r); A[22]->rotZ(-r); A[23]->rotZ(-r);
		A[24]->rotZ(-r); A[25]->rotZ(-r); A[26]->rotZ(-r);
	}
	
	updateGL();
	resetCube();
	
}

void  GLWidget::rot4()
{
	int tmp[8];
	if (way==1) {
		DESHIFT_FACE(3);
		tmp[0]=map[1][0][2]; tmp[1]=map[1][0][1]; tmp[2]=map[1][0][0];
		map[1][0][2]=map[5][0][0]; map[1][0][1]=map[5][0][1]; map[1][0][0]=map[5][0][2];
		map[5][0][0]=map[4][0][0]; map[5][0][1]=map[4][0][1]; map[5][0][2]=map[4][0][2];
		map[4][0][0]=map[0][0][2]; map[4][0][1]=map[0][0][1]; map[4][0][2]=map[0][0][0];
		map[0][0][2]=tmp[0]; map[0][0][1]=tmp[1]; map[0][0][0]=tmp[2];
	} else {
		SHIFT_FACE(3);
		tmp[0]=map[0][0][2]; tmp[1]=map[0][0][1]; tmp[2]=map[0][0][0];
		map[0][0][2]=map[4][0][0]; map[0][0][1]=map[4][0][1]; map[0][0][0]=map[4][0][2];
		map[4][0][0]=map[5][0][0]; map[4][0][1]=map[5][0][1]; map[4][0][2]=map[5][0][2];
		map[5][0][0]=map[1][0][2]; map[5][0][1]=map[1][0][1]; map[5][0][2]=map[1][0][0];
		map[1][0][2]=tmp[0]; map[1][0][1]=tmp[1]; map[1][0][0]=tmp[2];
	}

	int k;
	int r;
	float fr;
	QTime time;

	k=0;
	fr=0;
	while (k<90) {
		time.start();
		updateGL();
		fr+=(time.elapsed()*90.)/DT;
		r=fr; fr-=r;
		if (k+r>90) r=90-k;
		k=k+r;
		if (way==0) r=-r;
		A[0]->rotZ(r); A[1]->rotZ(r); A[2]->rotZ(r);
		A[3]->rotZ(r); A[4]->rotZ(r); A[5]->rotZ(r);
		A[6]->rotZ(r); A[7]->rotZ(r); A[8]->rotZ(r);
	}
	
	updateGL();
	resetCube();
	
}
void  GLWidget::rot5()
{
	int tmp[8];
	if (way==1) {
		DESHIFT_FACE(4);
		tmp[0]=map[2][2][2]; tmp[1]=map[2][1][2]; tmp[2]=map[2][0][2];
		map[2][2][2]=map[0][2][2]; map[2][1][2]=map[0][1][2]; map[2][0][2]=map[0][0][2];
		map[0][2][2]=map[3][0][2]; map[0][1][2]=map[3][1][2]; map[0][0][2]=map[3][2][2];
		map[3][0][2]=map[5][0][2]; map[3][1][2]=map[5][1][2]; map[3][2][2]=map[5][2][2];
		map[5][0][2]=tmp[0]; map[5][1][2]=tmp[1]; map[5][2][2]=tmp[2];
	} else {
		SHIFT_FACE(4);
		tmp[0]=map[5][0][2]; tmp[1]=map[5][1][2]; tmp[2]=map[5][2][2];
		map[5][0][2]=map[3][0][2]; map[5][1][2]=map[3][1][2]; map[5][2][2]=map[3][2][2];
		map[3][0][2]=map[0][2][2]; map[3][1][2]=map[0][1][2]; map[3][2][2]=map[0][0][2];
		map[0][2][2]=map[2][2][2]; map[0][1][2]=map[2][1][2]; map[0][0][2]=map[2][0][2];
		map[2][2][2]=tmp[0]; map[2][1][2]=tmp[1]; map[2][0][2]=tmp[2];
	}

	int k;
	int r;
	float fr;
	QTime time;

	k=0;
	fr=0;
	while (k<90) {
		time.start();
		updateGL();
		fr+=(time.elapsed()*90.)/DT;
		r=fr; fr-=r;
		if (k+r>90) r=90-k;
		k=k+r;
		if (way==0) r=-r;
		A[2]->rotX(-r); A[5]->rotX(-r); A[8]->rotX(-r);
		A[11]->rotX(-r); A[14]->rotX(-r); A[17]->rotX(-r);
		A[20]->rotX(-r); A[23]->rotX(-r); A[26]->rotX(-r);
	}
	
	updateGL();
	resetCube();

}




void  GLWidget::rot6()
{
	int tmp[8];
	if (way==1) {
		DESHIFT_FACE(5);
		tmp[0]=map[1][0][0]; tmp[1]=map[1][1][0]; tmp[2]=map[1][2][0];
		map[1][0][0]=map[2][2][0]; map[1][1][0]=map[2][2][1]; map[1][2][0]=map[2][2][2];
		map[2][2][0]=map[4][2][0]; map[2][2][1]=map[4][1][0]; map[2][2][2]=map[4][0][0];
		map[4][2][0]=map[3][2][2]; map[4][1][0]=map[3][2][1]; map[4][0][0]=map[3][2][0];
		map[3][2][2]=tmp[0]; map[3][2][1]=tmp[1]; map[3][2][0]=tmp[2];
	} else {
		SHIFT_FACE(5);
		tmp[0]=map[3][2][2]; tmp[1]=map[3][2][1]; tmp[2]=map[3][2][0];
		map[3][2][2]=map[4][2][0]; map[3][2][1]=map[4][1][0]; map[3][2][0]=map[4][0][0];
		map[4][2][0]=map[2][2][0]; map[4][1][0]=map[2][2][1]; map[4][0][0]=map[2][2][2];
		map[2][2][0]=map[1][0][0]; map[2][2][1]=map[1][1][0]; map[2][2][2]=map[1][2][0];
		map[1][0][0]=tmp[0]; map[1][1][0]=tmp[1]; map[1][2][0]=tmp[2];
	}
	
	int k;
	int r;
	float fr;
	QTime time;

	k=0;
	fr=0;
	while (k<90) {
		time.start();
		updateGL();
		fr+=(time.elapsed()*90.)/DT;
		r=fr; fr-=r;
		if (k+r>90) r=90-k;
		k=k+r;
		if(way==0) r=-r;
		A[6]->rotY(-r); A[7]->rotY(-r); A[8]->rotY(-r);
		A[15]->rotY(-r); A[16]->rotY(-r); A[17]->rotY(-r);
		A[24]->rotY(-r); A[25]->rotY(-r); A[26]->rotY(-r);
	}
	
	updateGL();
	resetCube();
	
}





 QSize GLWidget::minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize GLWidget::sizeHint() const
 {
     return QSize(400, 400);
 }

 

 void GLWidget::initializeGL()
 {
GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = {30.0};
GLfloat light_position[] = { 0, 0, 1, 0 };
	glClearColor(.2,0,.2,1);
     glShadeModel(GL_FLAT);
     glEnable(GL_DEPTH_TEST);
	 glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	 glColorMaterial(GL_FRONT, GL_DIFFUSE);
	 glEnable(GL_COLOR_MATERIAL);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glTranslated(0.0, 0.0, -dist);
     glEnable(GL_CULL_FACE);


 }

void GLWidget::paintGL()
{

	float ww=3.14159/180.;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	
	makeObject(); 
}

 void GLWidget::resizeGL(int width, int height)
 {

     int side = qMin(width, height);
     glViewport((width - side) / 2, (height - side) / 2, side, side);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     ORTHOFRUSTRUM;
     glMatrixMode(GL_MODELVIEW);
	 lastPos = QCursor::pos ();
 }

int processHits (GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	float z1,z2;
	int hit;

	float minZ;
	int ret=0;
	GLuint ii, jj, names, *ptr;
//	qDebug ("hits = %d\n", hits);
	ptr = (GLuint *) buffer;
	for (i = 0; i < hits; i++) { /* for each hit */
		names = *ptr;
//		qDebug (" number of names for this hit = %d\n", names);
		ptr++;
		z1=(float)*ptr/0x7fffffff;
//		qDebug(" z1 is %g;", z1); 
		ptr++;
		z2=(float)*ptr/0x7fffffff;
//		qDebug(" z2 is %g\n", z2); 
		ptr++;
//		qDebug (" names are ");
		for (j = 0; j < names; j++) { /* for each name */
			hit=*ptr;
//			qDebug ("%d ", *ptr);
			if (j == 0) /* set row and column */
			ii = *ptr;
			else if (j == 1)
			jj = *ptr;
			ptr++;
		}
//		qDebug ("\n");
		if (i==0) {
			ret=hit;
			minZ=(z1<z2)?z1:z2;
		} else {
			if (z1<minZ || z2<minZ) {
				ret=hit;
				minZ=(z1<z2)?z1:z2;
			}
		}

		
	}
//	qDebug("MinZ=%f, minHit=%d",minZ,ret);
	return ret;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	if (event->buttons() & Qt::RightButton) {

		GLint viewport[4];
		int x=lastPos.x();
		int y=lastPos.y();

		glGetIntegerv(GL_VIEWPORT,viewport);
		unsigned int buffer[1000];
		


		glSelectBuffer(1000,buffer);
		glRenderMode(GL_SELECT);
		glInitNames();
		glPushName(100);

		glMatrixMode (GL_PROJECTION);
		glPushMatrix();

		glLoadIdentity();
		
		gluPickMatrix(x,viewport[3]+2*viewport[1]-y,3,3,viewport);
		ORTHOFRUSTRUM;
		
		updateGL();
		
		glMatrixMode (GL_PROJECTION);
		glPopMatrix();

		int hits=glRenderMode(GL_RENDER);

		hits=processHits(hits,buffer);
		switch (hits) {
			case 1: rot1();break;
			case 2: rot2(); break;
			case 3: rot3(); break;
			case 4: rot4(); break;
			case 5: rot5(); break;
			case 6: rot6(); break;
		}
		if (hits>=1 && hits<=6) Buffer.pushBuffer(hits);
//		qDebug("Hits=%d",hits);
	}
}

 void GLWidget::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
		 

		 GLfloat pmat[16];
	
		 glMatrixMode (GL_MODELVIEW);	
	glGetFloatv(GL_MODELVIEW_MATRIX,pmat);
	glLoadIdentity();
	
	glRotatef(dy,1, 0,0);
	glRotatef(dx, 0,1,0);
	glMultMatrixf(pmat);
	glGetFloatv(GL_MODELVIEW_MATRIX,pmat);
	pmat[12]=0; pmat[13]=0; pmat[14]=-dist;
	glLoadMatrixf(pmat);


		 updateGL();
     
	 }
     lastPos = event->pos();
 }

void GLWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
		case Qt::Key_1: rot1(); Buffer.pushBuffer(1); break;
		case Qt::Key_2: rot2(); Buffer.pushBuffer(2); break;
		case Qt::Key_3: rot3(); Buffer.pushBuffer(3); break;
		case Qt::Key_4: rot4(); Buffer.pushBuffer(4); break;
		case Qt::Key_5: rot5(); Buffer.pushBuffer(5); break;
		case Qt::Key_6: rot6(); Buffer.pushBuffer(6); break;
		case Qt::Key_7: 
			{
				DT=100;
				int k;
				for (;;) {
					k=Buffer.popBuffer();
					if (k>0) way=0; else way=1;
					if (k<0) k=-k;
					switch (k) {
						case 1: rot1(); break;
						case 2: rot2(); break;
						case 3: rot3(); break;
						case 4: rot4(); break;
						case 5: rot5(); break;
						case 6: rot6(); break;
					}
					if (k==0) break;
				}
				DT=500;
				way=1;
			}
			break;

		default: event->ignore(); break;
	}
}
 

 GLuint GLWidget::makeObject()
 {

	
	int render_mode;
	glGetIntegerv(GL_RENDER_MODE,&render_mode);

	if (render_mode==GL_SELECT) {
		float a=R+S*.5;
		
		glLoadName( 1);
		glBegin(GL_QUADS);
		glVertex3f(-a,-a,-a);
		glVertex3f( a,-a,-a);
		glVertex3f( a,-a, a);
		glVertex3f(-a,-a, a);
		glEnd();

		glLoadName( 2);
		glBegin(GL_QUADS);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a,-a, a);
		glVertex3f(-a, a, a);
		glVertex3f(-a, a,-a);
		glEnd();

		glLoadName( 3);
		glBegin(GL_QUADS);
		glVertex3f(-a,-a, a);
		glVertex3f( a,-a, a);
		glVertex3f( a, a, a);
		glVertex3f(-a, a, a);
		glEnd();

		glLoadName( 4);
		glBegin(GL_QUADS);
		glVertex3f(-a,-a,-a);
		glVertex3f(-a, a,-a);
		glVertex3f( a, a,-a);
		glVertex3f( a,-a,-a);
		glEnd();

		glLoadName( 5);
		glBegin(GL_QUADS);
		glVertex3f( a,-a,-a);
		glVertex3f( a, a,-a);
		glVertex3f( a, a, a);
		glVertex3f( a,-a, a);
		glEnd();

		glLoadName( 6);
		glBegin(GL_QUADS);
		glVertex3f(-a, a,-a);
		glVertex3f(-a, a, a);
		glVertex3f( a, a, a);
		glVertex3f( a, a,-a);
		glEnd();

	} else {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);


		int k;

		for (k=0; k<27; k++) {
			if (A[k]) A[k]->drawCube();
		}


		glDisable(GL_LIGHTING);

		glColor3f(0,0,0);
		for (k=0; k<27; k++) {
			if (A[k]) A[k]->drawWireCube();
		}

		glBegin(GL_LINES);
		glLineWidth(6);
		glColor3f(1,.2,.2);
		glVertex3f(0,0,0); glVertex3f(1,0,0);
		glColor3f(.2,1,.2);
		glVertex3f(0,0,0); glVertex3f(0,1,0);
		glColor3f(.2,.2,1);
		glVertex3f(0,0,0); glVertex3f(0,0,1);
		glEnd();
	}
	 return 0;
}
