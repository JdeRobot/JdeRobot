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

var Ice = require("../Ice/ModuleRegistry").Ice;

//
// The Long type represents a signed 64-bit integer as two 32-bit values
// corresponding to the high and low words.
//

var Long = function () {
    //
    // If only one argument is provide we assume it is a JavaScript Number,
    // and we convert it to two 32 bit words to fit in the Ice.Long internal
    // representation.
    //
    // If two arguments are provided we asume these are the high and low words
    // respectively.
    //
    function Long() {
        var high = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : 0;
        var low = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : undefined;

        _classCallCheck(this, Long);

        if (!Number.isSafeInteger(high)) {
            throw new RangeError(low === undefined ? "Number must be a safe integer" : "High word must be a safe integer");
        }

        if (low === undefined) {
            this.low = high % Long.HIGH_MASK;
            this.high = Math.floor(high / Long.HIGH_MASK);
        } else {
            if (!Number.isSafeInteger(low)) {
                throw new RangeError("Low word must be a safe integer");
            }
            if (low < 0 || low > Long.MAX_UINT32) {
                throw new RangeError("Low word must be between 0 and 0xFFFFFFFF");
            }
            if (high < 0 || high > Long.MAX_UINT32) {
                throw new RangeError("High word must be between 0 and 0xFFFFFFFF");
            }

            this.high = high;
            this.low = low;
        }
    }

    _createClass(Long, [{
        key: "hashCode",
        value: function hashCode() {
            return this.low;
        }
    }, {
        key: "equals",
        value: function equals(rhs) {
            if (this === rhs) {
                return true;
            }
            if (!(rhs instanceof Long)) {
                return false;
            }
            return this.high === rhs.high && this.low === rhs.low;
        }
    }, {
        key: "toString",
        value: function toString() {
            return this.high + ":" + this.low;
        }
    }, {
        key: "toNumber",
        value: function toNumber() {
            if ((this.high & Long.SIGN_MASK) !== 0) {
                var l = ~this.low >>> 0;
                var h = ~this.high >>> 0;
                if (h > Long.HIGH_MAX || h == Long.HIGH_MAX && l == Long.MAX_UINT32) {
                    return Number.NEGATIVE_INFINITY;
                }
                return -(h * Long.HIGH_MASK + l + 1);
            } else {
                if (this.high > Long.HIGH_MAX) {
                    return Number.POSITIVE_INFINITY;
                }
                return this.high * Long.HIGH_MASK + this.low;
            }
        }
    }]);

    return Long;
}();

//
// 2^32
//


Long.MAX_UINT32 = 0xFFFFFFFF;

//
// (high & SIGN_MASK) != 0 denotes a negative number;
// that is, the most significant bit is set.
//
Long.SIGN_MASK = 0x80000000;

//
// When converting to a JavaScript Number we left shift the
// high word by 32 bits. As that isn't possible using JavaScript's
// left shift operator, we multiply the value by 2^32 which will
// produce the same result.
//
Long.HIGH_MASK = 0x100000000;

//
// The maximum value for the high word when coverting to
// a JavaScript Number is 2^21 - 1, in which case all
// 53 bits are used.
//
Long.HIGH_MAX = 0x1FFFFF;

Ice.Long = Long;
module.exports.Ice = Ice;