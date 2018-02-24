var expect       = require("expect.js");
var _            = require("highland");
var ros          = require("../lib");
var mesh         = require("mesh");
var EventEmitter = require("events").EventEmitter;

describe(__filename + "#", function() {

  it("can create a bus", function() {
    ros(mesh.noop, function() { }, mesh.noop);
  });


  it("can return data from a remote source", function(next) {
    var em = new EventEmitter();

    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.stream(function(operation, stream) {
      stream.end("data");
    }));

    var data;

    bus(mesh.op("load")).on("data", function(d) {
      data = d;
    }).on("end", function() {
      expect(data).to.be("data");
      next();
    });
  });

  it("properly sends back an error", function(next) {
    var em = new EventEmitter();

    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.wrap(function(operation, next) {
      next(new Error("error!"));
    }));


    bus(mesh.op("load")).on("error", function(err) {
      expect(err.message).to.be("error!");
      next();
    });
  });


  it("ignores remote operations from being re-used", function(next) {
    var em = new EventEmitter();

    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.stream(function(operation, stream) {
      bus(operation).pipe(stream);
    }));

    bus(mesh.op("load")).on("end", function() {
      next();
    });
  });

  it("can flag an operation to not look for a response", function(next) {
    var em = new EventEmitter();

    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.wrap(function(operation, next) {
      next(void 0, "abba");
    }));


    var _i = 0;

    bus(mesh.op("load", { resp: false })).on("data", function() {
      _i++;
    }).on("end", function() {
      expect(_i).to.be(0);
      next();
    });
  });

  it("can run a tail on a remote operation stream", function(next) {
    var em = new EventEmitter();
    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.tailable(mesh.limit(1, mesh.wrap(function(operation, next) {
      next(void 0, operation);
    })), function() {
      return true;
    }));

    var ops;

    var tail = bus(mesh.op("tail"))
    .pipe(_.pipeline(_.collect))
    .on("data", function(data) {
      ops = data;
    });

    bus(mesh.op("insert"));
    bus(mesh.op("insert"));
    bus(mesh.op("insert"));
    bus(mesh.op("insert")).on("end", function() {
      tail.end();
      setTimeout(function() {
        expect(ops.length).to.be(4);
        next();
      }, 10);
    });
  });

  it("removes an operation that has ended", function(next) {
    var em = new EventEmitter();
    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.wrap(function(operation, next) {
      next(void 0, operation);
      next(void 0, operation);
    }));

    bus(mesh.op("load")).on("data", function() {
      next();
    });
  });

  it("can emit the same operation to multiple clients", function(next) {
    var i = 0;

    var em = new EventEmitter();
    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.wrap(function(operation, next) {
      i++;
      next(void 0, operation);
    }));

    var em2 = new EventEmitter();
    var bus2 = ros(em2.on.bind(em2, "message"), em2.emit.bind(em2, "message"), mesh.wrap(function(operation, next) {
      i++;
      next(void 0, operation);
    }));

    var bus3 = mesh.parallel(bus, bus2);

    bus3(mesh.op("load")).on("end", function() {
      expect(i).to.be(2);
      next();
    })
  });

  xit("doesn't re-emit the same incomming operation", function(next) {
    var i = 0;
    var em = new EventEmitter();
    var bus = ros(em.on.bind(em, "message"), em.emit.bind(em, "message"), mesh.wrap(function(operation, next) {
      i++;
      next(void 0, operation);
    }));

    bus = mesh.tailable(bus);
    bus(mesh.op("tail")).pipe(mesh.open(bus));


    em.emit("message", mesh.op("abba"));

    setTimeout(function() {
      expect(i).to.be(0);
      return next();
    }, 1);
  });
});
