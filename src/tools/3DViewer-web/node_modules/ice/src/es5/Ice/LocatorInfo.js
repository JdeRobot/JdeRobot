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
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/Promise", "../Ice/Protocol", "../Ice/Locator", "../Ice/LocalException", "../Ice/Exception", "../Ice/HashMap", "../Ice/IdentityUtil"]);

var Debug = Ice.Debug;
var Protocol = Ice.Protocol;
var LocatorRegistryPrx = Ice.LocatorRegisterPrx;
var HashMap = Ice.HashMap;

var LocatorInfo = function () {
    function LocatorInfo(locator, table, background) {
        _classCallCheck(this, LocatorInfo);

        this._locator = locator;
        this._locatorRegistry = null;
        this._table = table;
        this._background = background;

        this._adapterRequests = new Map(); // Map<String, Request>
        this._objectRequests = new HashMap(HashMap.compareEquals); // Map<Ice.Identity, Request>
    }

    _createClass(LocatorInfo, [{
        key: "destroy",
        value: function destroy() {
            this._locatorRegistry = null;
            this._table.clear();
        }
    }, {
        key: "equals",
        value: function equals(rhs) {
            if (this === rhs) {
                return true;
            }

            if (rhs instanceof LocatorInfo) {
                return this._locator.equals(rhs._locator);
            }

            return false;
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            return this._locator.hashCode();
        }
    }, {
        key: "getLocator",
        value: function getLocator() {
            return this._locator;
        }
    }, {
        key: "getLocatorRegistry",
        value: function getLocatorRegistry() {
            var _this = this;

            if (this._locatorRegistry !== null) {
                return Ice.Promise.resolve(this._locatorRegistry);
            }

            return this._locator.getRegistry().then(function (reg) {
                //
                // The locator registry can't be located. We use ordered
                // endpoint selection in case the locator returned a proxy
                // with some endpoints which are prefered to be tried first.
                //
                _this._locatorRegistry = LocatorRegistryPrx.uncheckedCast(reg.ice_locator(null).ice_endpointSelection(Ice.EndpointSelectionType.Ordered));
                return _this._locatorRegistry;
            });
        }
    }, {
        key: "getEndpoints",
        value: function getEndpoints(ref, wellKnownRef, ttl, p) {
            var promise = p || new Ice.Promise(); // success callback receives (endpoints, cached)

            Debug.assert(ref.isIndirect());
            var endpoints = null;
            var cached = { value: false };
            if (!ref.isWellKnown()) {
                endpoints = this._table.getAdapterEndpoints(ref.getAdapterId(), ttl, cached);
                if (!cached.value) {
                    if (this._background && endpoints !== null) {
                        this.getAdapterRequest(ref).addCallback(ref, wellKnownRef, ttl, null);
                    } else {
                        this.getAdapterRequest(ref).addCallback(ref, wellKnownRef, ttl, promise);
                        return promise;
                    }
                }
            } else {
                var r = this._table.getObjectReference(ref.getIdentity(), ttl, cached);
                if (!cached.value) {
                    if (this._background && r !== null) {
                        this.getObjectRequest(ref).addCallback(ref, null, ttl, null);
                    } else {
                        this.getObjectRequest(ref).addCallback(ref, null, ttl, promise);
                        return promise;
                    }
                }

                if (!r.isIndirect()) {
                    endpoints = r.getEndpoints();
                } else if (!r.isWellKnown()) {
                    if (ref.getInstance().traceLevels().location >= 1) {
                        this.traceWellKnown("found adapter for well-known object in locator cache", ref, r);
                    }
                    this.getEndpoints(r, ref, ttl, promise);
                    return promise;
                }
            }

            Debug.assert(endpoints !== null);
            if (ref.getInstance().traceLevels().location >= 1) {
                this.getEndpointsTrace(ref, endpoints, true);
            }
            promise.resolve([endpoints, true]);

            return promise;
        }
    }, {
        key: "clearCache",
        value: function clearCache(ref) {
            Debug.assert(ref.isIndirect());

            if (!ref.isWellKnown()) {
                var endpoints = this._table.removeAdapterEndpoints(ref.getAdapterId());

                if (endpoints !== null && ref.getInstance().traceLevels().location >= 2) {
                    this.trace("removed endpoints for adapter from locator cache", ref, endpoints);
                }
            } else {
                var r = this._table.removeObjectReference(ref.getIdentity());
                if (r !== null) {
                    if (!r.isIndirect()) {
                        if (ref.getInstance().traceLevels().location >= 2) {
                            this.trace("removed endpoints for well-known object from locator cache", ref, r.getEndpoints());
                        }
                    } else if (!r.isWellKnown()) {
                        if (ref.getInstance().traceLevels().location >= 2) {
                            this.traceWellKnown("removed adapter for well-known object from locator cache", ref, r);
                        }
                        this.clearCache(r);
                    }
                }
            }
        }
    }, {
        key: "trace",
        value: function trace(msg, ref, endpoints) {
            Debug.assert(ref.isIndirect());

            var s = [];
            s.push(msg);
            s.push("\n");
            if (!ref.isWellKnown()) {
                s.push("adapter = ");
                s.push(ref.getAdapterId());
                s.push("\n");
            } else {
                s.push("well-known proxy = ");
                s.push(ref.toString());
                s.push("\n");
            }

            s.push("endpoints = ");
            s.push(endpoints.map(function (e) {
                return e.toString();
            }).join(":"));
            ref.getInstance().initializationData().logger.trace(ref.getInstance().traceLevels().locationCat, s.join(""));
        }
    }, {
        key: "traceWellKnown",
        value: function traceWellKnown(msg, ref, resolved) {
            Debug.assert(ref.isWellKnown());

            var s = [];
            s.push(msg);
            s.push("\n");
            s.push("well-known proxy = ");
            s.push(ref.toString());
            s.push("\n");

            s.push("adapter = ");
            s.push(resolved.getAdapterId());
            ref.getInstance().initializationData().logger.trace(ref.getInstance().traceLevels().locationCat, s.join(""));
        }
    }, {
        key: "getEndpointsException",
        value: function getEndpointsException(ref, exc) {
            Debug.assert(ref.isIndirect());

            var instance = ref.getInstance();
            try {
                throw exc;
            } catch (ex) {
                if (ex instanceof Ice.AdapterNotFoundException) {
                    if (instance.traceLevels().location >= 1) {
                        var s = [];
                        s.push("adapter not found\n");
                        s.push("adapter = ");
                        s.push(ref.getAdapterId());
                        instance.initializationData().logger.trace(instance.traceLevels().locationCat, s.join(""));
                    }

                    var e = new Ice.NotRegisteredException();
                    e.kindOfObject = "object adapter";
                    e.id = ref.getAdapterId();
                    throw e;
                } else if (ex instanceof Ice.ObjectNotFoundException) {
                    if (instance.traceLevels().location >= 1) {
                        var _s = [];
                        _s.push("object not found\n");
                        _s.push("object = ");
                        _s.push(Ice.identityToString(ref.getIdentity(), instance.toStringMode()));
                        instance.initializationData().logger.trace(instance.traceLevels().locationCat, _s.join(""));
                    }

                    var _e = new Ice.NotRegisteredException();
                    _e.kindOfObject = "object";
                    _e.id = Ice.identityToString(ref.getIdentity(), instance.toStringMode());
                    throw _e;
                } else if (ex instanceof Ice.NotRegisteredException) {
                    throw ex;
                } else if (ex instanceof Ice.LocalException) {
                    if (instance.traceLevels().location >= 1) {
                        var _s2 = [];
                        _s2.push("couldn't contact the locator to retrieve endpoints\n");
                        if (ref.getAdapterId().length > 0) {
                            _s2.push("adapter = ");
                            _s2.push(ref.getAdapterId());
                            _s2.push("\n");
                        } else {
                            _s2.push("well-known proxy = ");
                            _s2.push(ref.toString());
                            _s2.push("\n");
                        }
                        _s2.push("reason = " + ex.toString());
                        instance.initializationData().logger.trace(instance.traceLevels().locationCat, _s2.join(""));
                    }
                    throw ex;
                } else {
                    Debug.assert(false);
                }
            }
        }
    }, {
        key: "getEndpointsTrace",
        value: function getEndpointsTrace(ref, endpoints, cached) {
            if (endpoints !== null && endpoints.length > 0) {
                if (cached) {
                    if (ref.isWellKnown()) {
                        this.trace("found endpoints for well-known proxy in locator cache", ref, endpoints);
                    } else {
                        this.trace("found endpoints for adapter in locator cache", ref, endpoints);
                    }
                } else {
                    if (ref.isWellKnown()) {
                        this.trace("retrieved endpoints for well-known proxy from locator, adding to locator cache", ref, endpoints);
                    } else {
                        this.trace("retrieved endpoints for adapter from locator, adding to locator cache", ref, endpoints);
                    }
                }
            } else {
                var instance = ref.getInstance();
                var s = [];
                s.push("no endpoints configured for ");
                if (ref.getAdapterId().length > 0) {
                    s.push("adapter\n");
                    s.push("adapter = ");
                    s.push(ref.getAdapterId());
                    s.push("\n");
                } else {
                    s.push("well-known object\n");
                    s.push("well-known proxy = ");
                    s.push(ref.toString());
                    s.push("\n");
                }
                instance.initializationData().logger.trace(instance.traceLevels().locationCat, s.join(""));
            }
        }
    }, {
        key: "getAdapterRequest",
        value: function getAdapterRequest(ref) {
            if (ref.getInstance().traceLevels().location >= 1) {
                var instance = ref.getInstance();
                var s = [];
                s.push("searching for adapter by id\n");
                s.push("adapter = ");
                s.push(ref.getAdapterId());
                instance.initializationData().logger.trace(instance.traceLevels().locationCat, s.join(""));
            }

            var request = this._adapterRequests.get(ref.getAdapterId());
            if (request !== undefined) {
                return request;
            }
            request = new AdapterRequest(this, ref);
            this._adapterRequests.set(ref.getAdapterId(), request);
            return request;
        }
    }, {
        key: "getObjectRequest",
        value: function getObjectRequest(ref) {
            if (ref.getInstance().traceLevels().location >= 1) {
                var instance = ref.getInstance();
                var s = [];
                s.push("searching for well-known object\n");
                s.push("well-known proxy = ");
                s.push(ref.toString());
                instance.initializationData().logger.trace(instance.traceLevels().locationCat, s.join(""));
            }

            var request = this._objectRequests.get(ref.getIdentity());
            if (request !== undefined) {
                return request;
            }
            request = new ObjectRequest(this, ref);
            this._objectRequests.set(ref.getIdentity(), request);
            return request;
        }
    }, {
        key: "finishRequest",
        value: function finishRequest(ref, wellKnownRefs, proxy, notRegistered) {
            if (proxy === null || proxy._getReference().isIndirect()) {
                //
                // Remove the cached references of well-known objects for which we tried
                // to resolved the endpoints if these endpoints are empty.
                //
                for (var i = 0; i < wellKnownRefs.length; ++i) {
                    this._table.removeObjectReference(wellKnownRefs[i].getIdentity());
                }
            }

            if (!ref.isWellKnown()) {
                if (proxy !== null && !proxy._getReference().isIndirect()) {
                    // Cache the adapter endpoints.
                    this._table.addAdapterEndpoints(ref.getAdapterId(), proxy._getReference().getEndpoints());
                } else if (notRegistered) // If the adapter isn't registered anymore, remove it from the cache.
                    {
                        this._table.removeAdapterEndpoints(ref.getAdapterId());
                    }

                Debug.assert(this._adapterRequests.has(ref.getAdapterId()));
                this._adapterRequests.delete(ref.getAdapterId());
            } else {
                if (proxy !== null && !proxy._getReference().isWellKnown()) {
                    // Cache the well-known object reference.
                    this._table.addObjectReference(ref.getIdentity(), proxy._getReference());
                } else if (notRegistered) // If the well-known object isn't registered anymore, remove it from the cache.
                    {
                        this._table.removeObjectReference(ref.getIdentity());
                    }

                Debug.assert(this._objectRequests.has(ref.getIdentity()));
                this._objectRequests.delete(ref.getIdentity());
            }
        }
    }]);

    return LocatorInfo;
}();

