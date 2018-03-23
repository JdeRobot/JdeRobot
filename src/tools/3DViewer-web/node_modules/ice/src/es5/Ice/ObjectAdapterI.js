var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

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
Ice._ModuleRegistry.require(module, ["../Ice/AsyncResultBase", "../Ice/Debug", "../Ice/Identity", "../Ice/LocalException", "../Ice/PropertyNames", "../Ice/Router", "../Ice/ServantManager", "../Ice/StringUtil", "../Ice/UUID"]);

var AsyncResultBase = Ice.AsyncResultBase;
var Debug = Ice.Debug;
var Identity = Ice.Identity;
var PropertyNames = Ice.PropertyNames;
var ServantManager = Ice.ServantManager;
var StringUtil = Ice.StringUtil;

var _suffixes = ["ACM", "AdapterId", "Endpoints", "Locator", "Locator.EncodingVersion", "Locator.EndpointSelection", "Locator.ConnectionCached", "Locator.PreferSecure", "Locator.CollocationOptimized", "Locator.Router", "MessageSizeMax", "PublishedEndpoints", "ReplicaGroupId", "Router", "Router.EncodingVersion", "Router.EndpointSelection", "Router.ConnectionCached", "Router.PreferSecure", "Router.CollocationOptimized", "Router.Locator", "Router.Locator.EndpointSelection", "Router.Locator.ConnectionCached", "Router.Locator.PreferSecure", "Router.Locator.CollocationOptimized", "Router.Locator.LocatorCacheTimeout", "Router.Locator.InvocationTimeout", "Router.LocatorCacheTimeout", "Router.InvocationTimeout", "ProxyOptions", "ThreadPool.Size", "ThreadPool.SizeMax", "ThreadPool.SizeWarn", "ThreadPool.StackSize", "ThreadPool.Serialize"];

var StateUninitialized = 0; // Just constructed.
//const StateHeld = 1;
//const StateWaitActivate = 2;
//const StateActive = 3;
//const StateDeactivating = 4;
var StateDeactivated = 5;
var StateDestroyed = 6;

//
// Only for use by IceInternal.ObjectAdapterFactory
//

