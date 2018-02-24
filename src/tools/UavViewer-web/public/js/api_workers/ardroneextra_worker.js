// variables to make believe to ICE that is in the main thread
var window=self;
var global=self;

// importing required files
importScripts('../Ice.min.js');
importScripts('../jderobot/ardroneextra.js');


// variables related to the configuration and connection of ICE
var id = new Ice.InitializationData();
var communicator;

var Promise;
var Prx = jderobot.ArDroneExtraPrx;
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
		console.log("ArDroneExtra connected: " + ar);
         srv = ar;
         postMessage(true);
	},
	function(ex, ar){
		console.log("ArDroneExtra NOT connected: " + ex);
       postMessage(false);
	});
      

}


function toggleCam(){
    srv.toggleCam().then(function (data){
    },function(err){console.log(err);});
}

function land(){
    srv.land().then(function (data){
    },function(err){console.log(err);});
}

function takeoff(){
    srv.takeoff().then(function (data){
    },function(err){console.log(err);});
}

function reset(){
   console.log("reset");
    /*srv.reset().then(function (data){
    },function(err){console.log(err);});*/
}

function recordOnUsb(){
   console.log("recordOnUsb");
    /*srv.recordOnUsb().then(function (data){
      },function(err){console.log(err);});*/
}

function ledAnimation(){
   console.log("ledAnimation");
    /*srv.ledAnimation().then(function (data){
      },function(err){console.log(err);});*/
}

function flightAnimation(){
   console.log("flightAnimation");
    /*srv.flightAnimation().then(function (data){
       },function(err){console.log(err);});*/
}

function flatTrim(){
   console.log("flatTrim");
    /*srv.flatTrim().then(function (data){
      },function(err){console.log(err);});*/
}


onmessage = function(e) {
    //console.log("camera worker");
   console.log(e.data);
    var config={};
    // Collects the data provided by the main thread and initialized ICE
    config.server = e.data.serv;
    config.epname = e.data.epname;
   
    switch (e.data.func){
      case "toggleCam":
         toggleCam();
         break;
      case "land":
         land();
         break;
      case "takeoff":
         takeoff();
         break;
      case "reset":
         reset();
         break;
      case "recordOnUsb":
         recordOnUsb();
         break;
      case "ledAnimation":
         ledAnimation();
         break;
      case "flightAnimation":
         flightAnimation();
         break;
      case "flatTrim":
         flatTrim();
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