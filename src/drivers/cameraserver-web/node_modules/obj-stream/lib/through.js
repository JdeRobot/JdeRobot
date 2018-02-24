var protoclass = require("protoclass");
var Readable   = require("./readable");
var Stream     = require("./stream");
var Writable   = require("./writable");

/**
 */

function Through (stream) {
  this._stream = stream;
  this._reader = stream.reader;
}

/**
 */

protoclass(Through, {
  push: function(object) {

    if (!this._reader._events.data) {
      this._waitForListener();
    }

    this._stream.write(object);
  },

  /**
   */

  _waitForListener: function() {
    if (this._waiting) return;
    this._waiting = true;

    var self = this;
    function onListener(name) {
      if (name === "data") {
        self._reader.removeListener("newListener", onListener);
        process.nextTick(function() {
          self._reader.resume();
        });
      }
    }

    this._reader.on("newListener", onListener);
    this._reader.pause();
  }
});

/**
 */

module.exports = function(write, end) {

  var dstWriter = new Writable();
  var srcWriter = new Writable();
  var stream    = new Stream(dstWriter.reader, srcWriter);
  var through   = new Through(dstWriter);

  var buffer  = [];
  var running = false;
  var ended   = false;

  function _write() {
    if (running) return;

    if (buffer.length) {
      running = true;
      return write.call(through, buffer.shift(), function() {
        running = false;
        _write();
      });
    }

    if (ended) {
      if (end) end.call(through);
      dstWriter.end();
    }
  }

  srcWriter.reader.on("data", function(data) {
    buffer.push(data);
    _write();
  }).on("end", function() {
    ended = true;
    _write();
  });

  return stream;
};
