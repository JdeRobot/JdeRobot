var API = API || {};

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
API.Camera = function (config) {
	 var conf = config || {};
   
   
   this.workerFile = "js/api_workers/camera_worker.js";
   this.w = undefined; 
   this.onmessage = undefined;
   this.isRunning = false;
   
   
   this.conPromise = undefined;
   
   this.description = undefined;
   this.data = undefined;
   
   this.imgFormat = conf.imgFormat || "RGB8";
   this.epname = conf.epname || "cameraA";
   this.server = conf.server;
   
   this.toError = undefined; // connection error message timeout
   this.timeoutE = 3000;
   
   
   
   var self = this;
   
   
   this.createWork = function () {
      this.w = new Worker(this.workerFile);
      this.w.onerror = function (err) {
         console.log ("worker error: "+ err.message);
      };
   };
   
   this.deleteWork = function () {
      if (this.w){
         this.w.terminate();
         this.w = undefined;
      }
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.connect = function () {
      this.conPromise = new Promise(
        // The resolver function is called with the ability to resolve or
        // reject the promise
        function(resolve, reject) {
            self.w.onmessage = function(mes) {
               if (mes.data){
                  self.isRunning = true;
                  resolve();
               }else{
                  self.isRunning = false;
                  reject();
               }
            };
            self.w.postMessage({func:"connect",serv:self.server,epname: self.epname}); 
        });
   };
   
   this.disconnect = function (){
      this.w.postMessage({func:"disconnect"});
      this.isRunning = false;
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.startStreaming = function(){
      this.conPromise.then(
        function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"startImageStream", imgFormat: self.imgFormat});
            self.toError=setTimeout(self.conErr, self.timeoutE);
        }); 
   };
   
   this.getImage = function(){
      this.conPromise.then(
        function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"getImage", imgFormat: self.imgFormat});
        }); 
   };
   
   this.stopStreaming = function(){  
      this.conPromise.then(
        function() {
            self.w.postMessage({func:"stopImageStream"});
            if(self.toError){
               clearTimeout(self.toError);
               toError=undefined;
            }
        });  
   };
   
   this.getDescription = function(){
      this.conPromise.then(
        function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"getCameraDescription"});
        });
   };
   
   this.onmessageDefault = function(event) {
         if (self.toError){
            clearTimeout(self.toError);
            self.toError=setTimeout(self.conErr, self.timeoutE+10);
         }
         self.data=event.data.img;
         self.description=event.data.desc || self.description;
		   if (event.data.delay){
				 var tt = new Date().getTime();
				 self.delay = event.data.delay;
				 self.delay.worker = tt - event.data.delay._ctime;
			}
      };
   
   /*
    * conErr
    * Displays an error message and closes the webworker
    */
   this.conErr=function(){
      alert ("connection to server "+self.epname+" "+self.server.dir+":"+self.server.port+" failed");
      self.deleteWork();
   };
   
   this.createWork();
   
};

/*
 * API.Motors's Constructor.
 * Params:
 *     - config (contents client's Config)={server,epname,onmessage}:
 *       + server (server's direction and port)={dir:direction,port: port}
 *       + epname (name of motor endpoint, default Motors)
 */
