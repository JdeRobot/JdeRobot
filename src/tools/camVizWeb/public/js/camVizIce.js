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
function CameraViewIce (config){
   /*************************
    **** Public objects  ****
    *************************/
   this.serv=config.serv;
   this.camid=config.camid;
   this.fpsid=config.fpsid;
   this.sizeid = config.sizeid;
   this.endpointname=config.epname || "cameraA";

   /*************************
    **** Private objects ****
    *************************/
   var self=this;
   var camera = undefined;

   var canvas = undefined;
   var fps = undefined;



   /*************************
    ** Privileged methods ***
    *************************/

   this.setConfig = function(conf){
      this.serv=conf.serv || this.serv;
      this.camid=conf.camid || this.camid;
      this.fpsid=conf.fpsid || this.fpsid;
      this.sizeid= conf.sizeid || this.sizeid;
      this.endpointname=conf.epname || this.epname;
   };

   /*
    * start
    * run client
    */
   this.start= function(){
      //worker, serv, camid checks
      if (!window.Worker) {
         alert("This application does not work in this browser");
         return;
      }
      if (!this.serv||!this.serv.dir||!this.serv.port) {
         alert("The server's data are not well defined");
         return;
      }
      if (!this.camid) {
         alert("Not defined where the image (camid)");
         return;
      }
      if (!this.fpsid) {
         alert("Not defined where the FPS (fpsid)");
         return;
      }
      canvas = document.getElementById(self.camid);

      fps = $('#'+this.fpsid);
      camera = new API.CameraIce({server:this.serv,epname:this.endpointname,imgFormat:"RGB8"});
      camera.onmessage = function (event){
         camera.onmessageDefault(event);
         var respwork = camera.data;
         //camera
         var canvas2 = document.createElement('canvas');
         var ctx2=canvas2.getContext("2d");
         var imgData=ctx2.getImageData(0,0,respwork.width,respwork.height);
         ctx2.canvas.width=respwork.width;
         ctx2.canvas.height=respwork.height;
         var ctx=canvas.getContext("2d");
         imgData.data.set(respwork.imgData);
         ctx2.putImageData(imgData,0,0);
         ctx.drawImage(canvas2, 0, 0,ctx.canvas.width,ctx.canvas.height);
         if (self.sizeid){
            $("#"+self.sizeid).html(respwork.width+"x"+respwork.height);
         }
         //FPS
         if (respwork.fps){
            fps.html(Math.floor(respwork.fps));
         }
      };
      camera.connect();
      camera.startStreaming();
   };


   /*
    * stop
    * stop client
    */
   this.stop= function () {
      camera.deleteWork();

   };
   /*
    * isrunning
    * Returns a boolean indicating if the client is running
    */
   this.isrunning= function () {
      return camera.isRunning;
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
