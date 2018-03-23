var _get = function get(object, property, receiver) { if (object === null) object = Function.prototype; var desc = Object.getOwnPropertyDescriptor(object, property); if (desc === undefined) { var parent = Object.getPrototypeOf(object); if (parent === null) { return undefined; } else { return get(parent, property, receiver); } } else if ("value" in desc) { return desc.value; } else { var getter = desc.get; if (getter === undefined) { return undefined; } return getter.call(receiver); } };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

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
Ice._ModuleRegistry.require(module, ["../Ice/ArrayUtil", "../Ice/AsyncResultBase", "../Ice/ConnectionI", "../Ice/Debug", "../Ice/HashMap", "../Ice/Promise", "../Ice/EndpointTypes", "../Ice/LocalException", "../Ice/Exception", "../Ice/ACM"]);

var ArrayUtil = Ice.ArrayUtil;
var AsyncResultBase = Ice.AsyncResultBase;
var ConnectionI = Ice.ConnectionI;
var ConnectionReaper = Ice.ConnectionReaper;
var Debug = Ice.Debug;
var HashMap = Ice.HashMap;
var EndpointSelectionType = Ice.EndpointSelectionType;
var FactoryACMMonitor = Ice.FactoryACMMonitor;

//
// Only for use by Instance.
//

