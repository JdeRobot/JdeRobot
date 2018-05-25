var API = API || {};
var frameCount = 0;
var oldFrameTime = undefined;

/*
 * API.Camera's Constructor.
 * Params:
 *     - config (contents client's Config)={server,id,epname,fpsid,imgFormat}:
 *       + id (canvas' id to show RGB camera)
 *       + fpsid (id of element to show fps, is optional)
 *       + server (server's direction and port)={dir:direction,port: port}
 *       + epname (name of camera endpoint, default cameraA)
 *       + imgFormat (format of image that is going to request to the server, default RGB8)
 */
API.CameraRos = function (config){
   var conf = config || {};

   this.data = undefined;
   this.roscam = undefined;
   this.rosdescrip = undefined;
   this.ros = undefined;
   this.imgFormat = conf.imgFormat || "RGB8";
   this.server = conf.serv;


   var self = this;

   this.connect = function (){

        self.ros = new ROSLIB.Ros({
            url : "ws://" + self.server.dir + ":" + self.server.port
         });

         // This function is called upon the rosbridge connection event
         self.ros.on('connection', function() {
             // Write appropriate message to #feedback div when successfully connected to rosbridge
             console.log("Connect websocket")
         });
        // This function is called when there is an error attempting to connect to rosbridge
        self.ros.on('error', function(error) {
            // Write appropriate message to #feedback div upon error when attempting to connect to rosbridge
            console.log("Error to connect websocket")
        });

        // These lines create a topic object as defined by roslibjs
         self.roscam = new ROSLIB.Topic({
            ros : self.ros,
            name : conf.topic,
            messageType : conf.msgs
        });

        self.rosdescrip = new ROSLIB.Topic({
          ros: self.ros,
          name : "/usb_cam/camera_info",
          messageType: "sensor_msgs/CameraInfo"
        })
   };

   this.disconnect = function (){
     self.ros.close();
     self.ros.on('close',function(){
       console.log("Disconnect websocket");
     });
     self.ros = undefined;
   };

   this.stop = function (){
     self.roscam.unsubscribe();
     self.rosdescrip.unsubscribe();
     frameCount = 0;
   }

   this.startStreaming = function(){
        var canvas = document.getElementById("camView");
        var fps = $('#' + config.fpsid);
        var size = $('#' + config.sizeid);
        var ctx = canvas.getContext("2d");
        var imagedata = new Image();
        self.rosdescrip.subscribe(function(message){
          size.html(message.width + "x" + message.height);
        })

        self.roscam.subscribe(function(message){
          if (message.format != null){
            imagedata.src = "data:image/jpg;base64," + message.data;
            imagedata.onload = function(){
              ctx.drawImage(imagedata,0,0,canvas.width,canvas.height);
            }
          } else {
            console.log(message);
          }
          var fps_2 = calculateFPS();
          if (fps_2){
             fps.html(Math.floor(fps_2));

        }
      }
      );

   };

};

function calculateFPS(){
        var fps_2 = undefined;
        var d = new Date();
        var currentFrameTime = d.getTime();
        var diff = (currentFrameTime - oldFrameTime);
        if (diff < 1000){
           frameCount+=1;
        }else{
           oldFrameTime = currentFrameTime;
           fps_2 = frameCount*1000.0/diff;
           frameCount=0;
         }
        return fps_2;
}
