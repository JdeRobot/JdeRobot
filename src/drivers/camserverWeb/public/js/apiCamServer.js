var API = API || {};
API.CameraServer = function (conf){
  var config = conf || {};
  var videoSource;
  var cameraTimer;
  this.imageTopic = undefined;
  this.cameraInfor = undefined;
  this.ros = undefined;
  this.server = conf.serv;

  var self = this;

  this.connect = function (){

        // This function is called upon the rosbridge connection event
        self.ros = new ROSLIB.Ros();

        self.ros.on('connection', function() {
            // Write appropriate message to #feedback div when successfully connected to rosbridge
            console.log("Connect websocket")
        });
       // This function is called when there is an error attempting to connect to rosbridge
       self.ros.on('error', function(error) {
           // Write appropriate message to #feedback div upon error when attempting to connect to rosbridge
           console.log("Error to connect websocket")
       });

       self.imageTopic = new ROSLIB.Topic({
         ros : self.ros,
         name : config.topic,
         messageType : config.msgs
       });

       self.cameraInfo = new ROSLIB.Topic({
         ros: self.ros,
         name : "/usb_cam/camera_info",
         messageType: "sensor_msgs/CameraInfo"
       })
  };

  var hasRunOnce = false,
      video        = document.querySelector('#video'),
      canvas       = document.querySelector('#canvas'),
      width = 640,
      height,           // calculated once video stream size is known
      cameraStream;


      function cameraOn(){
          videoSource = $("#videoSource").val();
          var constraints = {};
          if (videoSource == null) {
            constraints = {audio: false, video: true};
          } else{
            constraints = { audio: false, video: { deviceId: {exact: videoSource}}};
          }
            navigator.mediaDevices.getUserMedia(constraints).then(function(stream) {
              video.srcObject = stream;
              video.onloadedmetadata = function(e) {
                video.play();
              };
            }).catch(function(err) {
              alert(err.name + ": " + err.message);
            });
     	 }

	function cameraOff(){
	    let stream = video.srcObject;
	    let tracks = stream.getTracks();
	    tracks.forEach(function(track) {
	      track.stop();
	    });
	    video.srcObject = null;
	  }

  video.addEventListener('canplay', function(ev){
    if (!hasRunOnce) {
      height = video.videoHeight / (video.videoWidth/width);
      video.setAttribute('width', width);
      video.setAttribute('height', height);
      canvas.setAttribute('width', width);
      canvas.setAttribute('height', height);
      hasRunOnce = true;
    }
  }, false);


  this.startStreaming = function () {
    if(cameraTimer == null) {
        self.ros.connect("ws://" + config.serv.dir + ":" + config.serv.port);
        document.getElementById("activeCam").innerHTML = "CamServer active in address " + config.serv.dir + " and port " + config.serv.port;
        cameraOn();
        cameraTimer = setInterval(function(){
          canvas.width = width;
          canvas.height = height;
          canvas.getContext('2d').drawImage(video, 0, 0, canvas.width, canvas.height);
          var data = canvas.toDataURL('image/jpeg');
          var imageMessage = new ROSLIB.Message({
            format : "jpeg",
            data : data.replace("data:image/jpeg;base64,", "")
          });

          var infoMessage = new ROSLIB.Message({
            height: canvas.height,
            width: canvas.width
          })
          self.imageTopic.publish(imageMessage);
          self.cameraInfo.publish(infoMessage);
         }, (1000/config.fps));
     } else {
         ros.close();
         clearInterval(cameraTimer);
         cameraTimer = null;
     }
   }

   this.restart = function() {
     cameraOff();
     self.ros.close();
   }
}
