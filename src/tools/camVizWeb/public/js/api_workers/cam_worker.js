// variables to make believe to ICE that is in the main thread
var window=self;
var global=self;

// importing required files
importScripts('../Ice.min.js');
importScripts('../jderobot/datetime.js');
importScripts('../jderobot/exceptions.js');
importScripts('../jderobot/containers.js');
importScripts('../jderobot/common.js');
importScripts('../jderobot/image.js');
importScripts('../jderobot/primitives.js');
importScripts('../jderobot/camera.js');


// variables related to the configuration and connection of ICE
var id = new Ice.InitializationData();
var communicator;

var Promise;
var Prx = jderobot.CameraPrx;
var stream = false;
var srv;

var oldFrameTime = undefined;
var frameCount = 0;

var description = undefined;
var img = undefined;

function calculateFPS(){
      var fps = undefined;
      var d = new Date();
      var currentFrameTime = d.getTime();
      var diff = (currentFrameTime - oldFrameTime);
      if (diff < 1000){
         frameCount+=1;
      }else{
         oldFrameTime = currentFrameTime;
         fps = frameCount*1000.0/diff;
         frameCount=0;
       }
      return fps;
   };



function connect (epname, server){

   id.properties = Ice.createProperties();
   //id.properties.setProperty("Ice.Trace.Network", "3");
   //id.properties.setProperty("Ice.Trace.Protocol", "1");
   communicator = Ice.initialize(id);


    // Create the proxy to connect
    var proxy = communicator.stringToProxy(epname+":ws -h " + server.dir + " -p " + server.port);
    console.log(epname);
    console.log(server.dir);
    console.log(server.port);
	// connects to the proxy and asks the image
    Promise = Prx.checkedCast(proxy).then(
       function(ar){
		console.log("Camera connected: " + ar);
         srv = ar;
         postMessage(true);
	},
	function(ex, ar){
		console.log("Camera NOT connected: " + ex);
       postMessage(false);
	});


}

// makes the request to the server
function getImage(imgFormat){
    srv.getImageData(imgFormat).then(function (data){
                var pixelData = data.pixelData;
                var width = data.description.width;
                var height = data.description.height;
                // Once received the image (RGB), I adapt it to the format that canvas needs (RGBA)
                var imgData=new Uint8Array(4*width*height);
                var j=0;
                var length = imgData.length;
                if (imgFormat.indexOf("depth")>-1 || imgFormat.indexOf("DEPTH")>-1){
                  for (var i=0;i<length;i+=4)
                  {
                    imgData[i+0]=pixelData[j+0];
                    imgData[i+1]=pixelData[j+0];
                    imgData[i+2]=pixelData[j+0];
                    imgData[i+3]=255;
                    j+=3;
                  }

                }else{
                  for (var i=0;i<length;i+=4)
                  {
                    imgData[i+0]=pixelData[j+0];
                    imgData[i+1]=pixelData[j+1];
                    imgData[i+2]=pixelData[j+2];
                    imgData[i+3]=255;
                    j+=3;
                  }
               }
               var fps = calculateFPS();
                img={width: width,
                              height: height,
                              imgData: imgData,
                              pixelData: pixelData,
                              fps:fps};
                postMessage({desc:description,img:img});
                if (stream){
                  getImage(imgFormat);
                };

        },function(err,ar){
       console.log(ar);
       console.log(err);});
}



// makes the request to the server
function getCameraDescription(){
    srv.getCameraDescription().then(function (data){
                description=data;
                postMessage({desc:description,img:img});

        },function(err,ar){
       console.log(ar);
       console.log(err);});
}



onmessage = function(e) {
    // Collects the data provided by the main thread and initialized ICE
    server = e.data.serv;
    epname = e.data.epname;
    imgForm = e.data.imgFormat;
      switch (e.data.func){
      case "getImage":
            getImage(imgForm);
            break;
      case "startImageStream":
            stream = true;
            getImage(imgForm);
            break;
      case "stopImageStream":
            stream = false;
            break;
      case "getCameraDescription":
            getCameraDescription();
            break;
      case "setCameraDescription":
            //setCameraDescription(server);
            break;
      case "connect":
            connect(epname,server);
            break;
      case "disconnect":
            srv = undefined;
            break;
      default:

      }

}
