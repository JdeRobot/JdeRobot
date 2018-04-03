var expect = require("expect.js");
var stream = require("..");
var sinon  = require("sinon");
var _      = require("highland");

describe(__filename + "#", function() {

  it("can be created with the new keyword", function() {
    expect(new stream.Writable()).to.be.a(stream.Writable);
  });

  it("can be created without the new keyword", function() {
    expect(stream.writable()).to.be.a(stream.Writable);
  });

  it("has a readable property", function() {
    expect(stream.writable().reader).to.be.a(stream.Readable);
  });

  it("pauses the writer when the reader is paused", function() {
    var writable = stream.writable();
    var stub = sinon.stub(writable, "_pause");
    writable.reader.pause();
    expect(stub.callCount).to.be(1);
  });

  it("resumes the writer when the reader is resumed", function() {
    var writable = stream.writable();
    var stub = sinon.stub(writable, "_resume");
    writable.reader.pause();
    expect(stub.callCount).to.be(0);
    writable.reader.resume();
    expect(stub.callCount).to.be(1);
  });

  it("buffers written data if the writable is paused", function() {
    var writable = stream.writable();
    writable.reader.pause();
    expect(writable._pool.length).to.be(0);
    writable.write({ name: "a" });
    expect(writable._pool.length).to.be(1);
  });

  it("emits drain wen the stream is resumed", function(next) {
    var writable = stream.writable();
    writable.reader.pause();
    writable.reader.once("drain", next);
    writable.reader.resume();
  });

  it("emits pooled data after resuming", function() {
    var items = [];
    var writable = stream.writable();
    writable.reader.pause();
    writable.write("data", "a");
    writable.write("data", "a");
    writable.write("data", "b");
    writable.reader.on("data", function(item) {
      items.push(item);
    });
    writable.reader.resume();
    expect(items.length).to.be(3);
  });

  it("emits end when end is called", function(next) {
    var writable = stream.writable();
    writable.reader.once("end", next);
    writable.end();
  });

  it("can pass data into end", function(next) {
    var writable = stream.writable();
    writable.reader.once("data", function() { next(); });
    writable.end("a");
  });

  it("only ends when flowing", function() {
    var writable = stream.writable();
    writable.reader.pause();
    var i = 0;
    writable.reader.on("end", function() { i++; });
    writable.write("a");
    writable.end();
    expect(i).to.be(0);
    writable.reader.resume();
    expect(i).to.be(1);
  });

  it("can collect items", function(next) {
    var writable = stream.writable();
    writable.reader.pipe(_.pipeline(_.collect)).on("data", function(items) {
      expect(items.length).to.be(3);
      next();
    });
    writable.write("a");
    writable.write("a");
    writable.end("a");
  });

  it("can be piped to", function(next) {
    var a = stream.writable();
    var b = stream.writable();
    a.reader.pipe(b).reader.on("data", function(data) {
      expect(data).to.be(1);
      next();
    });

    a.write(1);
  });
});
