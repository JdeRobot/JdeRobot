var io = require("socket.io-client");
var bus = require("./clientBus")();
var ros = require("../..");
var mesh = require("mesh");
var c = io("http://127.0.0.1:" + (process.env.PORT || 8082));

ros(c.on.bind(c, "operation"), c.emit.bind(c, "operation"), bus);

bus(mesh.op("tail")).on("data", function(op) {
  console.log("remote:", op);
});
