#ifndef JOINTMOTOR_ICE
#define JOINTMOTOR_ICE

// Multi-joint motor control component.

module RoboCompJointMotor
{
  exception HardwareFailedException{ string what; };
  exception OutOfRangeException{ string what; }; 
  exception UnknownMotorException{ string what; };
  
  struct MotorState    // In Radians
  {
    float pos;         //rads
    float vel;         //rads/sg
    float power; 
    string timeStamp;
    int p;             // in steps
    int v;             // steps/sg
    bool isMoving;
  } ;
  
  dictionary<string,MotorState> MotorStateMap;
  
   struct MotorParams    //Configuration Params  
  {
    string name;         // name of motor
    byte busId;          // bus identificacion
    float minPos;        //rads
    float maxPos;        //rads
    float maxVelocity;   //rads/sec
    float zeroPos;       // rads
    bool invertedSign;   // invert angle range
  };
   
  sequence<MotorParams> MotorParamsList;
   
  struct BusParams
  {
    string handler;
    string device;
    int numMotors;
    int baudRate;
    int basicPeriod; //milliseconds
  };
  
  struct MotorGoalPosition
  {
    string name;
    float position;
    float maxSpeed;
  };
   
  sequence<MotorGoalPosition> MotorGoalPositionList;
   
  struct MotorGoalVelocity
  {
    string name;
    float velocity;
    float maxAcc;
  };
   
  sequence<string> MotorList;
  
  interface JointMotor
  {
    void setPosition(MotorGoalPosition goal) throws UnknownMotorException, HardwareFailedException;         
    void setVelocity(MotorGoalVelocity goal) throws UnknownMotorException, HardwareFailedException;  
    void setSyncPosition(MotorGoalPositionList listGoals) throws UnknownMotorException, HardwareFailedException;
    MotorParams getMotorParams(string motor) throws UnknownMotorException;
    MotorState getMotorState(string motor) throws UnknownMotorException;
    MotorStateMap getMotorStateMap(MotorList mList) throws UnknownMotorException;
    void getAllMotorState(out MotorStateMap mstateMap) throws UnknownMotorException;
    MotorParamsList getAllMotorParams();
    BusParams getBusParams();
  };
};

#endif
