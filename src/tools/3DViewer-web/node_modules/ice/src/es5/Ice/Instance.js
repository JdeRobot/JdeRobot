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
Ice._ModuleRegistry.require(module, ["../Ice/AsyncResultBase", "../Ice/Debug", "../Ice/DefaultsAndOverrides", "../Ice/EndpointFactoryManager", "../Ice/ImplicitContextI", "../Ice/IdentityUtil", "../Ice/LocatorManager", "../Ice/Logger", "../Ice/ObjectAdapterFactory", "../Ice/ValueFactoryManagerI", "../Ice/OutgoingConnectionFactory", "../Ice/Promise", "../Ice/Properties", "../Ice/ProtocolInstance", "../Ice/ProxyFactory", "../Ice/RetryQueue", "../Ice/RouterManager", "../Ice/Timer", "../Ice/TraceLevels", "../Ice/TcpEndpointFactory", "../Ice/WSEndpointFactory", "../Ice/Reference", "../Ice/RequestHandlerFactory", "../Ice/LocalException", "../Ice/Exception", "../Ice/ProcessLogger", "../Ice/ACM", "../Ice/ToStringMode"]);

var IceSSL = Ice._ModuleRegistry.require(module, ["../Ice/EndpointInfo"]).IceSSL;

var AsyncResultBase = Ice.AsyncResultBase;
var Debug = Ice.Debug;
var DefaultsAndOverrides = Ice.DefaultsAndOverrides;
var EndpointFactoryManager = Ice.EndpointFactoryManager;
var ImplicitContextI = Ice.ImplicitContextI;
var LocatorManager = Ice.LocatorManager;
var Logger = Ice.Logger;
var ObjectAdapterFactory = Ice.ObjectAdapterFactory;
var ValueFactoryManagerI = Ice.ValueFactoryManagerI;
var OutgoingConnectionFactory = Ice.OutgoingConnectionFactory;
var Properties = Ice.Properties;
var ProxyFactory = Ice.ProxyFactory;
var RetryQueue = Ice.RetryQueue;
var RouterManager = Ice.RouterManager;
var Timer = Ice.Timer;
var TraceLevels = Ice.TraceLevels;
var ReferenceFactory = Ice.ReferenceFactory;
var RequestHandlerFactory = Ice.RequestHandlerFactory;
var ACMConfig = Ice.ACMConfig;

var StateActive = 0;
var StateDestroyInProgress = 1;
var StateDestroyed = 2;

//
// Instance - only for use by Communicator
//