Ice.LocatorInfo = LocatorInfo;

var RequestCallback = function () {
    function RequestCallback(ref, ttl, promise) {
        _classCallCheck(this, RequestCallback);

        this._ref = ref;
        this._ttl = ttl;
        this._promise = promise;
    }

    _createClass(RequestCallback, [{
        key: "response",
        value: function response(locatorInfo, proxy) {
            var _this2 = this;

            var endpoints = null;
            if (proxy !== null) {
                var r = proxy._getReference();
                if (this._ref.isWellKnown() && !Protocol.isSupported(this._ref.getEncoding(), r.getEncoding())) {
                    //
                    // If a well-known proxy and the returned proxy
                    // encoding isn't supported, we're done: there's
                    // no compatible endpoint we can use.
                    //
                } else if (!r.isIndirect()) {
                    endpoints = r.getEndpoints();
                } else if (this._ref.isWellKnown() && !r.isWellKnown()) {
                    //
                    // We're resolving the endpoints of a well-known object and the proxy returned
                    // by the locator is an indirect proxy. We now need to resolve the endpoints
                    // of this indirect proxy.
                    //
                    if (this._ref.getInstance().traceLevels().location >= 1) {
                        locatorInfo.traceWellKnown("retrieved adapter for well-known object from locator, " + "adding to locator cache", this._ref, r);
                    }
                    locatorInfo.getEndpoints(r, this._ref, this._ttl).then(function (values) {
                        if (_this2._promise !== null) {
                            _this2._promise.resolve(values);
                        }
                    }, function (ex) {
                        if (_this2._promise !== null) {
                            _this2._promise.reject(ex);
                        }
                    });
                    return;
                }
            }

            if (this._ref.getInstance().traceLevels().location >= 1) {
                locatorInfo.getEndpointsTrace(this._ref, endpoints, false);
            }

            if (this._promise !== null) {
                this._promise.resolve(endpoints === null ? [[], false] : [endpoints, false]);
            }
        }
    }, {
        key: "exception",
        value: function exception(locatorInfo, exc) {
            try {
                locatorInfo.getEndpointsException(this._ref, exc); // This throws.
            } catch (ex) {
                if (this._promise !== null) {
                    this._promise.reject(ex);
                }
            }
        }
    }]);

    return RequestCallback;
}();

