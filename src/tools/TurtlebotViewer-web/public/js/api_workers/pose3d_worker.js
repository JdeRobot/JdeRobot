// variables to make believe to ICE that is in the main thread
var window=self;
var global=self;

// importing required files
importScripts('../Ice.min.js');
importScripts('../jderobot/datetime.js');
importScripts('../jderobot/exceptions.js');
importScripts('../jderobot/containers.js');
importScripts('../jderobot/common.js');
importScripts('../jderobot/pose3d.js');


// variables related to the configuration and connection of ICE
var id = new Ice.InitializationData();
var communicator;

var Promise;
var Prx = jderobot.Pose3DPrx;
var stream = false;
var srv;

// Functions return the value of fliying parameters
function getYaw(qw,qx,qy,qz) {                     
       var rotateZa0=2.0*(qx*qy + qw*qz);
       var rotateZa1=qw*qw + qx*qx - qy*qy - qz*qz;
       var rotateZ=0.0;
       if(rotateZa0 != 0.0 && rotateZa1 != 0.0){
           rotateZ=Math.atan2(rotateZa0,rotateZa1);
       }
       return rotateZ;
}

function getRoll(qw,qx,qy,qz){
       rotateXa0=2.0*(qy*qz + qw*qx);
       rotateXa1=qw*qw - qx*qx - qy*qy + qz*qz;
       rotateX=0.0;

       if(rotateXa0 != 0.0 && rotateXa1 !=0.0){
           rotateX=Math.atan2(rotateXa0, rotateXa1);
       }   
       return rotateX;
}
function getPitch(qw,qx,qy,qz){
       rotateYa0=-2.0*(qx*qz - qw*qy);
       rotateY=0.0;
       if(rotateYa0>=1.0){
           rotateY=math.PI/2.0;
       } else if(rotateYa0<=-1.0){
           rotateY=-Math.PI/2.0
       } else {
           rotateY=Math.asin(rotateYa0)
       }

       return rotateY;
}

function connect (config){

   id.properties = Ice.createProperties();
   //id.properties.setProperty("Ice.Trace.Network", "3");
   //id.properties.setProperty("Ice.Trace.Protocol", "1");
    communicator = Ice.initialize(id);

   
    // Create the proxy to connect
    var proxy = communicator.stringToProxy(config.epname+":ws -h " + config.server.dir + " -p " + config.server.port);
	// connects to the proxy and asks the image
    Promise = Prx.checkedCast(proxy).then(
       function(ar){
		console.log("Pose3D connected: " + ar);
         srv = ar;
          
         postMessage(true);
	},
	function(ex, ar){
		console.log("Pose3D NOT connected: " + ex);
       postMessage(false);
	});
}


function getPose3D(){
    srv.getPose3DData().then(function (data){
       var response={};
       response.x=data.x;
       response.y=data.y;
       response.z=data.z;
       response.q0=data.q0;
       response.q1=data.q1;
       response.q2=data.q2;
       response.q3=data.q3;
       response.yaw=getYaw(data.q0,data.q1,data.q2,data.q3);
       response.pitch=getPitch(data.q0,data.q1,data.q2,data.q3);
       response.roll=getRoll(data.q0,data.q1,data.q2,data.q3);
       postMessage(response);
       if (stream){
         getPose3D();
       }

    },function(err){console.log(err);});
}


onmessage = function(e) {
    var config={};
    // Collects the data provided by the main thread and initialized ICE
    config.server = e.data.serv;
    config.epname = e.data.epname;
   
    switch (e.data.func){
      case "getPose3D":
         getPose3D();
         break;
      case "setPose3D":
         //setPose3D(config);
         break;
      case "startPose3DStream":
         stream = true;
         getPose3D();
         break;
      case "stopPose3DStream":
         stream = false;
         break;
      case "connect":
         connect(config);
         break;
      case "disconnect":
         srv = undefined;
         break;
      default:

    }
}