API.Motors = function (config){
   var conf = config || {};
   
   this.id = conf.id;
   
   this.workerFile = "js/api_workers/motors_worker.js";
   this.w = undefined; 
   this.onmessage = undefined;
   this.isRunning = false;
   
   this.conPromise = undefined;
   
   this.data=undefined;
   
   this.epname = conf.epname || "Motors";
   this.server = conf.server;
   
   var self = this;
   
   
   this.createWork = function(){
      this.w = new Worker(this.workerFile);
      this.w.onerror = function (err){
         console.log ("worker error: "+ err.message);
      };
   };
   
   this.deleteWork = function (){
      if (this.w){
         this.w.terminate();
         this.w = undefined;
      }
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.connect = function (){
      this.conPromise = new Promise(
        // The resolver function is called with the ability to resolve or
        // reject the promise
        function(resolve, reject) {
            self.w.onmessage = function(mes){
               if (mes.data){
                  self.isRunning = true;
                  resolve();
               }else{
                  self.isRunning = false;
                  reject();
               }
            };
            self.w.postMessage({func:"connect",serv:self.server,epname: self.epname}); 
        });
     
   };
   
   this.disconnect = function (){
      this.w.postMessage({func:"disconnect"});
      this.isRunning = false;
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   
   
   this.getAll = function(){
      this.conPromise.then(
         function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func: "getAll"});
         });
   };
   
   this.getV = function(){
      this.conPromise.then(
         function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func: "getV"});
         });
   };
   
   this.getW = function(){
      this.conPromise.then(
         function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func: "getW"});
         });
   };
   
   this.getL = function(){
      this.conPromise.then(
         function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func: "getL"});
         });
   };
   
   this.setAll = function(v,w,l){
      this.conPromise.then(
         function() {
            var data = {v:v,w:w,l:l};
            self.w.postMessage({func: "setAll", data: data});
         });
   };
   
   this.setV = function(v){
      this.conPromise.then(
         function() {
            self.w.postMessage({func: "setV", data: v});
         });
   };
   
   this.setW = function(w){
      this.conPromise.then(
         function() {
            self.w.postMessage({func: "setW", data: w});
         });
   };
   
   this.setL = function(l){
      this.conPromise.then(
         function() {
            self.w.postMessage({func: "setL", data: l});
         });
   };
   
     
   this.onmessageDefault = function(event) {
         self.data=event.data;
      };

   this.createWork();
};

/*
 * API.Pose3D's Constructor.
 * Params:
 *     - config (contents client's Config)={server,id,epname}:
 *       + server (server's direction and port)={dir:direction,port: port}
 *       + epname (name of pose3d endpoint, default Pose3D)
 */
API.Pose3D= function (config){
   var conf = config || {};
   
   this.workerFile = "js/api_workers/pose3d_worker.js";
   this.w = undefined; 
   this.onmessage = undefined;
   this.isRunning = false;
   
   this.conPromise = undefined;
   
   this.data = undefined;
   this.update = false;
   
   this.epname = conf.epname || "Pose3D";
   this.server = conf.server;
   
   this.toError = undefined; // connection error message timeout
   this.timeoutE = 3000;
   
   var self = this;
   
   
   this.createWork = function(){
      this.w = new Worker(this.workerFile);
      this.w.onerror = function (err){
         console.log ("worker error: "+ err.message);
      };
   };
   
   this.deleteWork = function (){
      if (this.w){
         this.w.terminate();
         this.w = undefined;
      }
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.connect = function (){
      this.conPromise = new Promise(
        // The resolver function is called with the ability to resolve or
        // reject the promise
        function(resolve, reject) {
            self.w.onmessage = function(mes){
               if (mes.data){
                  self.isRunning = true;
                  resolve();
               }else{
                  self.isRunning = false;
                  reject();
               }
            };
            self.w.postMessage({func:"connect",serv:self.server,epname: self.epname}); 
        });
   };
   
   this.disconnect = function (){
      this.w.postMessage({func:"disconnect"});
      this.isRunning = false;
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.getPose3D = function(){
      this.conPromise.then(
        function() {
           self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"getPose3D"});
        }); 
   };
   
   this.startStreaming = function(){
      this.conPromise.then(
        function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"startPose3DStream"});
            self.toError=setTimeout(self.conErr, self.timeoutE);
        }); 
   };
   
   this.stopStreaming = function(){
      this.conPromise.then(
        function() {
            self.w.postMessage({func:"stopPose3DStream"});
            if(self.toError){
               clearTimeout(self.toError);
               toError=undefined;
            }
        }); 
   };
   
   
   this.onmessageDefault = function(event) {
      if (self.toError){
         clearTimeout(this.toError);
         self.toError=setTimeout(self.conErr, self.timeoutE);
      }
      var respwork=event.data;
      self.data= respwork;
   };
   /*
    * conErr
    * Displays an error message and closes the webworker
    */
   this.conErr=function(){
      alert ("connection to server "+self.epname+" "+self.server.dir+":"+self.server.port+" failed");
     self.deleteWork();
   };
   
   this.createWork();
};

