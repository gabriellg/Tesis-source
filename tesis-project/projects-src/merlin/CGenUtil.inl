//genutil.inl

class CGenUtil
{
    public:

        static void appendComentary(class CArrayString *sourceCode);

        static void appendComentaryTextDestroying(class CArrayString *sourceCode, char **textComentary);

        static void appendLineWhite(class CArrayString *sourceCode);

        static void appendStaticFunctionDefinition(const char *typeReturn, const char *nameFuncion, const char *parametersWithoutParenthesis, class CArrayString *sourceCode);

        static void appendDefinitionStaticArrayData(unsigned long levelTab, const char *typeVariable, const char *nameVariable, unsigned long numElements, class CArrayString **dataCode, class CArrayString *sourceCode);

        static void appendBodyCode(unsigned long levelTab, class CArrayString **bodyCode, class CArrayString *sourceCode);
};
