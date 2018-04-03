var protoclass   = require("protoclass");
var EventEmitter = require("events").EventEmitter;
var Reader       = require("./readable");

/**
 */

function Writable () {
  if (!(this instanceof Writable)) return new Writable();
  EventEmitter.call(this);

  this._pool  = [];
  this.reader = new Reader();

  var self = this;

  this.reader.on("pause", function() {
    self._pause();
  });

  this.reader.on("resume", function() {
    self._resume();
  });

}

/**
 */

protoclass(EventEmitter, Writable, {

  /**
   */

  _flowing: true,
  readable: false,
  writable: true,

  /**
   */

  write: function(object) {
    if (!this._write(object)) {
      this._pool.push(object);
      return false;
    }
    return true;
  },

  /**
   */

  end: function(object) {

    this._ended = true;

    if (object != void 0) {
      this.write(object);
    }

    if (this._flowing) {
      this.reader.emit("end");
    }
  },

  /**
   */

  _write: function(object) {
    if (this._flowing) {
      this.reader.emit("data", object);

      // might have changed on emit
      return this._flowing;
    } else {
      return false;
    }
  },

  /**
   */

  _pause: function() {
    this._flowing = false;
  },

  /**
   */

  _resume: function() {
    if (this._flowing) return;
    this._flowing = true;
    this.reader.emit("drain");

    while (this._pool.length) {
      var item = this._pool.shift();
      if (!this._write(item)) {
        this._pool.unshift(item);
        break;
      }
    }

    if (!this._pool.length && this._ended) {
      this.end();
    }
  }
});

module.exports = Writable;
