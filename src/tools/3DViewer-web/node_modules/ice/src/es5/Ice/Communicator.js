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
Ice._ModuleRegistry.require(module, ["../Ice/Instance", "../Ice/UUID", "../Ice/AsyncResultBase"]);

var Instance = Ice.Instance;

//
// Ice.Communicator
//

var Communicator = function () {
    function Communicator(initData) {
        _classCallCheck(this, Communicator);

        this._instance = new Instance(initData);
    }

    //
    // Certain initialization tasks need to be completed after the
    // constructor.
    //


    _createClass(Communicator, [{
        key: "finishSetup",
        value: function finishSetup(promise) {
            this._instance.finishSetup(this, promise);
        }
    }, {
        key: "destroy",
        value: function destroy() {
            return this._instance.destroy();
        }
    }, {
        key: "shutdown",
        value: function shutdown() {
            this._instance.objectAdapterFactory().shutdown();
        }
    }, {
        key: "waitForShutdown",
        value: function waitForShutdown() {
            return this._instance.objectAdapterFactory().waitForShutdown();
        }
    }, {
        key: "isShutdown",
        value: function isShutdown() {
            return this._instance.objectAdapterFactory().isShutdown();
        }
    }, {
        key: "stringToProxy",
        value: function stringToProxy(s) {
            return this._instance.proxyFactory().stringToProxy(s);
        }
    }, {
        key: "proxyToString",
        value: function proxyToString(proxy) {
            return this._instance.proxyFactory().proxyToString(proxy);
        }
    }, {
        key: "propertyToProxy",
        value: function propertyToProxy(s) {
            return this._instance.proxyFactory().propertyToProxy(s);
        }
    }, {
        key: "proxyToProperty",
        value: function proxyToProperty(proxy, prefix) {
            return this._instance.proxyFactory().proxyToProperty(proxy, prefix);
        }
    }, {
        key: "stringToIdentity",
        value: function stringToIdentity(s) {
            return Ice.stringToIdentity(s);
        }
    }, {
        key: "identityToString",
        value: function identityToString(ident) {
            return Ice.identityToString(ident, this._instance.toStringMode());
        }
    }, {
        key: "createObjectAdapter",
        value: function createObjectAdapter(name) {
            var promise = new Ice.AsyncResultBase(this, "createObjectAdapter", this, null, null);
            this._instance.objectAdapterFactory().createObjectAdapter(name, null, promise);
            return promise;
        }
    }, {
        key: "createObjectAdapterWithEndpoints",
        value: function createObjectAdapterWithEndpoints(name, endpoints) {
            if (name.length === 0) {
                name = Ice.generateUUID();
            }

            this.getProperties().setProperty(name + ".Endpoints", endpoints);
            var promise = new Ice.AsyncResultBase(this, "createObjectAdapterWithEndpoints", this, null, null);
            this._instance.objectAdapterFactory().createObjectAdapter(name, null, promise);
            return promise;
        }
    }, {
        key: "createObjectAdapterWithRouter",
        value: function createObjectAdapterWithRouter(name, router) {
            var _this = this;

            if (name.length === 0) {
                name = Ice.generateUUID();
            }

            var promise = new Ice.AsyncResultBase(this, "createObjectAdapterWithRouter", this, null, null);

            //
            // We set the proxy properties here, although we still use the proxy supplied.
            //
            this.proxyToProperty(router, name + ".Router").forEach(function (value, key) {
                _this.getProperties().setProperty(key, value);
            });

            this._instance.objectAdapterFactory().createObjectAdapter(name, router, promise);
            return promise;
        }
    }, {
        key: "addObjectFactory",
        value: function addObjectFactory(factory, id) {
            this._instance.addObjectFactory(factory, id);
        }
    }, {
        key: "findObjectFactory",
        value: function findObjectFactory(id) {
            return this._instance.findObjectFactory(id);
        }
    }, {
        key: "getValueFactoryManager",
        value: function getValueFactoryManager() {
            return this._instance.initializationData().valueFactoryManager;
        }
    }, {
        key: "getImplicitContext",
        value: function getImplicitContext() {
            return this._instance.getImplicitContext();
        }
    }, {
        key: "getProperties",
        value: function getProperties() {
            return this._instance.initializationData().properties;
        }
    }, {
        key: "getLogger",
        value: function getLogger() {
            return this._instance.initializationData().logger;
        }
    }, {
        key: "getDefaultRouter",
        value: function getDefaultRouter() {
            return this._instance.referenceFactory().getDefaultRouter();
        }
    }, {
        key: "setDefaultRouter",
        value: function setDefaultRouter(router) {
            this._instance.setDefaultRouter(router);
        }
    }, {
        key: "getDefaultLocator",
        value: function getDefaultLocator() {
            return this._instance.referenceFactory().getDefaultLocator();
        }
    }, {
        key: "setDefaultLocator",
        value: function setDefaultLocator(locator) {
            this._instance.setDefaultLocator(locator);
        }
    }, {
        key: "flushBatchRequests",
        value: function flushBatchRequests() {
            return this._instance.outgoingConnectionFactory().flushAsyncBatchRequests();
        }
    }, {
        key: "instance",
        get: function get() {
            return this._instance;
        }
    }]);

    return Communicator;
}();

Ice.Communicator = Communicator;
module.exports.Ice = Ice;