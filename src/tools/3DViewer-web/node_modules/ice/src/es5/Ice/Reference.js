var _get = function get(object, property, receiver) { if (object === null) object = Function.prototype; var desc = Object.getOwnPropertyDescriptor(object, property); if (desc === undefined) { var parent = Object.getPrototypeOf(object); if (parent === null) { return undefined; } else { return get(parent, property, receiver); } } else if ("value" in desc) { return desc.value; } else { var getter = desc.get; if (getter === undefined) { return undefined; } return getter.call(receiver); } };

var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

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
Ice._ModuleRegistry.require(module, ["../Ice/ArrayUtil", "../Ice/BatchRequestQueue", "../Ice/Debug", "../Ice/HashUtil", "../Ice/OpaqueEndpointI", "../Ice/Promise", "../Ice/Protocol", "../Ice/ReferenceMode", "../Ice/StringUtil", "../Ice/BuiltinSequences", "../Ice/EndpointTypes", "../Ice/Identity", "../Ice/Router", "../Ice/Locator", "../Ice/LocalException", "../Ice/Version", "../Ice/PropertyNames", "../Ice/ConnectionRequestHandler", "../Ice/MapUtil"]);

var ArrayUtil = Ice.ArrayUtil;
var Debug = Ice.Debug;
var BatchRequestQueue = Ice.BatchRequestQueue;
var HashUtil = Ice.HashUtil;
var OpaqueEndpointI = Ice.OpaqueEndpointI;
var Protocol = Ice.Protocol;
var RefMode = Ice.ReferenceMode;
var StringUtil = Ice.StringUtil;
var StringSeqHelper = Ice.StringSeqHelper;
var EndpointSelectionType = Ice.EndpointSelectionType;
var Identity = Ice.Identity;
var RouterPrx = Ice.RouterPrx;
var LocatorPrx = Ice.LocatorPrx;
var PropertyNames = Ice.PropertyNames;
var ConnectionRequestHandler = Ice.ConnectionRequestHandler;
var MapUtil = Ice.MapUtil;

var suffixes = ["EndpointSelection", "ConnectionCached", "PreferSecure", "EncodingVersion", "LocatorCacheTimeout", "InvocationTimeout", "Locator", "Router", "CollocationOptimized"];

//
// Only for use by Instance
//