var ObjectAdapterI = function () {
    function ObjectAdapterI(instance, communicator, objectAdapterFactory, name, router, noConfig, promise) {
        var _this = this;

        _classCallCheck(this, ObjectAdapterI);

        this._instance = instance;
        this._communicator = communicator;
        this._objectAdapterFactory = objectAdapterFactory;
        this._servantManager = new ServantManager(instance, name);
        this._name = name;
        this._routerEndpoints = [];
        this._routerInfo = null;
        this._state = StateUninitialized;
        this._noConfig = noConfig;

        if (this._noConfig) {
            this._reference = this._instance.referenceFactory().createFromString("dummy -t", "");
            this._messageSizeMax = this._instance.messageSizeMax();
            promise.resolve(this);
            return;
        }

        var properties = this._instance.initializationData().properties;
        var unknownProps = [];
        var noProps = this.filterProperties(unknownProps);

        //
        // Warn about unknown object adapter properties.
        //
        if (unknownProps.length !== 0 && properties.getPropertyAsIntWithDefault("Ice.Warn.UnknownProperties", 1) > 0) {
            var message = ["found unknown properties for object adapter `" + name + "':"];
            unknownProps.forEach(function (unknownProp) {
                return message.push("\n    " + unknownProp);
            });
            this._instance.initializationData().logger.warning(message.join(""));
        }

        //
        // Make sure named adapter has some configuration.
        //
        if (router === null && noProps) {
            throw new Ice.InitializationException("object adapter `" + this._name + "' requires configuration");
        }

        //
        // Setup a reference to be used to get the default proxy options
        // when creating new proxies. By default, create twoway proxies.
        //
        var proxyOptions = properties.getPropertyWithDefault(this._name + ".ProxyOptions", "-t");
        try {
            this._reference = this._instance.referenceFactory().createFromString("dummy " + proxyOptions, "");
        } catch (e) {
            if (e instanceof Ice.ProxyParseException) {
                throw new Ice.InitializationException("invalid proxy options `" + proxyOptions + "' for object adapter `" + name + "'");
            } else {
                throw e;
            }
        }

        {
            var defaultMessageSizeMax = this._instance.messageSizeMax() / 1024;
            var num = properties.getPropertyAsIntWithDefault(this._name + ".MessageSizeMax", defaultMessageSizeMax);
            if (num < 1 || num > 0x7fffffff / 1024) {
                this._messageSizeMax = 0x7fffffff;
            } else {
                this._messageSizeMax = num * 1024; // Property is in kilobytes, _messageSizeMax in bytes
            }
        }

        try {

            if (router === null) {
                router = Ice.RouterPrx.uncheckedCast(this._instance.proxyFactory().propertyToProxy(this._name + ".Router"));
            }
            if (router !== null) {
                this._routerInfo = this._instance.routerManager().find(router);
                Debug.assert(this._routerInfo !== null);

                //
                // Make sure this router is not already registered with another adapter.
                //
                if (this._routerInfo.getAdapter() !== null) {
                    throw new Ice.AlreadyRegisteredException("object adapter with router", Ice.identityToString(router.ice_getIdentity(), this._instance.toStringMode()));
                }

                //
                // Add the router's server proxy endpoints to this object
                // adapter.
                //
                this._routerInfo.getServerEndpoints().then(function (endpoints) {
                    endpoints.forEach(function (endpoint) {
                        return _this._routerEndpoints.push(endpoint);
                    });
                    _this._routerEndpoints.sort(function (e1, e2) {
                        return e1.compareTo(e2);
                    }); // Must be sorted.

                    //
                    // Remove duplicate endpoints, so we have a list of unique
                    // endpoints.
                    //
                    for (var i = 0; i < _this._routerEndpoints.length - 1;) {
                        if (_this._routerEndpoints[i].equals(_this._routerEndpoints[i + 1])) {
                            _this._routerEndpoints.splice(i, 1);
                        } else {
                            ++i;
                        }
                    }

                    //
                    // Associate this object adapter with the router. This way,
                    // new outgoing connections to the router's client proxy will
                    // use this object adapter for callbacks.
                    //
                    _this._routerInfo.setAdapter(_this);

                    //
                    // Also modify all existing outgoing connections to the
                    // router's client proxy to use this object adapter for
                    // callbacks.
                    //
                    return _this._instance.outgoingConnectionFactory().setRouterInfo(_this._routerInfo);
                }).then(function () {
                    return promise.resolve(_this);
                }, promise.reject);
            } else {
                var endpoints = properties.getProperty(this._name + ".Endpoints");
                if (endpoints.length > 0) {
                    throw new Ice.FeatureNotSupportedException("object adapter endpoints not supported");
                }
                promise.resolve(this);
            }
        } catch (ex) {
            this.destroy();
            throw ex;
        }
    }

    _createClass(ObjectAdapterI, [{
        key: "getName",
        value: function getName() {
            //
            // No mutex lock necessary, _name is immutable.
            //
            return this._noConfig ? "" : this._name;
        }
    }, {
        key: "getCommunicator",
        value: function getCommunicator() {
            return this._communicator;
        }
    }, {
        key: "activate",
        value: function activate() {}
    }, {
        key: "hold",
        value: function hold() {
            this.checkForDeactivation();
        }
    }, {
        key: "waitForHold",
        value: function waitForHold() {
            var promise = new AsyncResultBase(this._communicator, "waitForHold", null, null, this);
            return this.checkForDeactivation(promise) ? promise : promise.resolve();
        }
    }, {
        key: "deactivate",
        value: function deactivate() {
            var promise = new AsyncResultBase(this._communicator, "deactivate", null, null, this);
            if (this._state < StateDeactivated) {
                this._state = StateDeactivated;
                this._instance.outgoingConnectionFactory().removeAdapter(this);
            }
            promise.resolve();
            return promise;
        }
    }, {
        key: "waitForDeactivate",
        value: function waitForDeactivate() {
            return new AsyncResultBase(this._communicator, "deactivate", null, null, this).resolve();
        }
    }, {
        key: "isDeactivated",
        value: function isDeactivated() {
            return this._state >= StateDeactivated;
        }
    }, {
        key: "destroy",
        value: function destroy() {
            var _this2 = this;

            var promise = new AsyncResultBase(this._communicator, "destroy", null, null, this);
            var destroyInternal = function destroyInternal() {
                if (_this2._state < StateDestroyed) {
                    _this2._state = StateDestroyed;
                    _this2._servantManager.destroy();
                    _this2._objectAdapterFactory.removeObjectAdapter(_this2);
                }
                return promise.resolve();
            };

            return this._state < StateDeactivated ? this.deactivate().then(destroyInternal) : destroyInternal();
        }
    }, {
        key: "add",
        value: function add(object, ident) {
            return this.addFacet(object, ident, "");
        }
    }, {
        key: "addFacet",
        value: function addFacet(object, ident, facet) {
            this.checkForDeactivation();
            this.checkIdentity(ident);
            this.checkServant(object);

            //
            // Create a copy of the Identity argument, in case the caller
            // reuses it.
            //
            var id = ident.clone();

            this._servantManager.addServant(object, id, facet);

            return this.newProxy(id, facet);
        }
    }, {
        key: "addWithUUID",
        value: function addWithUUID(object) {
            return this.addFacetWithUUID(object, "");
        }
    }, {
        key: "addFacetWithUUID",
        value: function addFacetWithUUID(object, facet) {
            return this.addFacet(object, new Identity(Ice.generateUUID(), ""), facet);
        }
    }, {
        key: "addDefaultServant",
        value: function addDefaultServant(servant, category) {
            this.checkServant(servant);
            this.checkForDeactivation();

            this._servantManager.addDefaultServant(servant, category);
        }
    }, {
        key: "remove",
        value: function remove(ident) {
            return this.removeFacet(ident, "");
        }
    }, {
        key: "removeFacet",
        value: function removeFacet(ident, facet) {
            this.checkForDeactivation();
            this.checkIdentity(ident);

            return this._servantManager.removeServant(ident, facet);
        }
    }, {
        key: "removeAllFacets",
        value: function removeAllFacets(ident) {
            this.checkForDeactivation();
            this.checkIdentity(ident);

            return this._servantManager.removeAllFacets(ident);
        }
    }, {
        key: "removeDefaultServant",
        value: function removeDefaultServant(category) {
            this.checkForDeactivation();

            return this._servantManager.removeDefaultServant(category);
        }
    }, {
        key: "find",
        value: function find(ident) {
            return this.findFacet(ident, "");
        }
    }, {
        key: "findFacet",
        value: function findFacet(ident, facet) {
            this.checkForDeactivation();
            this.checkIdentity(ident);
            return this._servantManager.findServant(ident, facet);
        }
    }, {
        key: "findAllFacets",
        value: function findAllFacets(ident) {
            this.checkForDeactivation();
            this.checkIdentity(ident);
            return this._servantManager.findAllFacets(ident);
        }
    }, {
        key: "findByProxy",
        value: function findByProxy(proxy) {
            this.checkForDeactivation();
            var ref = proxy._getReference();
            return this.findFacet(ref.getIdentity(), ref.getFacet());
        }
    }, {
        key: "findDefaultServant",
        value: function findDefaultServant(category) {
            this.checkForDeactivation();
            return this._servantManager.findDefaultServant(category);
        }
    }, {
        key: "addServantLocator",
        value: function addServantLocator(locator, prefix) {
            this.checkForDeactivation();
            this._servantManager.addServantLocator(locator, prefix);
        }
    }, {
        key: "removeServantLocator",
        value: function removeServantLocator(prefix) {
            this.checkForDeactivation();
            return this._servantManager.removeServantLocator(prefix);
        }
    }, {
        key: "findServantLocator",
        value: function findServantLocator(prefix) {
            this.checkForDeactivation();
            return this._servantManager.findServantLocator(prefix);
        }
    }, {
        key: "createProxy",
        value: function createProxy(ident) {
            this.checkForDeactivation();
            this.checkIdentity(ident);
            return this.newProxy(ident, "");
        }
    }, {
        key: "createDirectProxy",
        value: function createDirectProxy(ident) {
            return this.createProxy(ident);
        }
    }, {
        key: "createIndirectProxy",
        value: function createIndirectProxy(ident) {
            throw new Ice.FeatureNotSupportedException("createIndirectProxy not supported");
        }
    }, {
        key: "setLocator",
        value: function setLocator(locator) {
            throw new Ice.FeatureNotSupportedException("setLocator not supported");
        }
    }, {
        key: "getEndpoints",
        value: function getEndpoints() {
            return [];
        }
    }, {
        key: "refreshPublishedEndpoints",
        value: function refreshPublishedEndpoints() {
            throw new Ice.FeatureNotSupportedException("refreshPublishedEndpoints not supported");
        }
    }, {
        key: "getPublishedEndpoints",
        value: function getPublishedEndpoints() {
            return [];
        }
    }, {
        key: "setPublishedEndpoints",
        value: function setPublishedEndpoints(newEndpoints) {
            throw new Ice.FeatureNotSupportedException("setPublishedEndpoints not supported");
        }
    }, {
        key: "getServantManager",
        value: function getServantManager() {
            //
            // _servantManager is immutable.
            //
            return this._servantManager;
        }
    }, {
        key: "messageSizeMax",
        value: function messageSizeMax() {
            return this._messageSizeMax;
        }
    }, {
        key: "newProxy",
        value: function newProxy(ident, facet) {
            //
            // Now we also add the endpoints of the router's server proxy, if
            // any. This way, object references created by this object adapter
            // will also point to the router's server proxy endpoints.
            //
            //
            // Create a reference and return a proxy for this reference.
            //
            return this._instance.proxyFactory().referenceToProxy(this._instance.referenceFactory().create(ident, facet, this._reference, Array.from(this._routerEndpoints)));
        }
    }, {
        key: "checkForDeactivation",
        value: function checkForDeactivation(promise) {
            if (this._state >= StateDeactivated) {
                var ex = new Ice.ObjectAdapterDeactivatedException();
                ex.name = this.getName();

                if (promise !== undefined) {
                    promise.reject(ex);
                    return true;
                } else {
                    throw ex;
                }
            }

            return false;
        }
    }, {
        key: "checkIdentity",
        value: function checkIdentity(ident) {
            if (ident.name === undefined || ident.name === null || ident.name.length === 0) {
                throw new Ice.IllegalIdentityException(ident);
            }

            if (ident.category === undefined || ident.category === null) {
                ident.category = "";
            }
        }
    }, {
        key: "checkServant",
        value: function checkServant(servant) {
            if (servant === undefined || servant === null) {
                throw new Ice.IllegalServantException("cannot add null servant to Object Adapter");
            }
        }
    }, {
        key: "filterProperties",
        value: function filterProperties(unknownProps) {
            //
            // Do not create unknown properties list if Ice prefix, i.e., Ice, Glacier2, etc.
            //
            var addUnknown = true;
            var prefix = this._name + ".";
            for (var i = 0; i < PropertyNames.clPropNames.length; ++i) {
                if (prefix.indexOf(PropertyNames.clPropNames[i] + ".") === 0) {
                    addUnknown = false;
                    break;
                }
            }

            var noProps = true;
            var props = this._instance.initializationData().properties.getPropertiesForPrefix(prefix);
            var _iteratorNormalCompletion = true;
            var _didIteratorError = false;
            var _iteratorError = undefined;

            try {
                for (var _iterator = props[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                    var _step$value = _slicedToArray(_step.value, 2),
                        key = _step$value[0],
                        value = _step$value[1];

                    var valid = false;
                    for (var _i = 0; _i < _suffixes.length; ++_i) {
                        if (key === prefix + _suffixes[_i]) {
                            noProps = false;
                            valid = true;
                            break;
                        }
                    }

                    if (!valid && addUnknown) {
                        unknownProps.push(key);
                    }
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

            return noProps;
        }
    }]);

    return ObjectAdapterI;
}();

Ice.ObjectAdapterI = ObjectAdapterI;
module.exports.Ice = Ice;