/*
 * API.Laser's Constructor.
 * Params:
 *     - config (contents client's Config)={server,id,epname}:
 *       + server (server's direction and port)={dir:direction,port: port}
 *       + epname (name of laser endpoint, default Laser)
 *       + convertUpAxis (height is represented in Y instead of Z in 3d)
 *       + scale3d (scale of laser 3d representation )
 *       + canv2dWidth (canvas' width to show laser in 2D, default 360 )
 */
API.Laser= function (config){
   var conf = config || {};
   
   this.workerFile = "js/api_workers/laser_worker.js";
   this.w = undefined; 
   this.onmessage = undefined;
   this.isRunning = false;
   
   this.conPromise = undefined;
   
   this.data = undefined;
   this.update = false;
   
   this.epname = conf.epname || "Laser";
   this.server = conf.server;
   this.canv2dWidth = conf.canv2dWidth || 360;
   this.scale3d = conf.scale3d || 1;
   this.convertUpAxis = conf.convertUpAxis || false;
   
   this.toError = undefined; // connection error message timeout
   this.timeoutE = 3000;
   
   var self = this;
   
   
   this.createWork = function(){
      this.w = new Worker(this.workerFile);
      this.w.onerror = function (err){
         console.log ("worker error: "+ err.message);
      };
   };
   
   this.deleteWork = function (){
      if (this.w){
         this.w.terminate();
         this.w = undefined;
      }
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.connect = function (){
      this.conPromise = new Promise(
        // The resolver function is called with the ability to resolve or
        // reject the promise
        function(resolve, reject) {
            self.w.onmessage = function(mes){
               if (mes.data){
                  self.isRunning = true;
                  resolve();
               }else{
                  self.isRunning = false;
                  reject();
               }
            };
            self.w.postMessage({func:"connect",serv:self.server,epname: self.epname}); 
        });
   };
   
   this.disconnect = function (){
      this.w.postMessage({func:"disconnect"});
      this.isRunning = false;
      if(self.toError){
         clearTimeout(self.toError);
         toError=undefined;
      }
   };
   
   this.getLaser = function(){
      this.conPromise.then(
        function() {
           self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"getLaser",convertUpAxis:self.convertUpAxis,canv2dWidth:self.canv2dWidth, scale3d: self.scale3d, height3d: self.height3d});
        }); 
   };
   
   this.startStreaming = function(){
      this.conPromise.then(
        function() {
            self.w.onmessage = self.onmessage || self.onmessageDefault;
            self.w.postMessage({func:"startLaserStream",convertUpAxis:self.convertUpAxis,canv2dWidth:self.canv2dWidth, scale3d: self.scale3d , height3d: self.height3d});
            self.toError=setTimeout(self.conErr, self.timeoutE);
        }); 
   };
   
   this.stopStreaming = function(){
      this.conPromise.then(
        function() {
            self.w.postMessage({func:"stopLaserStream"});
            if(self.toError){
               clearTimeout(self.toError);
               toError=undefined;
            }
        }); 
   };
   
   
   this.onmessageDefault = function(event) {
      if (self.toError){
         clearTimeout(this.toError);
         self.toError=setTimeout(self.conErr, self.timeoutE);
      }
      var respwork=event.data;
      self.data= respwork;
   };
   /*
    * conErr
    * Displays an error message and closes the webworker
    */
   this.conErr=function(){
      alert ("connection to server "+self.epname+" "+self.server.dir+":"+self.server.port+" failed");
     // self.deleteWork();
   };
   
   this.createWork();
   
};
