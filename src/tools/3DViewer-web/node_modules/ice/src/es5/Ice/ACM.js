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
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/Connection"]);

var Debug = Ice.Debug;

var ACMConfig = function ACMConfig(p, l, prefix, dflt) {
    _classCallCheck(this, ACMConfig);

    if (p === undefined) {
        this.timeout = 60 * 1000;
        this.heartbeat = Ice.ACMHeartbeat.HeartbeatOnDispatch;
        this.close = Ice.ACMClose.CloseOnInvocationAndIdle;
        return;
    }

    var timeoutProperty = void 0;
    if ((prefix == "Ice.ACM.Client" || prefix == "Ice.ACM.Server") && p.getProperty(prefix + ".Timeout").length === 0) {
        timeoutProperty = prefix; // Deprecated property.
    } else {
        timeoutProperty = prefix + ".Timeout";
    }

    this.timeout = p.getPropertyAsIntWithDefault(timeoutProperty, dflt.timeout / 1000) * 1000; // To ms

    var hb = p.getPropertyAsIntWithDefault(prefix + ".Heartbeat", dflt.heartbeat.value);
    if (hb >= 0 && hb <= Ice.ACMHeartbeat.maxValue) {
        this.heartbeat = Ice.ACMHeartbeat.valueOf(hb);
    } else {
        l.warning("invalid value for property `" + prefix + ".Heartbeat" + "', default value will be used instead");
        this.heartbeat = dflt.heartbeat;
    }

    var cl = p.getPropertyAsIntWithDefault(prefix + ".Close", dflt.close.value);
    if (cl >= 0 && cl <= Ice.ACMClose.maxValue) {
        this.close = Ice.ACMClose.valueOf(cl);
    } else {
        l.warning("invalid value for property `" + prefix + ".Close" + "', default value will be used instead");
        this.close = dflt.close;
    }
};

var FactoryACMMonitor = function () {
    function FactoryACMMonitor(instance, config) {
        _classCallCheck(this, FactoryACMMonitor);

        this._instance = instance;
        this._config = config;
        this._reapedConnections = [];
        this._connections = [];
    }

    _createClass(FactoryACMMonitor, [{
        key: "destroy",
        value: function destroy() {
            if (this._instance === null) {
                return;
            }
            this._instance = null;
        }
    }, {
        key: "add",
        value: function add(connection) {
            var _this = this;

            if (this._config.timeout === 0) {
                return;
            }

            this._connections.push(connection);
            if (this._connections.length == 1) {
                this._timerToken = this._instance.timer().scheduleRepeated(function () {
                    return _this.runTimerTask();
                }, this._config.timeout / 2);
            }
        }
    }, {
        key: "remove",
        value: function remove(connection) {
            if (this._config.timeout === 0) {
                return;
            }

            var i = this._connections.indexOf(connection);
            Debug.assert(i >= 0);
            this._connections.splice(i, 1);
            if (this._connections.length === 0) {
                this._instance.timer().cancel(this._timerToken);
            }
        }
    }, {
        key: "reap",
        value: function reap(connection) {
            this._reapedConnections.push(connection);
        }
    }, {
        key: "acm",
        value: function acm(timeout, close, heartbeat) {
            Debug.assert(this._instance !== null);

            var config = new ACMConfig();
            config.timeout = this._config.timeout;
            config.close = this._config.close;
            config.heartbeat = this._config.heartbeat;
            if (timeout !== undefined) {
                config.timeout = timeout * 1000; // To milliseconds
            }
            if (close !== undefined) {
                config.close = close;
            }
            if (heartbeat !== undefined) {
                config.heartbeat = heartbeat;
            }
            return new ConnectionACMMonitor(this, this._instance.timer(), config);
        }
    }, {
        key: "getACM",
        value: function getACM() {
            return new Ice.ACM(this._config.timeout / 1000, this._config.close, this._config.heartbeat);
        }
    }, {
        key: "swapReapedConnections",
        value: function swapReapedConnections() {
            if (this._reapedConnections.length === 0) {
                return null;
            }
            var connections = this._reapedConnections;
            this._reapedConnections = [];
            return connections;
        }
    }, {
        key: "runTimerTask",
        value: function runTimerTask() {
            var _this2 = this;

            if (this._instance === null) {
                this._connections = null;
                return;
            }

            //
            // Monitor connections outside the thread synchronization, so
            // that connections can be added or removed during monitoring.
            //
            var now = Date.now();
            this._connections.forEach(function (connection) {
                try {
                    connection.monitor(now, _this2._config);
                } catch (ex) {
                    _this2.handleException(ex);
                }
            });
        }
    }, {
        key: "handleException",
        value: function handleException(ex) {
            if (this._instance === null) {
                return;
            }
            this._instance.initializationData().logger.error("exception in connection monitor:\n" + ex);
        }
    }]);

    return FactoryACMMonitor;
}();

var ConnectionACMMonitor = function () {
    function ConnectionACMMonitor(parent, timer, config) {
        _classCallCheck(this, ConnectionACMMonitor);

        this._parent = parent;
        this._timer = timer;
        this._config = config;
        this._connection = null;
    }

    _createClass(ConnectionACMMonitor, [{
        key: "add",
        value: function add(connection) {
            var _this3 = this;

            Debug.assert(this._connection === null);
            this._connection = connection;
            if (this._config.timeout > 0) {
                this._timerToken = this._timer.scheduleRepeated(function () {
                    return _this3.runTimerTask();
                }, this._config.timeout / 2);
            }
        }
    }, {
        key: "remove",
        value: function remove(connection) {
            Debug.assert(this._connection === connection);
            this._connection = null;
            if (this._config.timeout > 0) {
                this._timer.cancel(this._timerToken);
            }
        }
    }, {
        key: "reap",
        value: function reap(connection) {
            this._parent.reap(connection);
        }
    }, {
        key: "acm",
        value: function acm(timeout, close, heartbeat) {
            return this._parent.acm(timeout, close, heartbeat);
        }
    }, {
        key: "getACM",
        value: function getACM() {
            return new Ice.ACM(this._config.timeout / 1000, this._config.close, this._config.heartbeat);
        }
    }, {
        key: "runTimerTask",
        value: function runTimerTask() {
            try {
                this._connection.monitor(Date.now(), this._config);
            } catch (ex) {
                this._parent.handleException(ex);
            }
        }
    }]);

    return ConnectionACMMonitor;
}();

Ice.FactoryACMMonitor = FactoryACMMonitor;
Ice.ACMConfig = ACMConfig;
module.exports.Ice = Ice;