// ccube.c
// OpenGL SuperBible
// Demonstrates primative RGB Color Cube
// Program by Richard S. Wright Jr.

#include "opengl.inl"

#include "asrtbas.h"
#include "stdio.h"

// Rotation amounts
static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat z = 0.0f;
static GLfloat angulo = 0.0f;
static GLUquadricObj *quadric;
static bool mover_primero = true;
static unsigned long tipo_dibujo = 0;
static unsigned short i_LAPSO = 80;
static GLfloat z_camara = -30.;

enum i_tipo_animacion_t
{
	i_EMPEZAR,
	i_MOVER,
	i_ROTAR,
	i_FIN
};

static enum i_tipo_animacion_t tipo_animacion = i_FIN;

static void i_cambia(void)
{
	switch(tipo_animacion)
	{
		case i_EMPEZAR:
		{
			if (mover_primero == true)
				tipo_animacion = i_MOVER;
			else
				tipo_animacion = i_ROTAR;
			break;
		}
		case i_MOVER:
		{
			z += 0.5;
			
			if (z > 5.)
			{
				if (mover_primero == true)
					tipo_animacion = i_ROTAR;
				else
					tipo_animacion = i_FIN;
			}
			break;
		}
		case i_ROTAR:
		{
			angulo += 5.;
			
			if (angulo >= 90.)
			{
				if (mover_primero == false)
					tipo_animacion = i_MOVER;
				else
					tipo_animacion = i_FIN;
			}
			break;
		}
		case i_FIN:
		{
			break;
		}
		default_error();
	}
	
	if (mover_primero == true)
	{
		glRotated(angulo, 0., 1., 0);
		glTranslated(0., 0., z);
	}
	else
	{
		glTranslated(0., 0., z);
		glRotated(angulo, 0., 1., 0);
	}
}

//---------------------------------------------------------------

static void i_cilindro(void)
{
	gluQuadricDrawStyle(quadric, GLU_FILL);
	gluQuadricNormals(quadric, GLU_FLAT);
	gluCylinder(quadric, 0.5, 0.5, 8., 15, 10);
	
	glPushMatrix();
	
	glTranslated(0., 0., 8.);
	gluCylinder(quadric, 0.8, 0., 3., 15, 10);
	
	glPopMatrix();
}

//---------------------------------------------------------------

static void i_dibuja_eje()
{
	glPushMatrix();
	
    glColor3ub((GLubyte) 128, (GLubyte) 128, (GLubyte) 128);
    gluSphere(quadric, 1., 15, 10);
    
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

//---------------------------------------------------------------

static void i_colorAnimacion()
{
	switch(tipo_animacion)
	{
		case i_EMPEZAR:
			
			glColor3ub(104, 232, 209);
			break;
			   
		case i_MOVER:
			
			glColor3ub(149, 229, 167);
			break;
			
		case i_ROTAR:
			
			glColor3ub(203, 148, 205);
			break;
			
		case i_FIN:
			
			glColor3ub(232, 203, 147);
			break;
			
		default_error();
	}
}

//---------------------------------------------------------------

static void i_drawPrimitiveCube(void)
{
    static GLfloat points[] = 
    {
        0., 0., 0., //0       
        1., 0., 0., //1
        1., 1., 0., //2
        0., 1., 0., //3
        0., 0., 1., //4        
        1., 0., 1., //5
        1., 1., 1., //6
        0., 1., 1.  //7
    };

    static GLubyte indexQuads[] = 
    {
        3, 2, 1, 0,
        4, 5, 6, 7,
        1, 2, 6, 5,
        0, 4, 7, 3,
        3, 7, 6, 2,
        0, 1, 5, 4
    };
    
	glPushMatrix();
	glScaled(4., 4., 4.);
	glTranslated(-0.5, -0.5, -0.5);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, points);

    glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, indexQuads);

    glDisableClientState(GL_VERTEX_ARRAY);
    
	glPopMatrix();
}

// Called to draw scene
void RenderScene(void)
    {
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    glPushMatrix();

    glTranslated(-2., -2., z_camara);
    
    glRotatef(xRot, 1.0f, 0.0f, 0.0f);
    glRotatef(yRot, 0.0f, 1.0f, 0.0f);

    i_dibuja_eje();
    
	glPushMatrix();
	
	i_cambia();

	i_colorAnimacion();	
    i_drawPrimitiveCube();

	glPopMatrix();
	
    
    switch(tipo_dibujo)
    {
		case 0:
			//i_drawUnitSphere();			
			break;
		case 1:
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
    //glClearColor(0.75f, 0.75f, 0.75f, 0.0f );
	glClearColor(0.f, 0.f, 0.f, 0.0f );

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

void Keys(unsigned char key, int x, int y)
{
	if (key == 'i' || key == 'I')
	{
		z = 0.0f;
		angulo = 0.0f;
	}
	
	if (key == 's' || key == 'S')
	{
		z = 0.0f;
		angulo = 0.0f;
		
		tipo_animacion = i_EMPEZAR;
	}
	
	if (key == 'c' || key == 'c')
	{
		if (mover_primero == true)
			mover_primero = false;
		else
			mover_primero = true;
	}
	
	if (key == 'w' || key == 'W')
		z_camara = -30; 
	
	if (key == 'q' || key == 'Q')
		z_camara += 1.;
	
	if (key == 'a' || key == 'A')
		z_camara -= 1.;
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
    gluPerspective(65.0f, fAspect, 1.0f, 1000.0f);
     
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    }

//---------------------------------------------------------------

static void i_timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(i_LAPSO, i_timer, 1);
}

//---------------------------------------------------------------

int main(int argc, char* argv[])
	{
	
	printf("Mover y Rotar\n");
	printf("-------------\n");
	printf("i: Posición inicial\n");
	printf("s: Empezar\n");
	printf("c: Cambiar entre mover y rotar\n");
	printf("w: Inicio cámara\n");
	printf("q: Avanza cámara\n");
	printf("a: Retrocede cámara\n");
	
	quadric = gluNewQuadric(); 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,600);
        glutCreateWindow("Mover y Rotar");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(Keys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	glutTimerFunc(i_LAPSO, i_timer, 1);
	glutMainLoop();
	gluDeleteQuadric(quadric);

	return 0;
	}
