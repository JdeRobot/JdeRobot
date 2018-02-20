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
Ice._ModuleRegistry.require(module, ["../Ice/AsyncStatus", "../Ice/ConnectionRequestHandler", "../Ice/Debug", "../Ice/RetryException", "../Ice/ReferenceMode", "../Ice/Exception"]);

var AsyncStatus = Ice.AsyncStatus;
var ConnectionRequestHandler = Ice.ConnectionRequestHandler;
var Debug = Ice.Debug;
var RetryException = Ice.RetryException;
var ReferenceMode = Ice.ReferenceMode;
var LocalException = Ice.LocalException;

var ConnectRequestHandler = function () {
    function ConnectRequestHandler(ref, proxy) {
        _classCallCheck(this, ConnectRequestHandler);

        this._reference = ref;
        this._response = ref.getMode() === ReferenceMode.ModeTwoway;
        this._proxy = proxy;
        this._proxies = [];
        this._initialized = false;

        this._connection = null;
        this._exception = null;
        this._requests = [];
    }

    _createClass(ConnectRequestHandler, [{
        key: "connect",
        value: function connect(proxy) {
            if (!this.initialized()) {
                this._proxies.push(proxy);
            }
            return this._requestHandler ? this._requestHandler : this;
        }
    }, {
        key: "update",
        value: function update(previousHandler, newHandler) {
            return previousHandler === this ? newHandler : this;
        }
    }, {
        key: "sendAsyncRequest",
        value: function sendAsyncRequest(out) {
            if (!this._initialized) {
                out.cancelable(this); // This will throw if the request is canceled
            }

            if (!this.initialized()) {
                this._requests.push(out);
                return AsyncStatus.Queued;
            }
            return out.invokeRemote(this._connection, this._response);
        }
    }, {
        key: "asyncRequestCanceled",
        value: function asyncRequestCanceled(out, ex) {
            if (this._exception !== null) {
                return; // The request has been notified of a failure already.
            }

            if (!this.initialized()) {
                for (var i = 0; i < this._requests.length; i++) {
                    if (this._requests[i] === out) {
                        out.completedEx(ex);
                        this._requests.splice(i, 1);
                        return;
                    }
                }
                Debug.assert(false); // The request has to be queued if it timed out and we're not initialized yet.
            }
            this._connection.asyncRequestCanceled(out, ex);
        }
    }, {
        key: "getReference",
        value: function getReference() {
            return this._reference;
        }
    }, {
        key: "getConnection",
        value: function getConnection() {
            if (this._exception !== null) {
                throw this._exception;
            } else {
                return this._connection;
            }
        }

        //
        // Implementation of Reference_GetConnectionCallback
        //

    }, {
        key: "setConnection",
        value: function setConnection(connection) {
            var _this = this;

            Debug.assert(this._exception === null && this._connection === null);

            this._connection = connection;

            //
            // If this proxy is for a non-local object, and we are using a router, then
            // add this proxy to the router info object.
            //
            var ri = this._reference.getRouterInfo();
            if (ri !== null) {
                //
                ri.addProxy(this._proxy).then(function () {
                    return _this.flushRequests();
                }, // The proxy was added to the router
                // info, we're now ready to send the
                // queued requests.
                //
                function (ex) {
                    return _this.setException(ex);
                });
                return; // The request handler will be initialized once addProxy completes.
            }

            //
            // We can now send the queued requests.
            //
            this.flushRequests();
        }
    }, {
        key: "setException",
        value: function setException(ex) {
            var _this2 = this;

            Debug.assert(!this._initialized && this._exception === null);

            this._exception = ex;
            this._proxies.length = 0;
            this._proxy = null; // Break cyclic reference count.

            //
            // NOTE: remove the request handler *before* notifying the
            // requests that the connection failed. It's important to ensure
            // that future invocations will obtain a new connect request
            // handler once invocations are notified.
            //
            try {
                this._reference.getInstance().requestHandlerFactory().removeRequestHandler(this._reference, this);
            } catch (exc) {
                // Ignore
            }

            this._requests.forEach(function (request) {
                if (request !== null) {
                    request.completedEx(_this2._exception);
                }
            });
            this._requests.length = 0;
        }
    }, {
        key: "initialized",
        value: function initialized() {
            if (this._initialized) {
                Debug.assert(this._connection !== null);
                return true;
            } else {
                if (this._exception !== null) {
                    if (this._connection !== null) {
                        //
                        // Only throw if the connection didn't get established. If
                        // it died after being established, we allow the caller to
                        // retry the connection establishment by not throwing here
                        // (the connection will throw RetryException).
                        //
                        return true;
                    }
                    throw this._exception;
                } else {
                    return this._initialized;
                }
            }
        }
    }, {
        key: "flushRequests",
        value: function flushRequests() {
            var _this3 = this;

            Debug.assert(this._connection !== null && !this._initialized);

            var exception = null;
            this._requests.forEach(function (request) {
                try {
                    request.invokeRemote(_this3._connection, _this3._response);
                } catch (ex) {
                    if (ex instanceof RetryException) {
                        exception = ex.inner;

                        // Remove the request handler before retrying.
                        _this3._reference.getInstance().requestHandlerFactory().removeRequestHandler(_this3._reference, _this3);

                        request.retryException(ex.inner);
                    } else {
                        Debug.assert(ex instanceof LocalException);
                        exception = ex;
                        request.out.completedEx(ex);
                    }
                }
            });
            this._requests.length = 0;

            if (this._reference.getCacheConnection() && exception === null) {
                this._requestHandler = new ConnectionRequestHandler(this._reference, this._connection);
                this._proxies.forEach(function (proxy) {
                    return proxy._updateRequestHandler(_this3, _this3._requestHandler);
                });
            }

            Debug.assert(!this._initialized);
            this._exception = exception;
            this._initialized = this._exception === null;

            //
            // Only remove once all the requests are flushed to
            // guarantee serialization.
            //
            this._reference.getInstance().requestHandlerFactory().removeRequestHandler(this._reference, this);

            this._proxies.length = 0;
            this._proxy = null; // Break cyclic reference count.
        }
    }]);

    return ConnectRequestHandler;
}();

Ice.ConnectRequestHandler = ConnectRequestHandler;
module.exports.Ice = Ice;