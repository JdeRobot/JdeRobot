/*
 * Cameraview's Constructor.
 * Params:
 *     - config (contents client's Config)={serv,camid,fpsid}:
 *       + serv (server's direction and port)={dir:direction,port: port}
 *       + camid (canvas' id to show RGB camera)
 *       + camepname (name of camera endpoint, default cameraA)
 *       + fpsid (id of element to show fps, is optional)
 *       + sizeid (id to show image size)
 */
function CameraViewRos (config){
   /*************************
    **** Public objects  ****
    *************************/
   this.conf= config;


   /*************************
    **** Private objects ****
    *************************/
   var self=this;
   var camera = undefined;

   var canvas = undefined;
   var fps = $("#" + self.conf.fpsid);;
   var size = $("#" + self.conf.sizeid);

   /*
    * start
    * run client
    */
   this.start= function(){
     if (camera == undefined){
       camera = new API.CameraRos(this.conf);
       camera.connect();
     }
     camera.startStreaming();
   };


   /*
    * stop
    * stop client
    */
   this.stop= function () {
     camera.stop();
     camera.disconnect();
     camera = undefined;
   };

   /*
    * restart
    * stops and start client
    */
   this.restart= function () {
      stop();
      start();
   };
}
