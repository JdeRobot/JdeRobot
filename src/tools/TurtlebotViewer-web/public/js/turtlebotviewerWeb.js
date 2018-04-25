var config={};
try {
  const yaml = require('js-yaml');
  const fs = require('fs');
  config = yaml.safeLoad(fs.readFileSync('public/config.yml', 'utf8'));
  localStorage["turtlebotviewer"]=JSON.stringify(config);
} catch(e){
  console.log(e);
}
$(document).ready(function() {

   var viewer;
   load();
   config.controlid="control";
   config.camrightid="camView2";
   config.camleftid="camView";
   config.modelid="model";
   config.stopbtnid="stopR";

   $('#start').on('click', function(){
      viewer = new TurtlebotViewer(config);
      viewer.start();
       $('#mod-toggle').prop( "disabled", false );
	});
   $('#stop').on('click', function(){
         viewer.stop();
       $('#mod-toggle').prop( "disabled", true );
	});

    $('#mod-toggle').change(function(evt) {
      if ($(this).prop('checked') && viewer){
         viewer.modelON();
      }else if (viewer){
         viewer.modelOFF();
      }
    });

   var resize = function (){
      $(".cam").height( $(".cam").width()*3/4);
      if (viewer && viewer.isRunning()){
         viewer.resizeCameraModel();
      }

   };

   $(window).resize(resize);

   $('#save').on('click', function(){
      config.camrightserv.dir= $('#dirright').val();
      config.camrightserv.port= $('#portright').val();
      config.camrightepname= $('#epright').val();
      config.camleftserv.dir= $('#dirleft').val();
      config.camleftserv.port= $('#portleft').val();
      config.camleftepname= $('#epleft').val();
      config.motorserv.dir= $('#dirmotors').val();
      config.motorserv.port= $('#portmotors').val();
      config.motorsepname= $('#epmotors').val();
      config.pose3dserv.dir= $('#dirpose3d').val();
      config.pose3dserv.port= $('#portpose3d').val();
      config.pose3depname= $('#eppose3d').val();
      config.laserserv.dir= $('#dirlaser').val();
      config.laserserv.port= $('#portlaser').val();
      config.laserepname= $('#eplaser').val();
      config.maxv= $('#maxv').val();
      config.maxw= $('#maxw').val();
      localStorage["turtlebotviewer"]=JSON.stringify(config);
	});

   resize();
});

function load(){
   if (localStorage.getItem("turtlebotviewer")) {
       config = JSON.parse(localStorage.getItem("turtlebotviewer"));
      $('#dirright').val(config.camrightserv.dir);
      $('#portright').val(config.camrightserv.port);
      $('#epright').val(config.camrightepname);
      $('#dirleft').val(config.camleftserv.dir);
      $('#portleft').val(config.camleftserv.port);
      $('#epleft').val(config.camleftepname);
      $('#dirmotors').val(config.motorserv.dir);
      $('#portmotors').val(config.motorserv.port);
      $('#epmotors').val(config.motorsepname);
      $('#dirpose3d').val(config.pose3dserv.dir);
      $('#portpose3d').val(config.pose3dserv.port);
      $('#eppose3d').val(config.pose3depname);
      $('#dirlaser').val(config.laserserv.dir);
      $('#portlaser').val(config.laserserv.port);
      $('#eplaser').val(config.laserepname);
      $('#maxv').val(config.maxv);
      $('#maxw').val(config.maxw);

    } else{
      config.camrightserv={};
      config.motorserv={};
      config.pose3dserv={};
      config.laserserv={};
      config.camleftserv={};
      config.camrightserv.dir= $('#dirright').val();
      config.camrightserv.port= $('#portright').val();
      config.camrightepname= $('#epright').val();
      config.camleftserv.dir= $('#dirleft').val();
      config.camleftserv.port= $('#portleft').val();
      config.camleftepname= $('#epleft').val();
      config.motorserv.dir= $('#dirmotors').val();
      config.motorserv.port= $('#portmotors').val();
      config.motorsepname= $('#epmotors').val();
      config.pose3dserv.dir= $('#dirpose3d').val();
      config.pose3dserv.port= $('#portpose3d').val();
      config.pose3depname= $('#eppose3d').val();
      config.laserserv.dir= $('#dirlaser').val();
      config.laserserv.port= $('#portlaser').val();
      config.laserepname= $('#eplaser').val();
      config.maxv= $('#maxv').val();
      config.maxw= $('#maxw').val();
    }
}
