#ifndef EXCEPTIONS_ICE
#define EXCEPTIONS_ICE

module jderobot{

  exception JdeRobotException
  {
    //! Error description.
    string what;
  };
  
  //! Server failed to configure itself as requrested by client.
  exception ConfigurationNotExistException extends JdeRobotException {};

  /*!
    Raised when the server does not have the requested data.
    
    Typically, this is because the server has not fully initialized yet.
  */
  exception DataNotExistException extends JdeRobotException {};
  
  //! Indicates a problem with robot hardware, e.g. sensors and actuators.
  exception HardwareFailedException extends JdeRobotException {};

}; /*module*/

#endif /*EXCEPTIONS_ICE*/
