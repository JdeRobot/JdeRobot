var config={};
var camera;
//avoid browser error
try {
  const yaml = require('js-yaml');
  const fs = require('fs');
  config = yaml.safeLoad(fs.readFileSync('public/config.yml', 'utf8'));
} catch(e){
  config.serv = {};
  config.serv.dir= $('#dir').val();
  config.serv.port= $('#port').val();
  config.serv.tech = $('#serv').val();
  config.epname = $('#ep').val();
  console.log(e);
}

$(document).ready(function() {
  //Load configure parametres
   load();
   config.camid= "camView";
   config.fpsid= "fps";
   config.sizeid= "size";
   var server = config.serv.tech;

   $('#start').on('click', function(){
      $("#camView").removeClass("border-light");
      //In case of Ice, called at CameraView class for Ice
      if (server == "Ice"){
        camera = new CameraViewIce(config);
      //In case of Ros, called at CameraView class for Ros
      } else if (server == "Ros")
		    camera = new CameraViewRos(config);
      else {
        console.log("Error, not server name")
      }
      //Start camera
         camera.start();
	});
  //Stop camera
   $('#stop').on('click', function(){
         camera.stop();
	});

   var resize = function (){
      $(".cam").height( $(".cam").width()*3/4);
   };

   $(window).resize(function(){
      resize();
   });

   //In case of change the server in configuration, called  at function changeConfig
   $('#serv').change(function(){
      changeConfig($('#serv').val());
   })

   $('#save').on('click', function(){
    config.serv.dir= $('#dir').val();
    config.serv.port= $('#port').val();
    config.serv.tech = $('#serv').val();
    server = $('#serv').val();
    if ($('#serv').val() == "Ice") {
      config.epname = $('#ep').val();
    } else if ($('#serv').val() == "Ros"){
      config.topic = $('#topic').val();
      config.msgs = $('#messageType').val();
    }
    if (camera != undefined){
      camera.stop();
      camera = undefined;
    }
    localStorage["cameraviewconfig"]=JSON.stringify(config);
   });

   resize();
});

function load(){
          changeConfig(config.serv.tech);
          $('#serv').val(config.serv.tech);
          $('#dir').val(config.serv.dir);
          $('#port').val(config.serv.port);
          if (config.serv.tech == "Ice") {
            $('#ep').val(config.endpoint);
          } else if (config.serv.tech == "Ros"){
            changeConfig(config.serv.tech);
            $('#topic').val(config.topic);
            $('#messageType').val(config.msgs);
   }
}

//Change the configure menu dependent if the server is Ice or Ros
function changeConfig(serverTec){
  if (serverTec == "Ice"){
    if ($('#ep').length == 0){
      $('#rostopic').remove();
      $("#confignav").append('<div id = "endpoint" class="input-group">'+
         '<span class="input-group-addon" id="basic-addon1">EndPoint</span>'+
         '<input id="ep" type="text" class="form-control" value="cameraA" aria-describedby="basic-addon1">'+
      '</div>')
      }
  } else if (serverTec == "Ros") {
    if ($('#rostopic').length == 0){
    $("#endpoint").remove();
    $("#confignav").append('<div id = "rostopic"><div class="input-group"><span class="input-group-addon" id="basic-addon1">RosTopic</span>'+
    '<input id="topic" type="text" class="form-control" value="/usb_cam/image_raw/compressed" aria-describedby="basic-addon1"></div>'
    +'<br><div class="input-group"><span class="input-group-addon" id="basic-addon1">RosMessageType</span>'+
    '<input id="messageType" type="text" class="form-control" value="sensor_msgs/CompressedImage" aria-describedby="basic-addon1"></div></div>')
  }
  }
}
