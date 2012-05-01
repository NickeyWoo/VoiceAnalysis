#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sndfile.h>

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

extern void SoundDiagramDisplayFunc();

int main(int argc, char* argv[])
{
	SF_INFO sndinfo;
	SNDFILE* fdSnd = sf_open(argv[1], SFM_READ, &sndinfo);
	if(fdSnd == NULL)
	{
		printf("sf_open fail, %s.\n", sf_strerror(fdSnd));
		return -1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	int wnd = glutCreateWindow("Sound Diagram - OpenGl Window");
	glutDisplayFunc(SoundDiagramDisplayFunc);

	glutMainLoop();

	sf_close(fdSnd);
	return 0;
}

void SoundDiagramDisplayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(1.0, 1.0, 1.0);
//	glLineWidth(10);
	glBegin(GL_POLYGON);
	glVertex2f(30, 30);
	glVertex2f(30, 90);
	glVertex2f(90, 90);
	glVertex2f(90, 30);
	glEnd();

	glFlush();
}

