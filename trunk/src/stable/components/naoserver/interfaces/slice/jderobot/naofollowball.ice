#ifndef NAOFOLLOWBALL_ICE
#define NAOFOLLOWBALL_ICE

module jderobot {
    
    /**
     * Interface to the Nao follow-ball
     */
    interface NaoFollowBall
    {
        void setParams ( int rMin, int rMax, int gMin, int gMax, int bMin, int bMax );
        void setMinParams ( int rMin, int gMin, int bMin );
        void setMaxParams ( int rMax, int gMax, int bMax );
        void start ();
        void stop ();
    };
};

#endif // NAOFOLLOWBALL_ICE
