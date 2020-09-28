

#include <GL/gl.h>                // Заголовочные файлы для библиотеки OpenGL32

#include <GL/glut.h>              // Заголовочные файлы для библиотеки GLu32



/* Function plotting func */
void draw(void)
{
    // glLoadIdentity();

    // glTranslatef(-1.5f,0.0f,-6.0f);

    // glBegin(GL_TRIANGLES);
    //             glVertex3f( 0.0f, 1.0f, 0.0f);  // Вверх
    //             glVertex3f(-1.0f,-1.0f, 0.0f);  // Слева снизу
    //             glVertex3f( 1.0f,-1.0f, 0.0f);  // Справа снизу
    // glEnd();

    // glTranslatef(3.0f,0.0f,0.0f);
    // glBegin(GL_QUADS);
    //             glVertex3f(-1.0f, 1.0f, 0.0f);  // Слева вверху
    //             glVertex3f( 1.0f, 1.0f, 0.0f);  // Справа вверху
    //             glVertex3f( 1.0f,-1.0f, 0.0f);  // Справа внизу
    //             glVertex3f(-1.0f,-1.0f, 0.0f);  // Слева внизу
    // glEnd();
	float x, dx = 1.0/N;

	glPushMatrix(); /* GL_MODELVIEW is default */

	glTranslatef(100, -100, 0.0);
	glColor3f(1.0, 1.0, 1.0);

	glBegin(GL_LINE_STRIP);

	for(x = x1; x < x2; x += dx)
	{
		glVertex2f(x, func(x));
	}

	glEnd();

	glPopMatrix();
};

/* Redrawing func */
void redraw(void)
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	draw();

	glutSwapBuffers();
};


/* Window reashape */
void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
};

/* Main function */
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutCreateWindow("Graph plotter");

	/* Register GLUT callbacks. */
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);

	/* Init the GL state */
	glLineWidth(5.0);
  
	/* Main loop */
	glutMainLoop();
	return 0;
}