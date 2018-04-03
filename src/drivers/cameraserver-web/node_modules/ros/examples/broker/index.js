
var port = process.env.PORT || 8082;

var server = require('http').createServer(function() { });
var io     = require('socket.io')(server);
var mesh   = require("mesh");
var bus    = require("./serverBus")();
var readline = require("readline");


console.log("listening on port %d", port);

io.on("connection", function(client) {
  bus(mesh.op("addClient", { client: client }));
});

server.listen(port);

var rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});


rl.on("line", function(opStr) {

  try {
    var op = (new Function("return" + opStr))();
    bus(op).on("data", function(data) {
      console.log("data: ", data);
    }).on("end", function() {
      console.log("end");
    }).on("error", function() {

    });
  } catch(e) {
    console.log(e);
    mesh.noop();
  }

  rl.prompt();
});

rl.prompt();
