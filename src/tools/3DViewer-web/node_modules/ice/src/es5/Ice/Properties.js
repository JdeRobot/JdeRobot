var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

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
Ice._ModuleRegistry.require(module, ["../Ice/StringUtil", "../Ice/PropertyNames", "../Ice/Debug", "../Ice/ProcessLogger", "../Ice/ProcessLogger", "../Ice/LocalException"]);

var StringUtil = Ice.StringUtil;
var PropertyNames = Ice.PropertyNames;
var Debug = Ice.Debug;
var ProcessLogger = Ice.ProcessLogger;
var getProcessLogger = Ice.getProcessLogger;
var InitializationException = Ice.InitializationException;

var ParseStateKey = 0;
var ParseStateValue = 1;
//
// Ice.Properties
//

var Properties = function () {
    function Properties(args, defaults) {
        _classCallCheck(this, Properties);

        this._properties = new Map();

        if (defaults !== undefined && defaults !== null) {
            //
            // NOTE: we can't just do a shallow copy of the map as the map values
            // would otherwise be shared between the two PropertiesI object.
            //
            //_properties = new Map(pi._properties);
            var _iteratorNormalCompletion = true;
            var _didIteratorError = false;
            var _iteratorError = undefined;

            try {
                for (var _iterator = defaults._properties[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                    var _step$value = _slicedToArray(_step.value, 2),
                        key = _step$value[0],
                        property = _step$value[1];

                    this._properties.set(key, { 'value': property.value, 'used': false });
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

        if (args !== undefined && args !== null) {
            var v = this.parseIceCommandLineOptions(args);
            args.length = 0;
            for (var i = 0; i < v.length; ++i) {
                args.push(v[i]);
            }
        }
    }

    _createClass(Properties, [{
        key: "getProperty",
        value: function getProperty(key) {
            return this.getPropertyWithDefault(key, "");
        }
    }, {
        key: "getPropertyWithDefault",
        value: function getPropertyWithDefault(key, value) {
            var pv = this._properties.get(key);
            if (pv !== undefined) {
                pv.used = true;
                return pv.value;
            } else {
                return value;
            }
        }
    }, {
        key: "getPropertyAsInt",
        value: function getPropertyAsInt(key) {
            return this.getPropertyAsIntWithDefault(key, 0);
        }
    }, {
        key: "getPropertyAsIntWithDefault",
        value: function getPropertyAsIntWithDefault(key, value) {
            var pv = this._properties.get(key);
            if (pv !== undefined) {
                pv.used = true;
                return parseInt(pv.value);
            } else {
                return value;
            }
        }
    }, {
        key: "getPropertyAsList",
        value: function getPropertyAsList(key) {
            return this.getPropertyAsListWithDefault(key, 0);
        }
    }, {
        key: "getPropertyAsListWithDefault",
        value: function getPropertyAsListWithDefault(key, value) {
            if (value === undefined || value === null) {
                value = [];
            }

            var pv = this._properties.get(key);
            if (pv !== undefined) {
                pv.used = true;

                var result = StringUtil.splitString(pv.value, ", \t\r\n");
                if (result === null) {
                    getProcessLogger().warning("mismatched quotes in property " + key + "'s value, returning default value");
                    return value;
                }
                if (result.length === 0) {
                    result = value;
                }
                return result;
            } else {
                return value;
            }
        }
    }, {
        key: "getPropertiesForPrefix",
        value: function getPropertiesForPrefix() {
            var prefix = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : "";

            var result = new Map();
            this._properties.forEach(function (property, key) {
                if (key.indexOf(prefix) === 0) {
                    property.used = true;
                    result.set(key, property.value);
                }
            });
            return result;
        }
    }, {
        key: "setProperty",
        value: function setProperty() {
            var key = arguments.length > 0 && arguments[0] !== undefined ? arguments[0] : "";
            var value = arguments.length > 1 && arguments[1] !== undefined ? arguments[1] : "";

            //
            // Trim whitespace
            //
            if (key !== null) {
                key = key.trim();
            }

            //
            // Check if the property is legal.
            //
            var logger = getProcessLogger();
            if (key === null || key.length === 0) {
                throw new InitializationException("Attempt to set property with empty key");
            }

            var dotPos = key.indexOf(".");
            if (dotPos !== -1) {
                var prefix = key.substr(0, dotPos);
                for (var i = 0; i < PropertyNames.validProps.length; ++i) {
                    var pattern = PropertyNames.validProps[i][0].pattern;
                    dotPos = pattern.indexOf(".");
                    //
                    // Each top level prefix describes a non-empty namespace. Having a string without a
                    // prefix followed by a dot is an error.
                    //
                    Debug.assert(dotPos != -1);
                    if (pattern.substring(0, dotPos - 1) != prefix) {
                        continue;
                    }

                    var found = false;
                    var mismatchCase = false;
                    var otherKey = void 0;
                    for (var j = 0; j < PropertyNames.validProps[i][j].length && !found; ++j) {
                        pattern = PropertyNames.validProps[i][j].pattern();
                        var pComp = new RegExp(pattern);
                        found = pComp.test(key);

                        if (found && PropertyNames.validProps[i][j].deprecated) {
                            logger.warning("deprecated property: " + key);
                            if (PropertyNames.validProps[i][j].deprecatedBy !== null) {
                                key = PropertyNames.validProps[i][j].deprecatedBy;
                            }
                        }

                        if (found) {
                            break;
                        } else {
                            pComp = new RegExp(pattern.toUpperCase());
                            found = pComp.test(key.toUpperCase());
                            if (found) {
                                mismatchCase = true;
                                otherKey = pattern.substr(2);
                                otherKey = otherKey.substr(0, otherKey.length - 1);
                                otherKey = otherKey.replace(/\\/g, "");
                                break;
                            }
                        }
                    }

                    if (!found) {
                        logger.warning("unknown property: " + key);
                    } else if (mismatchCase) {
                        logger.warning("unknown property: `" + key + "'; did you mean `" + otherKey + "'");
                    }
                }
            }

            //
            // Set or clear the property.
            //
            if (value !== null && value.length > 0) {
                var pv = this._properties.get(key);
                if (pv !== undefined) {
                    pv.value = value;
                } else {
                    this._properties.set(key, { 'value': value, 'used': false });
                }
            } else {
                this._properties.delete(key);
            }
        }
    }, {
        key: "getCommandLineOptions",
        value: function getCommandLineOptions() {
            var result = [];
            this._properties.forEach(function (property, key) {
                result.push("--" + key + "=" + property.value);
            });
            return result;
        }
    }, {
        key: "parseCommandLineOptions",
        value: function parseCommandLineOptions(pfx, options) {
            var _this = this;

            if (pfx.length > 0 && pfx.charAt(pfx.length - 1) != ".") {
                pfx += ".";
            }
            pfx = "--" + pfx;

            var result = [];

            options.forEach(function (opt) {
                if (opt.indexOf(pfx) === 0) {
                    if (opt.indexOf('=') === -1) {
                        opt += "=1";
                    }

                    _this.parseLine(opt.substring(2));
                } else {
                    result.push(opt);
                }
            });
            return result;
        }
    }, {
        key: "parseIceCommandLineOptions",
        value: function parseIceCommandLineOptions(options) {
            var args = options.slice();
            for (var i = 0; i < PropertyNames.clPropNames.length; ++i) {
                args = this.parseCommandLineOptions(PropertyNames.clPropNames[i], args);
            }
            return args;
        }
    }, {
        key: "parse",
        value: function parse(data) {
            var _this2 = this;

            data.match(/[^\r\n]+/g).forEach(function (line) {
                return _this2.parseLine(line);
            });
        }
    }, {
        key: "parseLine",
        value: function parseLine(line) {
            var key = "";
            var value = "";

            var state = ParseStateKey;

            var whitespace = "";
            var escapedspace = "";
            var finished = false;

            for (var i = 0; i < line.length; ++i) {
                var c = line.charAt(i);
                switch (state) {
                    case ParseStateKey:
                        {
                            switch (c) {
                                case '\\':
                                    if (i < line.length - 1) {
                                        c = line.charAt(++i);
                                        switch (c) {
                                            case '\\':
                                            case '#':
                                            case '=':
                                                key += whitespace;
                                                whitespace = "";
                                                key += c;
                                                break;

                                            case ' ':
                                                if (key.length !== 0) {
                                                    whitespace += c;
                                                }
                                                break;

                                            default:
                                                key += whitespace;
                                                whitespace = "";
                                                key += '\\';
                                                key += c;
                                                break;
                                        }
                                    } else {
                                        key += whitespace;
                                        key += c;
                                    }
                                    break;

                                case ' ':
                                case '\t':
                                case '\r':
                                case '\n':
                                    if (key.length !== 0) {
                                        whitespace += c;
                                    }
                                    break;

                                case '=':
                                    whitespace = "";
                                    state = ParseStateValue;
                                    break;

                                case '#':
                                    finished = true;
                                    break;

                                default:
                                    key += whitespace;
                                    whitespace = "";
                                    key += c;
                                    break;
                            }
                            break;
                        }

                    case ParseStateValue:
                        {
                            switch (c) {
                                case '\\':
                                    if (i < line.length - 1) {
                                        c = line.charAt(++i);
                                        switch (c) {
                                            case '\\':
                                            case '#':
                                            case '=':
                                                value += value.length === 0 ? escapedspace : whitespace;
                                                whitespace = "";
                                                escapedspace = "";
                                                value += c;
                                                break;

                                            case ' ':
                                                whitespace += c;
                                                escapedspace += c;
                                                break;

                                            default:
                                                value += value.length === 0 ? escapedspace : whitespace;
                                                whitespace = "";
                                                escapedspace = "";
                                                value += '\\';
                                                value += c;
                                                break;
                                        }
                                    } else {
                                        value += value.length === 0 ? escapedspace : whitespace;
                                        value += c;
                                    }
                                    break;

                                case ' ':
                                case '\t':
                                case '\r':
                                case '\n':
                                    if (value.length !== 0) {
                                        whitespace += c;
                                    }
                                    break;

                                case '#':
                                    finished = true;
                                    break;

                                default:
                                    value += value.length === 0 ? escapedspace : whitespace;
                                    whitespace = "";
                                    escapedspace = "";
                                    value += c;
                                    break;
                            }
                            break;
                        }
                }
                if (finished) {
                    break;
                }
            }
            value += escapedspace;

            if (state === ParseStateKey && key.length !== 0 || state == ParseStateValue && key.length === 0) {
                getProcessLogger().warning("invalid config file entry: \"" + line + "\"");
                return;
            } else if (key.length === 0) {
                return;
            }

            this.setProperty(key, value);
        }
    }, {
        key: "clone",
        value: function clone() {
            return new Properties(null, this);
        }
    }, {
        key: "getUnusedProperties",
        value: function getUnusedProperties() {
            var unused = [];
            this._properties.forEach(function (property, key) {
                if (!property.used) {
                    unused.push(key);
                }
            });
            return unused;
        }
    }], [{
        key: "createProperties",
        value: function createProperties(args, defaults) {
            return new Properties(args, defaults);
        }
    }]);

    return Properties;
}();

Ice.Properties = Properties;
module.exports.Ice = Ice;