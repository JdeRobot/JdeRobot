var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

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
var _ModuleRegistry = Ice._ModuleRegistry;
_ModuleRegistry.require(module, ["../Ice/Current", "../Ice/Exception", "../Ice/FormatType", "../Ice/Object", "../Ice/ObjectPrx", "../Ice/OptionalFormat", "../Ice/StreamHelpers"]);

var builtinHelpers = [Ice.ByteHelper, Ice.BoolHelper, Ice.ShortHelper, Ice.IntHelper, Ice.LongHelper, Ice.FloatHelper, Ice.DoubleHelper, Ice.StringHelper, Ice.Value, Ice.ObjectPrx];

function parseParam(p) {
    var type = p[0];
    var t = typeof type === "undefined" ? "undefined" : _typeof(type);
    if (t === 'number') {
        type = builtinHelpers[p[0]];
    } else if (t === 'string') {
        type = _ModuleRegistry.type(type);
    }

    return {
        "type": type,
        "isObject": p[1] === true,
        "tag": p[2] // Optional tag, which may not be present - an undefined tag means "not optional".
    };
}

//
// Each operation descriptor is a property. The key is the "on-the-wire"
// name, and the value is an array consisting of the following elements:
//
//  0: native method name in case of a keyword conflict (e.g., "_while"),
//     otherwise an empty string
//  1: mode (undefined == Normal or int)
//  2: sendMode (undefined == Normal or int)
//  3: format (undefined == Default or int)
//  4: return type (undefined if void, or [type, tag])
//  5: in params (undefined if none, or array of [type, tag])
//  6: out params (undefined if none, or array of [type, tag])
//  7: exceptions (undefined if none, or array of types)
//  8: sends classes (true or undefined)
//  9: returns classes (true or undefined)
//
function parseOperation(name, arr) {
    var r = {};

    r.name = name;
    r.servantMethod = arr[0] ? arr[0] : name;
    r.mode = arr[1] ? Ice.OperationMode.valueOf(arr[1]) : Ice.OperationMode.Normal;
    r.sendMode = arr[2] ? Ice.OperationMode.valueOf(arr[2]) : Ice.OperationMode.Normal;
    r.format = arr[3] ? Ice.FormatType.valueOf(arr[3]) : Ice.FormatType.DefaultFormat;

    var ret = void 0;
    if (arr[4]) {
        ret = parseParam(arr[4]);
        ret.pos = 0;
    }
    r.returns = ret;

    var inParams = [];
    var inParamsOpt = [];
    if (arr[5]) {
        for (var i = 0; i < arr[5].length; ++i) {
            var p = parseParam(arr[5][i]);
            p.pos = i;
            inParams.push(p);
            if (p.tag) {
                inParamsOpt.push(p);
            }
        }
    }
    inParamsOpt.sort(function (p1, p2) {
        return p1.tag - p2.tag;
    }); // Sort by tag.
    r.inParams = inParams;
    r.inParamsOpt = inParamsOpt;

    var outParams = [];
    var outParamsOpt = [];
    if (arr[6]) {
        var offs = ret ? 1 : 0;
        for (var _i = 0; _i < arr[6].length; ++_i) {
            var _p = parseParam(arr[6][_i]);
            _p.pos = _i + offs;
            outParams.push(_p);
            if (_p.tag) {
                outParamsOpt.push(_p);
            }
        }
    }
    if (ret && ret.tag) {
        outParamsOpt.push(ret);
    }
    outParamsOpt.sort(function (p1, p2) {
        return p1.tag - p2.tag;
    }); // Sort by tag.
    r.outParams = outParams;
    r.outParamsOpt = outParamsOpt;

    var exceptions = [];
    if (arr[7]) {
        for (var _i2 = 0; _i2 < arr[7].length; ++_i2) {
            exceptions.push(arr[7][_i2]);
        }
    }
    r.exceptions = exceptions;

    r.sendsClasses = arr[8] === true;
    r.returnsClasses = arr[9] === true;

    return r;
}

var OpTable = function () {
    function OpTable(ops) {
        _classCallCheck(this, OpTable);

        this.raw = ops;
        this.parsed = {};
    }

    _createClass(OpTable, [{
        key: "find",
        value: function find(name) {
            //
            // Check if we've already parsed the operation.
            //
            var op = this.parsed[name];
            if (op === undefined && this.raw[name] !== undefined) {
                //
                // We haven't parsed it yet, but we found a match for the name, so parse it now.
                //
                op = parseOperation(name, this.raw[name]);
                this.parsed[name] = op;
            }
            return op;
        }
    }]);

    return OpTable;
}();

