var cluster = require("cluster");
var ros     = require("../..");
var mesh    = require("mesh");

var bus = mesh.wrap(function(operation, next) {
  console.log("worker request:", operation);
  next(void 0, "done");
});

if (cluster.isMaster) {
  for (var i = 2; i--;) {
    var worker = cluster.fork();

    // pipe all requests to the main bus
    ros(worker.on.bind(worker, "message"), worker.send.bind(worker), bus);
  }
} else {


  bus = ros(process.on.bind(process, "message"), process.send.bind(process), mesh.noop);

  // request from master
  bus(mesh.op("hello")).on("data", function(data) {
    console.log("master response: ", data);
  });
}
