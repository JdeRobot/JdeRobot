var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _get = function get(object, property, receiver) { if (object === null) object = Function.prototype; var desc = Object.getOwnPropertyDescriptor(object, property); if (desc === undefined) { var parent = Object.getPrototypeOf(object); if (parent === null) { return undefined; } else { return get(parent, property, receiver); } } else if ("value" in desc) { return desc.value; } else { var getter = desc.get; if (getter === undefined) { return undefined; } return getter.call(receiver); } };

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
Ice._ModuleRegistry.require(module, ["../Ice/Address", "../Ice/HashUtil", "../Ice/StringUtil", "../Ice/EndpointI", "../Ice/LocalException"]);

var Address = Ice.Address;
var HashUtil = Ice.HashUtil;
var StringUtil = Ice.StringUtil;
var EndpointParseException = Ice.EndpointParseException;

var IPEndpointI = function (_Ice$EndpointI) {
    _inherits(IPEndpointI, _Ice$EndpointI);

    function IPEndpointI(instance, ho, po, sa, conId) {
        _classCallCheck(this, IPEndpointI);

        var _this = _possibleConstructorReturn(this, (IPEndpointI.__proto__ || Object.getPrototypeOf(IPEndpointI)).call(this));

        _this._instance = instance;
        _this._host = ho === undefined ? null : ho;
        _this._port = po === undefined ? null : po;
        _this._sourceAddr = sa === undefined ? null : sa;
        _this._connectionId = conId === undefined ? "" : conId;
        return _this;
    }

    //
    // Marshal the endpoint
    //


    _createClass(IPEndpointI, [{
        key: "streamWrite",
        value: function streamWrite(s) {
            s.startEncapsulation();
            this.streamWriteImpl(s);
            s.endEncapsulation();
        }
    }, {
        key: "getInfo",
        value: function getInfo() {
            var info = new Ice.IPEndpointInfo();
            this.fillEndpointInfo(info);
            return info;
        }

        //
        // Return the endpoint type
        //

    }, {
        key: "type",
        value: function type() {
            return this._instance.type();
        }

        //
        // Return the protocol string
        //

    }, {
        key: "protocol",
        value: function protocol() {
            return this._instance.protocol();
        }

        //
        // Return true if the endpoint is secure.
        //

    }, {
        key: "secure",
        value: function secure() {
            return this._instance.secure();
        }
    }, {
        key: "connectionId",
        value: function connectionId() {
            return this._connectionId;
        }

        //
        // Return a new endpoint with a different connection id.
        //

    }, {
        key: "changeConnectionId",
        value: function changeConnectionId(connectionId) {
            if (connectionId === this._connectionId) {
                return this;
            } else {
                return this.createEndpoint(this._host, this._port, connectionId);
            }
        }

        //
        // Return the endpoint information.
        //

    }, {
        key: "hashCode",
        value: function hashCode() {
            if (this._hashCode === undefined) {
                this._hashCode = this.hashInit(5381);
            }
            return this._hashCode;
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
            var s = "";

            if (this._host !== null && this._host.length > 0) {
                s += " -h ";
                var addQuote = this._host.indexOf(':') != -1;
                if (addQuote) {
                    s += "\"";
                }
                s += this._host;
                if (addQuote) {
                    s += "\"";
                }
            }

            s += " -p " + this._port;

            if (this._sourceAddr !== null && this._sourceAddr.length > 0) {
                s += " --sourceAddr " + this._sourceAddr;
            }
            return s;
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

            if (!(p instanceof IPEndpointI)) {
                return this.type() < p.type() ? -1 : 1;
            }

            if (this._port < p._port) {
                return -1;
            } else if (p._port < this._port) {
                return 1;
            }

            if (this._host != p._host) {
                return this._host < p._host ? -1 : 1;
            }

            if (this._sourceAddr != p._sourceAddr) {
                return this._sourceAddr < p._sourceAddr ? -1 : 1;
            }

            if (this._connectionId != p._connectionId) {
                return this._connectionId < p._connectionId ? -1 : 1;
            }

            return 0;
        }
    }, {
        key: "getAddress",
        value: function getAddress() {
            return new Address(this._host, this._port);
        }

        //
        // Convert the endpoint to its Connector string form
        //

    }, {
        key: "toConnectorString",
        value: function toConnectorString() {
            return this._host + ":" + this._port;
        }
    }, {
        key: "streamWriteImpl",
        value: function streamWriteImpl(s) {
            s.writeString(this._host);
            s.writeInt(this._port);
        }
    }, {
        key: "hashInit",
        value: function hashInit(h) {
            h = HashUtil.addNumber(h, this.type());
            h = HashUtil.addString(h, this._host);
            h = HashUtil.addNumber(h, this._port);
            h = HashUtil.addString(h, this._sourceAddr);
            h = HashUtil.addString(h, this._connectionId);
            return h;
        }
    }, {
        key: "fillEndpointInfo",
        value: function fillEndpointInfo(info) {
            var _this2 = this;

            info.type = function () {
                return _this2.type();
            };
            info.datagram = function () {
                return _this2.datagram();
            };
            info.secure = function () {
                return _this2.secure();
            };
            info.host = this._host;
            info.port = this._port;
            info.sourceAddress = this._sourceAddr;
        }
    }, {
        key: "initWithOptions",
        value: function initWithOptions(args, oaEndpoint) {
            _get(IPEndpointI.prototype.__proto__ || Object.getPrototypeOf(IPEndpointI.prototype), "initWithOptions", this).call(this, args);

            if (this._host === null || this._host.length === 0) {
                this._host = this._instance.defaultHost();
            } else if (this._host == "*") {
                if (oaEndpoint) {
                    this._host = "";
                } else {
                    throw new EndpointParseException("`-h *' not valid for proxy endpoint `" + this + "'");
                }
            }

            if (this._host === null) {
                this._host = "";
            }

            if (this._sourceAddr === null) {
                if (!oaEndpoint) {
                    this._sourceAddr = this._instance.defaultSourceAddress();
                }
            } else if (oaEndpoint) {
                throw new EndpointParseException("`--sourceAddress not valid for object adapter endpoint `" + this + "'");
            }
        }
    }, {
        key: "initWithStream",
        value: function initWithStream(s) {
            this._host = s.readString();
            this._port = s.readInt();
        }
    }, {
        key: "checkOption",
        value: function checkOption(option, argument, str) {
            if (option === "-h") {
                if (argument === null) {
                    throw new EndpointParseException("no argument provided for -h option in endpoint " + str);
                }

                this._host = argument;
            } else if (option === "-p") {
                if (argument === null) {
                    throw new EndpointParseException("no argument provided for -p option in endpoint " + str);
                }

                try {
                    this._port = StringUtil.toInt(argument);
                } catch (ex) {
                    throw new EndpointParseException("invalid port value `" + argument + "' in endpoint " + str);
                }

                if (this._port < 0 || this._port > 65535) {
                    throw new EndpointParseException("port value `" + argument + "' out of range in endpoint " + str);
                }
            } else if (option === "--sourceAddress") {
                if (argument === null) {
                    throw new EndpointParseException("no argument provided for --sourceAddress option in endpoint " + str);
                }

                this._sourceAddr = argument;
            } else {
                return false;
            }
            return true;
        }
    }]);

    return IPEndpointI;
}(Ice.EndpointI);

Ice.IPEndpointI = IPEndpointI;
module.exports.Ice = Ice;