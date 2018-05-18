// variables to make believe to ICE that is in the main thread
var window=self;
var global=self;

// importing required files
importScripts('../Ice.min.js');
importScripts('../jderobot/cmdvel.js');


// variables related to the configuration and connection of ICE
var id = new Ice.InitializationData();
var communicator;

var Promise;
var Prx = jderobot.CMDVelPrx;
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
		console.log("CMDVel connected: " + ar);
         srv = ar;
          
         postMessage(true);
	},
	function(ex, ar){
		console.log("CMDVel NOT connected: " + ex);
       postMessage(false);
	});
      

}


function setCMDVelData(cmd){
   srv.setCMDVelData(cmd).then(function (data){
    },function(err){console.log(err);});
}




onmessage = function(e) {
    var config={};
    // Collects the data provided by the main thread and initialized ICE
    config.server = e.data.serv;
    config.epname = e.data.epname;
    var vel = e.data.vel;
   
    switch (e.data.func){
      case "setCMDVelData":
         var cmd = new jderobot.CMDVelData (vel.linearX,
                                            vel.linearY,
                                            vel.linearZ,
                                            vel.angularX,
                                            vel.angularY,
                                            vel.angularZ);
         setCMDVelData(cmd);
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