function unmarshalParams(is, retvalInfo, allParamInfo, optParamInfo, usesClasses, params, offset) {
    var readParam = function readParam(p, optional) {
        if (optional) {
            if (p.isObject) {
                is.readOptionalValue(p.tag, function (obj) {
                    return params[p.pos + offset] = obj;
                }, p.type);
            } else {
                params[p.pos + offset] = p.type.readOptional(is, p.tag);
            }
        } else {
            if (p.isObject) {
                is.readValue(function (obj) {
                    return params[p.pos + offset] = obj;
                }, p.type);
            } else {
                params[p.pos + offset] = p.type.read(is);
            }
        }
    };

    //
    // First read all required params.
    //
    for (var i = 0; i < allParamInfo.length; ++i) {
        if (!allParamInfo[i].tag) {
            readParam(allParamInfo[i], false);
        }
    }

    //
    // Then read a required return value (if any).
    //
    if (retvalInfo) {
        readParam(retvalInfo, false);
    }

    //
    // Then read all optional params.
    //
    for (var _i3 = 0; _i3 < optParamInfo.length; ++_i3) {
        readParam(optParamInfo[_i3], true);
    }

    if (usesClasses) {
        is.readPendingValues();
    }
}

function marshalParams(os, params, retvalInfo, paramInfo, optParamInfo, usesClasses) {
    //
    // Write the required params.
    //
    for (var i = 0; i < paramInfo.length; ++i) {
        var p = paramInfo[i];
        if (!p.tag) {
            p.type.write(os, params[p.pos]);
        }
    }

    //
    // retvalInfo should only be provided if there is a non-void required return value.
    //
    if (retvalInfo) {
        retvalInfo.type.write(os, params[retvalInfo.pos]);
    }

    //
    // Write the optional params.
    //
    for (var _i4 = 0; _i4 < optParamInfo.length; ++_i4) {
        var _p2 = optParamInfo[_i4];
        _p2.type.writeOptional(os, _p2.tag, params[_p2.pos]);
    }

    if (usesClasses) {
        os.writePendingValues();
    }
}

function dispatchImpl(servant, op, incomingAsync, current) {
    //
    // Check to make sure the servant implements the operation.
    //
    var method = servant[op.servantMethod];
    if (method === undefined || typeof method !== "function") {
        throw new Ice.UnknownException("servant for identity " + current.adapter.getCommunicator().identityToString(current.id) + " does not define operation `" + op.servantMethod + "'");
    }

    //
    // Unmarshal the in params (if any).
    //
    var params = [];
    if (op.inParams.length === 0) {
        incomingAsync.readEmptyParams();
    } else {
        var is = incomingAsync.startReadParams();
        unmarshalParams(is, undefined, op.inParams, op.inParamsOpt, op.sendsClasses, params, 0);
        incomingAsync.endReadParams();
    }

    params.push(current);

    incomingAsync.setFormat(op.format);

    var marshalFn = function marshalFn(params) {
        var numExpectedResults = op.outParams.length + (op.returns ? 1 : 0);
        if (numExpectedResults > 1 && !(params instanceof Array)) {
            throw new Ice.MarshalException("operation `" + op.servantMethod + "' should return an array");
        } else if (numExpectedResults === 1) {
            params = [params]; // Wrap a single out parameter in an array.
        }

        if (op.returns === undefined && op.outParams.length === 0) {
            if (params && params.length > 0) {
                throw new Ice.MarshalException("operation `" + op.servantMethod + "' shouldn't return any value");
            } else {
                incomingAsync.writeEmptyParams();
            }
        } else {
            var retvalInfo = void 0;
            if (op.returns && !op.returns.tag) {
                retvalInfo = op.returns;
            }

            var os = incomingAsync.startWriteParams();
            marshalParams(os, params, retvalInfo, op.outParams, op.outParamsOpt, op.returnsClasses);
            incomingAsync.endWriteParams();
        }
    };

    var results = method.apply(servant, params);
    if (results instanceof Promise) {
        return results.then(marshalFn);
    } else {
        marshalFn(results);
        return null;
    }
}

function getServantMethodFromInterfaces(interfaces, methodName, all) {
    var method = void 0;
    for (var i = 0; method === undefined && i < interfaces.length; ++i) {
        var intf = interfaces[i];
        method = intf[methodName];
        if (method === undefined) {
            if (all.indexOf(intf) === -1) {
                all.push(intf);
            }
            if (intf._iceImplements) {
                method = getServantMethodFromInterfaces(intf._iceImplements, methodName, all);
            }
        }
    }
    return method;
}

var dispatchPrefix = "_iceD_";

function getServantMethod(servantType, name) {
    //
    // The dispatch method is named _iceD_<Slice name> and is stored in the type (not the prototype).
    //
    var methodName = dispatchPrefix + name;

    //
    // First check the servant type.
    //
    var method = servantType[methodName];

    var allInterfaces = void 0;

    if (method === undefined) {
        allInterfaces = [];

        //
        // Now check the prototypes of the implemented interfaces.
        //
        var curr = servantType;
        while (curr && method === undefined) {
            if (curr._iceImplements) {
                method = getServantMethodFromInterfaces(curr._iceImplements, methodName, allInterfaces);
            }
            curr = Object.getPrototypeOf(curr);
        }

        if (method !== undefined) {
            //
            // Add the method to the servant's type.
            //
            servantType[methodName] = method;
        }
    }

    if (method === undefined) {
        //
        // Next check the op table for the servant's type.
        //
        var op = void 0;
        if (servantType._iceOps) {
            op = servantType._iceOps.find(name);
        }

        var source = void 0;
        if (op === undefined) {
            //
            // Now check the op tables of the base types.
            //
            var parent = Object.getPrototypeOf(servantType);
            while (op === undefined && parent) {
                if (parent._iceOps) {
                    if ((op = parent._iceOps.find(name)) !== undefined) {
                        source = parent;
                    }
                }
                parent = Object.getPrototypeOf(parent);
            }

            //
            // Now check the op tables of all base interfaces.
            //
            for (var i = 0; op === undefined && i < allInterfaces.length; ++i) {
                var intf = allInterfaces[i];
                if (intf._iceOps) {
                    if ((op = intf._iceOps.find(name)) !== undefined) {
                        source = intf;
                    }
                }
            }
        }

        if (op !== undefined) {
            method = function method(servant, incomingAsync, current) {
                return dispatchImpl(servant, op, incomingAsync, current);
            };

            //
            // Add the method to the servant type.
            //
            servantType[methodName] = method;

            //
            // Also add the method to the type in which the operation was found.
            //
            if (source) {
                source[methodName] = method;
            }
        }
    }

    return method;
}

