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
Ice._ModuleRegistry.require(module, ["../Ice/LocalException", "../Ice/TimerUtil"]);

var CommunicatorDestroyedException = Ice.CommunicatorDestroyedException;

var Timer = function () {
    function Timer(logger) {
        _classCallCheck(this, Timer);

        this._logger = logger;
        this._destroyed = false;
        this._tokenId = 0;
        this._tokens = new Map();
    }

    _createClass(Timer, [{
        key: "destroy",
        value: function destroy() {
            var _this = this;

            this._tokens.forEach(function (value, key) {
                return _this.cancel(key);
            });
            this._destroyed = true;
            this._tokens.clear();
        }
    }, {
        key: "schedule",
        value: function schedule(callback, delay) {
            var _this2 = this;

            if (this._destroyed) {
                throw new CommunicatorDestroyedException();
            }
            var token = this._tokenId++;
            var id = Timer.setTimeout(function () {
                return _this2.handleTimeout(token);
            }, delay);
            this._tokens.set(token, { callback: callback, id: id, isInterval: false });
            return token;
        }
    }, {
        key: "scheduleRepeated",
        value: function scheduleRepeated(callback, period) {
            var _this3 = this;

            if (this._destroyed) {
                throw new CommunicatorDestroyedException();
            }
            var token = this._tokenId++;
            var id = Timer.setInterval(function () {
                return _this3.handleInterval(token);
            }, period);
            this._tokens.set(token, { callback: callback, id: id, isInterval: true });
            return token;
        }
    }, {
        key: "cancel",
        value: function cancel(id) {
            if (this._destroyed) {
                return false;
            }

            var token = this._tokens.get(id);
            if (token === undefined) {
                return false;
            }

            this._tokens.delete(id);
            if (token.isInterval) {
                Timer.clearInterval(token.id);
            } else {
                Timer.clearTimeout(token.id);
            }

            return true;
        }
    }, {
        key: "handleTimeout",
        value: function handleTimeout(id) {
            if (this._destroyed) {
                return;
            }

            var token = this._tokens.get(id);
            if (token !== undefined) {
                this._tokens.delete(id);
                try {
                    token.callback();
                } catch (ex) {
                    this._logger.warning("uncaught exception while executing timer:\n" + ex);
                }
            }
        }
    }, {
        key: "handleInterval",
        value: function handleInterval(id) {
            if (this._destroyed) {
                return;
            }

            var token = this._tokens.get(id);
            if (token !== undefined) {
                try {
                    token.callback();
                } catch (ex) {
                    this._logger.warning("uncaught exception while executing timer:\n" + ex);
                }
            }
        }
    }]);

    return Timer;
}();

Timer.setTimeout = Ice.Timer.setTimeout;
Timer.clearTimeout = Ice.Timer.clearTimeout;
Timer.setInterval = Ice.Timer.setInterval;
Timer.clearInterval = Ice.Timer.clearInterval;
Timer.setImmediate = Ice.Timer.setImmediate;

Ice.Timer = Timer;
module.exports.Ice = Ice;