#ifndef BODYMOVEMENTS_ICE
#define BODYMOVEMENTS_ICE

module jderobot{  

	/**
	* PCL
	*/
	struct Punto3DRGB{
      float x;
      float y;
      float z;
      float r;
      float g;
      float b;
	};


	
	sequence<Punto3DRGB> CloudPoints;

	class CloudPointsData{
		CloudPoints points;
	};


	interface CloudPointsInterface{
		idempotent	CloudPointsData getKinectData();
	};
};
#endif

