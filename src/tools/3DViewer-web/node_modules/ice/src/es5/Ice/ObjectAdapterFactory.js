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
Ice._ModuleRegistry.require(module, ["../Ice/AsyncResultBase", "../Ice/LocalException", "../Ice/ObjectAdapterI", "../Ice/Promise", "../Ice/UUID"]);

var AsyncResultBase = Ice.AsyncResultBase;
var ObjectAdapterI = Ice.ObjectAdapterI;
var _Promise = Ice.Promise;

//
// Only for use by Instance.
//

var ObjectAdapterFactory = function () {
    function ObjectAdapterFactory(instance, communicator) {
        _classCallCheck(this, ObjectAdapterFactory);

        this._instance = instance;
        this._communicator = communicator;
        this._adapters = [];
        this._adapterNamesInUse = [];
        this._shutdownPromise = new _Promise();
    }

    _createClass(ObjectAdapterFactory, [{
        key: "shutdown",
        value: function shutdown() {
            var _this = this;

            //
            // Ignore shutdown requests if the object adapter factory has
            // already been shut down.
            //
            if (this._instance === null) {
                return this._shutdownPromise;
            }

            this._instance = null;
            this._communicator = null;
            _Promise.all(this._adapters.map(function (adapter) {
                return adapter.deactivate();
            })).then(function () {
                return _this._shutdownPromise.resolve();
            });
            return this._shutdownPromise;
        }
    }, {
        key: "waitForShutdown",
        value: function waitForShutdown() {
            var _this2 = this;

            return this._shutdownPromise.then(function () {
                return _Promise.all(_this2._adapters.map(function (adapter) {
                    return adapter.waitForDeactivate();
                }));
            });
        }
    }, {
        key: "isShutdown",
        value: function isShutdown() {
            return this._instance === null;
        }
    }, {
        key: "destroy",
        value: function destroy() {
            var _this3 = this;

            return this.waitForShutdown().then(function () {
                return _Promise.all(_this3._adapters.map(function (adapter) {
                    return adapter.destroy();
                }));
            });
        }
    }, {
        key: "createObjectAdapter",
        value: function createObjectAdapter(name, router, promise) {
            if (this._instance === null) {
                throw new Ice.ObjectAdapterDeactivatedException();
            }

            var adapter = null;
            try {
                if (name.length === 0) {
                    adapter = new ObjectAdapterI(this._instance, this._communicator, this, Ice.generateUUID(), null, true, promise);
                } else {
                    if (this._adapterNamesInUse.indexOf(name) !== -1) {
                        throw new Ice.AlreadyRegisteredException("object adapter", name);
                    }
                    adapter = new ObjectAdapterI(this._instance, this._communicator, this, name, router, false, promise);
                    this._adapterNamesInUse.push(name);
                }
                this._adapters.push(adapter);
            } catch (ex) {
                promise.reject(ex);
            }
        }
    }, {
        key: "removeObjectAdapter",
        value: function removeObjectAdapter(adapter) {
            if (this._instance === null) {
                return;
            }

            var n = this._adapters.indexOf(adapter);
            if (n !== -1) {
                this._adapters.splice(n, 1);
            }

            n = this._adapterNamesInUse.indexOf(adapter.getName());
            if (n !== -1) {
                this._adapterNamesInUse.splice(n, 1);
            }
        }
    }]);

    return ObjectAdapterFactory;
}();

Ice.ObjectAdapterFactory = ObjectAdapterFactory;
module.exports.Ice = Ice;