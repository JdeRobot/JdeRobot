var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

function _extendableBuiltin(cls) {
    function ExtendableBuiltin() {
        var instance = Reflect.construct(cls, Array.from(arguments));
        Object.setPrototypeOf(instance, Object.getPrototypeOf(this));
        return instance;
    }

    ExtendableBuiltin.prototype = Object.create(cls.prototype, {
        constructor: {
            value: cls,
            enumerable: false,
            writable: true,
            configurable: true
        }
    });

    if (Object.setPrototypeOf) {
        Object.setPrototypeOf(ExtendableBuiltin, cls);
    } else {
        ExtendableBuiltin.__proto__ = cls;
    }

    return ExtendableBuiltin;
}

// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

var Ice = require("../Ice/Timer").Ice;
var Timer = Ice.Timer;

var P = function (_extendableBuiltin2) {
    _inherits(P, _extendableBuiltin2);

    function P(cb) {
        _classCallCheck(this, P);

        var res = void 0,
            rej = void 0;

        var _this = _possibleConstructorReturn(this, (P.__proto__ || Object.getPrototypeOf(P)).call(this, function (resolve, reject) {
            res = resolve;
            rej = reject;

            if (cb !== undefined) {
                cb(resolve, reject);
            }
        }));

        _this.resolve = res;
        _this.reject = rej;
        return _this;
    }

    _createClass(P, [{
        key: "finally",
        value: function _finally(cb) {
            return this.then(function (value) {
                return P.resolve(cb()).then(function () {
                    return value;
                });
            }, function (reason) {
                return P.resolve(cb()).then(function () {
                    throw reason;
                });
            });
        }
    }, {
        key: "delay",
        value: function delay(ms) {
            return this.then(function (value) {
                return new P(function (resolve, reject) {
                    return Timer.setTimeout(function () {
                        return resolve(value);
                    }, ms);
                });
            }, function (reason) {
                return new P(function (resolve, reject) {
                    return Timer.setTimeout(function () {
                        return reject(reason);
                    }, ms);
                });
            });
        }
    }], [{
        key: "delay",
        value: function delay(ms, value) {
            return new P(function (resolve) {
                return Timer.setTimeout(function () {
                    return resolve(value);
                }, ms);
            });
        }
    }, {
        key: "try",
        value: function _try(cb) {
            return P.resolve().then(cb);
        }
    }, {
        key: Symbol.species,
        get: function get() {
            return P;
        }
    }]);

    return P;
}(_extendableBuiltin(Promise));

Ice.Promise = P;
module.exports.Ice = Ice;