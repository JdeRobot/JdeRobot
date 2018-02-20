var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

var Ice = require("../Ice/Long").Ice;
var Long = Ice.Long;

var bufferOverflowExceptionMsg = "BufferOverflowException";
var bufferUnderflowExceptionMsg = "BufferUnderflowException";
var indexOutOfBoundsExceptionMsg = "IndexOutOfBoundsException";

//
// Buffer implementation to be used by web browsers, it uses ArrayBuffer as
// the store.
//

var Buffer = function () {
    function Buffer(buffer) {
        _classCallCheck(this, Buffer);

        if (buffer !== undefined) {
            this.b = buffer;
            this.v = new DataView(this.b);
        } else {
            this.b = null; // ArrayBuffer
            this.v = null; // DataView
        }
        this._position = 0;
        this._limit = 0;
        this._shrinkCounter = 0;
    }

    _createClass(Buffer, [{
        key: "empty",
        value: function empty() {
            return this._limit === 0;
        }
    }, {
        key: "resize",
        value: function resize(n) {
            if (n === 0) {
                this.clear();
            } else if (n > this.capacity) {
                this.reserve(n);
            }
            this._limit = n;
        }
    }, {
        key: "clear",
        value: function clear() {
            this.b = null;
            this.v = null;
            this._position = 0;
            this._limit = 0;
        }

        //
        // Call expand(n) to add room for n additional bytes. Note that expand()
        // examines the current position of the buffer first; we don't want to
        // expand the buffer if the caller is writing to a location that is
        // already in the buffer.
        //

    }, {
        key: "expand",
        value: function expand(n) {
            var sz = this.capacity === 0 ? n : this._position + n;
            if (sz > this._limit) {
                this.resize(sz);
            }
        }
    }, {
        key: "reset",
        value: function reset() {
            if (this._limit > 0 && this._limit * 2 < this.capacity) {
                //
                // If the current buffer size is smaller than the
                // buffer capacity, we shrink the buffer memory to the
                // current size. This is to avoid holding on to too much
                // memory if it's not needed anymore.
                //
                if (++this._shrinkCounter > 2) {
                    this.reserve(this._limit);
                    this._shrinkCounter = 0;
                }
            } else {
                this._shrinkCounter = 0;
            }
            this._limit = 0;
            this._position = 0;
        }
    }, {
        key: "reserve",
        value: function reserve(n) {
            if (n > this.capacity) {
                var capacity = Math.max(n, 2 * this.capacity);
                capacity = Math.max(1024, capacity);
                if (!this.b) {
                    this.b = new ArrayBuffer(capacity);
                } else {
                    var b = new Uint8Array(capacity);
                    b.set(new Uint8Array(this.b));
                    this.b = b.buffer;
                }
                this.v = new DataView(this.b);
            } else if (n < this.capacity) {
                this.b = this.b.slice(0, this.capacity);
                this.v = new DataView(this.b);
            } else {
                return;
            }
        }
    }, {
        key: "put",
        value: function put(v) {
            if (this._position === this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            this.v.setUint8(this._position, v);
            this._position++;
        }
    }, {
        key: "putAt",
        value: function putAt(i, v) {
            if (i >= this._limit) {
                throw new Error(indexOutOfBoundsExceptionMsg);
            }
            this.v.setUint8(i, v);
        }
    }, {
        key: "putArray",
        value: function putArray(v) {
            //Expects an Uint8Array
            if (!(v instanceof Uint8Array)) {
                throw new TypeError('argument is not a Uint8Array');
            }
            if (v.byteLength > 0) {
                if (this._position + v.length > this._limit) {
                    throw new Error(bufferOverflowExceptionMsg);
                }
                new Uint8Array(this.b, 0, this.b.byteLength).set(v, this._position);
                this._position += v.byteLength;
            }
        }
    }, {
        key: "putShort",
        value: function putShort(v) {
            if (this._position + 2 > this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            this.v.setInt16(this._position, v, true);
            this._position += 2;
        }
    }, {
        key: "putInt",
        value: function putInt(v) {
            if (this._position + 4 > this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            this.v.setInt32(this._position, v, true);
            this._position += 4;
        }
    }, {
        key: "putIntAt",
        value: function putIntAt(i, v) {
            if (i + 4 > this._limit || i < 0) {
                throw new Error(indexOutOfBoundsExceptionMsg);
            }
            this.v.setInt32(i, v, true);
        }
    }, {
        key: "putFloat",
        value: function putFloat(v) {
            if (this._position + 4 > this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            this.v.setFloat32(this._position, v, true);
            this._position += 4;
        }
    }, {
        key: "putDouble",
        value: function putDouble(v) {
            if (this._position + 8 > this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            this.v.setFloat64(this._position, v, true);
            this._position += 8;
        }
    }, {
        key: "putLong",
        value: function putLong(v) {
            if (this._position + 8 > this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            this.v.setInt32(this._position, v.low, true);
            this._position += 4;
            this.v.setInt32(this._position, v.high, true);
            this._position += 4;
        }
    }, {
        key: "writeString",
        value: function writeString(stream, v) {
            //
            // Encode the string as utf8
            //
            var encoded = unescape(encodeURIComponent(v));

            stream.writeSize(encoded.length);
            stream.expand(encoded.length);
            this.putString(encoded, encoded.length);
        }
    }, {
        key: "putString",
        value: function putString(v, sz) {
            if (this._position + sz > this._limit) {
                throw new Error(bufferOverflowExceptionMsg);
            }
            for (var i = 0; i < sz; ++i) {
                this.v.setUint8(this._position, v.charCodeAt(i));
                this._position++;
            }
        }
    }, {
        key: "get",
        value: function get() {
            if (this._position >= this._limit) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var v = this.v.getUint8(this._position);
            this._position++;
            return v;
        }
    }, {
        key: "getAt",
        value: function getAt(i) {
            if (i < 0 || i >= this._limit) {
                throw new Error(indexOutOfBoundsExceptionMsg);
            }
            return this.v.getUint8(i);
        }
    }, {
        key: "getArray",
        value: function getArray(length) {
            if (this._position + length > this._limit) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var buffer = this.b.slice(this._position, this._position + length);
            this._position += length;
            return new Uint8Array(buffer);
        }
    }, {
        key: "getArrayAt",
        value: function getArrayAt(position, length) {
            if (position + length > this._limit) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            length = length === undefined ? this.b.byteLength - position : length;
            return new Uint8Array(this.b.slice(position, position + length));
        }
    }, {
        key: "getShort",
        value: function getShort() {
            if (this._limit - this._position < 2) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var v = this.v.getInt16(this._position, true);
            this._position += 2;
            return v;
        }
    }, {
        key: "getInt",
        value: function getInt() {
            if (this._limit - this._position < 4) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var v = this.v.getInt32(this._position, true);
            this._position += 4;
            return v;
        }
    }, {
        key: "getFloat",
        value: function getFloat() {
            if (this._limit - this._position < 4) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var v = this.v.getFloat32(this._position, true);
            this._position += 4;
            return v;
        }
    }, {
        key: "getDouble",
        value: function getDouble() {
            if (this._limit - this._position < 8) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var v = this.v.getFloat64(this._position, true);
            this._position += 8;
            return v;
        }
    }, {
        key: "getLong",
        value: function getLong() {
            if (this._limit - this._position < 8) {
                throw new Error(bufferUnderflowExceptionMsg);
            }
            var low = this.v.getUint32(this._position, true);
            this._position += 4;
            var high = this.v.getUint32(this._position, true);
            this._position += 4;

            return new Long(high, low);
        }
    }, {
        key: "getString",
        value: function getString(length) {
            if (this._position + length > this._limit) {
                throw new Error(bufferUnderflowExceptionMsg);
            }

            var data = new DataView(this.b, this._position, length);
            var s = "";

            for (var i = 0; i < length; ++i) {
                s += String.fromCharCode(data.getUint8(i));
            }
            this._position += length;
            s = decodeURIComponent(escape(s));
            return s;
        }
    }, {
        key: "position",
        get: function get() {
            return this._position;
        },
        set: function set(value) {
            if (value >= 0 && value <= this._limit) {
                this._position = value;
            }
        }
    }, {
        key: "limit",
        get: function get() {
            return this._limit;
        },
        set: function set(value) {
            if (value <= this.capacity) {
                this._limit = value;
                if (this._position > value) {
                    this._position = value;
                }
            }
        }
    }, {
        key: "capacity",
        get: function get() {
            return this.b === null ? 0 : this.b.byteLength;
        }
    }, {
        key: "remaining",
        get: function get() {
            return this._limit - this._position;
        }
    }]);

    return Buffer;
}();

Ice.Buffer = Buffer;
module.exports.Ice = Ice;