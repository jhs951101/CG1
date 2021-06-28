#include <stdio.h>
#include <stdlib.h>

#include <GL\glut.h>
#include <GL\gl.h>
#include <GL\glu.h>
#include "bmp.h"

float ballX = 0.0;
float ballY = 0.0;
float ballmoveX = 0.1;
float ballmoveY = -0.1;

float BarX = 0.0;
float BarRangeA = BarX;
float BarRangeB = BarX + 0.8;

GLfloat xrot = 0.9f, yrot = 0.8f, zrot = 1.0f;
unsigned int MyTextureObject[1];
AUX_RGBImageRec* pTextureImage[1];  //�ؽ��� ���� ������ ����Ű�� ������

void checking() {

	if (ballY <= -1.0)
		exit(0);

	if ((ballY <= -0.7 && (ballX >= BarRangeA && ballX <= BarRangeB))) {
		ballmoveY = ballmoveY * -1;
	}

	if ((ballX <= -0.9 || ballX >= 0.9)) {
		ballmoveX = ballmoveX * -1;
	}

	if (ballY >= 0.9) {
		ballmoveY = ballmoveY * -1;
	}
}

void MyReshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void MyDisplay() {
	glViewport(0, 0, 500, 500);
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(BarX, -0.8, 0.0);  //�ո�
	glTexCoord2f(1.0f, 0.0f); glVertex3f(BarX + 0.8, -0.8, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(BarX + 0.8, -1.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(BarX, -1.0, 0.0);
	glEnd();

	glColor3f(1.0, 1.0, 0.0);
	glPushMatrix();
	glTranslatef(ballX, ballY, 0.0);
	glutSolidSphere(0.1, 50.0, 50.0);
	glPopMatrix();

	glFlush();

	glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
	switch (KeyPressed) {
	case 'a':
		BarX = BarX - 0.1;
		break;
	case 'd':
		BarX = BarX + 0.1;
		break;
	case 27:                //'esc' Ű�� �ƽ�Ű �ڵ� ��
		exit(0);
		break;
	}

	BarRangeA = BarX;
	BarRangeB = BarX + 0.8;
}

void MyTimer(int Value) {

	checking();

	ballX = ballX + ballmoveX;
	ballY = ballY + ballmoveY;

	glutPostRedisplay();
	glutTimerFunc(50, MyTimer, 1);
}

AUX_RGBImageRec* LoadBMP(char* szFilename) {
	FILE* pFile = NULL;
	if (!szFilename) {
		return NULL;
	}
	pFile = fopen(szFilename, "r");
	if (pFile) {
		fclose(pFile);
		return auxDIBImageLoad(szFilename);     //���Ϸκ��� �޸𸮷�  (!)
	}
	return NULL;
}

int LoadGLTextures(char* szFilePath) {       //������ �ε��ϰ� �ؽ��ķ� ��ȯ
	int Status = FALSE;
	glClearColor(0.0, 0.0, 0.0, 0.0);
	memset(pTextureImage, 0, sizeof(void*) * 1);    //�����͸� �η�

	if (pTextureImage[0] = LoadBMP(szFilePath)) {   //��Ʈ���� �ε��ϰ� ����Ȯ��
		Status = TRUE;                              //���� �÷��� True��
		glGenTextures(1, &MyTextureObject[0]);      //�ؽ��� ����
		glBindTexture(GL_TEXTURE_2D, MyTextureObject[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3,
			pTextureImage[0]->sizeX, pTextureImage[0]->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pTextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glEnable(GL_TEXTURE_2D);
	}
	if (pTextureImage[0]) {                 //�ؽ��İ� �����ϸ�
		if (pTextureImage[0]->data) {       //�ؽ��� ������ �����ϸ�
			free(pTextureImage[0]->data);   //�ؽ��� ������� �ݳ�
		}
		free(pTextureImage[0]);             //�ؽ��� �ݳ�
	}
	return Status;
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Ball Game !!");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);

	if (LoadGLTextures("kon.bmp")) {
		glutDisplayFunc(MyDisplay);
		glutKeyboardFunc(MyKeyboard);
		glutTimerFunc(50, MyTimer, 1);
		glutMainLoop();
	}
	else {
		printf("cannot find an image file...\n");
	}
}