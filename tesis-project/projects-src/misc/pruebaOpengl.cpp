// Ejemplo de opengl.

#include "opengl.inl"

void display_func()
{
    glClearColor(1., 0., 0., 1.); /* Red */
    glClear(GL_COLOR_BUFFER_BIT);
}

void keyboard_func(unsigned char key, int x, int y)
{
    ;
}

//----------------------------------------------------

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutCreateWindow("Hello GLUT!");
    glutDisplayFunc(display_func);
    glutKeyboardFunc(keyboard_func);
    glutMainLoop();
    return 0;
}
