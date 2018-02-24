var expect = require("expect.js");
var stream = require("..");
var sinon  = require("sinon");

describe(__filename + "#", function() {

  it("can be created with the new keyword", function() {
    expect(new stream.Stream()).to.be.a(stream.Stream);
  });

  it("can be created without the new keyword", function() {
    expect(stream.Stream()).to.be.a(stream.Stream);
  });

  it("can pipe data do another stream", function(next) {
    var s1 = stream.stream();
    var s2 = stream.stream();
    var items = [];

    s1.pipe(s2).on("data", items.push.bind(items)).on("end", function() {
      expect(items.length).to.be(2);
      next();
    });

    s1.write("a");
    s1.end("b");
  });

  it("can pause downstream", function() {
    var s1 = stream.stream();
    var s2 = stream.stream();
    var s3 = stream.stream();
    s1.pipe(s2).pipe(s3);
    s3.pause();
    s1.write("a");
    expect(s1._writer._flowing).to.be(false);
  });

  it("waits for all pipes to resume before emitting data", function() {
    var s1 = stream.stream();
    var s2 = stream.stream();
    var s3 = stream.stream();

    s1.pipe(s2);
    s1.pipe(s3);

    s3.pause();
    s2.pause();
    s1.write("a");
    expect(s1._writer._flowing).to.be(false);
    s3.resume();
    expect(s1._writer._flowing).to.be(false);
    s2.resume();
    expect(s1._writer._flowing).to.be(true);
  });
  
});