var Instance = function () {
    function Instance(initData) {
        _classCallCheck(this, Instance);

        this._state = StateActive;
        this._initData = initData;

        this._traceLevels = null;
        this._defaultsAndOverrides = null;
        this._messageSizeMax = 0;
        this._batchAutoFlushSize = 0;
        this._clientACM = null;
        this._toStringMode = Ice.ToStringMode.Unicode;
        this._implicitContext = null;
        this._routerManager = null;
        this._locatorManager = null;
        this._referenceFactory = null;
        this._requestHandlerFactory = null;
        this._proxyFactory = null;
        this._outgoingConnectionFactory = null;
        this._objectAdapterFactory = null;
        this._retryQueue = null;
        this._endpointHostResolver = null;
        this._endpointFactoryManager = null;
        this._objectFactoryMap = null;
    }

    _createClass(Instance, [{
        key: "initializationData",
        value: function initializationData() {
            //
            // No check for destruction. It must be possible to access the
            // initialization data after destruction.
            //
            // This value is immutable.
            //
            return this._initData;
        }
    }, {
        key: "traceLevels",
        value: function traceLevels() {
            // This value is immutable.
            Debug.assert(this._traceLevels !== null);
            return this._traceLevels;
        }
    }, {
        key: "defaultsAndOverrides",
        value: function defaultsAndOverrides() {
            // This value is immutable.
            Debug.assert(this._defaultsAndOverrides !== null);
            return this._defaultsAndOverrides;
        }
    }, {
        key: "routerManager",
        value: function routerManager() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._routerManager !== null);
            return this._routerManager;
        }
    }, {
        key: "locatorManager",
        value: function locatorManager() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._locatorManager !== null);
            return this._locatorManager;
        }
    }, {
        key: "referenceFactory",
        value: function referenceFactory() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._referenceFactory !== null);
            return this._referenceFactory;
        }
    }, {
        key: "requestHandlerFactory",
        value: function requestHandlerFactory() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._requestHandlerFactory !== null);
            return this._requestHandlerFactory;
        }
    }, {
        key: "proxyFactory",
        value: function proxyFactory() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._proxyFactory !== null);
            return this._proxyFactory;
        }
    }, {
        key: "outgoingConnectionFactory",
        value: function outgoingConnectionFactory() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._outgoingConnectionFactory !== null);
            return this._outgoingConnectionFactory;
        }
    }, {
        key: "objectAdapterFactory",
        value: function objectAdapterFactory() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._objectAdapterFactory !== null);
            return this._objectAdapterFactory;
        }
    }, {
        key: "retryQueue",
        value: function retryQueue() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._retryQueue !== null);
            return this._retryQueue;
        }
    }, {
        key: "timer",
        value: function timer() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._timer !== null);
            return this._timer;
        }
    }, {
        key: "endpointFactoryManager",
        value: function endpointFactoryManager() {
            if (this._state === StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            Debug.assert(this._endpointFactoryManager !== null);
            return this._endpointFactoryManager;
        }
    }, {
        key: "messageSizeMax",
        value: function messageSizeMax() {
            // This value is immutable.
            return this._messageSizeMax;
        }
    }, {
        key: "batchAutoFlushSize",
        value: function batchAutoFlushSize() {
            // This value is immutable.
            return this._batchAutoFlushSize;
        }
    }, {
        key: "clientACM",
        value: function clientACM() {
            // This value is immutable.
            return this._clientACM;
        }
    }, {
        key: "toStringMode",
        value: function toStringMode() {
            // this value is immutable
            return this._toStringMode;
        }
    }, {
        key: "getImplicitContext",
        value: function getImplicitContext() {
            return this._implicitContext;
        }
    }, {
        key: "setDefaultLocator",
        value: function setDefaultLocator(locator) {
            if (this._state == StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            this._referenceFactory = this._referenceFactory.setDefaultLocator(locator);
        }
    }, {
        key: "setDefaultRouter",
        value: function setDefaultRouter(router) {
            if (this._state == StateDestroyed) {
                throw new Ice.CommunicatorDestroyedException();
            }

            this._referenceFactory = this._referenceFactory.setDefaultRouter(router);
        }
    }, {
        key: "setLogger",
        value: function setLogger(logger) {
            this._initData.logger = logger;
        }
    }, {
        key: "finishSetup",
        value: function finishSetup(communicator, promise) {
            //
            // If promise == null, it means the caller is requesting a synchronous setup.
            // Otherwise, we resolve the promise after all initialization is complete.
            //
            try {
                if (this._initData.properties === null) {
                    this._initData.properties = Properties.createProperties();
                }

                if (Ice._oneOfDone === undefined) {
                    Ice._printStackTraces = this._initData.properties.getPropertyAsIntWithDefault("Ice.PrintStackTraces", 0) > 0;

                    Ice._oneOfDone = true;
                }

                if (this._initData.logger === null) {
                    this._initData.logger = Ice.getProcessLogger();
                }

                this._traceLevels = new TraceLevels(this._initData.properties);

                this._defaultsAndOverrides = new DefaultsAndOverrides(this._initData.properties, this._initData.logger);

                var defMessageSizeMax = 1024;
                var num = this._initData.properties.getPropertyAsIntWithDefault("Ice.MessageSizeMax", defMessageSizeMax);
                if (num < 1 || num > 0x7fffffff / 1024) {
                    this._messageSizeMax = 0x7fffffff;
                } else {
                    this._messageSizeMax = num * 1024; // Property is in kilobytes, _messageSizeMax in bytes
                }

                if (this._initData.properties.getProperty("Ice.BatchAutoFlushSize").length === 0 && this._initData.properties.getProperty("Ice.BatchAutoFlush").length > 0) {
                    if (this._initData.properties.getPropertyAsInt("Ice.BatchAutoFlush") > 0) {
                        this._batchAutoFlushSize = this._messageSizeMax;
                    }
                } else {
                    num = this._initData.properties.getPropertyAsIntWithDefault("Ice.BatchAutoFlushSize", 1024); // 1MB
                    if (num < 1) {
                        this._batchAutoFlushSize = num;
                    } else if (num > 0x7fffffff / 1024) {
                        this._batchAutoFlushSize = 0x7fffffff;
                    } else {
                        this._batchAutoFlushSize = num * 1024; // Property is in kilobytes, _batchAutoFlushSize in bytes
                    }
                }

                this._clientACM = new ACMConfig(this._initData.properties, this._initData.logger, "Ice.ACM.Client", new ACMConfig(this._initData.properties, this._initData.logger, "Ice.ACM", new ACMConfig()));

                var toStringModeStr = this._initData.properties.getPropertyWithDefault("Ice.ToStringMode", "Unicode");
                if (toStringModeStr === "ASCII") {
                    this._toStringMode = Ice.ToStringMode.ASCII;
                } else if (toStringModeStr === "Compat") {
                    this._toStringMode = Ice.ToStringMode.Compat;
                } else if (toStringModeStr !== "Unicode") {
                    throw new Ice.InitializationException("The value for Ice.ToStringMode must be Unicode, ASCII or Compat");
                }

                this._implicitContext = ImplicitContextI.create(this._initData.properties.getProperty("Ice.ImplicitContext"));

                this._routerManager = new RouterManager();

                this._locatorManager = new LocatorManager(this._initData.properties);

                this._referenceFactory = new ReferenceFactory(this, communicator);

                this._requestHandlerFactory = new RequestHandlerFactory(this, communicator);

                this._proxyFactory = new ProxyFactory(this);

                this._endpointFactoryManager = new EndpointFactoryManager(this);

                var tcpInstance = new Ice.ProtocolInstance(this, Ice.TCPEndpointType, "tcp", false);
                var tcpEndpointFactory = new Ice.TcpEndpointFactory(tcpInstance);
                this._endpointFactoryManager.add(tcpEndpointFactory);

                var wsInstance = new Ice.ProtocolInstance(this, Ice.WSEndpointType, "ws", false);
                var wsEndpointFactory = new Ice.WSEndpointFactory(wsInstance, tcpEndpointFactory.clone(wsInstance));
                this._endpointFactoryManager.add(wsEndpointFactory);

                var sslInstance = new Ice.ProtocolInstance(this, Ice.SSLEndpointType, "ssl", true);
                var sslEndpointFactory = new Ice.TcpEndpointFactory(sslInstance);
                this._endpointFactoryManager.add(sslEndpointFactory);

                var wssInstance = new Ice.ProtocolInstance(this, Ice.WSSEndpointType, "wss", true);
                var wssEndpointFactory = new Ice.WSEndpointFactory(wssInstance, sslEndpointFactory.clone(wssInstance));
                this._endpointFactoryManager.add(wssEndpointFactory);

                this._outgoingConnectionFactory = new OutgoingConnectionFactory(communicator, this);

                if (this._initData.valueFactoryManager === null) {
                    this._initData.valueFactoryManager = new ValueFactoryManagerI();
                }

                this._objectAdapterFactory = new ObjectAdapterFactory(this, communicator);

                this._retryQueue = new RetryQueue(this);
                this._timer = new Timer(this._initData.logger);

                var router = Ice.RouterPrx.uncheckedCast(this._proxyFactory.propertyToProxy("Ice.Default.Router"));
                if (router !== null) {
                    this._referenceFactory = this._referenceFactory.setDefaultRouter(router);
                }

                var loc = Ice.LocatorPrx.uncheckedCast(this._proxyFactory.propertyToProxy("Ice.Default.Locator"));
                if (loc !== null) {
                    this._referenceFactory = this._referenceFactory.setDefaultLocator(loc);
                }

                if (promise !== null) {
                    promise.resolve(communicator);
                }
            } catch (ex) {
                if (promise !== null) {
                    if (ex instanceof Ice.LocalException) {
                        this.destroy().finally(function () {
                            return promise.reject(ex);
                        });
                    } else {
                        promise.reject(ex);
                    }
                } else {
                    if (ex instanceof Ice.LocalException) {
                        this.destroy();
                    }
                    throw ex;
                }
            }
        }

        //
        // Only for use by Ice.CommunicatorI
        //

    }, {
        key: "destroy",
        value: function destroy() {
            var _this = this;

            var promise = new AsyncResultBase(null, "destroy", null, this, null);

            //
            // If destroy is in progress, wait for it to be done. This is
            // necessary in case destroy() is called concurrently by
            // multiple threads.
            //
            if (this._state == StateDestroyInProgress) {
                if (!this._destroyPromises) {
                    this._destroyPromises = [];
                }
                this._destroyPromises.push(promise);
                return promise;
            }
            this._state = StateDestroyInProgress;

            //
            // Shutdown and destroy all the incoming and outgoing Ice
            // connections and wait for the connections to be finished.
            //
            Ice.Promise.try(function () {
                if (_this._objectAdapterFactory) {
                    return _this._objectAdapterFactory.shutdown();
                }
            }).then(function () {
                if (_this._outgoingConnectionFactory !== null) {
                    _this._outgoingConnectionFactory.destroy();
                }

                if (_this._objectAdapterFactory !== null) {
                    return _this._objectAdapterFactory.destroy();
                }
            }).then(function () {
                if (_this._outgoingConnectionFactory !== null) {
                    return _this._outgoingConnectionFactory.waitUntilFinished();
                }
            }).then(function () {
                if (_this._retryQueue) {
                    _this._retryQueue.destroy();
                }
                if (_this._timer) {
                    _this._timer.destroy();
                }

                if (_this._objectFactoryMap !== null) {
                    _this._objectFactoryMap.forEach(function (factory) {
                        return factory.destroy();
                    });
                    _this._objectFactoryMap.clear();
                }

                if (_this._routerManager) {
                    _this._routerManager.destroy();
                }
                if (_this._locatorManager) {
                    _this._locatorManager.destroy();
                }
                if (_this._endpointFactoryManager) {
                    _this._endpointFactoryManager.destroy();
                }

                if (_this._initData.properties.getPropertyAsInt("Ice.Warn.UnusedProperties") > 0) {
                    var unusedProperties = _this._initData.properties.getUnusedProperties();
                    if (unusedProperties.length > 0) {
                        var message = [];
                        message.push("The following properties were set but never read:");
                        unusedProperties.forEach(function (p) {
                            return message.push("\n    ", p);
                        });
                        _this._initData.logger.warning(message.join(""));
                    }
                }

                _this._objectAdapterFactory = null;
                _this._outgoingConnectionFactory = null;
                _this._retryQueue = null;
                _this._timer = null;

                _this._referenceFactory = null;
                _this._requestHandlerFactory = null;
                _this._proxyFactory = null;
                _this._routerManager = null;
                _this._locatorManager = null;
                _this._endpointFactoryManager = null;

                _this._state = StateDestroyed;

                if (_this._destroyPromises) {
                    _this._destroyPromises.forEach(function (p) {
                        return p.resolve();
                    });
                }
                promise.resolve();
            }).catch(function (ex) {
                if (_this._destroyPromises) {
                    _this._destroyPromises.forEach(function (p) {
                        return p.reject(ex);
                    });
                }
                promise.reject(ex);
            });
            return promise;
        }
    }, {
        key: "addObjectFactory",
        value: function addObjectFactory(factory, id) {
            //
            // Create a ValueFactory wrapper around the given ObjectFactory and register the wrapper
            // with the value factory manager. This may raise AlreadyRegisteredException.
            //
            this._initData.valueFactoryManager.add(function (typeId) {
                return factory.create(typeId);
            }, id);

            if (this._objectFactoryMap === null) {
                this._objectFactoryMap = new Map();
            }

            this._objectFactoryMap.set(id, factory);
        }
    }, {
        key: "findObjectFactory",
        value: function findObjectFactory(id) {
            var factory = null;
            if (this._objectFactoryMap !== null) {
                factory = this._objectFactoryMap.get(id);
            }
            return factory !== undefined ? factory : null;
        }
    }]);

    return Instance;
}();

Ice.Instance = Instance;
module.exports.Ice = Ice;