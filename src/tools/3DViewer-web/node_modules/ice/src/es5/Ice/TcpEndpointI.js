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
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/HashUtil", "../Ice/StringUtil", "../Ice/IPEndpointI", "../Ice/TcpTransceiver", "../Ice/LocalException", "../Ice/EndpointInfo"]);

var IceSSL = Ice._ModuleRegistry.require(module, ["../Ice/EndpointInfo"]).IceSSL;

var Debug = Ice.Debug;
var HashUtil = Ice.HashUtil;
var StringUtil = Ice.StringUtil;
var TcpTransceiver = typeof Ice.TcpTransceiver !== "undefined" ? Ice.TcpTransceiver : null;

var TcpEndpointI = function (_Ice$IPEndpointI) {
    _inherits(TcpEndpointI, _Ice$IPEndpointI);

    function TcpEndpointI(instance, ho, po, sif, ti, conId, co) {
        _classCallCheck(this, TcpEndpointI);

        var _this = _possibleConstructorReturn(this, (TcpEndpointI.__proto__ || Object.getPrototypeOf(TcpEndpointI)).call(this, instance, ho, po, sif, conId));

        _this._timeout = ti === undefined ? instance ? instance.defaultTimeout() : undefined : ti;
        _this._compress = co === undefined ? false : co;
        return _this;
    }

    //
    // Return the endpoint information.
    //


    _createClass(TcpEndpointI, [{
        key: "getInfo",
        value: function getInfo() {
            var info = new Ice.TCPEndpointInfo();
            this.fillEndpointInfo(info);
            return this.secure() ? new IceSSL.EndpointInfo(info, info.timeout, info.compress) : info;
        }

        //
        // Return the timeout for the endpoint in milliseconds. 0 means
        // non-blocking, -1 means no timeout.
        //

    }, {
        key: "timeout",
        value: function timeout() {
            return this._timeout;
        }

        //
        // Return a new endpoint with a different timeout value, provided
        // that timeouts are supported by the endpoint. Otherwise the same
        // endpoint is returned.
        //

    }, {
        key: "changeTimeout",
        value: function changeTimeout(timeout) {
            if (timeout === this._timeout) {
                return this;
            } else {
                return new TcpEndpointI(this._instance, this._host, this._port, this._sourceAddr, timeout, this._connectionId, this._compress);
            }
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
                return new TcpEndpointI(this._instance, this._host, this._port, this._sourceAddr, this._timeout, connectionId, this._compress);
            }
        }

        //
        // Return true if the endpoints support bzip2 compress, or false
        // otherwise.
        //

    }, {
        key: "compress",
        value: function compress() {
            return this._compress;
        }

        //
        // Return a new endpoint with a different compression value,
        // provided that compression is supported by the
        // endpoint. Otherwise the same endpoint is returned.
        //

    }, {
        key: "changeCompress",
        value: function changeCompress(compress) {
            if (compress === this._compress) {
                return this;
            } else {
                return new TcpEndpointI(this._instance, this._host, this._port, this._sourceAddr, this._timeout, this._connectionId, compress);
            }
        }

        //
        // Return true if the endpoint is datagram-based.
        //

    }, {
        key: "datagram",
        value: function datagram() {
            return false;
        }
    }, {
        key: "connectable",
        value: function connectable() {
            //
            // TCP endpoints are not connectable when running in a browser, SSL
            // isn't currently supported.
            //
            return TcpTransceiver !== null && !this.secure();
        }
    }, {
        key: "connect",
        value: function connect() {
            Debug.assert(!this.secure());
            return TcpTransceiver.createOutgoing(this._instance, this.getAddress(), this._sourceAddr);
        }

        //
        // Convert the endpoint to its string form
        //

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
            var s = _get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "options", this).call(this);
            if (this._timeout == -1) {
                s += " -t infinite";
            } else {
                s += " -t " + this._timeout;
            }

            if (this._compress) {
                s += " -z";
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

            if (!(p instanceof TcpEndpointI)) {
                return this.type() < p.type() ? -1 : 1;
            }

            if (this._timeout < p._timeout) {
                return -1;
            } else if (p._timeout < this._timeout) {
                return 1;
            }

            if (!this._compress && p._compress) {
                return -1;
            } else if (!p._compress && this._compress) {
                return 1;
            }

            return _get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "compareTo", this).call(this, p);
        }
    }, {
        key: "streamWriteImpl",
        value: function streamWriteImpl(s) {
            _get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "streamWriteImpl", this).call(this, s);
            s.writeInt(this._timeout);
            s.writeBool(this._compress);
        }
    }, {
        key: "hashInit",
        value: function hashInit(h) {
            h = _get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "hashInit", this).call(this, h);
            h = HashUtil.addNumber(h, this._timeout);
            h = HashUtil.addBoolean(h, this._compress);
            return h;
        }
    }, {
        key: "fillEndpointInfo",
        value: function fillEndpointInfo(info) {
            _get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "fillEndpointInfo", this).call(this, info);
            info.timeout = this._timeout;
            info.compress = this._compress;
        }
    }, {
        key: "initWithStream",
        value: function initWithStream(s) {
            _get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "initWithStream", this).call(this, s);
            this._timeout = s.readInt();
            this._compress = s.readBool();
        }
    }, {
        key: "checkOption",
        value: function checkOption(option, argument, endpoint) {
            if (_get(TcpEndpointI.prototype.__proto__ || Object.getPrototypeOf(TcpEndpointI.prototype), "checkOption", this).call(this, option, argument, endpoint)) {
                return true;
            }

            if (option === "-t") {
                if (argument === null) {
                    throw new Ice.EndpointParseException("no argument provided for -t option in endpoint " + endpoint);
                }

                if (argument == "infinite") {
                    this._timeout = -1;
                } else {
                    var invalid = false;
                    try {
                        this._timeout = StringUtil.toInt(argument);
                    } catch (ex) {
                        invalid = true;
                    }
                    if (invalid || this._timeout < 1) {
                        throw new Ice.EndpointParseException("invalid timeout value `" + argument + "' in endpoint " + endpoint);
                    }
                }
            } else if (option === "-z") {
                if (argument !== null) {
                    throw new Ice.EndpointParseException("unexpected argument `" + argument + "' provided for -z option in " + endpoint);
                }

                this._compress = true;
            } else {
                return false;
            }
            return true;
        }
    }, {
        key: "createEndpoint",
        value: function createEndpoint(host, port, conId) {
            return new TcpEndpointI(this._instance, host, port, this._sourceAddr, this._timeout, conId, this._compress);
        }
    }]);

    return TcpEndpointI;
}(Ice.IPEndpointI);

Ice.TcpEndpointI = TcpEndpointI;
module.exports.Ice = Ice;