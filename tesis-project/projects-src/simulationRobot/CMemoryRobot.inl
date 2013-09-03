/* -----------------------------------------------------------------------------
 *  CMemoryRobot.inl
 *  License: GNU Public License
 * ----------------------------------------------------------------------------*/

class CMemoryRobot
{
    public:

        CMemoryRobot(
                    unsigned long widthMemory, unsigned long heightMemory,
                    unsigned long widthRobot, unsigned long heightRobot,
                    long xRobot, long yRobot,
                    double angleRobot);

        ~CMemoryRobot();

        void saveInMemoryLaser(double distance, double angle);

        void saveInMemoryMarkVideo(const char *mark);

        class CAgent *decide(const char *markObjective);

        class CFigure *figureImageMemory(void) const;


    private:

        struct prv_dataPrivateMemoryRobot_t *m_dataPrivate;
};
