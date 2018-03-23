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

var modules = {};

var _ModuleRegistry = function () {
    function _ModuleRegistry() {
        _classCallCheck(this, _ModuleRegistry);
    }

    _createClass(_ModuleRegistry, null, [{
        key: "module",
        value: function module(name) {
            var m = modules[name];
            if (m === undefined) {
                m = {};
                modules[name] = m;
            }
            return m;
        }
    }, {
        key: "require",
        value: function require(m, paths) {
            var o = void 0;
            paths.forEach(function (path) {
                o = m.require(path);
            });
            return o;
        }
    }, {
        key: "type",
        value: function type(scoped) {
            if (scoped === undefined) {
                return undefined;
            }

            var components = scoped.split(".");
            var T = modules;

            for (var i = 0; i < components.length; ++i) {
                T = T[components[i]];
                if (T === undefined) {
                    return undefined;
                }
            }
            return T;
        }
    }]);

    return _ModuleRegistry;
}();

var Ice = _ModuleRegistry.module("Ice");
Ice.Slice = Ice.Slice || {};
Ice._ModuleRegistry = _ModuleRegistry;
exports.Ice = Ice;