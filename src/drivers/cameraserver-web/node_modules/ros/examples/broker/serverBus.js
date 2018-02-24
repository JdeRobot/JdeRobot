var mesh = require("mesh");
var ros  = require("../..");
var sift = require("sift");

module.exports = function() {
  var bus = mesh.noop;
  bus = clients(bus);
  return bus;
}

function clients(bus) {

  var clients = [];

  return mesh.fallback(

    mesh.accept("addClient", mesh.wrap(function(operation, next) {
      var c = operation.client;

      console.log("add client");

      var cbus = ros(c.on.bind(c, "operation"), c.emit.bind(c, "operation"), bus);

      clients.push(cbus);

      c.once("disconnect", function() {
        console.log("remove client");
        clients.splice(clients.indexOf(cbus), 1);
      });

      next(void 0, true);
    })),


    broker(clients)
  );
}

function broker(clients) {
  return mesh.fallback(

    // fanout
    mesh.accept(
      sift({ type: "fanout" }),
      mesh.parallel(clients)
    ),

    // fanout
    mesh.accept(
      sift({ type: "sequence" }),
      mesh.sequence(clients)
    ),

    mesh.accept(
      sift({ type: "race" }),
      mesh.race(clients)
    ),

    mesh.accept(
      sift({ type: "fallback" }),
      mesh.fallback(clients)
    ),

    mesh.accept(
      sift({ type: "random" }),
      random(clients)
    ),

    mesh.accept(
      sift({ type: "roundrobin" }),
      roundrobin(clients)
    )
  )
}

function roundrobin(busses) {
  var _i = 0;
  return function(operation) {
    return busses[_i++ % busses.length](operation);
  };
}

function random(busses) {
  var _i = 0;
  return function(operation) {
    return busses[Math.floor(Math.random() * busses.length)](operation);
  };
}

// function retry(count, bus) {
//   return mesh.stream(function(operation, stream) {
//     var i = count;
//     var err;
//
//     function run() {
//       if (i-- < 0) return
//     }
//
//     bus(operation)
//     .on("error", run)
//     .on("data", stream.write.bind(stream))
//     .on("")
//   });
// }