var Request = function () {
    function Request(locatorInfo, ref) {
        _classCallCheck(this, Request);

        this._locatorInfo = locatorInfo;
        this._ref = ref;

        this._callbacks = []; // Array<RequestCallback>
        this._wellKnownRefs = []; // Array<Reference>
        this._sent = false;
        this._response = false;
        this._proxy = null;
        this._exception = null;
    }

    _createClass(Request, [{
        key: "addCallback",
        value: function addCallback(ref, wellKnownRef, ttl, promise) {
            var callback = new RequestCallback(ref, ttl, promise);
            if (this._response) {
                callback.response(this._locatorInfo, this._proxy);
            } else if (this._exception !== null) {
                callback.exception(this._locatorInfo, this._exception);
            } else {
                this._callbacks.push(callback);
                if (wellKnownRef !== null) // This request is to resolve the endpoints of a cached well-known object ref
                    {
                        this._wellKnownRefs.push(wellKnownRef);
                    }
                if (!this._sent) {
                    this._sent = true;
                    this.send();
                }
            }
        }
    }, {
        key: "response",
        value: function response(proxy) {
            this._locatorInfo.finishRequest(this._ref, this._wellKnownRefs, proxy, false);
            this._response = true;
            this._proxy = proxy;
            for (var i = 0; i < this._callbacks.length; ++i) {
                this._callbacks[i].response(this._locatorInfo, proxy);
            }
        }
    }, {
        key: "exception",
        value: function exception(ex) {
            this._locatorInfo.finishRequest(this._ref, this._wellKnownRefs, null, ex instanceof Ice.UserException);
            this._exception = ex;
            for (var i = 0; i < this._callbacks.length; ++i) {
                this._callbacks[i].exception(this._locatorInfo, ex);
            }
        }
    }]);

    return Request;
}();

