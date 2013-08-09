/* -----------------------------------------------------------------------------
 *  CGraphicsSprite.hpp
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CGraphicsSprite
{
    public:

         enum EJustificeHorizontal_t
         {
             LEFT, CENTER_HORIZONTAL, RIGHT
         };

         enum EJustificeVertical_t
         {
             UP, CENTER_VERTICAL, DOWN
         };

        CGraphicsSprite(unsigned long width, unsigned long height, unsigned char r, unsigned char g, unsigned char b);
        CGraphicsSprite(const class CImg *imageBackgroud);
        virtual ~CGraphicsSprite();

        void setOriginCartesian(void);
        void setOriginImage(void);

        void drawImage(const class CImg *image);

        void drawImageWithJustifice(
                                const class CImg *image,
                                enum EJustificeHorizontal_t justificeHorizontal,
                                enum EJustificeVertical_t justificeVertical);

        void translate(double dx, double dy);

        void scale(double sx, double sy);

        void rotate(double angle);

        void pushTransformation(void);

        void popTransformation(void);

        class CImg *getImage(void);
        void getRotate(double *angle) const;

    private:

        struct SPrvDataPrivateGraphicsSprite *m_dataPrivate;
};
