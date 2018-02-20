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
Ice._ModuleRegistry.require(module, ["../Ice/StringUtil", "../Ice/Stream", "../Ice/Debug", "../Ice/OpaqueEndpointI", "../Ice/Protocol", "../Ice/LocalException"]);

//
// Local aliases.
//
var Debug = Ice.Debug;
var InputStream = Ice.InputStream;
var OutputStream = Ice.OutputStream;
var EndpointParseException = Ice.EndpointParseException;
var OpaqueEndpointI = Ice.OpaqueEndpointI;
var Protocol = Ice.Protocol;
var StringUtil = Ice.StringUtil;

var EndpointFactoryManager = function () {
    function EndpointFactoryManager(instance) {
        _classCallCheck(this, EndpointFactoryManager);

        this._instance = instance;
        this._factories = [];
    }

    _createClass(EndpointFactoryManager, [{
        key: "add",
        value: function add(factory) {
            Debug.assert(this._factories.find(function (f) {
                return factory.type() == f.type();
            }) === undefined);
            this._factories.push(factory);
        }
    }, {
        key: "get",
        value: function get(type) {
            return this._factories.find(function (f) {
                return type == f.type();
            }) || null;
        }
    }, {
        key: "create",
        value: function create(str, oaEndpoint) {
            var s = str.trim();
            if (s.length === 0) {
                throw new EndpointParseException("value has no non-whitespace characters");
            }

            var arr = StringUtil.splitString(s, " \t\n\r");
            if (arr.length === 0) {
                throw new EndpointParseException("value has no non-whitespace characters");
            }

            var protocol = arr[0];
            arr.splice(0, 1);

            if (protocol === "default") {
                protocol = this._instance.defaultsAndOverrides().defaultProtocol;
            }
            for (var i = 0, length = this._factories.length; i < length; ++i) {
                if (this._factories[i].protocol() === protocol) {
                    var e = this._factories[i].create(arr, oaEndpoint);
                    if (arr.length > 0) {
                        throw new EndpointParseException("unrecognized argument `" + arr[0] + "' in endpoint `" + str + "'");
                    }
                    return e;
                }
            }

            //
            // If the stringified endpoint is opaque, create an unknown endpoint,
            // then see whether the type matches one of the known endpoints.
            //
            if (protocol === "opaque") {
                var ue = new OpaqueEndpointI();
                ue.initWithOptions(arr);
                if (arr.length > 0) {
                    throw new EndpointParseException("unrecognized argument `" + arr[0] + "' in endpoint `" + str + "'");
                }

                for (var _i = 0, _length = this._factories.length; _i < _length; ++_i) {
                    if (this._factories[_i].type() == ue.type()) {
                        //
                        // Make a temporary stream, write the opaque endpoint data into the stream,
                        // and ask the factory to read the endpoint data from that stream to create
                        // the actual endpoint.
                        //
                        var os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                        os.writeShort(ue.type());
                        ue.streamWrite(os);
                        var is = new InputStream(this._instance, Protocol.currentProtocolEncoding, os.buffer);
                        is.pos = 0;
                        is.readShort(); // type
                        is.startEncapsulation();
                        var _e = this._factories[_i].read(is);
                        is.endEncapsulation();
                        return _e;
                    }
                }
                return ue; // Endpoint is opaque, but we don't have a factory for its type.
            }

            return null;
        }
    }, {
        key: "read",
        value: function read(s) {
            var type = s.readShort();

            var factory = this.get(type);
            var e = null;
            s.startEncapsulation();
            if (factory) {
                e = factory.read(s);
            }
            //
            // If the factory failed to read the endpoint, return an opaque endpoint. This can
            // occur if for example the factory delegates to another factory and this factory
            // isn't available. In this case, the factory needs to make sure the stream position
            // is preserved for reading the opaque endpoint.
            //
            if (!e) {
                e = new OpaqueEndpointI(type);
                e.initWithStream(s);
            }
            s.endEncapsulation();
            return e;
        }
    }, {
        key: "destroy",
        value: function destroy() {
            this._factories.forEach(function (factory) {
                return factory.destroy();
            });
            this._factories = [];
        }
    }]);

    return EndpointFactoryManager;
}();

Ice.EndpointFactoryManager = EndpointFactoryManager;
module.exports.Ice = Ice;