// variables to make believe to ICE that is in the main thread
var window=self;
var global=self;

// importing required files
importScripts('../Ice.min.js');
importScripts('../jderobot/datetime.js');
importScripts('../jderobot/exceptions.js');
importScripts('../jderobot/containers.js');
importScripts('../jderobot/common.js');
importScripts('../jderobot/motors.js');


// variables related to the configuration and connection of ICE
var id = new Ice.InitializationData();
var communicator;

var Promise;
var Prx = jderobot.MotorsPrx;
var srv;
var L=undefined,W=undefined,V=undefined;



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
		console.log("Notors connected: " + ar);
         srv = ar;
          
         postMessage(true);
	},
	function(ex, ar){
		console.log("Notors NOT connected: " + ex);
       postMessage(false);
	});
      

}


function getW(){
      srv.getW().then(function (data){
            W = data;   
         response = {v:V,l:L,w:W};
         postMessage(response);
      },function(err){console.log(err);});
}

function getV(){
   srv.getV().then(function (data){
         V = data;   
         response = {v:V,l:L,w:W};
         postMessage(response);
   },function(err){console.log(err);});
}

function getL(){
   srv.getL().then(function (data){
            L = data;   
         response = {v:V,l:L,w:W};
         postMessage(response);
   },function(err){console.log(err);});
}

function getAll(){
   console.log("getAll");
   
}

function setV(data){
   srv.setV(data).then(function (data){
                },function(err){console.log(err);});
}
function setW(data){
   srv.setW(data).then(function (data){
                },function(err){console.log(err);});
}
function setL(data){
   srv.setL(data).then(function (data){
                },function(err){console.log(err);});
}


function setAll(data){
   if (data.w){
      setW(data.w);
   }
   
   if (data.v){
      setV(data.v);
   }
   
   if (data.l){
      setL(data.l);
   }
}



onmessage = function(e) {
    var config={};
    // Collects the data provided by the main thread and initialized ICE
    config.server = e.data.serv;
    config.epname = e.data.epname;
    var vel = e.data.data;
   
    switch (e.data.func){
    case "getAll":
          getAll();
          break;
    case "getV":
          getV();
          break;
    case "getW":
          getW();
          break;
    case "getL":
          getL();
          break;
    case "setAll":
          setAll(vel);
          break;
    case "setV":
          setV(vel);
          break;
    case "setW":
          setW(vel);
          break;
    case "setL":
          setL(vel);
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
