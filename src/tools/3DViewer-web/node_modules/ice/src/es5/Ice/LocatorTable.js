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
Ice._ModuleRegistry.require(module, ["../Ice/HashMap", "../Ice/Debug", "../Ice/IdentityUtil"]);

var HashMap = Ice.HashMap;
var Debug = Ice.Debug;

var EndpointTableEntry = function EndpointTableEntry(time, endpoints) {
    _classCallCheck(this, EndpointTableEntry);

    this.time = time;
    this.endpoints = endpoints;
};

var ReferenceTableEntry = function ReferenceTableEntry(time, reference) {
    _classCallCheck(this, ReferenceTableEntry);

    this.time = time;
    this.reference = reference;
};

var LocatorTable = function () {
    function LocatorTable() {
        _classCallCheck(this, LocatorTable);

        this._adapterEndpointsTable = new Map(); // Map<String, EndpointTableEntry>
        this._objectTable = new HashMap(HashMap.compareEquals); // Map<Ice.Identity, ReferenceTableEntry>
    }

    _createClass(LocatorTable, [{
        key: "clear",
        value: function clear() {
            this._adapterEndpointsTable.clear();
            this._objectTable.clear();
        }
    }, {
        key: "getAdapterEndpoints",
        value: function getAdapterEndpoints(adapter, ttl, cached) {
            if (ttl === 0) // Locator cache disabled.
                {
                    cached.value = false;
                    return null;
                }

            var entry = this._adapterEndpointsTable.get(adapter);
            if (entry !== undefined) {
                cached.value = this.checkTTL(entry.time, ttl);
                return entry.endpoints;
            }
            cached.value = false;
            return null;
        }
    }, {
        key: "addAdapterEndpoints",
        value: function addAdapterEndpoints(adapter, endpoints) {
            this._adapterEndpointsTable.set(adapter, new EndpointTableEntry(Date.now(), endpoints));
        }
    }, {
        key: "removeAdapterEndpoints",
        value: function removeAdapterEndpoints(adapter) {
            var entry = this._adapterEndpointsTable.get(adapter);
            this._adapterEndpointsTable.delete(adapter);
            return entry !== undefined ? entry.endpoints : null;
        }
    }, {
        key: "getObjectReference",
        value: function getObjectReference(id, ttl, cached) {
            if (ttl === 0) // Locator cache disabled.
                {
                    cached.value = false;
                    return null;
                }

            var entry = this._objectTable.get(id);
            if (entry !== undefined) {
                cached.value = this.checkTTL(entry.time, ttl);
                return entry.reference;
            }
            cached.value = false;
            return null;
        }
    }, {
        key: "addObjectReference",
        value: function addObjectReference(id, ref) {
            this._objectTable.set(id, new ReferenceTableEntry(Date.now(), ref));
        }
    }, {
        key: "removeObjectReference",
        value: function removeObjectReference(id) {
            var entry = this._objectTable.get(id);
            this._objectTable.delete(id);
            return entry !== undefined ? entry.reference : null;
        }
    }, {
        key: "checkTTL",
        value: function checkTTL(time, ttl) {
            Debug.assert(ttl !== 0);
            if (ttl < 0) // TTL = infinite
                {
                    return true;
                } else {
                return Date.now() - time <= ttl * 1000;
            }
        }
    }]);

    return LocatorTable;
}();

Ice.LocatorTable = LocatorTable;
module.exports.Ice = Ice;