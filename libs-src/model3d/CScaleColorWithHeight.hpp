/* -----------------------------------------------------------------------------
 *  CScaleColorWithHeight.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CScaleColorWithHeight
{
    public:

        CScaleColorWithHeight();
        CScaleColorWithHeight(double heightDefault);

        virtual ~CScaleColorWithHeight();

        void appendColorWithHeight(unsigned char r, unsigned char g, unsigned char b, double height);

        double getHeight(unsigned char r, unsigned char g, unsigned b) const;

    private:

        struct SPrvDataPrivateScaleColorWithHeight *m_dataPrivate;
};