var OutgoingConnectionFactory = function () {
    function OutgoingConnectionFactory(communicator, instance) {
        _classCallCheck(this, OutgoingConnectionFactory);

        this._communicator = communicator;
        this._instance = instance;
        this._destroyed = false;

        this._monitor = new FactoryACMMonitor(this._instance, this._instance.clientACM());

        this._connectionsByEndpoint = new ConnectionListMap(); // map<EndpointI, Array<Ice.ConnectionI>>
        this._pending = new HashMap(HashMap.compareEquals); // map<EndpointI, Array<ConnectCallback>>
        this._pendingConnectCount = 0;

        this._waitPromise = null;
    }

    _createClass(OutgoingConnectionFactory, [{
        key: "destroy",
        value: function destroy() {
            if (this._destroyed) {
                return;
            }

            this._connectionsByEndpoint.forEach(function (connection) {
                return connection.destroy(ConnectionI.CommunicatorDestroyed);
            });

            this._destroyed = true;
            this._communicator = null;
            this.checkFinished();
        }
    }, {
        key: "waitUntilFinished",
        value: function waitUntilFinished() {
            this._waitPromise = new Ice.Promise();
            this.checkFinished();
            return this._waitPromise;
        }

        //
        // Returns a promise, success callback receives the connection
        //

    }, {
        key: "create",
        value: function create(endpts, hasMore, selType) {
            Debug.assert(endpts.length > 0);

            //
            // Apply the overrides.
            //
            var endpoints = this.applyOverrides(endpts);

            //
            // Try to find a connection to one of the given endpoints.
            //
            try {
                var connection = this.findConnectionByEndpoint(endpoints);
                if (connection !== null) {
                    return Ice.Promise.resolve(connection);
                }
            } catch (ex) {
                return Ice.Promise.reject(ex);
            }

            return new ConnectCallback(this, endpoints, hasMore, selType).start();
        }
    }, {
        key: "setRouterInfo",
        value: function setRouterInfo(routerInfo) {
            var _this = this;

            return Ice.Promise.try(function () {
                if (_this._destroyed) {
                    throw new Ice.CommunicatorDestroyedException();
                }
                return routerInfo.getClientEndpoints();
            }).then(function (endpoints) {
                //
                // Search for connections to the router's client proxy
                // endpoints, and update the object adapter for such
                // connections, so that callbacks from the router can be
                // received over such connections.
                //
                var adapter = routerInfo.getAdapter();
                var defaultsAndOverrides = _this._instance.defaultsAndOverrides();
                endpoints.forEach(function (endpoint) {
                    //
                    // Modify endpoints with overrides.
                    //
                    if (defaultsAndOverrides.overrideTimeout) {
                        endpoint = endpoint.changeTimeout(defaultsAndOverrides.overrideTimeoutValue);
                    }

                    //
                    // The Connection object does not take the compression flag of
                    // endpoints into account, but instead gets the information
                    // about whether messages should be compressed or not from
                    // other sources. In order to allow connection sharing for
                    // endpoints that differ in the value of the compression flag
                    // only, we always set the compression flag to false here in
                    // this connection factory.
                    //
                    endpoint = endpoint.changeCompress(false);

                    _this._connectionsByEndpoint.forEach(function (connection) {
                        if (connection.endpoint().equals(endpoint)) {
                            connection.setAdapter(adapter);
                        }
                    });
                });
            });
        }
    }, {
        key: "removeAdapter",
        value: function removeAdapter(adapter) {
            if (this._destroyed) {
                return;
            }
            this._connectionsByEndpoint.forEach(function (connection) {
                if (connection.getAdapter() === adapter) {
                    connection.setAdapter(null);
                }
            });
        }
    }, {
        key: "flushAsyncBatchRequests",
        value: function flushAsyncBatchRequests() {
            var promise = new AsyncResultBase(this._communicator, "flushBatchRequests", null, null, null);
            if (this._destroyed) {
                promise.resolve();
                return;
            }

            Ice.Promise.all(this._connectionsByEndpoint.map(function (connection) {
                if (connection.isActiveOrHolding()) {
                    return connection.flushBatchRequests().catch(function (ex) {
                        if (ex instanceof Ice.LocalException) {
                            // Ignore
                        } else {
                            throw ex;
                        }
                    });
                }
            })).then(promise.resolve, promise.reject);
            return promise;
        }
    }, {
        key: "applyOverrides",
        value: function applyOverrides(endpts) {
            var defaultsAndOverrides = this._instance.defaultsAndOverrides();
            return endpts.map(function (endpoint) {
                //
                // Modify endpoints with overrides.
                //
                return defaultsAndOverrides.overrideTimeout ? endpoint.changeTimeout(defaultsAndOverrides.overrideTimeoutValue) : endpoint;
            });
        }
    }, {
        key: "findConnectionByEndpoint",
        value: function findConnectionByEndpoint(endpoints) {
            if (this._destroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            var defaultsAndOverrides = this._instance.defaultsAndOverrides();
            Debug.assert(endpoints.length > 0);

            for (var i = 0; i < endpoints.length; ++i) {
                var endpoint = endpoints[i];

                if (this._pending.has(endpoint)) {
                    continue;
                }

                var connectionList = this._connectionsByEndpoint.get(endpoint);
                if (connectionList === undefined) {
                    continue;
                }

                for (var j = 0; j < connectionList.length; ++j) {
                    if (connectionList[j].isActiveOrHolding()) // Don't return destroyed or un-validated connections
                        {
                            return connectionList[j];
                        }
                }
            }

            return null;
        }
    }, {
        key: "incPendingConnectCount",
        value: function incPendingConnectCount() {
            //
            // Keep track of the number of pending connects. The outgoing connection factory
            // waitUntilFinished() method waits for all the pending connects to terminate before
            // to return. This ensures that the communicator client thread pool isn't destroyed
            // too soon and will still be available to execute the ice_exception() callbacks for
            // the asynchronous requests waiting on a connection to be established.
            //

            if (this._destroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }
            ++this._pendingConnectCount;
        }
    }, {
        key: "decPendingConnectCount",
        value: function decPendingConnectCount() {
            --this._pendingConnectCount;
            Debug.assert(this._pendingConnectCount >= 0);
            if (this._destroyed && this._pendingConnectCount === 0) {
                this.checkFinished();
            }
        }
    }, {
        key: "getConnection",
        value: function getConnection(endpoints, cb) {
            var _this2 = this;

            if (this._destroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            //
            // Reap closed connections
            //
            var cons = this._monitor.swapReapedConnections();
            if (cons !== null) {
                cons.forEach(function (c) {
                    _this2._connectionsByEndpoint.removeConnection(c.endpoint(), c);
                    _this2._connectionsByEndpoint.removeConnection(c.endpoint().changeCompress(true), c);
                });
            }

            //
            // Try to get the connection.
            //
            while (true) {
                if (this._destroyed) {
                    throw new Ice.CommunicatorDestroyedException();
                }

                //
                // Search for a matching connection. If we find one, we're done.
                //
                var connection = this.findConnectionByEndpoint(endpoints);
                if (connection !== null) {
                    return connection;
                }

                if (this.addToPending(cb, endpoints)) {
                    //
                    // A connection is already pending.
                    //
                    return null;
                } else {
                    //
                    // No connection is currently pending to one of our endpoints, so we
                    // get out of this loop and start the connection establishment to one of the
                    // given endpoints.
                    //
                    break;
                }
            }

            //
            // At this point, we're responsible for establishing the connection to one of
            // the given endpoints. If it's a non-blocking connect, calling nextEndpoint
            // will start the connection establishment. Otherwise, we return null to get
            // the caller to establish the connection.
            //
            cb.nextEndpoint();

            return null;
        }
    }, {
        key: "createConnection",
        value: function createConnection(transceiver, endpoint) {
            Debug.assert(this._pending.has(endpoint) && transceiver !== null);

            //
            // Create and add the connection to the connection map. Adding the connection to the map
            // is necessary to support the interruption of the connection initialization and validation
            // in case the communicator is destroyed.
            //
            var connection = null;
            try {
                if (this._destroyed) {
                    throw new Ice.CommunicatorDestroyedException();
                }

                connection = new ConnectionI(this._communicator, this._instance, this._monitor, transceiver, endpoint.changeCompress(false), false, null);
            } catch (ex) {
                if (ex instanceof Ice.LocalException) {
                    try {
                        transceiver.close();
                    } catch (exc) {
                        // Ignore
                    }
                }
                throw ex;
            }

            this._connectionsByEndpoint.set(connection.endpoint(), connection);
            this._connectionsByEndpoint.set(connection.endpoint().changeCompress(true), connection);
            return connection;
        }
    }, {
        key: "finishGetConnection",
        value: function finishGetConnection(endpoints, endpoint, connection, cb) {
            var _this3 = this;

            // cb is-a ConnectCallback

            var connectionCallbacks = [];
            if (cb !== null) {
                connectionCallbacks.push(cb);
            }

            var callbacks = [];
            endpoints.forEach(function (endpt) {
                var cbs = _this3._pending.get(endpt);
                if (cbs !== undefined) {
                    _this3._pending.delete(endpt);
                    cbs.forEach(function (cc) {
                        if (cc.hasEndpoint(endpoint)) {
                            if (connectionCallbacks.indexOf(cc) === -1) {
                                connectionCallbacks.push(cc);
                            }
                        } else {
                            if (callbacks.indexOf(cc) === -1) {
                                callbacks.push(cc);
                            }
                        }
                    });
                }
            });

            connectionCallbacks.forEach(function (cc) {
                cc.removeFromPending();
                var idx = callbacks.indexOf(cc);
                if (idx !== -1) {
                    callbacks.splice(idx, 1);
                }
            });

            callbacks.forEach(function (cc) {
                return cc.removeFromPending();
            });

            callbacks.forEach(function (cc) {
                return cc.getConnection();
            });
            connectionCallbacks.forEach(function (cc) {
                return cc.setConnection(connection);
            });

            this.checkFinished();
        }
    }, {
        key: "finishGetConnectionEx",
        value: function finishGetConnectionEx(endpoints, ex, cb) {
            var _this4 = this;

            // cb is-a ConnectCallback

            var failedCallbacks = [];
            if (cb !== null) {
                failedCallbacks.push(cb);
            }

            var callbacks = [];
            endpoints.forEach(function (endpt) {
                var cbs = _this4._pending.get(endpt);
                if (cbs !== undefined) {
                    _this4._pending.delete(endpt);
                    cbs.forEach(function (cc) {
                        if (cc.removeEndpoints(endpoints)) {
                            if (failedCallbacks.indexOf(cc) === -1) {
                                failedCallbacks.push(cc);
                            }
                        } else {
                            if (callbacks.indexOf(cc) === -1) {
                                callbacks.push(cc);
                            }
                        }
                    });
                }
            });

            callbacks.forEach(function (cc) {
                Debug.assert(failedCallbacks.indexOf(cc) === -1);
                cc.removeFromPending();
            });
            this.checkFinished();
            callbacks.forEach(function (cc) {
                return cc.getConnection();
            });
            failedCallbacks.forEach(function (cc) {
                return cc.setException(ex);
            });
        }
    }, {
        key: "addToPending",
        value: function addToPending(cb, endpoints) {
            var _this5 = this;

            // cb is-a ConnectCallback

            //
            // Add the callback to each pending list.
            //
            var found = false;
            if (cb !== null) {
                endpoints.forEach(function (p) {
                    var cbs = _this5._pending.get(p);
                    if (cbs !== undefined) {
                        found = true;
                        if (cbs.indexOf(cb) === -1) {
                            cbs.push(cb); // Add the callback to each pending endpoint.
                        }
                    }
                });
            }

            if (found) {
                return true;
            }

            //
            // If there's no pending connection for the given endpoints, we're
            // responsible for its establishment. We add empty pending lists,
            // other callbacks to the same endpoints will be queued.
            //
            endpoints.forEach(function (p) {
                if (!_this5._pending.has(p)) {
                    _this5._pending.set(p, []);
                }
            });

            return false;
        }
    }, {
        key: "removeFromPending",
        value: function removeFromPending(cb, endpoints) {
            var _this6 = this;

            // cb is-a ConnectCallback
            endpoints.forEach(function (p) {
                var cbs = _this6._pending.get(p);
                if (cbs !== undefined) {
                    var idx = cbs.indexOf(cb);
                    if (idx !== -1) {
                        cbs.splice(idx, 1);
                    }
                }
            });
        }
    }, {
        key: "handleConnectionException",
        value: function handleConnectionException(ex, hasMore) {
            var traceLevels = this._instance.traceLevels();
            if (traceLevels.network >= 2) {
                var s = [];
                s.push("connection to endpoint failed");
                if (ex instanceof Ice.CommunicatorDestroyedException) {
                    s.push("\n");
                } else {
                    if (hasMore) {
                        s.push(", trying next endpoint\n");
                    } else {
                        s.push(" and no more endpoints to try\n");
                    }
                }
                s.push(ex.toString());
                this._instance.initializationData().logger.trace(traceLevels.networkCat, s.join(""));
            }
        }
    }, {
        key: "handleException",
        value: function handleException(ex, hasMore) {
            var traceLevels = this._instance.traceLevels();
            if (traceLevels.network >= 2) {
                var s = [];
                s.push("couldn't resolve endpoint host");
                if (ex instanceof Ice.CommunicatorDestroyedException) {
                    s.push("\n");
                } else {
                    if (hasMore) {
                        s.push(", trying next endpoint\n");
                    } else {
                        s.push(" and no more endpoints to try\n");
                    }
                }
                s.push(ex.toString());
                this._instance.initializationData().logger.trace(traceLevels.networkCat, s.join(""));
            }
        }
    }, {
        key: "checkFinished",
        value: function checkFinished() {
            var _this7 = this;

            //
            // Can't continue until the factory is destroyed and there are no pending connections.
            //
            if (!this._waitPromise || !this._destroyed || this._pending.size > 0 || this._pendingConnectCount > 0) {
                return;
            }

            Ice.Promise.all(this._connectionsByEndpoint.map(function (connection) {
                return connection.waitUntilFinished().catch(function (ex) {
                    return Debug.assert(false);
                });
            })).then(function () {
                var cons = _this7._monitor.swapReapedConnections();
                if (cons !== null) {
                    var arr = [];
                    _this7._connectionsByEndpoint.forEach(function (connection) {
                        if (arr.indexOf(connection) === -1) {
                            arr.push(connection);
                        }
                    });
                    Debug.assert(cons.length === arr.length);
                    _this7._connectionsByEndpoint.clear();
                } else {
                    Debug.assert(_this7._connectionsByEndpoint.size === 0);
                }

                Debug.assert(_this7._waitPromise !== null);
                _this7._waitPromise.resolve();
                _this7._monitor.destroy();
            });
        }
    }]);

    return OutgoingConnectionFactory;
}();

Ice.OutgoingConnectionFactory = OutgoingConnectionFactory;
module.exports.Ice = Ice;

//
// Value is a Vector<Ice.ConnectionI>
//

var ConnectionListMap = function (_HashMap) {
    _inherits(ConnectionListMap, _HashMap);

    function ConnectionListMap(h) {
        _classCallCheck(this, ConnectionListMap);

        return _possibleConstructorReturn(this, (ConnectionListMap.__proto__ || Object.getPrototypeOf(ConnectionListMap)).call(this, h || HashMap.compareEquals));
    }

    _createClass(ConnectionListMap, [{
        key: "set",
        value: function set(key, value) {
            var list = this.get(key);
            if (list === undefined) {
                list = [];
                _get(ConnectionListMap.prototype.__proto__ || Object.getPrototypeOf(ConnectionListMap.prototype), "set", this).call(this, key, list);
            }
            Debug.assert(value instanceof ConnectionI);
            list.push(value);
            return undefined;
        }
    }, {
        key: "removeConnection",
        value: function removeConnection(key, conn) {
            var list = this.get(key);
            Debug.assert(list !== null);
            var idx = list.indexOf(conn);
            Debug.assert(idx !== -1);
            list.splice(idx, 1);
            if (list.length === 0) {
                this.delete(key);
            }
        }
    }, {
        key: "map",
        value: function map(fn) {
            var arr = [];
            this.forEach(function (c) {
                return arr.push(fn(c));
            });
            return arr;
        }
    }, {
        key: "forEach",
        value: function forEach(fn) {
            var _iteratorNormalCompletion = true;
            var _didIteratorError = false;
            var _iteratorError = undefined;

            try {
                for (var _iterator = this.values()[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                    var connections = _step.value;

                    connections.forEach(fn);
                }
            } catch (err) {
                _didIteratorError = true;
                _iteratorError = err;
            } finally {
                try {
                    if (!_iteratorNormalCompletion && _iterator.return) {
                        _iterator.return();
                    }
                } finally {
                    if (_didIteratorError) {
                        throw _iteratorError;
                    }
                }
            }
        }
    }]);

    return ConnectionListMap;
}(HashMap);

var ConnectCallback = function () {
    function ConnectCallback(f, endpoints, more, selType) {
        _classCallCheck(this, ConnectCallback);

        this._factory = f;
        this._endpoints = endpoints;
        this._hasMore = more;
        this._selType = selType;
        this._promise = new Ice.Promise();
        this._index = 0;
        this._current = null;
    }

    //
    // Methods from ConnectionI_StartCallback
    //


    _createClass(ConnectCallback, [{
        key: "connectionStartCompleted",
        value: function connectionStartCompleted(connection) {
            connection.activate();
            this._factory.finishGetConnection(this._endpoints, this._current, connection, this);
        }
    }, {
        key: "connectionStartFailed",
        value: function connectionStartFailed(connection, ex) {
            Debug.assert(this._current !== null);
            if (this.connectionStartFailedImpl(ex)) {
                this.nextEndpoint();
            }
        }
    }, {
        key: "setConnection",
        value: function setConnection(connection) {
            //
            // Callback from the factory: the connection to one of the callback
            // connectors has been established.
            //
            this._promise.resolve(connection);
            this._factory.decPendingConnectCount(); // Must be called last.
        }
    }, {
        key: "setException",
        value: function setException(ex) {
            //
            // Callback from the factory: connection establishment failed.
            //
            this._promise.reject(ex);
            this._factory.decPendingConnectCount(); // Must be called last.
        }
    }, {
        key: "hasEndpoint",
        value: function hasEndpoint(endpoint) {
            return this.findEndpoint(endpoint) !== -1;
        }
    }, {
        key: "findEndpoint",
        value: function findEndpoint(endpoint) {
            return this._endpoints.findIndex(function (value) {
                return endpoint.equals(value);
            });
        }
    }, {
        key: "removeEndpoints",
        value: function removeEndpoints(endpoints) {
            var _this9 = this;

            endpoints.forEach(function (endpoint) {
                var idx = _this9.findEndpoint(endpoint);
                if (idx !== -1) {
                    _this9._endpoints.splice(idx, 1);
                }
            });
            this._index = 0;
            return this._endpoints.length === 0;
        }
    }, {
        key: "removeFromPending",
        value: function removeFromPending() {
            this._factory.removeFromPending(this, this._endpoints);
        }
    }, {
        key: "start",
        value: function start() {
            try {
                //
                // Notify the factory that there's an async connect pending. This is necessary
                // to prevent the outgoing connection factory to be destroyed before all the
                // pending asynchronous connects are finished.
                //
                this._factory.incPendingConnectCount();
            } catch (ex) {
                this._promise.reject(ex);
                return;
            }

            this.getConnection();
            return this._promise;
        }
    }, {
        key: "getConnection",
        value: function getConnection() {
            try {
                //
                // Ask the factory to get a connection.
                //
                var connection = this._factory.getConnection(this._endpoints, this);
                if (connection === null) {
                    //
                    // A null return value from getConnection indicates that the connection
                    // is being established and that everthing has been done to ensure that
                    // the callback will be notified when the connection establishment is
                    // done.
                    //
                    return;
                }

                this._promise.resolve(connection);
                this._factory.decPendingConnectCount(); // Must be called last.
            } catch (ex) {
                this._promise.reject(ex);
                this._factory.decPendingConnectCount(); // Must be called last.
            }
        }
    }, {
        key: "nextEndpoint",
        value: function nextEndpoint() {
            var _this10 = this;

            var start = function start(connection) {
                connection.start().then(function () {
                    _this10.connectionStartCompleted(connection);
                }, function (ex) {
                    _this10.connectionStartFailed(connection, ex);
                });
            };

            while (true) {
                var traceLevels = this._factory._instance.traceLevels();
                try {
                    Debug.assert(this._index < this._endpoints.length);
                    this._current = this._endpoints[this._index++];

                    if (traceLevels.network >= 2) {
                        var s = [];
                        s.push("trying to establish ");
                        s.push(this._current.protocol());
                        s.push(" connection to ");
                        s.push(this._current.toConnectorString());
                        this._factory._instance.initializationData().logger.trace(traceLevels.networkCat, s.join(""));
                    }

                    start(this._factory.createConnection(this._current.connect(), this._current));
                } catch (ex) {
                    if (traceLevels.network >= 2) {
                        var _s = [];
                        _s.push("failed to establish ");
                        _s.push(this._current.protocol());
                        _s.push(" connection to ");
                        _s.push(this._current.toString());
                        _s.push("\n");
                        _s.push(ex.toString());
                        this._factory._instance.initializationData().logger.trace(traceLevels.networkCat, _s.join(""));
                    }

                    if (this.connectionStartFailedImpl(ex)) {
                        continue;
                    }
                }
                break;
            }
        }
    }, {
        key: "connectionStartFailedImpl",
        value: function connectionStartFailedImpl(ex) {
            if (ex instanceof Ice.LocalException) {
                this._factory.handleConnectionException(ex, this._hasMore || this._index < this._endpoints.length);
                if (ex instanceof Ice.CommunicatorDestroyedException) // No need to continue.
                    {
                        this._factory.finishGetConnectionEx(this._endpoints, ex, this);
                    } else if (this._index < this._endpoints.length) // Try the next endpoint.
                    {
                        return true;
                    } else {
                    this._factory.finishGetConnectionEx(this._endpoints, ex, this);
                }
            } else {
                this._factory.finishGetConnectionEx(this._endpoints, ex, this);
            }
            return false;
        }
    }]);

    return ConnectCallback;
}();