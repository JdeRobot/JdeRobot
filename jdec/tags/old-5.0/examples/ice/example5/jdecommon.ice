#ifndef JDECOMMON_ICE
#define JDECOMMON_ICE

module jde{
  exception JDEException
  {
    //! Error description.
    string what;
  };
  
  //! Server failed to configure itself as requrested by client.
  exception ConfigurationNotExistException extends JDEException {};

  /*!
    Raised when the server does not have the requested data.
    
    Typically, this is because the server has not fully initialized yet.
  */
  exception DataNotExistException extends JDEException {};
  
  //! Indicates a problem with robot hardware, e.g. sensors and actuators.
  exception HardwareFailedException extends JDEException {};

  //! A sequence of bytes.
  sequence<byte> ByteSeq;

  struct Time
  {
    //! Number of seconds
    int seconds;
    //! Number of microseconds
    int useconds;
  };

}; /*module*/

#endif /*JDECOMMON_ICE*/
