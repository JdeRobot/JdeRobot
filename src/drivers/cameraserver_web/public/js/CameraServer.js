function CameraServer (conf) {
  var server = undefined;
  this.config = conf;

  this.start= function(){
    if (server == undefined){
      server = new API.CameraServer(this.config);
      server.connect();
    }
    server.startStreaming();
  }

  this.restart = function(){
    server.restart();
    server = undefined;
    server = new API.CameraServer(this.config);
    server.connect();
    server.startStreaming();
  }
}