var ObjectRequest = function (_Request) {
    _inherits(ObjectRequest, _Request);

    function ObjectRequest(locatorInfo, reference) {
        _classCallCheck(this, ObjectRequest);

        var _this3 = _possibleConstructorReturn(this, (ObjectRequest.__proto__ || Object.getPrototypeOf(ObjectRequest)).call(this, locatorInfo, reference));

        Debug.assert(reference.isWellKnown());
        return _this3;
    }

    _createClass(ObjectRequest, [{
        key: "send",
        value: function send() {
            var _this4 = this;

            try {
                this._locatorInfo.getLocator().findObjectById(this._ref.getIdentity()).then(function (proxy) {
                    return _this4.response(proxy);
                }, function (ex) {
                    return _this4.exception(ex);
                });
            } catch (ex) {
                this.exception(ex);
            }
        }
    }]);

    return ObjectRequest;
}(Request);

var AdapterRequest = function (_Request2) {
    _inherits(AdapterRequest, _Request2);

    function AdapterRequest(locatorInfo, reference) {
        _classCallCheck(this, AdapterRequest);

        var _this5 = _possibleConstructorReturn(this, (AdapterRequest.__proto__ || Object.getPrototypeOf(AdapterRequest)).call(this, locatorInfo, reference));

        Debug.assert(reference.isIndirect());
        return _this5;
    }

    _createClass(AdapterRequest, [{
        key: "send",
        value: function send() {
            var _this6 = this;

            try {
                this._locatorInfo.getLocator().findAdapterById(this._ref.getAdapterId()).then(function (proxy) {
                    return _this6.response(proxy);
                }, function (ex) {
                    return _this6.exception(ex);
                });
            } catch (ex) {
                this.exception(ex);
            }
        }
    }]);

    return AdapterRequest;
}(Request);

module.exports.Ice = Ice;