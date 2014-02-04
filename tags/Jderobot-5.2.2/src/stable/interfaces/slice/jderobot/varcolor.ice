#ifndef VARCOLOR_ICE
#define VARCOLOR_ICE


#include <jderobot/image.ice>

module jderobot{  

  /** 
   * Interface to the image provider.
   */
  interface VarColor
  {
    /** 
     * Returns the image source description.
     */
    idempotent ImageDescription getDescription();
    
    /**
     * Returns the latest data.
     */
    ["amd"] idempotent ImageData getData()
      throws DataNotExistException, HardwareFailedException;
  };

}; /*module*/

#endif /*VARCOLOR_ICE*/
