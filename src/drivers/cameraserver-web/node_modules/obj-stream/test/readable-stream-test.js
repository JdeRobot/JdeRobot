var expect = require("expect.js");
var stream = require("..");
var _      = require("highland");
var sinon  = require("sinon");

describe(__filename + "#", function() {

  it("can be created with the new keyword", function() {
    expect(new stream.Readable()).to.be.a(stream.Readable);
  });

  it("can be created without the new keyword", function() {
    expect(stream.readable()).to.be.a(stream.Readable);
  });

  it("emits 'pause' when paused", function(next) {
    var readable = new stream.Readable();
    readable.on("pause", next);
    readable.pause();
    readable.pause();
  });

  it("returns isPaused TRUE if paused", function() {
    var readable = new stream.Readable();
    expect(readable.isPaused()).to.be(false);
    readable.pause();
    expect(readable.isPaused()).to.be(true);
  });

  it("emits 'resume' when paused", function(next) {
    var readable = new stream.Readable();
    readable.on("resume", next);
    readable.pause();
    readable.resume();
  });

  it("emits pipe on the dest when pipe() is called", function(next) {
    var dest = _();
    var reader = stream.readable();
    dest.on("pipe", function(src) {
      expect(src).to.be(reader);
      next();
    });
    reader.pipe(dest);
  });

  it("ends the piped stream when 'end' is emitted from the src", function(next) {
    var readable = stream.readable();
    var items = [];

    readable.pipe(_()).on("data", function() { }).on("end", next);

    readable.emit("end");
  });

  it("ends the piped stream when 'close' is emitted from the src", function(next) {
    var readable = stream.readable();
    var items = [];
    readable.pipe(_()).on("data", function() { }).on("end", next);
    readable.emit("close");
  });

  it("doesn't end the dest stream if ops.end is false", function() {
    var readable = stream.readable();
    var items = [];
    var i = 0;
    readable.pipe(_(), { end: false }).on("end", function() {
      i++;
    });
    readable.emit("end");
    expect(i).to.be(0);
  });

  it("can write data to the dst", function(next) {
    var readable = stream.readable();
    var items = [];

    readable.pipe(_.pipeline(
      _.collect
    )).on("data", function(data) {
      items = data;
    }).on("end", function() {
      expect(items[0]).to.be("a");
      expect(items[1]).to.be("b");
      next();
    });

    readable.emit("data", "a");
    readable.emit("data", "b");
    readable.emit("end");
  });

  it("pauses the readable stream if the dest stream is paused", function(next) {
    var readable = stream.readable();
    var dest = _();
    dest.pause();
    readable.once("pause", next);
    readable.pipe(dest);
    readable.emit("data", "a");
  });

  it("resumes the readable stream if the dest stream is paused", function(next) {
    var readable = stream.readable();
    var dest = _();
    dest.pause();
    readable.pipe(dest);
    readable.once("resume", next);
    readable.emit("data", "a");
    dest.resume();
  });

  it("cleans up the pipe when the readable source stream is closed", function() {
    var readable = stream.readable();
    readable.pipe(_());
    expect(readable._events.end).not.to.be(void 0);
    readable.emit("close");
    expect(readable._events.end).to.be(void 0);
  });

  it("cleans up the pipe when the readable source stream is ended", function() {
    var readable = stream.readable();
    readable.pipe(_());
    expect(readable._events.end).not.to.be(void 0);
    readable.emit("end");
    expect(readable._events.end).to.be(void 0);
  });

  it("cleans up the pipe when the readable dest is closed", function() {
    var readable = stream.readable();
    var dest = _();
    readable.pipe(dest);
    expect(readable._events.end).not.to.be(void 0);
    dest.emit("close");
    expect(readable._events.end).to.be(void 0);
  });

  it("passes the error down stream", function(next) {
    var readable = stream.readable();
    readable.pipe(_.pipeline()).on("error", function(err) {
      expect(err.message).to.be("err");
      next();
    });
    readable.emit("error", new Error("err"));
  });

  it("cleans up the pipe when the src emits an error", function() {
    var readable = stream.readable();
    var dest = _();
    readable.pipe(dest).on("error", function() { });
    expect(readable._events.end).not.to.be(void 0);
    readable.emit("error", new Error("e"));
    expect(readable._events.end).to.be(void 0);
  });

  it("calls destroy on the dest when close is emitted", function() {
    var readable = stream.readable();
    var dest = _();
    var stub = sinon.stub(dest, "destroy");
    readable.pipe(dest);
    readable.emit("close");
    expect(stub.callCount).to.be(1);
  });

  it("does not write to the destination if it's not writable", function() {
    var readable = stream.readable();
    var dest = _();
    dest.writable = false;
    var stub = sinon.stub(dest, "write");
    readable.pipe(dest);
    readable.emit("data", "a");
    expect(stub.callCount).to.be(0);
  });

  it("does resume the readable stream if it's not readable", function() {
    var readable = stream.readable();
    readable.readable = false;
    var dest = _();
    dest.pause();
    var stub = sinon.stub(readable, "resume");
    readable.pipe(dest);
    readable.emit("data", "a");
    dest.resume();
    expect(stub.callCount).to.be(0);
  });
});
