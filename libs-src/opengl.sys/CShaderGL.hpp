/* -----------------------------------------------------------------------------
 *  CShaderGL.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CShaderGL
{
    public:

        CShaderGL(const char *idProgram);
        ~CShaderGL();

        void appendVertexShader(const char *idVertexShader, const char *codeVertexShader);
        void appendFragmentShader(const char *idFragmedShader, const char *codeFragmentShader);

        void appendFileShader(const char *nameFileShaderVSorFS);

        bool compileAndLinkProgram(class CArrayString **msgsErrorOpt);

        void run(void);
        void stop(void);

    private:

        struct SPrvDataPrivateShaderGL *m_dataPrivate;
};
