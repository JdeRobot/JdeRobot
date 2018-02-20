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
Ice._ModuleRegistry.require(module, ["../Ice/LocalException", "../Ice/Current"]);

var Context = Ice.Context;
var InitializationException = Ice.InitializationException;

//
// The base class for all ImplicitContext implementations
//

var ImplicitContextI = function () {
    function ImplicitContextI() {
        _classCallCheck(this, ImplicitContextI);

        this._context = new Context();
    }

    _createClass(ImplicitContextI, [{
        key: "getContext",
        value: function getContext() {
            return new Context(this._context);
        }
    }, {
        key: "setContext",
        value: function setContext(context) {
            if (context !== null && context.size > 0) {
                this._context = new Context(context);
            } else {
                this._context.clear();
            }
        }
    }, {
        key: "containsKey",
        value: function containsKey(key) {
            if (key === null) {
                key = "";
            }

            return this._context.has(key);
        }
    }, {
        key: "get",
        value: function get(key) {
            if (key === null) {
                key = "";
            }

            var val = this._context.get(key);
            if (val === null) {
                val = "";
            }

            return val;
        }
    }, {
        key: "put",
        value: function put(key, value) {
            if (key === null) {
                key = "";
            }
            if (value === null) {
                value = "";
            }

            var oldVal = this._context.get(key);
            if (oldVal === null) {
                oldVal = "";
            }

            this._context.set(key, value);

            return oldVal;
        }
    }, {
        key: "remove",
        value: function remove(key) {
            if (key === null) {
                key = "";
            }

            var val = this._context.get(key);
            this._context.delete(key);

            if (val === null) {
                val = "";
            }
            return val;
        }
    }, {
        key: "write",
        value: function write(prxContext, os) {
            if (prxContext.size === 0) {
                Ice.ContextHelper.write(os, this._context);
            } else {
                var ctx = null;
                if (this._context.size === 0) {
                    ctx = prxContext;
                } else {
                    ctx = new Context(this._context);
                    var _iteratorNormalCompletion = true;
                    var _didIteratorError = false;
                    var _iteratorError = undefined;

                    try {
                        for (var _iterator = prxContext[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                            var _step$value = _slicedToArray(_step.value, 2),
                                key = _step$value[0],
                                value = _step$value[1];

                            ctx.set(key, value);
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
                Ice.ContextHelper.write(os, ctx);
            }
        }
    }], [{
        key: "create",
        value: function create(kind) {
            if (kind.length === 0 || kind === "None") {
                return null;
            } else if (kind === "Shared") {
                return new ImplicitContextI();
            } else {
                throw new InitializationException("'" + kind + "' is not a valid value for Ice.ImplicitContext");
            }
        }
    }]);

    return ImplicitContextI;
}();

Ice.ImplicitContextI = ImplicitContextI;
module.exports.Ice = Ice;