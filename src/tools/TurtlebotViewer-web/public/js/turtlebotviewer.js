/*
 * TurtlebotViewer's Constructor.
 * Params:
 *     - config (contents client's Config)={serv,camid,fpsid,modelid}:
 *       + camleftserv (server's direction and port)={dir:direction,port: port}
 *       + camleftid (canvas' id to show cam left)
 *       + camleftepname (name of camera left endpoint, default cam_sensor_left)
 *       + camrightserv (server's direction and port)={dir:direction,port: port}
 *       + camrightid (canvas' id to show cam right)
 *       + camrightepname (name of camera right endpoint, default cam_sensor_right)
 *       + motorserv (server's direction and port)={dir:direction,port: port}
 *       + motorsepname (name of Motors endpoint, default Motors)
 *       + controlid (canvas' id to show controls)
 *       + modelid (canvas' id to show model of robot)
 *       + stopbtnid (button to stop robot)
 *       + pose3dserv (server's direction and port)={dir:direction,port: port}
 *       + pose3depname (name of Pose3D endpoint, default Pose3D)
 *       + laserserv (server's direction and port)={dir:direction,port: port}
 *       + laserepname (name of Laser endpoint, default Laser)
 */
function TurtlebotViewer (config){
   /*************************
    **** Public objects  ****
    *************************/
   this.motorserv=config.motorserv;
   this.camleftserv=config.camleftserv;
   this.camrightserv=config.camrightserv;
   this.pose3dserv=config.pose3dserv;
   this.laserserv=config.laserserv;

   this.camleftid=config.camleftid;
   this.camrightid=config.camrightid;
   this.controlid=config.controlid;
   this.modelid=config.modelid;
   this.stopbtnid=config.stopbtnid;

   this.camleftepname=config.camleftepname || "cam_sensor_left";
   this.camrightepname= config.camrightepname || "cam_sensor_right";
   this.motorsepname = config.motorsepname || "Motors";
   this.pose3depname = config.pose3depname || "Pose3D";
   this.laserepname = config.laserepname || "Laser";

   this.maxv=config.maxv;
   this.maxw=config.maxw;








   /*************************
    **** Private objects ****
    *************************/
   var control=undefined;

   var model = {id: this.modelid,
                container: $('#'+this.modelid),
                WIDTH: 320,
                HEIGHT: 320,
                VIEW_ANGLE: 50,
                NEAR: 0.1,
                FAR: 5000,
                robot:undefined,
                renderer:undefined,
                camera: undefined,
                controls: undefined,
                laser:undefined,
                animation: undefined,
                active: false,
   };
   model.ASPECT=model.WIDTH / model.HEIGHT;

   var lasercanv = undefined;

   var motors;
   var cameraleft,cameraright;
   var pose3d;
   var laser;
   var timeout = 10000;

   var lastV=0, lastW=0;

   var self=this;



   /*************************
    **** Private methods ****
    *************************/

   var onGetMotors= function(event){
      console.log(event.data);
   };

   var calcDist= function (num1, num2){
         var max = Math.max(num1,num2);
         var min = Math.min(num1,num2);

         return max-min;
      }


   /*
    * drawCamera
    * Draw the cloud of points received from the server
    */
   var drawCamera = function (data,canvas){
      //camera
      var canvas2 = document.createElement('canvas');
      var ctx2=canvas2.getContext("2d");
      var imgData=ctx2.getImageData(0,0,data.width,data.height);
      ctx2.canvas.width=data.width;
      ctx2.canvas.height=data.height;
      var ctx=canvas.getContext("2d");
      imgData.data.set(data.imgData);
      ctx2.putImageData(imgData,0,0);
      ctx.drawImage(canvas2, 0, 0,ctx.canvas.width,ctx.canvas.height);
   };

   var stopbot = function(){
      control.removeListeners();
      initControl();
      lastV = 0;
      lastW = 0;
      motors.setV(0);
      motors.setW(0);
	  };

   /*********** WebGL ******/
   var initControl = function (){
      control = new GUI.Control ({id:self.controlid});
      maxv=this.maxv;
      maxw=this.maxw;

      lastW=0;
      lastV=0;

      control.onPointerM = function (event){
         control.onPointerMDefault(event);
         var distSend = 2;
         var pos = control.position;


         if (calcDist(pos.x,lastW)>=distSend){
            motors.setW(pos.x*self.maxw/control.WIDTH);
            lastW = pos.x;
         }

         if (calcDist(pos.z,lastV)>=distSend){
            motors.setV(pos.z*self.maxv/control.HEIGHT);
            lastV = pos.z;
         }
      };

      control.initControl();
   };

    var initModel = function (){
      var canv = document.getElementById(model.id);
      model.ASPECT=canv.width/canv.height;
      model.camera = new THREE.PerspectiveCamera(model.VIEW_ANGLE, model.ASPECT, model.NEAR, model.FAR);
      model.camera.position.set( -5, 5, 2 );

      model.camera.lookAt(new THREE.Vector3( 0,0,0 ));

      model.renderer = new THREE.WebGLRenderer({canvas:canv, antialias: true});
      model.renderer.setPixelRatio( window.devicePixelRatio );
      model.renderer.setClearColor( 0xffffff);

      model.scene=new THREE.Scene();

      //controls
      model.controls = new THREE.TrackballControls( model.camera );

      model.controls.rotateSpeed = 2.0;
      model.controls.zoomSpeed = 2.2;
      model.controls.panSpeed = 1.8;

      model.controls.noZoom = false;
      model.controls.noPan = false;

      model.controls.staticMoving = true;
      model.controls.dynamicDampingFactor = 0.3;
      model.controls.enabled=false;

      model.controls.keys = [ 65, 83, 68 ];

      canv.addEventListener("mouseover", function(){
            model.controls.enabled=true;
      });

      canv.addEventListener("mouseout", function(){
            model.controls.enabled=false;
      });

      //lights
      var light = new THREE.HemisphereLight( 0xffffbb, 0x080820, 1 );
      light.position.set( 0, 500, 0 );
      model.scene.add(light);

      // Grid
      var size = 25;
      var step = 1;

      var gridHelper = new THREE.GridHelper( size, step );
      model.scene.add( gridHelper );

      var axisHelper = new THREE.AxisHelper( 2 );
      model.scene.add( axisHelper );

      //ground
      var groundMat = new THREE.MeshBasicMaterial({color:0xd8d8d8});
      var groundGeo = new THREE.PlaneGeometry(50,50);
      var ground = new THREE.Mesh(groundGeo,groundMat);
      ground.position.y = -0.05; //lower it
      ground.rotation.x = -Math.PI/2; //-90 degrees around the xaxis

      model.scene.add(ground);



      var modelAnimation = function(){
         model.animation = requestAnimationFrame(modelAnimation);
         model.controls.update();
         model.renderer.render(model.scene,model.camera);

      };

      if (model.robot==undefined) {
         var loader = new GUI.RobotLoader();
         loader.loadTurtlebot(1,function () {
            model.robot=loader.robot;
            model.scene.add( model.robot );
            modelAnimation();
	     });
      } else {
         model.scene.add( model.robot );
         modelAnimation();
      }

   };

   /*
    * initGL
    * Prepare reconstruction with webgl
    */
   var initGL = function(){
      initControl();
       if (self.modelid && model.active){
         initModel();
      }
   }

   /*************************
    ** Privileged methods ***
    *************************/

   this.setConfig = function(conf){



      this.motorserv=conf.motorserv || this.motorserv;
      this.camleftserv=conf.camleftserv || this.camleftserv;
      this.camrightserv=conf.camrightserv || this.camrightserv;
      this.pose3dserv=conf.pose3dserv || this.pose3dserv;
      this.laserserv=conf.laserserv || this.laserserv;

      this.camleftid=conf.camleftid || this.camleftid;
      this.camrightid=conf.camrightid || this.camrightid;
      this.controlid=conf.controlid || this.controlid;
      this.modelid=conf.modelid || this.modelid;
      this.stopbtnid=conf.stopbtnid || this.stopbtnid;

      this.camleftepname=conf.camleftepname || this.camleftepname;
      this.camrightepname= conf.camrightepname || this.camrightepname;
      this.motorsepname = conf.motorsepname || this.motorsepname;
      this.pose3depname = conf.pose3depname || this.pose3depname;
      this.laserepname = conf.laserepname || this.laserepname;

      this.maxv=conf.maxv || this.maxv;
      this.maxw=conf.maxw || this.maxw;

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
      if (!this.motorserv||!this.motorserv.dir||!this.motorserv.port) {
         alert("The motorserv's data are not well defined");
         return;
      }
      if (!this.camleftserv||!this.camleftserv.dir||!this.camleftserv.port) {
         alert("The camleftserv's data are not well defined");
         return;
      }
      if (!this.camrightserv||!this.camrightserv.dir||!this.camrightserv.port) {
         alert("The camrightserv's data are not well defined");
         return;
      }
      if (!this.pose3dserv||!this.pose3dserv.dir||!this.pose3dserv.port) {
         alert("The pose3dserv's data are not well defined");
         return;
      }
      if (!this.camleftid) {
         alert("Not defined camleftid");
         return;
      }
      if (!this.camrightid) {
         alert("Not defined camrightid");
         return;
      }
      if (!this.controlid) {
         alert("Not defined controlid");
         return;
      }
      if (!this.modelid) {
         alert("Not defined modelid");
         return;
      }

      lasercanv = document.getElementById("laser");
      lasercanv.height = lasercanv.width/2;

      laser= new API.Laser({server:self.laserserv,epname:self.laserepname,canv2dWidth:lasercanv.width,scale3d:0.001,convertUpAxis:true});
      laser.onmessage= function (event){
         laser.onmessageDefault(event);
         //2D
         var dist = laser.data.canv2dData;
         var ctx = lasercanv.getContext("2d");
         ctx.beginPath();
         ctx.clearRect(0,0,lasercanv.width,lasercanv.height);
         ctx.fillRect(0,0,lasercanv.width,lasercanv.height);
         ctx.strokeStyle="white";
         ctx.moveTo(dist[0], dist[1]);
         for (var i = 2;i<dist.length; i = i+2 ){
            ctx.lineTo(dist[i], dist[i+1]);
         }
         ctx.moveTo(lasercanv.width/2, lasercanv.height);
         ctx.lineTo(lasercanv.width/2, lasercanv.height-10);
         ctx.stroke();

         //3D
         if (model.active){
            var geometry = new THREE.BufferGeometry();
            geometry.addAttribute( 'position', new THREE.BufferAttribute( new Float32Array(laser.data.array3dData), 3 ) );
            var material = new THREE.MeshBasicMaterial( { color: 0x00ff00 } );
            material.transparent = true;
            material.opacity=0.5;
            material.side = THREE.DoubleSide;
            var las = new THREE.Mesh( geometry, material );
            if (model.laser){
               model.robot.remove(model.laser);
            };
            model.robot.add(las);
            model.laser = las;
         }
      };
      laser.connect();
      laser.startStreaming();

      pose3d = new API.Pose3D({server:self.pose3dserv,epname:self.pose3depname});
      pose3d.onmessage = function(event) {
         pose3d.onmessageDefault(event);
         if (model.active) {
            model.robot.position.set(pose3d.data.x,pose3d.data.z,-pose3d.data.y);
            model.robot.rotation.y=(pose3d.data.yaw);
            model.robot.updateMatrix();
         }
      };
      pose3d.timeoutE=timeout;
      pose3d.connect();
      pose3d.startStreaming();

      motors= new API.Motors({server:this.motorserv,onmessage:onGetMotors,epname:this.motorsepname});
      cameraleft = new API.Camera ({server:this.camleftserv,epname:this.camleftepname});
      cameraleft.canvas = document.getElementById(self.camleftid);
      cameraleft.onmessage= function (event){
            cameraleft.onmessageDefault(event);
            drawCamera(cameraleft.data,cameraleft.canvas);
      };
      cameraright = new API.Camera({server:this.camrightserv,id:this.camrightid,epname: this.camrightepname});
      cameraright.canvas = document.getElementById(self.camrightid);
      cameraright.onmessage= function (event){
            cameraright.onmessageDefault(event);
            drawCamera(cameraright.data,cameraright.canvas);
      };
      cameraleft.timeoutE=timeout;
      cameraright.timeoutE=timeout;

      $('#'+this.stopbtnid).on('click', stopbot);

      motors.connect();
      cameraleft.connect();
      cameraright.connect();
      cameraleft.startStreaming();
      cameraright.startStreaming();
      initGL();
   };

   this.stop = function(){
      laser.deleteWork();
      motors.deleteWork();
      pose3d.deleteWork();
      cameraleft.deleteWork();
      cameraright.deleteWork();

   };

   /*
    * isrunning
    * Returns a boolean indicating if the client is running
    */
   this.isRunning= function () {
      return motors.isRunning || cameraleft.isRunning || cameraright.isRunning || pose3d.isRunning || laser.isRunning;
   };

   this.resizeCameraModel= function(){
      if (model.active) {
         model.camera.aspect = model.renderer.domElement.width / model.renderer.domElement.height;
         model.camera.updateProjectionMatrix();
         model.renderer.render(model.scene,model.camera);
      }

   };

    this.modelON = function() {
      model.active = true;
      if (self.modelid && !model.renderer){
         initModel();
      }
   };

   this.modelOFF = function() {

      model.active = false;
      if (self.modelid && model.renderer){
         cancelAnimationFrame(model.animation);// Stop the animation

         model.scene = null;
         model.renderer = undefined;
         model.camera = undefined;
         model.controls = undefined;
         var m = model.container.clone();
         var parent = model.container.parent();
         model.container.remove();
         parent.append(m);
         model.container = m;

      }
   };
}
