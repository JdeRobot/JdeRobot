try{
const yaml = require('js-yaml');
const fs = require('fs');
} catch (e){
  console.log(e);
}

//load the yml configuration file
var config = {};
try {
    config = yaml.safeLoad(fs.readFileSync('public/config.yml', 'utf8'));
} catch (e) {
  console.log(e);
  config.serv = {};
  config.serv.dir = "localhost";
  config.serv.port = "11000";
  config.topic = '/usb_cam/image_raw/compressed';
  config.msgs = "sensor_msgs/CompressedImage";
}
var cameraTimer;
// setup connection to the ROS server and prepare the topic
  var ros = new ROSLIB.Ros();

  ros.on('connection', function() { console.log('Connected to websocket server.');});

  ros.on('error', function(error) { console.log('Error connecting to websocket server: ', error); window.alert('Error connecting to websocket server'); });

  ros.on('close', function() { console.log('Connection to websocket server closed.');});

  var imageTopic = new ROSLIB.Topic({
    ros : ros,
    name : config.topic,
    messageType : config.msgs
  });

  var cameraInfo = new ROSLIB.Topic({
    ros: ros,
    name : "/usb_cam/camera_info",
    messageType: "sensor_msgs/CameraInfo"
  })

  // request access to the video camera and start the video stream
    var hasRunOnce = false,
        video        = document.querySelector('#video'),
        canvas       = document.querySelector('#canvas'),
        width = 640,
        height,           // calculated once video stream size is known
        cameraStream;

    // access to the webcam by getUserMedia
    function cameraOn() {
            navigator.getMedia = ( navigator.getUserMedia ||
                                   navigator.webkitGetUserMedia ||
                                   navigator.mozGetUserMedia ||
                                   navigator.msGetUserMedia);

            navigator.getMedia(
              {
                video: true,
                audio: false
              },
              function(stream) {
                cameraStream = stream;

                if (navigator.mozGetUserMedia) {
                  video.mozSrcObject = stream;
                } else {
                  var vendorURL = window.URL || window.webkitURL;
                  video.src = vendorURL.createObjectURL(stream);
                }
                video.play();
              },
              function(err) {
                console.log("An error occured! " + err);
                window.alert("An error occured! " + err);
              }
            );
            document.getElementById("activeCam").innerHTML = "CamereServer active in address " + config.serv.dir + " and port " + config.serv.port;
    }

  // function that is run once scale the height of the video stream to match the configured target width
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

  // function that is run by trigger several times a second
  // takes snapshot of video to canvas, encodes the images as base 64 and sends it to the ROS topic
    function takepicture() {
      canvas.width = width;
      canvas.height = height;

      canvas.getContext('2d').drawImage(video, 0, 0, canvas.width, canvas.height);
      var data = canvas.toDataURL('image/jpeg');
      var imageMessage = new ROSLIB.Message({
          format : "jpeg",
          data : data.replace("data:image/jpeg;base64,", "")
      });
      //Send camera info
      var infoMessage = new ROSLIB.Message({
        height: canvas.height,
        width: canvas.width
      })

      imageTopic.publish(imageMessage);
      cameraInfo.publish(infoMessage);
    }

  // turning on the timer that triggers sending pictures information several times a second
    window.addEventListener('load', function(ev){
        if(cameraTimer == null) {
            console.log(config.serv.dir);
            ros.connect("ws://" + config.serv.dir + ":" + config.serv.port);
            cameraOn();
            cameraTimer = setInterval(function(){
                  takepicture();
             }, (1000/config.fps));
         } else {
             ros.close();
             cameraOff();
             clearInterval(cameraTimer);
             cameraTimer = null;
         }
    }, false);
