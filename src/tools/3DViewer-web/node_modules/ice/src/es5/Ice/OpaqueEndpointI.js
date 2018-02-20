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
Ice._ModuleRegistry.require(module, ["../Ice/Base64", "../Ice/Debug", "../Ice/FormatType", "../Ice/HashUtil", "../Ice/Protocol", "../Ice/StringUtil", "../Ice/EndpointI", "../Ice/LocalException"]);

var Base64 = Ice.Base64;
var Debug = Ice.Debug;
var HashUtil = Ice.HashUtil;
var Protocol = Ice.Protocol;
var StringUtil = Ice.StringUtil;
var EndpointParseException = Ice.EndpointParseException;

var OpaqueEndpointI = function (_Ice$EndpointI) {
    _inherits(OpaqueEndpointI, _Ice$EndpointI);

    function OpaqueEndpointI(type) {
        _classCallCheck(this, OpaqueEndpointI);

        var _this = _possibleConstructorReturn(this, (OpaqueEndpointI.__proto__ || Object.getPrototypeOf(OpaqueEndpointI)).call(this));

        _this._rawEncoding = Ice.Encoding_1_0;
        _this._type = type === undefined ? -1 : type;
        _this._rawBytes = null;
        return _this;
    }

    //
    // Marshal the endpoint
    //


    _createClass(OpaqueEndpointI, [{
        key: "streamWrite",
        value: function streamWrite(s) {
            s.startEncapsulation(this._rawEncoding, Ice.FormatType.DefaultFormat);
            s.writeBlob(this._rawBytes);
            s.endEncapsulation();
        }

        //
        // Return the endpoint information.
        //

    }, {
        key: "getInfo",
        value: function getInfo() {
            return new OpaqueEndpointInfoI(null, -1, false, this._rawEncoding, this._rawBytes, this._type);
        }

        //
        // Return the endpoint type
        //

    }, {
        key: "type",
        value: function type() {
            return this._type;
        }
    }, {
        key: "protocol",
        value: function protocol() {
            return "opaque";
        }

        //
        // Return the timeout for the endpoint in milliseconds. 0 means
        // non-blocking, -1 means no timeout.
        //

    }, {
        key: "timeout",
        value: function timeout() {
            return -1;
        }

        //
        // Return a new endpoint with a different timeout value, provided
        // that timeouts are supported by the endpoint. Otherwise the same
        // endpoint is returned.
        //

    }, {
        key: "changeTimeout",
        value: function changeTimeout(t) {
            return this;
        }

        //
        // Return a new endpoint with a different connection id.
        //

    }, {
        key: "changeConnectionId",
        value: function changeConnectionId(connectionId) {
            return this;
        }

        //
        // Return true if the endpoints support bzip2 compress, or false
        // otherwise.
        //

    }, {
        key: "compress",
        value: function compress() {
            return false;
        }

        //
        // Return a new endpoint with a different compression value,
        // provided that compression is supported by the
        // endpoint. Otherwise the same endpoint is returned.
        //

    }, {
        key: "changeCompress",
        value: function changeCompress(compress) {
            return this;
        }

        //
        // Return true if the endpoint is datagram-based.
        //

    }, {
        key: "datagram",
        value: function datagram() {
            return false;
        }

        //
        // Return true if the endpoint is secure.
        //

    }, {
        key: "secure",
        value: function secure() {
            return false;
        }

        //
        // Get the encoded endpoint.
        //

    }, {
        key: "rawBytes",
        value: function rawBytes() {
            return this._rawBytes; // Returns a Uint8Array
        }

        //
        // Return a server side transceiver for this endpoint, or null if a
        // transceiver can only be created by an acceptor. In case a
        // transceiver is created, this operation also returns a new
        // "effective" endpoint, which might differ from this endpoint,
        // for example, if a dynamic port number is assigned.
        //

    }, {
        key: "transceiver",
        value: function transceiver(endpoint) {
            endpoint.value = null;
            return null;
        }

        //
        // Return an acceptor for this endpoint, or null if no acceptors
        // is available. In case an acceptor is created, this operation
        // also returns a new "effective" endpoint, which might differ
        // from this endpoint, for example, if a dynamic port number is
        // assigned.
        //

    }, {
        key: "acceptor",
        value: function acceptor(endpoint, adapterName) {
            endpoint.value = this;
            return null;
        }
    }, {
        key: "connect",
        value: function connect() {
            return null;
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            if (this._hashCode === undefined) {
                var h = 5381;
                h = HashUtil.addNumber(h, this._type);
                h = HashUtil.addHashable(h, this._rawEncoding);
                h = HashUtil.addArray(h, this._rawBytes, HashUtil.addNumber);
                this._hashCode = h;
            }
            return this._hashCode;
        }
    }, {
        key: "options",
        value: function options() {
            var s = "";
            s += " -t " + this._type;
            s += " -e " + Ice.encodingVersionToString(this._rawEncoding);
            s += " -v " + Base64.encode(this._rawBytes);
            return s;
        }

        //
        // Compare endpoints for sorting purposes
        //

    }, {
        key: "equals",
        value: function equals(p) {
            if (!(p instanceof OpaqueEndpointI)) {
                return false;
            }

            if (this === p) {
                return true;
            }

            if (this._type !== p._type) {
                return false;
            }

            if (!this._rawEncoding.equals(p._rawEncoding)) {
                return false;
            }

            if (this._rawBytes.length !== p._rawBytes.length) {
                return false;
            }
            for (var i = 0; i < this._rawBytes.length; i++) {
                if (this._rawBytes[i] !== p._rawBytes[i]) {
                    return false;
                }
            }

            return true;
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

            if (!(p instanceof OpaqueEndpointI)) {
                return this.type() < p.type() ? -1 : 1;
            }

            if (this._type < p._type) {
                return -1;
            } else if (p._type < this._type) {
                return 1;
            }

            if (this._rawEncoding.major < p._rawEncoding.major) {
                return -1;
            } else if (p._rawEncoding.major < this._rawEncoding.major) {
                return 1;
            }

            if (this._rawEncoding.minor < p._rawEncoding.minor) {
                return -1;
            } else if (p._rawEncoding.minor < this._rawEncoding.minor) {
                return 1;
            }

            if (this._rawBytes.length < p._rawBytes.length) {
                return -1;
            } else if (p._rawBytes.length < this._rawBytes.length) {
                return 1;
            }
            for (var i = 0; i < this._rawBytes.length; i++) {
                if (this._rawBytes[i] < p._rawBytes[i]) {
                    return -1;
                } else if (p._rawBytes[i] < this._rawBytes[i]) {
                    return 1;
                }
            }

            return 0;
        }
    }, {
        key: "checkOption",
        value: function checkOption(option, argument, endpoint) {
            switch (option.charAt(1)) {
                case 't':
                    {
                        if (this._type > -1) {
                            throw new EndpointParseException("multiple -t options in endpoint " + endpoint);
                        }
                        if (argument === null) {
                            throw new EndpointParseException("no argument provided for -t option in endpoint " + endpoint);
                        }

                        var type = void 0;

                        try {
                            type = StringUtil.toInt(argument);
                        } catch (ex) {
                            throw new EndpointParseException("invalid type value `" + argument + "' in endpoint " + endpoint);
                        }

                        if (type < 0 || type > 65535) {
                            throw new EndpointParseException("type value `" + argument + "' out of range in endpoint " + endpoint);
                        }

                        this._type = type;
                        return true;
                    }

                case 'v':
                    {
                        if (this._rawBytes) {
                            throw new EndpointParseException("multiple -v options in endpoint " + endpoint);
                        }
                        if (argument === null || argument.length === 0) {
                            throw new EndpointParseException("no argument provided for -v option in endpoint " + endpoint);
                        }
                        for (var i = 0; i < argument.length; ++i) {
                            if (!Base64.isBase64(argument.charAt(i))) {
                                throw new EndpointParseException("invalid base64 character `" + argument.charAt(i) + "' (ordinal " + argument.charCodeAt(i) + ") in endpoint " + endpoint);
                            }
                        }
                        this._rawBytes = Base64.decode(argument);
                        return true;
                    }

                case 'e':
                    {
                        if (argument === null) {
                            throw new EndpointParseException("no argument provided for -e option in endpoint " + endpoint);
                        }
                        try {
                            this._rawEncoding = Ice.stringToEncodingVersion(argument);
                        } catch (e) {
                            throw new EndpointParseException("invalid encoding version `" + argument + "' in endpoint " + endpoint + ":\n" + e.str);
                        }
                        return true;
                    }

                default:
                    {
                        return false;
                    }
            }
        }
    }, {
        key: "initWithOptions",
        value: function initWithOptions(args) {
            _get(OpaqueEndpointI.prototype.__proto__ || Object.getPrototypeOf(OpaqueEndpointI.prototype), "initWithOptions", this).call(this, args);
            Debug.assert(this._rawEncoding);

            if (this._type < 0) {
                throw new EndpointParseException("no -t option in endpoint `" + this + "'");
            }
            if (this._rawBytes === null || this._rawBytes.length === 0) {
                throw new EndpointParseException("no -v option in endpoint `" + this + "'");
            }
        }
    }, {
        key: "initWithStream",
        value: function initWithStream(s) {
            this._rawEncoding = s.getEncoding();
            this._rawBytes = s.readBlob(s.getEncapsulationSize());
        }
    }]);

    return OpaqueEndpointI;
}(Ice.EndpointI);

var OpaqueEndpointInfoI = function (_Ice$OpaqueEndpointIn) {
    _inherits(OpaqueEndpointInfoI, _Ice$OpaqueEndpointIn);

    function OpaqueEndpointInfoI(timeout, compress, rawEncoding, rawBytes, type) {
        _classCallCheck(this, OpaqueEndpointInfoI);

        var _this2 = _possibleConstructorReturn(this, (OpaqueEndpointInfoI.__proto__ || Object.getPrototypeOf(OpaqueEndpointInfoI)).call(this, -1, false, rawEncoding, rawBytes));

        _this2._type = type;
        return _this2;
    }

    _createClass(OpaqueEndpointInfoI, [{
        key: "type",
        value: function type() {
            return this._type;
        }
    }, {
        key: "datagram",
        value: function datagram() {
            return false;
        }
    }, {
        key: "secure",
        value: function secure() {
            return false;
        }
    }]);

    return OpaqueEndpointInfoI;
}(Ice.OpaqueEndpointInfo);

Ice.OpaqueEndpointI = OpaqueEndpointI;
module.exports.Ice = Ice;