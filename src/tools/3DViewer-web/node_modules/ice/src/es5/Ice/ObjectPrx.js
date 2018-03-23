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
Ice._ModuleRegistry.require(module, ["../Ice/ArrayUtil", "../Ice/AsyncResult", "../Ice/Debug", "../Ice/FormatType", "../Ice/OutgoingAsync", "../Ice/ReferenceMode", "../Ice/Current", "../Ice/Exception", "../Ice/BuiltinSequences", "../Ice/LocalException", "../Ice/Object"]);

var ArrayUtil = Ice.ArrayUtil;
var AsyncResultBase = Ice.AsyncResultBase;
var AsyncResult = Ice.AsyncResult;
var Debug = Ice.Debug;
var FormatType = Ice.FormatType;
var OutgoingAsync = Ice.OutgoingAsync;
var ProxyFlushBatch = Ice.ProxyFlushBatch;
var ProxyGetConnection = Ice.ProxyGetConnection;
var RefMode = Ice.ReferenceMode;
var OperationMode = Ice.OperationMode;

//
// Ice.ObjectPrx
//

var ObjectPrx = function () {
    function ObjectPrx() {
        _classCallCheck(this, ObjectPrx);

        this._reference = null;
        this._requestHandler = null;
    }

    _createClass(ObjectPrx, [{
        key: "hashCode",
        value: function hashCode(r) {
            return this._reference.hashCode();
        }
    }, {
        key: "ice_getCommunicator",
        value: function ice_getCommunicator() {
            return this._reference.getCommunicator();
        }
    }, {
        key: "toString",
        value: function toString() {
            return this._reference.toString();
        }
    }, {
        key: "ice_getIdentity",
        value: function ice_getIdentity() {
            return this._reference.getIdentity().clone();
        }
    }, {
        key: "ice_identity",
        value: function ice_identity(newIdentity) {
            if (newIdentity === undefined || newIdentity === null || newIdentity.name.length === 0) {
                throw new Ice.IllegalIdentityException();
            }
            if (newIdentity.equals(this._reference.getIdentity())) {
                return this;
            } else {
                var proxy = new ObjectPrx();
                proxy._setup(this._reference.changeIdentity(newIdentity));
                return proxy;
            }
        }
    }, {
        key: "ice_getContext",
        value: function ice_getContext() {
            return new Map(this._reference.getContext());
        }
    }, {
        key: "ice_context",
        value: function ice_context(newContext) {
            return this._newInstance(this._reference.changeContext(newContext));
        }
    }, {
        key: "ice_getFacet",
        value: function ice_getFacet() {
            return this._reference.getFacet();
        }
    }, {
        key: "ice_facet",
        value: function ice_facet(newFacet) {
            if (newFacet === undefined || newFacet === null) {
                newFacet = "";
            }

            if (newFacet === this._reference.getFacet()) {
                return this;
            } else {
                var proxy = new ObjectPrx();
                proxy._setup(this._reference.changeFacet(newFacet));
                return proxy;
            }
        }
    }, {
        key: "ice_getAdapterId",
        value: function ice_getAdapterId() {
            return this._reference.getAdapterId();
        }
    }, {
        key: "ice_adapterId",
        value: function ice_adapterId(newAdapterId) {
            if (newAdapterId === undefined || newAdapterId === null) {
                newAdapterId = "";
            }

            if (newAdapterId === this._reference.getAdapterId()) {
                return this;
            } else {
                return this._newInstance(this._reference.changeAdapterId(newAdapterId));
            }
        }
    }, {
        key: "ice_getEndpoints",
        value: function ice_getEndpoints() {
            return ArrayUtil.clone(this._reference.getEndpoints());
        }
    }, {
        key: "ice_endpoints",
        value: function ice_endpoints(newEndpoints) {
            if (newEndpoints === undefined || newEndpoints === null) {
                newEndpoints = [];
            }

            if (ArrayUtil.equals(newEndpoints, this._reference.getEndpoints())) {
                return this;
            } else {
                return this._newInstance(this._reference.changeEndpoints(newEndpoints));
            }
        }
    }, {
        key: "ice_getLocatorCacheTimeout",
        value: function ice_getLocatorCacheTimeout() {
            return this._reference.getLocatorCacheTimeout();
        }
    }, {
        key: "ice_locatorCacheTimeout",
        value: function ice_locatorCacheTimeout(newTimeout) {
            if (newTimeout < -1) {
                throw new Error("invalid value passed to ice_locatorCacheTimeout: " + newTimeout);
            }
            if (newTimeout === this._reference.getLocatorCacheTimeout()) {
                return this;
            } else {
                return this._newInstance(this._reference.changeLocatorCacheTimeout(newTimeout));
            }
        }
    }, {
        key: "ice_getInvocationTimeout",
        value: function ice_getInvocationTimeout() {
            return this._reference.getInvocationTimeout();
        }
    }, {
        key: "ice_invocationTimeout",
        value: function ice_invocationTimeout(newTimeout) {
            if (newTimeout < 1 && newTimeout !== -1) {
                throw new Error("invalid value passed to ice_invocationTimeout: " + newTimeout);
            }
            if (newTimeout === this._reference.getInvocationTimeout()) {
                return this;
            } else {
                return this._newInstance(this._reference.changeInvocationTimeout(newTimeout));
            }
        }
    }, {
        key: "ice_isConnectionCached",
        value: function ice_isConnectionCached() {
            return this._reference.getCacheConnection();
        }
    }, {
        key: "ice_connectionCached",
        value: function ice_connectionCached(newCache) {
            if (newCache === this._reference.getCacheConnection()) {
                return this;
            } else {
                return this._newInstance(this._reference.changeCacheConnection(newCache));
            }
        }
    }, {
        key: "ice_getEndpointSelection",
        value: function ice_getEndpointSelection() {
            return this._reference.getEndpointSelection();
        }
    }, {
        key: "ice_endpointSelection",
        value: function ice_endpointSelection(newType) {
            if (newType === this._reference.getEndpointSelection()) {
                return this;
            } else {
                return this._newInstance(this._reference.changeEndpointSelection(newType));
            }
        }
    }, {
        key: "ice_isSecure",
        value: function ice_isSecure() {
            return this._reference.getSecure();
        }
    }, {
        key: "ice_secure",
        value: function ice_secure(b) {
            if (b === this._reference.getSecure()) {
                return this;
            } else {
                return this._newInstance(this._reference.changeSecure(b));
            }
        }
    }, {
        key: "ice_getEncodingVersion",
        value: function ice_getEncodingVersion() {
            return this._reference.getEncoding().clone();
        }
    }, {
        key: "ice_encodingVersion",
        value: function ice_encodingVersion(e) {
            if (e.equals(this._reference.getEncoding())) {
                return this;
            } else {
                return this._newInstance(this._reference.changeEncoding(e));
            }
        }
    }, {
        key: "ice_isPreferSecure",
        value: function ice_isPreferSecure() {
            return this._reference.getPreferSecure();
        }
    }, {
        key: "ice_preferSecure",
        value: function ice_preferSecure(b) {
            if (b === this._reference.getPreferSecure()) {
                return this;
            } else {
                return this._newInstance(this._reference.changePreferSecure(b));
            }
        }
    }, {
        key: "ice_getRouter",
        value: function ice_getRouter() {
            var ri = this._reference.getRouterInfo();
            return ri !== null ? ri.getRouter() : null;
        }
    }, {
        key: "ice_router",
        value: function ice_router(router) {
            var ref = this._reference.changeRouter(router);
            if (ref.equals(this._reference)) {
                return this;
            } else {
                return this._newInstance(ref);
            }
        }
    }, {
        key: "ice_getLocator",
        value: function ice_getLocator() {
            var ri = this._reference.getLocatorInfo();
            return ri !== null ? ri.getLocator() : null;
        }
    }, {
        key: "ice_locator",
        value: function ice_locator(locator) {
            var ref = this._reference.changeLocator(locator);
            if (ref.equals(this._reference)) {
                return this;
            } else {
                return this._newInstance(ref);
            }
        }
    }, {
        key: "ice_isTwoway",
        value: function ice_isTwoway() {
            return this._reference.getMode() === RefMode.ModeTwoway;
        }
    }, {
        key: "ice_twoway",
        value: function ice_twoway() {
            if (this._reference.getMode() === RefMode.ModeTwoway) {
                return this;
            } else {
                return this._newInstance(this._reference.changeMode(RefMode.ModeTwoway));
            }
        }
    }, {
        key: "ice_isOneway",
        value: function ice_isOneway() {
            return this._reference.getMode() === RefMode.ModeOneway;
        }
    }, {
        key: "ice_oneway",
        value: function ice_oneway() {
            if (this._reference.getMode() === RefMode.ModeOneway) {
                return this;
            } else {
                return this._newInstance(this._reference.changeMode(RefMode.ModeOneway));
            }
        }
    }, {
        key: "ice_isBatchOneway",
        value: function ice_isBatchOneway() {
            return this._reference.getMode() === RefMode.ModeBatchOneway;
        }
    }, {
        key: "ice_batchOneway",
        value: function ice_batchOneway() {
            if (this._reference.getMode() === RefMode.ModeBatchOneway) {
                return this;
            } else {
                return this._newInstance(this._reference.changeMode(RefMode.ModeBatchOneway));
            }
        }
    }, {
        key: "ice_isDatagram",
        value: function ice_isDatagram() {
            return this._reference.getMode() === RefMode.ModeDatagram;
        }
    }, {
        key: "ice_datagram",
        value: function ice_datagram() {
            if (this._reference.getMode() === RefMode.ModeDatagram) {
                return this;
            } else {
                return this._newInstance(this._reference.changeMode(RefMode.ModeDatagram));
            }
        }
    }, {
        key: "ice_isBatchDatagram",
        value: function ice_isBatchDatagram() {
            return this._reference.getMode() === RefMode.ModeBatchDatagram;
        }
    }, {
        key: "ice_batchDatagram",
        value: function ice_batchDatagram() {
            if (this._reference.getMode() === RefMode.ModeBatchDatagram) {
                return this;
            } else {
                return this._newInstance(this._reference.changeMode(RefMode.ModeBatchDatagram));
            }
        }
    }, {
        key: "ice_timeout",
        value: function ice_timeout(t) {
            if (t < 1 && t !== -1) {
                throw new Error("invalid value passed to ice_timeout: " + t);
            }
            var ref = this._reference.changeTimeout(t);
            if (ref.equals(this._reference)) {
                return this;
            } else {
                return this._newInstance(ref);
            }
        }
    }, {
        key: "ice_getConnectionId",
        value: function ice_getConnectionId() {
            return this._reference.getConnectionId();
        }
    }, {
        key: "ice_connectionId",
        value: function ice_connectionId(id) {
            var ref = this._reference.changeConnectionId(id);
            if (ref.equals(this._reference)) {
                return this;
            } else {
                return this._newInstance(ref);
            }
        }
    }, {
        key: "ice_getConnection",
        value: function ice_getConnection() {
            var r = new ProxyGetConnection(this, "ice_getConnection");
            try {
                r.invoke();
            } catch (ex) {
                r.abort(ex);
            }
            return r;
        }
    }, {
        key: "ice_getCachedConnection",
        value: function ice_getCachedConnection() {
            return this._requestHandler ? this._requestHandler.getConnection() : null;
        }
    }, {
        key: "ice_flushBatchRequests",
        value: function ice_flushBatchRequests() {
            var r = new ProxyFlushBatch(this, "ice_flushBatchRequests");
            try {
                r.invoke();
            } catch (ex) {
                r.abort(ex);
            }
            return r;
        }
    }, {
        key: "equals",
        value: function equals(r) {
            if (this === r) {
                return true;
            }

            if (r instanceof ObjectPrx) {
                return this._reference.equals(r._reference);
            }

            return false;
        }
    }, {
        key: "_write",
        value: function _write(os) {
            this._reference.getIdentity()._write(os);
            this._reference.streamWrite(os);
        }
    }, {
        key: "_getReference",
        value: function _getReference() {
            return this._reference;
        }
    }, {
        key: "_copyFrom",
        value: function _copyFrom(from) {
            Debug.assert(this._reference === null);
            Debug.assert(this._requestHandler === null);

            this._reference = from._reference;
            this._requestHandler = from._requestHandler;
        }
    }, {
        key: "_handleException",
        value: function _handleException(ex, handler, mode, sent, sleep, cnt) {
            this._updateRequestHandler(handler, null); // Clear the request handler

            //
            // We only retry local exception, system exceptions aren't retried.
            //
            // A CloseConnectionException indicates graceful server shutdown, and is therefore
            // always repeatable without violating "at-most-once". That's because by sending a
            // close connection message, the server guarantees that all outstanding requests
            // can safely be repeated.
            //
            // An ObjectNotExistException can always be retried as well without violating
            // "at-most-once" (see the implementation of the checkRetryAfterException method
            //  of the ProxyFactory class for the reasons why it can be useful).
            //
            // If the request didn't get sent or if it's non-mutating or idempotent it can
            // also always be retried if the retry count isn't reached.
            //
            if (ex instanceof Ice.LocalException && (!sent || mode == OperationMode.Nonmutating || mode == OperationMode.Idempotent || ex instanceof Ice.CloseConnectionException || ex instanceof Ice.ObjectNotExistException)) {
                try {
                    return this._reference.getInstance().proxyFactory().checkRetryAfterException(ex, this._reference, sleep, cnt);
                } catch (exc) {
                    if (exc instanceof Ice.CommunicatorDestroyedException) {
                        //
                        // The communicator is already destroyed, so we cannot retry.
                        //
                        throw ex;
                    } else {
                        throw exc;
                    }
                }
            } else {
                throw ex;
            }
        }
    }, {
        key: "_checkAsyncTwowayOnly",
        value: function _checkAsyncTwowayOnly(name) {
            if (!this.ice_isTwoway()) {
                throw new Ice.TwowayOnlyException(name);
            }
        }
    }, {
        key: "_getRequestHandler",
        value: function _getRequestHandler() {
            if (this._reference.getCacheConnection()) {
                if (this._requestHandler) {
                    return this._requestHandler;
                }
            }
            return this._reference.getRequestHandler(this);
        }
    }, {
        key: "_getBatchRequestQueue",
        value: function _getBatchRequestQueue() {
            if (!this._batchRequestQueue) {
                this._batchRequestQueue = this._reference.getBatchRequestQueue();
            }
            return this._batchRequestQueue;
        }
    }, {
        key: "_setRequestHandler",
        value: function _setRequestHandler(handler) {
            if (this._reference.getCacheConnection()) {
                if (!this._requestHandler) {
                    this._requestHandler = handler;
                }
                return this._requestHandler;
            }
            return handler;
        }
    }, {
        key: "_updateRequestHandler",
        value: function _updateRequestHandler(previous, handler) {
            if (this._reference.getCacheConnection() && previous !== null) {
                if (this._requestHandler && this._requestHandler !== handler) {
                    this._requestHandler = this._requestHandler.update(previous, handler);
                }
            }
        }

        //
        // Only for use by IceInternal.ProxyFactory
        //

    }, {
        key: "_setup",
        value: function _setup(ref) {
            Debug.assert(this._reference === null);

            this._reference = ref;
        }
    }, {
        key: "_newInstance",
        value: function _newInstance(ref) {
            var proxy = new this.constructor();
            proxy._setup(ref);
            return proxy;
        }
    }, {
        key: "ice_instanceof",
        value: function ice_instanceof(T) {
            if (T) {
                if (this instanceof T) {
                    return true;
                }
                return this.constructor._instanceof(T);
            }
            return false;
        }

        //
        // Generic invocation for operations that have input parameters.
        //

    }], [{
        key: "_invoke",
        value: function _invoke(p, name, mode, fmt, ctx, marshalFn, unmarshalFn, userEx, args) {
            var _this = this;

            if (unmarshalFn !== null || userEx.length > 0) {
                p._checkAsyncTwowayOnly(name);
            }

            var r = new OutgoingAsync(p, name, function (res) {
                _this._completed(res, unmarshalFn, userEx);
            });

            try {
                r.prepare(name, mode, ctx);
                if (marshalFn === null) {
                    r.writeEmptyParams();
                } else {
                    var ostr = r.startWriteParams(fmt);
                    marshalFn.call(null, ostr, args);
                    r.endWriteParams();
                }
                r.invoke();
            } catch (ex) {
                r.abort(ex);
            }
            return r;
        }

        //
        // Handles the completion of an invocation.
        //

    }, {
        key: "_completed",
        value: function _completed(r, unmarshalFn, userEx) {
            if (!this._check(r, userEx)) {
                return;
            }

            try {
                if (unmarshalFn === null) {
                    r.readEmptyParams();
                    r.resolve();
                } else {
                    r.resolve(unmarshalFn(r));
                }
            } catch (ex) {
                this.dispatchLocalException(r, ex);
                return;
            }
        }

        //
        // Handles user exceptions.
        //

    }, {
        key: "_check",
        value: function _check(r, uex) {
            //
            // If uex is non-null, it must be an array of exception types.
            //
            try {
                r.throwUserException();
            } catch (ex) {
                if (ex instanceof Ice.UserException) {
                    if (uex !== null) {
                        for (var i = 0; i < uex.length; ++i) {
                            if (ex instanceof uex[i]) {
                                r.reject(ex);
                                return false;
                            }
                        }
                    }
                    r.reject(new Ice.UnknownUserException(ex.ice_id()));
                    return false;
                } else {
                    r.reject(ex);
                    return false;
                }
            }

            return true;
        }
    }, {
        key: "dispatchLocalException",
        value: function dispatchLocalException(r, ex) {
            r.reject(ex);
        }
    }, {
        key: "checkedCast",
        value: function checkedCast(prx, facet, ctx) {
            var _this2 = this;

            var r = null;

            if (prx === undefined || prx === null) {
                r = new AsyncResultBase(null, "checkedCast", null, null, null);
                r.resolve(null);
            } else {
                if (facet !== undefined) {
                    prx = prx.ice_facet(facet);
                }

                r = new AsyncResultBase(prx.ice_getCommunicator(), "checkedCast", null, prx, null);
                prx.ice_isA(this.ice_staticId(), ctx).then(function (ret) {
                    if (ret) {
                        var h = new _this2();
                        h._copyFrom(prx);
                        r.resolve(h);
                    } else {
                        r.resolve(null);
                    }
                }).catch(function (ex) {
                    if (ex instanceof Ice.FacetNotExistException) {
                        r.resolve(null);
                    } else {
                        r.reject(ex);
                    }
                });
            }

            return r;
        }
    }, {
        key: "uncheckedCast",
        value: function uncheckedCast(prx, facet) {
            var r = null;
            if (prx !== undefined && prx !== null) {
                r = new this();
                if (facet !== undefined) {
                    prx = prx.ice_facet(facet);
                }
                r._copyFrom(prx);
            }
            return r;
        }
    }, {
        key: "write",
        value: function write(os, v) {
            os.writeProxy(v);
        }
    }, {
        key: "read",
        value: function read(is) {
            return is.readProxy(this);
        }
    }, {
        key: "writeOptional",
        value: function writeOptional(os, tag, v) {
            os.writeOptionalProxy(tag, v);
        }
    }, {
        key: "readOptional",
        value: function readOptional(is, tag) {
            return is.readOptionalProxy(tag, this);
        }
    }, {
        key: "_instanceof",
        value: function _instanceof(T) {
            if (T === this) {
                return true;
            }

            for (var i in this._implements) {
                if (this._implements[i]._instanceof(T)) {
                    return true;
                }
            }

            return false;
        }
    }, {
        key: "ice_staticId",
        value: function ice_staticId() {
            return this._id;
        }
    }, {
        key: "minWireSize",
        get: function get() {
            return 2;
        }
    }, {
        key: "_implements",
        get: function get() {
            return [];
        }
    }]);

    return ObjectPrx;
}();

Ice.ObjectPrx = ObjectPrx;
module.exports.Ice = Ice;