/* -----------------------------------------------------------------------------
 *  options.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

#include "IOptionOGL.hpp"

class COptionCreateModel3d : public IOptionOGL
{
    public:

        virtual ~COptionCreateModel3d();

    protected:

        COptionCreateModel3d(class CModel3d **modelApplication);
        virtual class CModel3d *createModel(void) = 0;

    private:

        virtual enum IOptionOGL::EResult execute(void);

        struct SPrvDataPrivateOptionCreateModel3d *m_dataPrivate;
};

class COptionCreateCube : public COptionCreateModel3d
{
    public:

        COptionCreateCube(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreateGrid3d : public COptionCreateModel3d
{
    public:

        COptionCreateGrid3d(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreatePathOpen : public COptionCreateModel3d
{
    public:

        COptionCreatePathOpen(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreatePathClosed : public COptionCreateModel3d
{
    public:

        COptionCreatePathClosed(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreatePrism : public COptionCreateModel3d
{
    public:

        COptionCreatePrism(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreateSurface : public COptionCreateModel3d
{
    public:

        COptionCreateSurface(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreateCylinder : public COptionCreateModel3d
{
    public:

        COptionCreateCylinder(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);
};

class COptionCreateModelRead : public COptionCreateModel3d
{
    public:

        COptionCreateModelRead(const char *nameFile, class CModel3d **modelApplication);
        virtual ~COptionCreateModelRead();

    protected:

        virtual class CModel3d *createModel(void);

    private:

        char *m_nameFile;
};

class COptionCreateWithImage : public COptionCreateModel3d
{
    public:

        COptionCreateWithImage(class CModel3d **modelApplication) : COptionCreateModel3d(modelApplication) {;};

    protected:

        virtual class CModel3d *createModel(void);

};


