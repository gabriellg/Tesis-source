// ccube.c
// OpenGL SuperBible
// Demonstrates primative RGB Color Cube
// Program by Richard S. Wright Jr.

#include "opengl.inl"

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLUquadricObj *quadric;
static unsigned long tipo_dibujo = 0;

//---------------------------------------------------------------

static void i_cilindro(void)
{
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_FLAT);
	gluCylinder(quadric, 5., 5., 150., 15, 10);

	glPushMatrix();
	
	glTranslated(0., 0., 150.);
	gluCylinder(quadric, 10., 0., 30., 15, 10);
	
	glPopMatrix();
}


//---------------------------------------------------------------

static void i_ejes(void)
{
	glPushMatrix();
	
	glTranslated(-50., -50., -50.);
	
	glColor3ub((GLubyte) 128, (GLubyte) 128, (GLubyte) 128);
	gluSphere(quadric, 10., 15, 10);
	
	glColor3ub((GLubyte) 255, 0, 0);
	i_cilindro();
	
	glRotatef(90., 0.0f, 1.0f, 0.0f);
	glColor3ub(0, (GLubyte) 255, 0);
	i_cilindro();
	
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3ub(0, 0, (GLubyte) 255);
	i_cilindro();

	glPopMatrix();
}

void i_diagonal(void)
{
	glBegin(GL_QUADS);
	
		// Front Face
		// Black
		glColor3ub((GLubyte) 0, (GLubyte)0, (GLubyte)0);
		glVertex3f(-50.0f,-50.0f,-50.0f);
	
		// Yellow
		glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)255);
		glVertex3f(50.0f,50.0f,50.0f);
	
		// Red
		glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)255);
		glVertex3f(50.0f,55.0f,50.0f);
	
		// Magenta
		glColor3ub((GLubyte) 0, (GLubyte)0, (GLubyte)0);
		glVertex3f(-50.0f,-55.0f,-50.0f);
	
	glEnd();
}


void i_cubo(void)
{
	// Draw six quads
	glBegin(GL_QUADS);
		// Front Face
		// White
		glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)255);
		glVertex3f(50.0f,50.0f,50.0f);
	
		// Yellow
		glColor3ub((GLubyte) 255, (GLubyte)255, (GLubyte)0);
		glVertex3f(50.0f,-50.0f,50.0f);
	
		// Red
		glColor3ub((GLubyte) 255, (GLubyte)0, (GLubyte)0);
		glVertex3f(-50.0f,-50.0f,50.0f);
	
		// Magenta
		glColor3ub((GLubyte) 255, (GLubyte)0, (GLubyte)255);
		glVertex3f(-50.0f,50.0f,50.0f);
	
	
	// Back Face
		// Cyan
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(50.0f,50.0f,-50.0f);
	
		// Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(50.0f,-50.0f,-50.0f);
		
		// Black
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-50.0f,-50.0f,-50.0f);
	
		// Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-50.0f,50.0f,-50.0f);
	
	// Top Face
		// Cyan
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(50.0f,50.0f,-50.0f);
	
		// White
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(50.0f,50.0f,50.0f);
	
		// Magenta
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-50.0f,50.0f,50.0f);
	
		// Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-50.0f,50.0f,-50.0f);
	
	// Bottom Face
		// Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(50.0f,-50.0f,-50.0f);
	
		// Yellow
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(50.0f,-50.0f,50.0f);
	
		// Red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-50.0f,-50.0f,50.0f);
	
		// Black
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-50.0f,-50.0f,-50.0f);
	
	// Left face
		// White
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(50.0f,50.0f,50.0f);
	
		// Cyan
		glColor3f(0.0f, 1.0f, 1.0f);
		glVertex3f(50.0f,50.0f,-50.0f);
	
		// Green
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(50.0f,-50.0f,-50.0f);
	
		// Yellow
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3f(50.0f,-50.0f,50.0f);
	
	// Right face
		// Magenta
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f(-50.0f,50.0f,50.0f);
	
		// Blue
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-50.0f,50.0f,-50.0f);
	
		// Black
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-50.0f,-50.0f,-50.0f);
	
		// Red
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-50.0f,-50.0f,50.0f);
	glEnd();
}

// Called to draw scene
void RenderScene(void)
    {
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glPushMatrix();

    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    i_ejes();
    
    switch(tipo_dibujo)
    {
		case 0:
			i_cubo();
			break;
		case 1:
			i_diagonal();
			break;
		default:
			break;
    }

    glPopMatrix();

    // Show the graphics
    glutSwapBuffers();
    }

// This function does any needed initialization on the rendering
// context. 
void SetupRC()
    {
    // Black background
    glClearColor(0.75f, 0.75f, 0.75f, 0.0f );

    glEnable(GL_DEPTH_TEST);	
    glEnable(GL_DITHER);
    glShadeModel(GL_SMOOTH);
    }

/////////////////////////////////////////////////
// Get Arrow Keys
void SpecialKeys(int key, int x, int y)
    {
    if(key == GLUT_KEY_UP)
        xRot-= 5.0f;

    if(key == GLUT_KEY_DOWN)
        xRot += 5.0f;

    if(key == GLUT_KEY_LEFT)
        yRot -= 5.0f;

    if(key == GLUT_KEY_RIGHT)
        yRot += 5.0f;
    
    if(key == GLUT_KEY_F1)
    	tipo_dibujo = (tipo_dibujo + 1) % 2;

    if(key > 356.0f)
        xRot = 0.0f;

    if(key < -1.0f)
        xRot = 355.0f;

    if(key > 356.0f)
        yRot = 0.0f;

    if(key < -1.0f)
        yRot = 355.0f;
    
    // Refresh the Window
    glutPostRedisplay();
    }


void ChangeSize(int w, int h)
    {
    GLfloat fAspect;

    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    fAspect = (GLfloat)w / (GLfloat)h;
    gluPerspective(35.0f, fAspect, 1.0f, 1000.0f);
     
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -600.0f);
    }

int main(int argc, char* argv[])
	{
	quadric = gluNewQuadric(); 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
        glutCreateWindow("RGB Cube");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutMainLoop();
	gluDeleteQuadric(quadric);

	return 0;
	}
