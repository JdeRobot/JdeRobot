#ifndef NAOMOTIONS_ICE
#define NAOMOTIONS_ICE

module jderobot {
    
    enum MotionType { RigthKick, LeftKick, StandupBack, StandupFront, Intercept, ChangeCamera, ResetNaoqi };
    
    /**
     * Interface to the Nao motions
     */
    interface NaoMotions
    {
        int setMotion ( MotionType motion );
    };
};

#endif // NAOMOTIONS_ICE
