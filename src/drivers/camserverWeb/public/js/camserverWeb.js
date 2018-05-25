var config={};
var camera;
var videoSelect = document.querySelector('select#videoSource');
//avoid browser error
try {
  const yaml = require('js-yaml');
  const fs = require('fs');
  config = yaml.safeLoad(fs.readFileSync('public/config.yml', 'utf8'));
  $('#dir').val(config.serv.dir);
  $('#port').val(config.serv.port);
  $('#topic').val(config.topic);
  $('#messageType').val(config.msgs);
  $('#fps').val(config.fps);
  localStorage["cameraserverconfig"]=JSON.stringify(config);
  //in case of error, get the configuration of Configure menu
  } catch (e) {
    if (localStorage.getItem("cameraserverconfig")) {
        config = JSON.parse(localStorage.getItem("cameraserverconfig"));
       $('#dir').val(config.serv.dir);
       $('#port').val(config.serv.port);
       $('#topic').val(config.topic);
       $('#messageType').val(config.msgs);
       } else {
         config.serv={};
         config.serv.dir= $('#dir').val();
         config.serv.port= $('#port').val();
         config.topic = $('#topic').val();
         config.msgs = $('#messageType').val();
         config.fps = $('#fps').val();
       }
    }


    navigator.mediaDevices.enumerateDevices()
    .then(function (devices) {
      for (var i = 0; i !== devices.length; ++i) {
        var deviceInfo = devices[i];
        var option = document.createElement('option');
        option.value = deviceInfo.deviceId;
        if (deviceInfo.kind === 'videoinput') {
          option.text = deviceInfo.label || 'Camera ' +
            (videoSelect.length + 1);
            videoSelect.appendChild(option);
        }
      }});

$(document).ready(function() {
  //Load configure parametres
  camServer = new CameraServer(config);
  camServer.start();

   $('#save').on('click', function(){
    config.serv.dir = $('#dir').val();
    config.serv.port = $('#port').val();
    config.topic = $('#topic').val();
    config.msgs = $('#messageType').val();
    config.fps = $('#fps').val();
    camServer.restart();
    localStorage["cameraserverconfig"]=JSON.stringify(config);
   });

});
