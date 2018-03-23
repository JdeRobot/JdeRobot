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

//
// NOTE: the protocol instance class is a bit different from other
// language mappinps since it also provides the secure property for
// the tranport. Since we don't support SSL but still want to be able
// to parse SSL endpoints, we simply re-use the TCP endpoint with a
// different protocol instance to support SSL endpoints.
//
// If SSL endpoints attributes were to diverge from TCP endpoints or
// if we want to support SSL, we'd have to change this and instead, do
// like in other mappings: have a separate implementation for the SSL
// endpoint and suppress the secure member of the protocol instance
// class bellow.
//

var ProtocolInstance = function () {
    function ProtocolInstance(instance, type, protocol, secure) {
        _classCallCheck(this, ProtocolInstance);

        this._instance = instance;
        this._traceLevel = instance.traceLevels().network;
        this._traceCategory = instance.traceLevels().networkCat;
        this._logger = instance.initializationData().logger;
        this._properties = instance.initializationData().properties;
        this._type = type;
        this._protocol = protocol;
        this._secure = secure;
    }

    _createClass(ProtocolInstance, [{
        key: "traceLevel",
        value: function traceLevel() {
            return this._traceLevel;
        }
    }, {
        key: "traceCategory",
        value: function traceCategory() {
            return this._traceCategory;
        }
    }, {
        key: "logger",
        value: function logger() {
            return this._logger;
        }
    }, {
        key: "protocol",
        value: function protocol() {
            return this._protocol;
        }
    }, {
        key: "type",
        value: function type() {
            return this._type;
        }
    }, {
        key: "secure",
        value: function secure() {
            return this._secure;
        }
    }, {
        key: "properties",
        value: function properties() {
            return this._properties;
        }
    }, {
        key: "defaultHost",
        value: function defaultHost() {
            return this._instance.defaultsAndOverrides().defaultHost;
        }
    }, {
        key: "defaultSourceAddress",
        value: function defaultSourceAddress() {
            return this._instance.defaultsAndOverrides().defaultSourceAddress;
        }
    }, {
        key: "defaultEncoding",
        value: function defaultEncoding() {
            return this._instance.defaultsAndOverrides().defaultEncoding;
        }
    }, {
        key: "defaultTimeout",
        value: function defaultTimeout() {
            return this._instance.defaultsAndOverrides().defaultTimeout;
        }
    }, {
        key: "messageSizeMax",
        value: function messageSizeMax() {
            return this._instance.messageSizeMax();
        }
    }]);

    return ProtocolInstance;
}();

Ice.ProtocolInstance = ProtocolInstance;
module.exports.Ice = Ice;