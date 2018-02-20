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
var _ModuleRegistry = Ice._ModuleRegistry;
var Slice = Ice.Slice;

var eq = function eq(e1, e2) {
    if (e1 === e2) {
        return true; // If identity compare equals members are equal.
    } else if (e1 === null || e1 === undefined || e2 === null || e2 === undefined) {
        return false;
    } else if (e1.prototype !== e2.prototype) {
        return false;
    } else if (typeof e1.equals == "function") {
        return e1.equals(e2);
    } else if (e1 instanceof Array) {
        return ArrayUtil.equals(e1, e2, eq);
    }
    return false;
};

var ArrayUtil = function () {
    function ArrayUtil() {
        _classCallCheck(this, ArrayUtil);
    }

    _createClass(ArrayUtil, null, [{
        key: "clone",
        value: function clone(arr) {
            if (arr === undefined) {
                return arr;
            } else if (arr === null) {
                return [];
            } else {
                return arr.slice();
            }
        }
    }, {
        key: "equals",
        value: function equals(v1, v2, valuesEqual) {
            if (v1.length != v2.length) {
                return false;
            }

            var equalFn = valuesEqual || eq;
            for (var i = 0; i < v1.length; ++i) {
                if (!equalFn.call(equalFn, v1[i], v2[i])) {
                    return false;
                }
            }

            return true;
        }
    }, {
        key: "shuffle",
        value: function shuffle(arr) {
            for (var i = arr.length; i > 1; --i) {
                var e = arr[i - 1];
                var rand = Math.floor(Math.random() * i);
                arr[i - 1] = arr[rand];
                arr[rand] = e;
            }
        }
    }]);

    return ArrayUtil;
}();

ArrayUtil.eq = eq;

Slice.defineSequence = function (module, name, valueHelper, fixed, elementType) {
    var helper = null;
    Object.defineProperty(module, name, {
        get: function get() {
            if (helper === null) {
                helper = Ice.StreamHelpers.generateSeqHelper(_ModuleRegistry.type(valueHelper), fixed, _ModuleRegistry.type(elementType));
            }
            return helper;
        }
    });
};

Ice.ArrayUtil = ArrayUtil;
module.exports.Ice = Ice;