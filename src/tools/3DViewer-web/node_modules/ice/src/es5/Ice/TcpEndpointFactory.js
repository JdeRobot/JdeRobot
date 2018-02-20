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

var Ice = require("../Ice/TcpEndpointI").Ice;
var TcpEndpointI = Ice.TcpEndpointI;

var TcpEndpointFactory = function () {
    function TcpEndpointFactory(instance) {
        _classCallCheck(this, TcpEndpointFactory);

        this._instance = instance;
    }

    _createClass(TcpEndpointFactory, [{
        key: "type",
        value: function type() {
            return this._instance.type();
        }
    }, {
        key: "protocol",
        value: function protocol() {
            return this._instance.protocol();
        }
    }, {
        key: "create",
        value: function create(args, oaEndpoint) {
            var e = new TcpEndpointI(this._instance);
            e.initWithOptions(args, oaEndpoint);
            return e;
        }
    }, {
        key: "read",
        value: function read(s) {
            var e = new TcpEndpointI(this._instance);
            e.initWithStream(s);
            return e;
        }
    }, {
        key: "destroy",
        value: function destroy() {
            this._instance = null;
        }
    }, {
        key: "clone",
        value: function clone(instance) {
            return new TcpEndpointFactory(instance);
        }
    }]);

    return TcpEndpointFactory;
}();

Ice.TcpEndpointFactory = TcpEndpointFactory;
module.exports.Ice = Ice;