function addProxyOperation(proxyType, name, data) {
    var method = data[0] ? data[0] : name;

    var op = null;

    proxyType.prototype[method] = function () {
        var args = arguments;

        //
        // Parse the operation data on the first invocation of a proxy method.
        //
        if (op === null) {
            op = parseOperation(name, data);
        }

        var ctx = args[op.inParams.length]; // The request context is the last argument (if present).

        var marshalFn = null;
        if (op.inParams.length > 0) {
            marshalFn = function marshalFn(os, params) {
                //
                // Validate the parameters.
                //
                for (var i = 0; i < op.inParams.length; ++i) {
                    var p = op.inParams[i];
                    var v = params[p.pos];
                    if (!p.tag || v !== undefined) {
                        if (typeof p.type.validate === "function") {
                            if (!p.type.validate(v)) {
                                throw new Ice.MarshalException("invalid value for argument " + (i + 1) + " in operation `" + op.servantMethod + "'");
                            }
                        }
                    }
                }

                marshalParams(os, params, undefined, op.inParams, op.inParamsOpt, op.sendsClasses);
            };
        }

        var unmarshalFn = null;
        if (op.returns || op.outParams.length > 0) {
            unmarshalFn = function unmarshalFn(asyncResult) {
                //
                // The results array holds the out parameters in the following format:
                //
                // [retval, out1, out2, ..., asyncResult]
                //
                var results = [];

                var is = asyncResult.startReadParams();
                var retvalInfo = void 0;
                if (op.returns && !op.returns.tag) {
                    retvalInfo = op.returns;
                }
                unmarshalParams(is, retvalInfo, op.outParams, op.outParamsOpt, op.returnsClasses, results, 0);
                asyncResult.endReadParams();
                return results.length == 1 ? results[0] : results;
            };
        }
        return Ice.ObjectPrx._invoke(this, op.name, op.sendMode, op.format, ctx, marshalFn, unmarshalFn, op.exceptions, Array.prototype.slice.call(args));
    };
}

var Slice = Ice.Slice;
Slice.defineOperations = function (classType, proxyType, ids, pos, ops) {
    if (ops) {
        classType._iceOps = new OpTable(ops);
    }

    classType.prototype._iceDispatch = function (incomingAsync, current) {
        //
        // Retrieve the dispatch method for this operation.
        //
        var method = getServantMethod(classType, current.operation);

        if (method === undefined || typeof method !== 'function') {
            throw new Ice.OperationNotExistException(current.id, current.facet, current.operation);
        }

        return method.call(method, this, incomingAsync, current);
    };

    classType.prototype._iceMostDerivedType = function () {
        return classType;
    };

    Object.defineProperty(classType, "_iceIds", {
        get: function get() {
            return ids;
        }
    });

    Object.defineProperty(classType, "_iceId", {
        get: function get() {
            return ids[pos];
        }
    });

    classType.ice_staticId = function () {
        return classType._iceId;
    };

    if (proxyType !== undefined) {
        if (ops) {
            for (var name in ops) {
                addProxyOperation(proxyType, name, ops[name]);
            }
        }

        //
        // Copy proxy methods from super-interfaces.
        //
        if (proxyType._implements) {
            for (var intf in proxyType._implements) {
                var proto = proxyType._implements[intf].prototype;
                for (var f in proto) {
                    if (typeof proto[f] == "function" && proxyType.prototype[f] === undefined) {
                        proxyType.prototype[f] = proto[f];
                    }
                }
            }
        }

        Object.defineProperty(proxyType, "_id", {
            get: function get() {
                return ids[pos];
            }
        });
    }
};

//
// Define the "built-in" operations for all Ice objects.
//
Slice.defineOperations(Ice.Object, Ice.ObjectPrx, ["::Ice::Object"], 0, {
    "ice_ping": [, 1, 1,,,,,],
    "ice_isA": [, 1, 1,, [1], [[7]],,],
    "ice_id": [, 1, 1,, [7],,,],
    "ice_ids": [, 1, 1,, ["Ice.StringSeqHelper"],,,]
});

module.exports.Ice = Ice;