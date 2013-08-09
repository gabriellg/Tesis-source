/* -----------------------------------------------------------------------------
 *  CToolsOpengl.cpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "CToolsOpengl.inl"
#include "opengl.inl"
#include "asrtbas.h"

//-----------------------------------------------------------------------

void CToolsOpengl::assertError()
{
    GLenum error;

    error = glGetError();

    if (error != GL_NO_ERROR)
    {
        const char *txtError;

        txtError = (const char *)gluErrorString(error);
        assert_message(txtError);
    }
}
