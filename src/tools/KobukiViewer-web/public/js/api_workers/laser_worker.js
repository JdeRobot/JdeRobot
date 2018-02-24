// variables to make believe to ICE that is in the main thread
var window=self;
var global=self;

// importing required files
importScripts('../Ice.min.js');
importScripts('../jderobot/datetime.js');
importScripts('../jderobot/exceptions.js');
importScripts('../jderobot/containers.js');
importScripts('../jderobot/common.js');
importScripts('../jderobot/laser.js');


// variables related to the configuration and connection of ICE
var id = new Ice.InitializationData();
var communicator;

var Prx = jderobot.LaserPrx;
var stream = false;
var Promise;
var srv;

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
		console.log("Laser connected: " + ar);
         srv = ar;     
         postMessage(true);
	},
	function(ex, ar){
		console.log("Laser NOT connected: " + ex);
       postMessage(false);
	});
      

}

function calc3dPoint (dist, ang, convertUpAxis, scale){
   var a = scale * dist * Math.cos(ang);
   var x = scale * dist * Math.sin(ang);
   if (convertUpAxis){
      return {x:x,y:0,z:a};
   } else {
      return {x:x,y:a,z:0};
   }
}

function calcNorm2DPoint (dist, maxRange, ang, max2d, center){
   var d = maxRange/max2d;
   var x = center.x + ((dist /d) * (Math.cos(ang)));
   var y = center.y - ((dist /d) * (Math.sin(ang)));
   var a = {x:x,y:y};
   return a;

}


function getLaser(convertUpAxis,canv2dWidth, scale3d){
   srv.getLaserData().then(function (data){

          
      var response={};
      var i,j;

      var numlaser = data.numLaser;
      var dist =  data.distanceData;
      var minAngle = data.minAngle;
      var maxAngle = data.maxAngle;
      var minRange = data.minRange;
      var maxRange = data.maxRange;

      var step = (maxAngle - minAngle) / numlaser;

      var array2d = []; 
      var array3d = [];
      var max = canv2dWidth/2;

      var center = {x:canv2dWidth/2 , y:canv2dWidth/2};
      for (i = 0; i< numlaser; i++ ){
         var ang = minAngle + i * step;
         var dd = calcNorm2DPoint (dist[i], maxRange, ang, max, center);
         array2d[2*i] = dd.x;
         array2d[2*i+1] = dd.y;
      }
      j=9;
      var d1 = calc3dPoint (dist[0], Math.PI, convertUpAxis, scale3d);
      var d2 = calc3dPoint (dist[1], Math.PI-Math.PI/numlaser, convertUpAxis, scale3d);
      array3d[0] = d1.x;
      array3d[1] = d1.y;
      array3d[2] = d1.z;
      array3d[3] = 0;
      array3d[4] = 0;
      array3d[5] = 0;
      array3d[6] = d2.x;
      array3d[7] = d2.y;
      array3d[8] = d2.z;
      for (i=2;i< numlaser; i++){
         ang = (i*Math.PI/numlaser);
         var d = calc3dPoint (dist[i], ang, convertUpAxis, scale3d);
         array3d[j+0] = array3d[j-3];
         array3d[j+1] = array3d[j-2];
         array3d[j+2] = array3d[j-1];
         array3d[j+3] = 0;
         array3d[j+4] = 0;
         array3d[j+5] = 0;
         array3d[j+6] = d.x;
         array3d[j+7] = d.y;
         array3d[j+8] = d.z;
         j = j + 9;
      }
      response.distanceData = dist;
      response.numLaser = numlaser;
      response.maxAngle = maxAngle;
      response.minAngle = minAngle;
      response.maxRange = maxRange;
      response.minRange = minRange;
      response.canv2dData = array2d;
      response.array3dData = array3d;

      postMessage(response);
      if (stream){
         getLaser(convertUpAxis,canv2dWidth, scale3d);
      }

  },function(err){console.log(err);});
}


onmessage = function(e) {
   var config={};
   // Collects the data provided by the main thread and initialized ICE
   config.server = e.data.serv;
   config.epname = e.data.epname;
   var convertUpAxis = e.data.convertUpAxis;
   var canv2dWidth = e.data.canv2dWidth;
   var scale3d = e.data.scale3d;
   
   switch (e.data.func){
      case "getLaser":
         getLaser(convertUpAxis, canv2dWidth, scale3d);
         break;
      case "startLaserStream":
         stream = true;
         getLaser(convertUpAxis, canv2dWidth, scale3d);
         break;
      case "stopLaserStream":
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
};
