var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

var Ice = require("../Ice/ModuleRegistry").Ice;

Ice._ModuleRegistry.require(module, ["../Ice/HashUtil", "../Ice/StringUtil", "../Ice/EndpointI", "../Ice/LocalException", "../Ice/WSTransceiver", "../Ice/EndpointInfo"]);

var HashUtil = Ice.HashUtil;
var StringUtil = Ice.StringUtil;
var EndpointI = Ice.EndpointI;

var WSEndpoint = function (_EndpointI) {
    _inherits(WSEndpoint, _EndpointI);

    function WSEndpoint(instance, del, re) {
        _classCallCheck(this, WSEndpoint);

        var _this = _possibleConstructorReturn(this, (WSEndpoint.__proto__ || Object.getPrototypeOf(WSEndpoint)).call(this));

        _this._instance = instance;
        _this._delegate = del;
        _this._resource = re || "/";
        return _this;
    }

    _createClass(WSEndpoint, [{
        key: "getInfo",
        value: function getInfo() {
            var info = new Ice.WSEndpointInfo();
            info.resource = this._resource;
            info.underlying = this._delegate.getInfo();
            info.timeout = info.underlying.timeout;
            info.compress = info.underlying.compress;
            return info;
        }
    }, {
        key: "type",
        value: function type() {
            return this._delegate.type();
        }
    }, {
        key: "protocol",
        value: function protocol() {
            return this._delegate.protocol();
        }
    }, {
        key: "streamWrite",
        value: function streamWrite(s) {
            s.startEncapsulation();
            this._delegate.streamWriteImpl(s);
            s.writeString(this._resource);
            s.endEncapsulation();
        }
    }, {
        key: "timeout",
        value: function timeout() {
            return this._delegate.timeout();
        }
    }, {
        key: "changeTimeout",
        value: function changeTimeout(timeout) {
            if (timeout === this._delegate.timeout()) {
                return this;
            } else {
                return new WSEndpoint(this._instance, this._delegate.changeTimeout(timeout), this._resource);
            }
        }
    }, {
        key: "changeConnectionId",
        value: function changeConnectionId(connectionId) {
            if (connectionId === this._delegate.connectionId()) {
                return this;
            } else {
                return new WSEndpoint(this._instance, this._delegate.changeConnectionId(connectionId), this._resource);
            }
        }
    }, {
        key: "compress",
        value: function compress() {
            return this._delegate.compress();
        }
    }, {
        key: "changeCompress",
        value: function changeCompress(compress) {
            if (compress === this._delegate.compress()) {
                return this;
            } else {
                return new WSEndpoint(this._instance, this._delegate.changeCompress(compress), this._resource);
            }
        }
    }, {
        key: "datagram",
        value: function datagram() {
            return this._delegate.datagram();
        }
    }, {
        key: "secure",
        value: function secure() {
            return this._delegate.secure();
        }
    }, {
        key: "connect",
        value: function connect() {
            return Ice.WSTransceiver.createOutgoing(this._instance, this._delegate.secure(), this._delegate.getAddress(), this._resource);
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            if (this._hashCode === undefined) {
                this._hashCode = this._delegate.hashCode();
                this._hashCode = HashUtil.addString(this._hashCode, this._resource);
            }
            return this._hashCode;
        }
    }, {
        key: "compareTo",
        value: function compareTo(p) {
            if (this === p) {
                return 0;
            }

            if (p === null) {
                return 1;
            }

            if (!(p instanceof WSEndpoint)) {
                return this.type() < p.type() ? -1 : 1;
            }

            var r = this._delegate.compareTo(p._delegate);
            if (r !== 0) {
                return r;
            }

            if (this._resource !== p._resource) {
                return this._resource < p._resource ? -1 : 1;
            }

            return 0;
        }
    }, {
        key: "options",
        value: function options() {
            //
            // WARNING: Certain features, such as proxy validation in Glacier2,
            // depend on the format of proxy strings. Changes to toString() and
            // methods called to generate parts of the reference string could break
            // these features. Please review for all features that depend on the
            // format of proxyToString() before changing this and related code.
            //
            var s = this._delegate.options();

            if (this._resource !== null && this._resource.length > 0) {
                s += " -r ";
                s += this._resource.indexOf(':') !== -1 ? "\"" + this._resource + "\"" : this._resource;
            }

            return s;
        }
    }, {
        key: "toConnectorString",
        value: function toConnectorString() {
            return this._delegate.toConnectorString();
        }
    }, {
        key: "initWithStream",
        value: function initWithStream(s) {
            this._resource = s.readString();
        }
    }, {
        key: "checkOption",
        value: function checkOption(option, argument, endpoint) {
            if (option === "-r") {
                if (argument === null) {
                    throw new Ice.EndpointParseException("no argument provided for -r option in endpoint " + endpoint);
                }
                this._resource = argument;
            } else {
                return false;
            }
            return true;
        }
    }]);

    return WSEndpoint;
}(EndpointI);

if (typeof Ice.WSTransceiver !== "undefined") {
    WSEndpoint.prototype.connectable = function () {
        return true;
    };
} else {
    WSEndpoint.prototype.connectable = function () {
        return false;
    };
}

Ice.WSEndpoint = WSEndpoint;
exports.Ice = Ice;