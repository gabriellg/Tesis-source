//CSurface2d.hpp:

class CSurface2d
{
    public:

        CSurface2d();
        virtual ~CSurface2d();

        void appendContour(class CArrPoint2d **polyline);
        void appendHole(class CArrPoint2d **polyline);

        class CArray<CArrPoint3d> *applyTransformation3d(const class CStackTransformation *stackTransformation) const;

    private:

        struct SDataPrivateSurface2d *m_dataPrivate;
};