var ReferenceFactory = function () {
    function ReferenceFactory(instance, communicator) {
        _classCallCheck(this, ReferenceFactory);

        this._instance = instance;
        this._communicator = communicator;
        this._defaultRouter = null;
        this._defaultLocator = null;
    }

    _createClass(ReferenceFactory, [{
        key: "create",
        value: function create(ident, facet, tmpl, endpoints) {
            if (ident.name.length === 0 && ident.category.length === 0) {
                return null;
            }

            return this.createImpl(ident, facet, tmpl.getMode(), tmpl.getSecure(), tmpl.getProtocol(), tmpl.getEncoding(), endpoints, null, null);
        }
    }, {
        key: "createWithAdapterId",
        value: function createWithAdapterId(ident, facet, tmpl, adapterId) {
            if (ident.name.length === 0 && ident.category.length === 0) {
                return null;
            }

            return this.createImpl(ident, facet, tmpl.getMode(), tmpl.getSecure(), tmpl.getProtocol(), tmpl.getEncoding(), null, adapterId, null);
        }
    }, {
        key: "createFixed",
        value: function createFixed(ident, fixedConnection) {
            if (ident.name.length === 0 && ident.category.length === 0) {
                return null;
            }

            //
            // Create new reference
            //
            return new FixedReference(this._instance, this._communicator, ident, "", // Facet
            fixedConnection.endpoint().datagram() ? RefMode.ModeDatagram : RefMode.ModeTwoway, fixedConnection.endpoint().secure(), this._instance.defaultsAndOverrides().defaultEncoding, fixedConnection);
        }
    }, {
        key: "copy",
        value: function copy(r) {
            var ident = r.getIdentity();
            if (ident.name.length === 0 && ident.category.length === 0) {
                return null;
            }
            return r.clone();
        }
    }, {
        key: "createFromString",
        value: function createFromString(s, propertyPrefix) {
            if (s === undefined || s === null || s.length === 0) {
                return null;
            }

            var delim = " \t\n\r";

            var end = 0;
            var beg = StringUtil.findFirstNotOf(s, delim, end);
            if (beg == -1) {
                throw new Ice.ProxyParseException("no non-whitespace characters found in `" + s + "'");
            }

            //
            // Extract the identity, which may be enclosed in single
            // or double quotation marks.
            //
            var idstr = null;
            end = StringUtil.checkQuote(s, beg);
            if (end === -1) {
                throw new Ice.ProxyParseException("mismatched quotes around identity in `" + s + "'");
            } else if (end === 0) {
                end = StringUtil.findFirstOf(s, delim + ":@", beg);
                if (end === -1) {
                    end = s.length;
                }
                idstr = s.substring(beg, end);
            } else {
                beg++; // Skip leading quote
                idstr = s.substring(beg, end);
                end++; // Skip trailing quote
            }

            if (beg === end) {
                throw new Ice.ProxyParseException("no identity in `" + s + "'");
            }

            //
            // Parsing the identity may raise IdentityParseException.
            //
            var ident = Ice.stringToIdentity(idstr);

            if (ident.name.length === 0) {
                //
                // An identity with an empty name and a non-empty
                // category is illegal.
                //
                if (ident.category.length > 0) {
                    throw new Ice.IllegalIdentityException(ident);
                }
                //
                // Treat a stringified proxy containing two double
                // quotes ("") the same as an empty string, i.e.,
                // a null proxy, but only if nothing follows the
                // quotes.
                //
                else if (StringUtil.findFirstNotOf(s, delim, end) != -1) {
                        throw new Ice.ProxyParseException("invalid characters after identity in `" + s + "'");
                    } else {
                        return null;
                    }
            }

            var facet = "";
            var mode = RefMode.ModeTwoway;
            var secure = false;
            var encoding = this._instance.defaultsAndOverrides().defaultEncoding;
            var protocol = Ice.Protocol_1_0;
            var adapter = "";

            while (true) {
                beg = StringUtil.findFirstNotOf(s, delim, end);
                if (beg === -1) {
                    break;
                }

                if (s.charAt(beg) == ':' || s.charAt(beg) == '@') {
                    break;
                }

                end = StringUtil.findFirstOf(s, delim + ":@", beg);
                if (end == -1) {
                    end = s.length;
                }

                if (beg == end) {
                    break;
                }

                var option = s.substring(beg, end);
                if (option.length != 2 || option.charAt(0) != '-') {
                    throw new Ice.ProxyParseException("expected a proxy option but found `" + option + "' in `" + s + "'");
                }

                //
                // Check for the presence of an option argument. The
                // argument may be enclosed in single or double
                // quotation marks.
                //
                var argument = null;
                var argumentBeg = StringUtil.findFirstNotOf(s, delim, end);
                if (argumentBeg != -1) {
                    var ch = s.charAt(argumentBeg);
                    if (ch != "@" && ch != ":" && ch != "-") {
                        beg = argumentBeg;
                        end = StringUtil.checkQuote(s, beg);
                        if (end == -1) {
                            throw new Ice.ProxyParseException("mismatched quotes around value for " + option + " option in `" + s + "'");
                        } else if (end === 0) {
                            end = StringUtil.findFirstOf(s, delim + ":@", beg);
                            if (end === -1) {
                                end = s.length;
                            }
                            argument = s.substring(beg, end);
                        } else {
                            beg++; // Skip leading quote
                            argument = s.substring(beg, end);
                            end++; // Skip trailing quote
                        }
                    }
                }

                //
                // If any new options are added here,
                // IceInternal::Reference::toString() and its derived classes must be updated as well.
                //
                switch (option.charAt(1)) {
                    case 'f':
                        {
                            if (argument === null) {
                                throw new Ice.ProxyParseException("no argument provided for -f option in `" + s + "'");
                            }

                            try {
                                facet = StringUtil.unescapeString(argument, 0, argument.length);
                            } catch (ex) {
                                throw new Ice.ProxyParseException("invalid facet in `" + s + "': " + ex.message);
                            }

                            break;
                        }

                    case 't':
                        {
                            if (argument !== null) {
                                throw new Ice.ProxyParseException("unexpected argument `" + argument + "' provided for -t option in `" + s + "'");
                            }
                            mode = RefMode.ModeTwoway;
                            break;
                        }

                    case 'o':
                        {
                            if (argument !== null) {
                                throw new Ice.ProxyParseException("unexpected argument `" + argument + "' provided for -o option in `" + s + "'");
                            }
                            mode = RefMode.ModeOneway;
                            break;
                        }

                    case 'O':
                        {
                            if (argument !== null) {
                                throw new Ice.ProxyParseException("unexpected argument `" + argument + "' provided for -O option in `" + s + "'");
                            }
                            mode = RefMode.ModeBatchOneway;
                            break;
                        }

                    case 'd':
                        {
                            if (argument !== null) {
                                throw new Ice.ProxyParseException("unexpected argument `" + argument + "' provided for -d option in `" + s + "'");
                            }
                            mode = RefMode.ModeDatagram;
                            break;
                        }

                    case 'D':
                        {
                            if (argument !== null) {
                                throw new Ice.ProxyParseException("unexpected argument `" + argument + "' provided for -D option in `" + s + "'");
                            }
                            mode = RefMode.ModeBatchDatagram;
                            break;
                        }

                    case 's':
                        {
                            if (argument !== null) {
                                throw new Ice.ProxyParseException("unexpected argument `" + argument + "' provided for -s option in `" + s + "'");
                            }
                            secure = true;
                            break;
                        }

                    case 'e':
                        {
                            if (argument === null) {
                                throw new Ice.ProxyParseException("no argument provided for -e option in `" + s + "'");
                            }

                            try {
                                encoding = Ice.stringToEncodingVersion(argument);
                            } catch (e) // VersionParseException
                            {
                                throw new Ice.ProxyParseException("invalid encoding version `" + argument + "' in `" + s + "':\n" + e.str);
                            }
                            break;
                        }

                    case 'p':
                        {
                            if (argument === null) {
                                throw new Ice.ProxyParseException("no argument provided for -p option in `" + s + "'");
                            }

                            try {
                                protocol = Ice.stringToProtocolVersion(argument);
                            } catch (e) // VersionParseException
                            {
                                throw new Ice.ProxyParseException("invalid protocol version `" + argument + "' in `" + s + "':\n" + e.str);
                            }
                            break;
                        }

                    default:
                        {
                            throw new Ice.ProxyParseException("unknown option `" + option + "' in `" + s + "'");
                        }
                }
            }

            if (beg === -1) {
                return this.createImpl(ident, facet, mode, secure, protocol, encoding, null, null, propertyPrefix);
            }

            var endpoints = [];

            if (s.charAt(beg) == ':') {
                var unknownEndpoints = [];
                end = beg;

                while (end < s.length && s.charAt(end) == ':') {
                    beg = end + 1;

                    end = beg;
                    while (true) {
                        end = s.indexOf(':', end);
                        if (end == -1) {
                            end = s.length;
                            break;
                        } else {
                            var quoted = false;
                            var quote = beg;
                            while (true) {
                                quote = s.indexOf("\"", quote);
                                if (quote == -1 || end < quote) {
                                    break;
                                } else {
                                    quote = s.indexOf('\"', ++quote);
                                    if (quote == -1) {
                                        break;
                                    } else if (end < quote) {
                                        quoted = true;
                                        break;
                                    }
                                    ++quote;
                                }
                            }
                            if (!quoted) {
                                break;
                            }
                            ++end;
                        }
                    }

                    var es = s.substring(beg, end);
                    var endp = this._instance.endpointFactoryManager().create(es, false);
                    if (endp !== null) {
                        endpoints.push(endp);
                    } else {
                        unknownEndpoints.push(es);
                    }
                }
                if (endpoints.length === 0) {
                    Debug.assert(unknownEndpoints.length > 0);
                    throw new Ice.EndpointParseException("invalid endpoint `" + unknownEndpoints[0] + "' in `" + s + "'");
                } else if (unknownEndpoints.length !== 0 && this._instance.initializationData().properties.getPropertyAsIntWithDefault("Ice.Warn.Endpoints", 1) > 0) {
                    var msg = [];
                    msg.push("Proxy contains unknown endpoints:");
                    unknownEndpoints.forEach(function (unknownEndpoint) {
                        msg.push(" `");
                        msg.push(unknownEndpoint);
                        msg.push("'");
                    });
                    this._instance.initializationData().logger.warning(msg.join(""));
                }

                return this.createImpl(ident, facet, mode, secure, protocol, encoding, endpoints, null, propertyPrefix);
            } else if (s.charAt(beg) == '@') {
                beg = StringUtil.findFirstNotOf(s, delim, beg + 1);
                if (beg == -1) {
                    throw new Ice.ProxyParseException("missing adapter id in `" + s + "'");
                }

                var adapterstr = null;
                end = StringUtil.checkQuote(s, beg);
                if (end === -1) {
                    throw new Ice.ProxyParseException("mismatched quotes around adapter id in `" + s + "'");
                } else if (end === 0) {
                    end = StringUtil.findFirstOf(s, delim, beg);
                    if (end === -1) {
                        end = s.length;
                    }
                    adapterstr = s.substring(beg, end);
                } else {
                    beg++; // Skip leading quote
                    adapterstr = s.substring(beg, end);
                    end++; // Skip trailing quote
                }

                if (end !== s.length && StringUtil.findFirstNotOf(s, delim, end) !== -1) {
                    throw new Ice.ProxyParseException("invalid trailing characters after `" + s.substring(0, end + 1) + "' in `" + s + "'");
                }

                try {
                    adapter = StringUtil.unescapeString(adapterstr, 0, adapterstr.length);
                } catch (ex) {
                    throw new Ice.ProxyParseException("invalid adapter id in `" + s + "': " + ex.message);
                }
                if (adapter.length === 0) {
                    throw new Ice.ProxyParseException("empty adapter id in `" + s + "'");
                }
                return this.createImpl(ident, facet, mode, secure, protocol, encoding, null, adapter, propertyPrefix);
            }

            throw new Ice.ProxyParseException("malformed proxy `" + s + "'");
        }
    }, {
        key: "createFromStream",
        value: function createFromStream(ident, s) {
            //
            // Don't read the identity here. Operations calling this
            // constructor read the identity, and pass it as a parameter.
            //

            if (ident.name.length === 0 && ident.category.length === 0) {
                return null;
            }

            //
            // For compatibility with the old FacetPath.
            //
            var facetPath = StringSeqHelper.read(s); // String[]
            var facet = void 0;
            if (facetPath.length > 0) {
                if (facetPath.length > 1) {
                    throw new Ice.ProxyUnmarshalException();
                }
                facet = facetPath[0];
            } else {
                facet = "";
            }

            var mode = s.readByte();
            if (mode < 0 || mode > RefMode.ModeLast) {
                throw new Ice.ProxyUnmarshalException();
            }

            var secure = s.readBool();

            var protocol = null;
            var encoding = null;
            if (!s.getEncoding().equals(Ice.Encoding_1_0)) {
                protocol = new Ice.ProtocolVersion();
                protocol._read(s);
                encoding = new Ice.EncodingVersion();
                encoding._read(s);
            } else {
                protocol = Ice.Protocol_1_0;
                encoding = Ice.Encoding_1_0;
            }

            var endpoints = null; // EndpointI[]
            var adapterId = null;

            var sz = s.readSize();
            if (sz > 0) {
                endpoints = [];
                for (var i = 0; i < sz; i++) {
                    endpoints[i] = this._instance.endpointFactoryManager().read(s);
                }
            } else {
                adapterId = s.readString();
            }

            return this.createImpl(ident, facet, mode, secure, protocol, encoding, endpoints, adapterId, null);
        }
    }, {
        key: "setDefaultRouter",
        value: function setDefaultRouter(defaultRouter) {
            if (this._defaultRouter === null ? defaultRouter === null : this._defaultRouter.equals(defaultRouter)) {
                return this;
            }

            var factory = new ReferenceFactory(this._instance, this._communicator);
            factory._defaultLocator = this._defaultLocator;
            factory._defaultRouter = defaultRouter;
            return factory;
        }
    }, {
        key: "getDefaultRouter",
        value: function getDefaultRouter() {
            return this._defaultRouter;
        }
    }, {
        key: "setDefaultLocator",
        value: function setDefaultLocator(defaultLocator) {
            if (this._defaultLocator === null ? defaultLocator === null : this._defaultLocator.equals(defaultLocator)) {
                return this;
            }

            var factory = new ReferenceFactory(this._instance, this._communicator);
            factory._defaultRouter = this._defaultRouter;
            factory._defaultLocator = defaultLocator;
            return factory;
        }
    }, {
        key: "getDefaultLocator",
        value: function getDefaultLocator() {
            return this._defaultLocator;
        }
    }, {
        key: "checkForUnknownProperties",
        value: function checkForUnknownProperties(prefix) {
            var unknownProps = [];
            //
            // Do not warn about unknown properties for Ice prefixes (Ice, Glacier2, etc.)
            //
            for (var i = 0; i < PropertyNames.clPropNames.length; ++i) {
                if (prefix.indexOf(PropertyNames.clPropNames[i] + ".") === 0) {
                    return;
                }
            }

            var properties = this._instance.initializationData().properties.getPropertiesForPrefix(prefix + ".");
            unknownProps = unknownProps.concat(Array.from(properties.keys()).filter(function (key) {
                return !suffixes.some(function (suffix) {
                    return key === prefix + "." + suffix;
                });
            }));
            if (unknownProps.length > 0) {
                var message = [];
                message.push("found unknown properties for proxy '");
                message.push(prefix);
                message.push("':");
                unknownProps.forEach(function (unknownProp) {
                    return message.push("\n    ", unknownProp);
                });
                this._instance.initializationData().logger.warning(message.join(""));
            }
        }
    }, {
        key: "createImpl",
        value: function createImpl(ident, facet, mode, secure, protocol, encoding, endpoints, adapterId, propertyPrefix) {
            var defaultsAndOverrides = this._instance.defaultsAndOverrides();

            //
            // Default local proxy options.
            //
            var locatorInfo = null;
            if (this._defaultLocator !== null) {
                if (!this._defaultLocator._getReference().getEncoding().equals(encoding)) {
                    locatorInfo = this._instance.locatorManager().find(this._defaultLocator.ice_encodingVersion(encoding));
                } else {
                    locatorInfo = this._instance.locatorManager().find(this._defaultLocator);
                }
            }
            var routerInfo = this._instance.routerManager().find(this._defaultRouter);
            var cacheConnection = true;
            var preferSecure = defaultsAndOverrides.defaultPreferSecure;
            var endpointSelection = defaultsAndOverrides.defaultEndpointSelection;
            var locatorCacheTimeout = defaultsAndOverrides.defaultLocatorCacheTimeout;
            var invocationTimeout = defaultsAndOverrides.defaultInvocationTimeout;

            //
            // Override the defaults with the proxy properties if a property prefix is defined.
            //
            if (propertyPrefix !== null && propertyPrefix.length > 0) {
                var properties = this._instance.initializationData().properties;

                //
                // Warn about unknown properties.
                //
                if (properties.getPropertyAsIntWithDefault("Ice.Warn.UnknownProperties", 1) > 0) {
                    this.checkForUnknownProperties(propertyPrefix);
                }

                var property = propertyPrefix + ".Locator";
                var locator = LocatorPrx.uncheckedCast(this._communicator.propertyToProxy(property));
                if (locator !== null) {
                    if (!locator._getReference().getEncoding().equals(encoding)) {
                        locatorInfo = this._instance.locatorManager().find(locator.ice_encodingVersion(encoding));
                    } else {
                        locatorInfo = this._instance.locatorManager().find(locator);
                    }
                }

                property = propertyPrefix + ".Router";
                var router = RouterPrx.uncheckedCast(this._communicator.propertyToProxy(property));
                if (router !== null) {
                    var match = ".Router";
                    if (propertyPrefix.lastIndexOf(match) == propertyPrefix.length - match.length) {
                        this._instance.initializationData().logger.warning("`" + property + "=" + properties.getProperty(property) + "': cannot set a router on a router; setting ignored");
                    } else {
                        routerInfo = this._instance.routerManager().find(router);
                    }
                }

                property = propertyPrefix + ".ConnectionCached";
                cacheConnection = properties.getPropertyAsIntWithDefault(property, cacheConnection ? 1 : 0) > 0;

                property = propertyPrefix + ".PreferSecure";
                preferSecure = properties.getPropertyAsIntWithDefault(property, preferSecure ? 1 : 0) > 0;

                property = propertyPrefix + ".EndpointSelection";
                if (properties.getProperty(property).length > 0) {
                    var type = properties.getProperty(property);
                    if (type == "Random") {
                        endpointSelection = EndpointSelectionType.Random;
                    } else if (type == "Ordered") {
                        endpointSelection = EndpointSelectionType.Ordered;
                    } else {
                        throw new Ice.EndpointSelectionTypeParseException("illegal value `" + type + "'; expected `Random' or `Ordered'");
                    }
                }

                property = propertyPrefix + ".LocatorCacheTimeout";
                var value = properties.getProperty(property);
                if (value.length !== 0) {
                    locatorCacheTimeout = properties.getPropertyAsIntWithDefault(property, locatorCacheTimeout);
                    if (locatorCacheTimeout < -1) {
                        locatorCacheTimeout = -1;
                        this._instance.initializationData().logger.warning("invalid value for" + property + "`" + properties.getProperty(property) + "': defaulting to -1");
                    }
                }

                property = propertyPrefix + ".InvocationTimeout";
                value = properties.getProperty(property);
                if (value.length !== 0) {
                    invocationTimeout = properties.getPropertyAsIntWithDefault(property, invocationTimeout);
                    if (invocationTimeout < 1 && invocationTimeout !== -1) {
                        invocationTimeout = -1;
                        this._instance.initializationData().logger.warning("invalid value for" + property + "`" + properties.getProperty(property) + "': defaulting to -1");
                    }
                }
            }

            //
            // Create new reference
            //
            return new RoutableReference(this._instance, this._communicator, ident, facet, mode, secure, protocol, encoding, endpoints, adapterId, locatorInfo, routerInfo, cacheConnection, preferSecure, endpointSelection, locatorCacheTimeout, invocationTimeout);
        }
    }]);

    return ReferenceFactory;
}();

