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
Ice._ModuleRegistry.require(module, ["../Ice/ArrayUtil", "../Ice/Debug", "../Ice/HashMap", "../Ice/Promise", "../Ice/LocalException", "../Ice/Exception"]);

var ArrayUtil = Ice.ArrayUtil;
var Debug = Ice.Debug;
var HashMap = Ice.HashMap;

var RouterInfo = function () {
    function RouterInfo(router) {
        _classCallCheck(this, RouterInfo);

        this._router = router;

        Debug.assert(this._router !== null);

        this._clientEndpoints = null;
        this._serverEndpoints = null;
        this._adapter = null;
        this._identities = new HashMap(HashMap.compareEquals); // Set<Identity> = Map<Identity, 1>
        this._evictedIdentities = [];
        this._hasRoutingTable = false;
    }

    _createClass(RouterInfo, [{
        key: "destroy",
        value: function destroy() {
            this._clientEndpoints = [];
            this._serverEndpoints = [];
            this._adapter = null;
            this._identities.clear();
        }
    }, {
        key: "equals",
        value: function equals(rhs) {
            if (this === rhs) {
                return true;
            }

            if (rhs instanceof RouterInfo) {
                return this._router.equals(rhs._router);
            }

            return false;
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            return this._router.hashCode();
        }
    }, {
        key: "getRouter",
        value: function getRouter() {
            //
            // No mutex lock necessary, _router is immutable.
            //
            return this._router;
        }
    }, {
        key: "getClientEndpoints",
        value: function getClientEndpoints() {
            var _this = this;

            var promise = new Ice.Promise();
            if (this._clientEndpoints !== null) {
                promise.resolve(this._clientEndpoints);
            } else {
                this._router.getClientProxy().then(function (result) {
                    return _this.setClientEndpoints(result[0], result[1] !== undefined ? result[1] : true, promise);
                }).catch(promise.reject);
            }
            return promise;
        }
    }, {
        key: "getServerEndpoints",
        value: function getServerEndpoints() {
            var _this2 = this;

            if (this._serverEndpoints !== null) // Lazy initialization.
                {
                    return Ice.Promise.resolve(this._serverEndpoints);
                } else {
                return this._router.getServerProxy().then(function (proxy) {
                    return _this2.setServerEndpoints(proxy);
                });
            }
        }
    }, {
        key: "addProxy",
        value: function addProxy(proxy) {
            var _this3 = this;

            Debug.assert(proxy !== null);
            if (!this._hasRoutingTable) {
                return Ice.Promise.resolve(); // The router implementation doesn't maintain a routing table.
            } else if (this._identities.has(proxy.ice_getIdentity())) {
                //
                // Only add the proxy to the router if it's not already in our local map.
                //
                return Ice.Promise.resolve();
            } else {
                return this._router.addProxies([proxy]).then(function (evictedProxies) {
                    _this3.addAndEvictProxies(proxy, evictedProxies);
                });
            }
        }
    }, {
        key: "setAdapter",
        value: function setAdapter(adapter) {
            this._adapter = adapter;
        }
    }, {
        key: "getAdapter",
        value: function getAdapter() {
            return this._adapter;
        }
    }, {
        key: "clearCache",
        value: function clearCache(ref) {
            this._identities.delete(ref.getIdentity());
        }
    }, {
        key: "setClientEndpoints",
        value: function setClientEndpoints(clientProxy, hasRoutingTable, promise) {
            var _this4 = this;

            if (this._clientEndpoints === null) {
                this._hasRoutingTable = hasRoutingTable;
                if (clientProxy === null) {
                    //
                    // If getClientProxy() return nil, use router endpoints.
                    //
                    this._clientEndpoints = this._router._getReference().getEndpoints();
                    promise.resolve(this._clientEndpoints);
                } else {
                    clientProxy = clientProxy.ice_router(null); // The client proxy cannot be routed.

                    //
                    // In order to avoid creating a new connection to the
                    // router, we must use the same timeout as the already
                    // existing connection.
                    //
                    this._router.ice_getConnection().then(function (con) {
                        _this4._clientEndpoints = clientProxy.ice_timeout(con.timeout())._getReference().getEndpoints();
                        promise.resolve(_this4._clientEndpoints);
                    }).catch(promise.reject);
                }
            } else {
                promise.resolve(this._clientEndpoints);
            }
        }
    }, {
        key: "setServerEndpoints",
        value: function setServerEndpoints(serverProxy) {
            if (serverProxy === null) {
                throw new Ice.NoEndpointException();
            }

            serverProxy = serverProxy.ice_router(null); // The server proxy cannot be routed.
            this._serverEndpoints = serverProxy._getReference().getEndpoints();
            return this._serverEndpoints;
        }
    }, {
        key: "addAndEvictProxies",
        value: function addAndEvictProxies(proxy, evictedProxies) {
            var _this5 = this;

            //
            // Check if the proxy hasn't already been evicted by a
            // concurrent addProxies call. If it's the case, don't
            // add it to our local map.
            //
            var index = this._evictedIdentities.findIndex(function (e) {
                return e.equals(proxy.ice_getIdentity());
            });
            if (index >= 0) {
                this._evictedIdentities.splice(index, 1);
            } else {
                //
                // If we successfully added the proxy to the router,
                // we add it to our local map.
                //
                this._identities.set(proxy.ice_getIdentity(), 1);
            }

            //
            // We also must remove whatever proxies the router evicted.
            //
            evictedProxies.forEach(function (proxy) {
                _this5._identities.delete(proxy.ice_getIdentity());
            });
        }
    }]);

    return RouterInfo;
}();

Ice.RouterInfo = RouterInfo;
module.exports.Ice = Ice;