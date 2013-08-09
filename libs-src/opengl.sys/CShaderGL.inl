/* -----------------------------------------------------------------------------
 *  CShaderGL.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CShaderGL
{
    public:

        CShaderGL(const char *idProgram);
        ~CShaderGL();

        void appendVertexShader(const char *codeVertexShader);
        void appendFragmentShader(const char *codeFragmentShader);

        bool compileAndLinkProgram(void);

    private:

        struct SPrvDataPrivateShaderGL *m_dataPrivate;
};