Ice.ReferenceFactory = ReferenceFactory;

var Reference = function () {
    function Reference(instance, communicator, identity, facet, mode, secure, protocol, encoding, invocationTimeout) {
        _classCallCheck(this, Reference);

        //
        // Validate string arguments.
        //
        Debug.assert(identity === undefined || identity.name !== null);
        Debug.assert(identity === undefined || identity.category !== null);
        Debug.assert(facet === undefined || facet !== null);

        this._instance = instance;
        this._communicator = communicator;
        this._mode = mode;
        this._secure = secure;
        this._identity = identity;
        this._context = Reference._emptyContext;
        this._facet = facet;
        this._protocol = protocol;
        this._encoding = encoding;
        this._invocationTimeout = invocationTimeout;
        this._hashInitialized = false;
    }

    _createClass(Reference, [{
        key: "getMode",
        value: function getMode() {
            return this._mode;
        }
    }, {
        key: "getSecure",
        value: function getSecure() {
            return this._secure;
        }
    }, {
        key: "getProtocol",
        value: function getProtocol() {
            return this._protocol;
        }
    }, {
        key: "getEncoding",
        value: function getEncoding() {
            return this._encoding;
        }
    }, {
        key: "getIdentity",
        value: function getIdentity() {
            return this._identity;
        }
    }, {
        key: "getFacet",
        value: function getFacet() {
            return this._facet;
        }
    }, {
        key: "getInstance",
        value: function getInstance() {
            return this._instance;
        }
    }, {
        key: "getContext",
        value: function getContext() {
            return this._context; // Map
        }
    }, {
        key: "getInvocationTimeout",
        value: function getInvocationTimeout() {
            return this._invocationTimeout;
        }
    }, {
        key: "getCommunicator",
        value: function getCommunicator() {
            return this._communicator;
        }
    }, {
        key: "getEndpoints",
        value: function getEndpoints() {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "getAdapterId",
        value: function getAdapterId() {
            // Abstract
            Debug.assert(false);
            return "";
        }
    }, {
        key: "getRouterInfo",
        value: function getRouterInfo() {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "getLocatorInfo",
        value: function getLocatorInfo() {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "getCacheConnection",
        value: function getCacheConnection() {
            // Abstract
            Debug.assert(false);
            return false;
        }
    }, {
        key: "getPreferSecure",
        value: function getPreferSecure() {
            // Abstract
            Debug.assert(false);
            return false;
        }
    }, {
        key: "getEndpointSelection",
        value: function getEndpointSelection() {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "getLocatorCacheTimeout",
        value: function getLocatorCacheTimeout() {
            // Abstract
            Debug.assert(false);
            return 0;
        }
    }, {
        key: "getConnectionId",
        value: function getConnectionId() {
            // Abstract
            Debug.assert(false);
            return "";
        }

        //
        // The change* methods (here and in derived classes) create
        // a new reference based on the existing one, with the
        // corresponding value changed.
        //

    }, {
        key: "changeContext",
        value: function changeContext(newContext) {
            if (newContext === undefined || newContext === null) {
                newContext = Reference._emptyContext;
            }
            var r = this._instance.referenceFactory().copy(this);
            if (newContext.size === 0) {
                r._context = Reference._emptyContext;
            } else {
                r._context = new Map(newContext);
            }
            return r;
        }
    }, {
        key: "changeMode",
        value: function changeMode(newMode) {
            if (newMode === this._mode) {
                return this;
            }
            var r = this._instance.referenceFactory().copy(this);
            r._mode = newMode;
            return r;
        }
    }, {
        key: "changeSecure",
        value: function changeSecure(newSecure) {
            if (newSecure === this._secure) {
                return this;
            }
            var r = this._instance.referenceFactory().copy(this);
            r._secure = newSecure;
            return r;
        }
    }, {
        key: "changeIdentity",
        value: function changeIdentity(newIdentity) {
            if (newIdentity.equals(this._identity)) {
                return this;
            }
            var r = this._instance.referenceFactory().copy(this);
            r._identity = new Identity(newIdentity.name, newIdentity.category);
            return r;
        }
    }, {
        key: "changeFacet",
        value: function changeFacet(newFacet) {
            if (newFacet === this._facet) {
                return this;
            }
            var r = this._instance.referenceFactory().copy(this);
            r._facet = newFacet;
            return r;
        }
    }, {
        key: "changeInvocationTimeout",
        value: function changeInvocationTimeout(newInvocationTimeout) {
            if (newInvocationTimeout === this._invocationTimeout) {
                return this;
            }
            var r = this._instance.referenceFactory().copy(this);
            r._invocationTimeout = newInvocationTimeout;
            return r;
        }
    }, {
        key: "changeEncoding",
        value: function changeEncoding(newEncoding) {
            if (newEncoding.equals(this._encoding)) {
                return this;
            }
            var r = this._instance.referenceFactory().copy(this);
            r._encoding = newEncoding;
            return r;
        }
    }, {
        key: "changeAdapterId",
        value: function changeAdapterId(newAdapterId) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeEndpoints",
        value: function changeEndpoints(newEndpoints) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeLocator",
        value: function changeLocator(newLocator) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeRouter",
        value: function changeRouter(newRouter) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeCacheConnection",
        value: function changeCacheConnection(newCache) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changePreferSecure",
        value: function changePreferSecure(newPreferSecure) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeEndpointSelection",
        value: function changeEndpointSelection(newType) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeLocatorCacheTimeout",
        value: function changeLocatorCacheTimeout(newTimeout) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeTimeout",
        value: function changeTimeout(newTimeout) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "changeConnectionId",
        value: function changeConnectionId(connectionId) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            if (this._hashInitialized) {
                return this._hashValue;
            }

            var h = 5381;
            h = HashUtil.addNumber(h, this._mode);
            h = HashUtil.addBoolean(h, this._secure);
            h = HashUtil.addHashable(h, this._identity);
            if (this._context !== null && this._context !== undefined) {
                var _iteratorNormalCompletion = true;
                var _didIteratorError = false;
                var _iteratorError = undefined;

                try {
                    for (var _iterator = this._context[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                        var _step$value = _slicedToArray(_step.value, 2),
                            key = _step$value[0],
                            value = _step$value[1];

                        h = HashUtil.addString(h, key);
                        h = HashUtil.addString(h, value);
                    }
                } catch (err) {
                    _didIteratorError = true;
                    _iteratorError = err;
                } finally {
                    try {
                        if (!_iteratorNormalCompletion && _iterator.return) {
                            _iterator.return();
                        }
                    } finally {
                        if (_didIteratorError) {
                            throw _iteratorError;
                        }
                    }
                }
            }
            h = HashUtil.addString(h, this._facet);
            h = HashUtil.addHashable(h, this._protocol);
            h = HashUtil.addHashable(h, this._encoding);
            h = HashUtil.addNumber(h, this._invocationTimeout);

            this._hashValue = h;
            this._hashInitialized = true;

            return this._hashValue;
        }

        //
        // Utility methods
        //

    }, {
        key: "isIndirect",
        value: function isIndirect() {
            // Abstract
            Debug.assert(false);
            return false;
        }
    }, {
        key: "isWellKnown",
        value: function isWellKnown() {
            // Abstract
            Debug.assert(false);
            return false;
        }

        //
        // Marshal the reference.
        //

    }, {
        key: "streamWrite",
        value: function streamWrite(s) {
            //
            // Don't write the identity here. Operations calling streamWrite
            // write the identity.
            //

            //
            // For compatibility with the old FacetPath.
            //
            if (this._facet.length === 0) {
                s.writeSize(0); // Empty string sequence
            } else {
                s.writeSize(1); // String sequence with one element
                s.writeString(this._facet);
            }

            s.writeByte(this._mode);

            s.writeBool(this._secure);

            if (!s.getEncoding().equals(Ice.Encoding_1_0)) {
                this._protocol._write(s);
                this._encoding._write(s);
            }

            // Derived class writes the remainder of the reference.
        }

        //
        // Convert the reference to its string form.
        //

    }, {
        key: "toString",
        value: function toString() {
            //
            // WARNING: Certain features, such as proxy validation in Glacier2,
            // depend on the format of proxy strings. Changes to toString() and
            // methods called to generate parts of the reference string could break
            // these features. Please review for all features that depend on the
            // format of proxyToString() before changing this and related code.
            //
            var s = [];

            var toStringMode = this._instance.toStringMode();

            //
            // If the encoded identity string contains characters which
            // the reference parser uses as separators, then we enclose
            // the identity string in quotes.
            //

            var id = Ice.identityToString(this._identity, toStringMode);
            if (id.search(/[ :@]/) != -1) {
                s.push('"');
                s.push(id);
                s.push('"');
            } else {
                s.push(id);
            }

            if (this._facet.length > 0) {
                //
                // If the encoded facet string contains characters which
                // the reference parser uses as separators, then we enclose
                // the facet string in quotes.
                //
                s.push(" -f ");
                var fs = StringUtil.escapeString(this._facet, "", toStringMode);
                if (fs.search(/[ :@]/) != -1) {
                    s.push('"');
                    s.push(fs);
                    s.push('"');
                } else {
                    s.push(fs);
                }
            }

            switch (this._mode) {
                case RefMode.ModeTwoway:
                    {
                        s.push(" -t");
                        break;
                    }

                case RefMode.ModeOneway:
                    {
                        s.push(" -o");
                        break;
                    }

                case RefMode.ModeBatchOneway:
                    {
                        s.push(" -O");
                        break;
                    }

                case RefMode.ModeDatagram:
                    {
                        s.push(" -d");
                        break;
                    }

                case RefMode.ModeBatchDatagram:
                    {
                        s.push(" -D");
                        break;
                    }
            }

            if (this._secure) {
                s.push(" -s");
            }

            if (!this._protocol.equals(Ice.Protocol_1_0)) {
                //
                // We only print the protocol if it's not 1.0. It's fine as
                // long as we don't add Ice.Default.ProtocolVersion, a
                // stringified proxy will convert back to the same proxy with
                // stringToProxy.
                //
                s.push(" -p ");
                s.push(Ice.protocolVersionToString(this._protocol));
            }

            //
            // Always print the encoding version to ensure a stringified proxy
            // will convert back to a proxy with the same encoding with
            // stringToProxy (and won't use Ice.Default.EncodingVersion).
            //
            s.push(" -e ");
            s.push(Ice.encodingVersionToString(this._encoding));

            return s.join("");

            // Derived class writes the remainder of the string.
        }

        //
        // Convert the reference to its property form.
        //

    }, {
        key: "toProperty",
        value: function toProperty(prefix) {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "getRequestHandler",
        value: function getRequestHandler(proxy) {
            // Abstract
            Debug.assert(false);
        }
    }, {
        key: "getBatchRequestQueue",
        value: function getBatchRequestQueue() {
            // Abstract
            Debug.assert(false);
        }
    }, {
        key: "equals",
        value: function equals(r) {
            //
            // Note: if(this === r) and type test are performed by each non-abstract derived class.
            //

            if (this._mode !== r._mode) {
                return false;
            }

            if (this._secure !== r._secure) {
                return false;
            }

            if (!this._identity.equals(r._identity)) {
                return false;
            }

            if (!MapUtil.equals(this._context, r._context)) {
                return false;
            }

            if (this._facet !== r._facet) {
                return false;
            }

            if (!this._protocol.equals(r._protocol)) {
                return false;
            }

            if (!this._encoding.equals(r._encoding)) {
                return false;
            }

            if (this._invocationTimeout !== r._invocationTimeout) {
                return false;
            }

            return true;
        }
    }, {
        key: "clone",
        value: function clone() {
            // Abstract
            Debug.assert(false);
            return null;
        }
    }, {
        key: "copyMembers",
        value: function copyMembers(r) {
            //
            // Copy the members that are not passed to the constructor.
            //
            r._context = this._context;
        }
    }]);

    return Reference;
}();

Reference._emptyContext = new Map();
Reference._emptyEndpoints = [];

Ice.Reference = Reference;

var FixedReference = function (_Reference) {
    _inherits(FixedReference, _Reference);

    function FixedReference(instance, communicator, identity, facet, mode, secure, encoding, connection) {
        _classCallCheck(this, FixedReference);

        var _this = _possibleConstructorReturn(this, (FixedReference.__proto__ || Object.getPrototypeOf(FixedReference)).call(this, instance, communicator, identity, facet, mode, secure, Ice.Protocol_1_0, encoding));

        _this._fixedConnection = connection;
        return _this;
    }

    _createClass(FixedReference, [{
        key: "getEndpoints",
        value: function getEndpoints() {
            return Reference._emptyEndpoints;
        }
    }, {
        key: "getAdapterId",
        value: function getAdapterId() {
            return "";
        }
    }, {
        key: "getRouterInfo",
        value: function getRouterInfo() {
            return null;
        }
    }, {
        key: "getLocatorInfo",
        value: function getLocatorInfo() {
            return null;
        }
    }, {
        key: "getCacheConnection",
        value: function getCacheConnection() {
            return true;
        }
    }, {
        key: "getPreferSecure",
        value: function getPreferSecure() {
            return false;
        }
    }, {
        key: "getEndpointSelection",
        value: function getEndpointSelection() {
            return EndpointSelectionType.Random;
        }
    }, {
        key: "getLocatorCacheTimeout",
        value: function getLocatorCacheTimeout() {
            return 0;
        }
    }, {
        key: "getConnectionId",
        value: function getConnectionId() {
            return "";
        }
    }, {
        key: "changeAdapterId",
        value: function changeAdapterId(newAdapterId) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeEndpoints",
        value: function changeEndpoints(newEndpoints) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeLocato",
        value: function changeLocato(newLocator) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeRouter",
        value: function changeRouter(newRouter) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeCacheConnection",
        value: function changeCacheConnection(newCache) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changePreferSecure",
        value: function changePreferSecure(prefSec) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeEndpointSelection",
        value: function changeEndpointSelection(newType) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeLocatorCacheTimeout",
        value: function changeLocatorCacheTimeout(newTimeout) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeTimeout",
        value: function changeTimeout(newTimeout) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "changeConnectionId",
        value: function changeConnectionId(connectionId) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "isIndirect",
        value: function isIndirect() {
            return false;
        }
    }, {
        key: "isWellKnown",
        value: function isWellKnown() {
            return false;
        }
    }, {
        key: "streamWrite",
        value: function streamWrite(s) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "toProperty",
        value: function toProperty(prefix) {
            throw new Ice.FixedProxyException();
        }
    }, {
        key: "clone",
        value: function clone() {
            var r = new FixedReference(this.getInstance(), this.getCommunicator(), this.getIdentity(), this.getFacet(), this.getMode(), this.getSecure(), this.getEncoding(), this._fixedConnection);
            this.copyMembers(r);
            return r;
        }
    }, {
        key: "getRequestHandler",
        value: function getRequestHandler(proxy) {
            switch (this.getMode()) {
                case RefMode.ModeTwoway:
                case RefMode.ModeOneway:
                case RefMode.ModeBatchOneway:
                    {
                        if (this._fixedConnection.endpoint().datagram()) {
                            throw new Ice.NoEndpointException("");
                        }
                        break;
                    }

                case RefMode.ModeDatagram:
                case RefMode.ModeBatchDatagram:
                    {
                        if (!this._fixedConnection.endpoint().datagram()) {
                            throw new Ice.NoEndpointException("");
                        }
                        break;
                    }
            }

            //
            // If a secure connection is requested or secure overrides is set,
            // check if the connection is secure.
            //
            var defaultsAndOverrides = this.getInstance().defaultsAndOverrides();
            var secure = defaultsAndOverrides.overrideSecure ? defaultsAndOverrides.overrideSecureValue : this.getSecure();
            if (secure && !this._fixedConnection.endpoint().secure()) {
                throw new Ice.NoEndpointException("");
            }

            this._fixedConnection.throwException(); // Throw in case our connection is already destroyed.

            return proxy._setRequestHandler(new ConnectionRequestHandler(this, this._fixedConnection));
        }
    }, {
        key: "getBatchRequestQueue",
        value: function getBatchRequestQueue() {
            return this._fixedConnection.getBatchRequestQueue();
        }
    }, {
        key: "equals",
        value: function equals(rhs) {
            if (this === rhs) {
                return true;
            }
            if (!(rhs instanceof FixedReference)) {
                return false;
            }
            if (!_get(FixedReference.prototype.__proto__ || Object.getPrototypeOf(FixedReference.prototype), "equals", this).call(this, rhs)) {
                return false;
            }
            return this._fixedConnection.equals(rhs._fixedConnection);
        }
    }]);

    return FixedReference;
}(Reference);

Ice.FixedReference = FixedReference;

var RoutableReference = function (_Reference2) {
    _inherits(RoutableReference, _Reference2);

    function RoutableReference(instance, communicator, identity, facet, mode, secure, protocol, encoding, endpoints, adapterId, locatorInfo, routerInfo, cacheConnection, preferSecure, endpointSelection, locatorCacheTimeout, invocationTimeout) {
        _classCallCheck(this, RoutableReference);

        var _this2 = _possibleConstructorReturn(this, (RoutableReference.__proto__ || Object.getPrototypeOf(RoutableReference)).call(this, instance, communicator, identity, facet, mode, secure, protocol, encoding, invocationTimeout));

        _this2._endpoints = endpoints;
        _this2._adapterId = adapterId;
        _this2._locatorInfo = locatorInfo;
        _this2._routerInfo = routerInfo;
        _this2._cacheConnection = cacheConnection;
        _this2._preferSecure = preferSecure;
        _this2._endpointSelection = endpointSelection;
        _this2._locatorCacheTimeout = locatorCacheTimeout;
        _this2._overrideTimeout = false;
        _this2._timeout = -1;

        if (_this2._endpoints === null) {
            _this2._endpoints = Reference._emptyEndpoints;
        }
        if (_this2._adapterId === null) {
            _this2._adapterId = "";
        }
        _this2._connectionId = "";
        Debug.assert(_this2._adapterId.length === 0 || _this2._endpoints.length === 0);
        return _this2;
    }

    _createClass(RoutableReference, [{
        key: "getEndpoints",
        value: function getEndpoints() {
            return this._endpoints;
        }
    }, {
        key: "getAdapterId",
        value: function getAdapterId() {
            return this._adapterId;
        }
    }, {
        key: "getRouterInfo",
        value: function getRouterInfo() {
            return this._routerInfo;
        }
    }, {
        key: "getLocatorInfo",
        value: function getLocatorInfo() {
            return this._locatorInfo;
        }
    }, {
        key: "getCacheConnection",
        value: function getCacheConnection() {
            return this._cacheConnection;
        }
    }, {
        key: "getPreferSecure",
        value: function getPreferSecure() {
            return this._preferSecure;
        }
    }, {
        key: "getEndpointSelection",
        value: function getEndpointSelection() {
            return this._endpointSelection;
        }
    }, {
        key: "getLocatorCacheTimeout",
        value: function getLocatorCacheTimeout() {
            return this._locatorCacheTimeout;
        }
    }, {
        key: "getConnectionId",
        value: function getConnectionId() {
            return this._connectionId;
        }
    }, {
        key: "changeEncoding",
        value: function changeEncoding(newEncoding) {
            var r = _get(RoutableReference.prototype.__proto__ || Object.getPrototypeOf(RoutableReference.prototype), "changeEncoding", this).call(this, newEncoding);
            if (r !== this) {
                if (r._locatorInfo !== null && !r._locatorInfo.getLocator().ice_getEncodingVersion().equals(newEncoding)) {
                    r._locatorInfo = this.getInstance().locatorManager().find(r._locatorInfo.getLocator().ice_encodingVersion(newEncoding));
                }
            }
            return r;
        }
    }, {
        key: "changeAdapterId",
        value: function changeAdapterId(newAdapterId) {
            if (this._adapterId === newAdapterId) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._adapterId = newAdapterId;
            r._endpoints = Reference._emptyEndpoints;
            return r;
        }
    }, {
        key: "changeEndpoints",
        value: function changeEndpoints(newEndpoints) {
            if (ArrayUtil.equals(newEndpoints, this._endpoints, function (e1, e2) {
                return e1.equals(e2);
            })) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._endpoints = newEndpoints;
            r._adapterId = "";
            r.applyOverrides(r._endpoints);
            return r;
        }
    }, {
        key: "changeLocator",
        value: function changeLocator(newLocator) {
            var newLocatorInfo = this.getInstance().locatorManager().find(newLocator);
            if (newLocatorInfo !== null && this._locatorInfo !== null && newLocatorInfo.equals(this._locatorInfo)) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._locatorInfo = newLocatorInfo;
            return r;
        }
    }, {
        key: "changeRouter",
        value: function changeRouter(newRouter) {
            var newRouterInfo = this.getInstance().routerManager().find(newRouter);
            if (newRouterInfo !== null && this._routerInfo !== null && newRouterInfo.equals(this._routerInfo)) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._routerInfo = newRouterInfo;
            return r;
        }
    }, {
        key: "changeCacheConnection",
        value: function changeCacheConnection(newCache) {
            if (newCache === this._cacheConnection) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._cacheConnection = newCache;
            return r;
        }
    }, {
        key: "changePreferSecure",
        value: function changePreferSecure(newPreferSecure) {
            if (newPreferSecure === this._preferSecure) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._preferSecure = newPreferSecure;
            return r;
        }
    }, {
        key: "changeEndpointSelection",
        value: function changeEndpointSelection(newType) {
            if (newType === this._endpointSelection) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._endpointSelection = newType;
            return r;
        }
    }, {
        key: "changeLocatorCacheTimeout",
        value: function changeLocatorCacheTimeout(newTimeout) {
            if (this._locatorCacheTimeout === newTimeout) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._locatorCacheTimeout = newTimeout;
            return r;
        }
    }, {
        key: "changeTimeout",
        value: function changeTimeout(newTimeout) {
            if (this._overrideTimeout && this._timeout === newTimeout) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._timeout = newTimeout;
            r._overrideTimeout = true;
            r._endpoints = this._endpoints.map(function (endpoint) {
                return endpoint.changeTimeout(newTimeout);
            });
            return r;
        }
    }, {
        key: "changeConnectionId",
        value: function changeConnectionId(id) {
            if (this._connectionId === id) {
                return this;
            }
            var r = this.getInstance().referenceFactory().copy(this);
            r._connectionId = id;
            r._endpoints = this._endpoints.map(function (endpoint) {
                return endpoint.changeConnectionId(id);
            });
            return r;
        }
    }, {
        key: "isIndirect",
        value: function isIndirect() {
            return this._endpoints.length === 0;
        }
    }, {
        key: "isWellKnown",
        value: function isWellKnown() {
            return this._endpoints.length === 0 && this._adapterId.length === 0;
        }
    }, {
        key: "streamWrite",
        value: function streamWrite(s) {
            _get(RoutableReference.prototype.__proto__ || Object.getPrototypeOf(RoutableReference.prototype), "streamWrite", this).call(this, s);

            s.writeSize(this._endpoints.length);
            if (this._endpoints.length > 0) {
                Debug.assert(this._adapterId.length === 0);
                this._endpoints.forEach(function (endpoint) {
                    s.writeShort(endpoint.type());
                    endpoint.streamWrite(s);
                });
            } else {
                s.writeString(this._adapterId); // Adapter id.
            }
        }
    }, {
        key: "toString",
        value: function toString() {
            //
            // WARNING: Certain features, such as proxy validation in Glacier2,
            // depend on the format of proxy strings. Changes to toString() and
            // methods called to generate parts of the reference string could break
            // these features. Please review for all features that depend on the
            // format of proxyToString() before changing this and related code.
            //
            var s = [];
            s.push(_get(RoutableReference.prototype.__proto__ || Object.getPrototypeOf(RoutableReference.prototype), "toString", this).call(this));
            if (this._endpoints.length > 0) {
                this._endpoints.forEach(function (endpoint) {
                    var endp = endpoint.toString();
                    if (endp !== null && endp.length > 0) {
                        s.push(':');
                        s.push(endp);
                    }
                });
            } else if (this._adapterId.length > 0) {
                s.push(" @ ");

                //
                // If the encoded adapter id string contains characters which
                // the reference parser uses as separators, then we enclose
                // the adapter id string in quotes.
                //
                var a = StringUtil.escapeString(this._adapterId, null, this._instance.toStringMode());
                if (a.search(/[ :@]/) != -1) {
                    s.push('"');
                    s.push(a);
                    s.push('"');
                } else {
                    s.push(a);
                }
            }
            return s.join("");
        }
    }, {
        key: "toProperty",
        value: function toProperty(prefix) {
            var properties = new Map();

            properties.set(prefix, this.toString());
            properties.set(prefix + ".CollocationOptimized", "0");
            properties.set(prefix + ".ConnectionCached", this._cacheConnection ? "1" : "0");
            properties.set(prefix + ".PreferSecure", this._preferSecure ? "1" : "0");
            properties.set(prefix + ".EndpointSelection", this._endpointSelection === EndpointSelectionType.Random ? "Random" : "Ordered");

            properties.set(prefix + ".LocatorCacheTimeout", "" + this._locatorCacheTimeout);
            properties.set(prefix + ".InvocationTimeout", "" + this.getInvocationTimeout());

            if (this._routerInfo !== null) {
                this._routerInfo.getRouter()._getReference().toProperty(prefix + ".Router").forEach(function (value, key) {
                    return properties.set(key, value);
                });
            }

            if (this._locatorInfo !== null) {
                this._locatorInfo.getLocator()._getReference().toProperty(prefix + ".Locator").forEach(function (value, key) {
                    return properties.set(key, value);
                });
            }

            return properties;
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            if (!this._hashInitialized) {
                _get(RoutableReference.prototype.__proto__ || Object.getPrototypeOf(RoutableReference.prototype), "hashCode", this).call(this); // Initializes _hashValue.
                this._hashValue = HashUtil.addString(this._hashValue, this._adapterId);
            }
            return this._hashValue;
        }
    }, {
        key: "equals",
        value: function equals(rhs) {
            if (this === rhs) {
                return true;
            }
            if (!(rhs instanceof RoutableReference)) {
                return false;
            }

            if (!_get(RoutableReference.prototype.__proto__ || Object.getPrototypeOf(RoutableReference.prototype), "equals", this).call(this, rhs)) {
                return false;
            }

            if (this._locatorInfo === null ? rhs._locatorInfo !== null : !this._locatorInfo.equals(rhs._locatorInfo)) {
                return false;
            }
            if (this._routerInfo === null ? rhs._routerInfo !== null : !this._routerInfo.equals(rhs._routerInfo)) {
                return false;
            }
            if (this._cacheConnection !== rhs._cacheConnection) {
                return false;
            }
            if (this._preferSecure !== rhs._preferSecure) {
                return false;
            }
            if (this._endpointSelection !== rhs._endpointSelection) {
                return false;
            }
            if (this._locatorCacheTimeout !== rhs._locatorCacheTimeout) {
                return false;
            }
            if (this._connectionId !== rhs._connectionId) {
                return false;
            }
            if (this._overrideTimeout !== rhs._overrideTimeout) {
                return false;
            }
            if (this._overrideTimeout && this._timeout !== rhs._timeout) {
                return false;
            }
            if (!ArrayUtil.equals(this._endpoints, rhs._endpoints, function (e1, e2) {
                return e1.equals(e2);
            })) {
                return false;
            }
            if (this._adapterId !== rhs._adapterId) {
                return false;
            }
            return true;
        }
    }, {
        key: "getRequestHandler",
        value: function getRequestHandler(proxy) {
            return this._instance.requestHandlerFactory().getRequestHandler(this, proxy);
        }
    }, {
        key: "getBatchRequestQueue",
        value: function getBatchRequestQueue() {
            return new BatchRequestQueue(this._instance, this._mode === RefMode.ModeBatchDatagram);
        }
    }, {
        key: "getConnection",
        value: function getConnection() {
            var _this3 = this;

            var p = new Ice.Promise(); // success callback receives (connection)

            if (this._routerInfo !== null) {
                //
                // If we route, we send everything to the router's client
                // proxy endpoints.
                //
                this._routerInfo.getClientEndpoints().then(function (endpts) {
                    if (endpts.length > 0) {
                        _this3.applyOverrides(endpts);
                        _this3.createConnection(endpts).then(p.resolve, p.reject);
                    } else {
                        _this3.getConnectionNoRouterInfo(p);
                    }
                }).catch(p.reject);
            } else {
                this.getConnectionNoRouterInfo(p);
            }
            return p;
        }
    }, {
        key: "getConnectionNoRouterInfo",
        value: function getConnectionNoRouterInfo(p) {
            var _this4 = this;

            if (this._endpoints.length > 0) {
                this.createConnection(this._endpoints).then(p.resolve).catch(p.reject);
                return;
            }

            if (this._locatorInfo !== null) {
                this._locatorInfo.getEndpoints(this, null, this._locatorCacheTimeout).then(function (values) {
                    var _values = _slicedToArray(values, 2),
                        endpoints = _values[0],
                        cached = _values[1];

                    if (endpoints.length === 0) {
                        p.reject(new Ice.NoEndpointException(_this4.toString()));
                        return;
                    }

                    _this4.applyOverrides(endpoints);
                    _this4.createConnection(endpoints).then(p.resolve, function (ex) {
                        if (ex instanceof Ice.NoEndpointException) {
                            //
                            // No need to retry if there's no endpoints.
                            //
                            p.reject(ex);
                        } else {
                            Debug.assert(_this4._locatorInfo !== null);
                            _this4.getLocatorInfo().clearCache(_this4);
                            if (cached) {
                                var traceLevels = _this4.getInstance().traceLevels();
                                if (traceLevels.retry >= 2) {
                                    _this4.getInstance().initializationData().logger.trace(traceLevels.retryCat, "connection to cached endpoints failed\n" + "removing endpoints from cache and trying again\n" + ex.toString());
                                }
                                _this4.getConnectionNoRouterInfo(p); // Retry.
                                return;
                            }
                            p.reject(ex);
                        }
                    });
                }).catch(p.reject);
            } else {
                p.reject(new Ice.NoEndpointException(this.toString()));
            }
        }
    }, {
        key: "clone",
        value: function clone() {
            var r = new RoutableReference(this.getInstance(), this.getCommunicator(), this.getIdentity(), this.getFacet(), this.getMode(), this.getSecure(), this.getProtocol(), this.getEncoding(), this._endpoints, this._adapterId, this._locatorInfo, this._routerInfo, this._cacheConnection, this._preferSecure, this._endpointSelection, this._locatorCacheTimeout, this._invocationTimeout);
            this.copyMembers(r);
            return r;
        }
    }, {
        key: "copyMembers",
        value: function copyMembers(rhs) {
            //
            // Copy the members that are not passed to the constructor.
            //
            _get(RoutableReference.prototype.__proto__ || Object.getPrototypeOf(RoutableReference.prototype), "copyMembers", this).call(this, rhs);
            rhs._overrideTimeout = this._overrideTimeout;
            rhs._timeout = this._timeout;
            rhs._connectionId = this._connectionId;
        }
    }, {
        key: "applyOverrides",
        value: function applyOverrides(endpts) {
            //
            // Apply the endpoint overrides to each endpoint.
            //
            for (var i = 0; i < endpts.length; ++i) {
                endpts[i] = endpts[i].changeConnectionId(this._connectionId);
                if (this._overrideTimeout) {
                    endpts[i] = endpts[i].changeTimeout(this._timeout);
                }
            }
        }
    }, {
        key: "filterEndpoints",
        value: function filterEndpoints(allEndpoints) {
            //
            // Filter out opaque endpoints or endpoints which can't connect.
            //
            var endpoints = allEndpoints.filter(function (e) {
                return !(e instanceof OpaqueEndpointI) && e.connectable();
            });

            //
            // Filter out endpoints according to the mode of the reference.
            //
            switch (this.getMode()) {
                case RefMode.ModeTwoway:
                case RefMode.ModeOneway:
                case RefMode.ModeBatchOneway:
                    {
                        //
                        // Filter out datagram endpoints.
                        //
                        endpoints = endpoints.filter(function (e) {
                            return !e.datagram();
                        });
                        break;
                    }

                case RefMode.ModeDatagram:
                case RefMode.ModeBatchDatagram:
                    {
                        //
                        // Filter out non-datagram endpoints.
                        //
                        endpoints = endpoints.filter(function (e) {
                            return e.datagram();
                        });
                        break;
                    }
            }

            //
            // Sort the endpoints according to the endpoint selection type.
            //
            switch (this.getEndpointSelection()) {
                case EndpointSelectionType.Random:
                    {
                        //
                        // Shuffle the endpoints.
                        //
                        ArrayUtil.shuffle(endpoints);
                        break;
                    }
                case EndpointSelectionType.Ordered:
                    {
                        // Nothing to do.
                        break;
                    }
                default:
                    {
                        Debug.assert(false);
                        break;
                    }
            }

            //
            // If a secure connection is requested or secure overrides is
            // set, remove all non-secure endpoints. Otherwise if preferSecure is set
            // make secure endpoints prefered. By default make non-secure
            // endpoints preferred over secure endpoints.
            //
            var overrides = this.getInstance().defaultsAndOverrides();
            if (overrides.overrideSecure ? overrides.overrideSecureValue : this.getSecure()) {
                endpoints = endpoints.filter(function (e) {
                    return e.secure();
                });
            } else {
                var preferSecure = this.getPreferSecure();
                var compare = function compare(e1, e2) {
                    var ls = e1.secure();
                    var rs = e2.secure();
                    if (ls && rs || !ls && !rs) {
                        return 0;
                    } else if (!ls && rs) {
                        return preferSecure ? 1 : -1;
                    } else {
                        return preferSecure ? -1 : 1;
                    }
                };
                endpoints.sort(compare);
            }
            return endpoints;
        }
    }, {
        key: "createConnection",
        value: function createConnection(allEndpoints) {
            var endpoints = this.filterEndpoints(allEndpoints);
            if (endpoints.length === 0) {
                return Ice.Promise.reject(new Ice.NoEndpointException(this.toString()));
            }

            //
            // Finally, create the connection.
            //
            var promise = new Ice.Promise();
            var factory = this.getInstance().outgoingConnectionFactory();
            if (this.getCacheConnection() || endpoints.length == 1) {
                //
                // Get an existing connection or create one if there's no
                // existing connection to one of the given endpoints.
                //
                var cb = new CreateConnectionCallback(this, null, promise);
                factory.create(endpoints, false, this.getEndpointSelection()).then(function (connection) {
                    return cb.setConnection(connection);
                }).catch(function (ex) {
                    return cb.setException(ex);
                });
            } else {
                //
                // Go through the list of endpoints and try to create the
                // connection until it succeeds. This is different from just
                // calling create() with the given endpoints since this might
                // create a new connection even if there's an existing
                // connection for one of the endpoints.
                //
                var _cb = new CreateConnectionCallback(this, endpoints, promise);
                factory.create([endpoints[0]], true, this.getEndpointSelection()).then(function (connection) {
                    return _cb.setConnection(connection);
                }).catch(function (ex) {
                    return _cb.setException(ex);
                });
            }
            return promise;
        }
    }]);

    return RoutableReference;
}(Reference);

Ice.RoutableReference = RoutableReference;
module.exports.Ice = Ice;

var CreateConnectionCallback = function () {
    function CreateConnectionCallback(r, endpoints, promise) {
        _classCallCheck(this, CreateConnectionCallback);

        this.ref = r;
        this.endpoints = endpoints;
        this.promise = promise;
        this.i = 0;
        this.exception = null;
    }

    _createClass(CreateConnectionCallback, [{
        key: "setConnection",
        value: function setConnection(connection) {
            //
            // If we have a router, set the object adapter for this router
            // (if any) to the new connection, so that callbacks from the
            // router can be received over this new connection.
            //
            if (this.ref.getRouterInfo() !== null && this.ref.getRouterInfo().getAdapter() !== null) {
                connection.setAdapter(this.ref.getRouterInfo().getAdapter());
            }
            this.promise.resolve(connection);
        }
    }, {
        key: "setException",
        value: function setException(ex) {
            var _this5 = this;

            if (this.exception === null) {
                this.exception = ex;
            }

            if (this.endpoints === null || ++this.i === this.endpoints.length) {
                this.promise.reject(this.exception);
                return;
            }

            this.ref.getInstance().outgoingConnectionFactory().create([this.endpoints[this.i]], this.i != this.endpoints.length - 1, this.ref.getEndpointSelection()).then(function (connection) {
                return _this5.setConnection(connection);
            }).catch(function (ex) {
                return _this5.setException(ex);
            });
        }
    }]);

    return CreateConnectionCallback;
}();