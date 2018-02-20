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
Ice._ModuleRegistry.require(module, ["../Ice/HashMap", "../Ice/RouterInfo", "../Ice/Router"]);

var HashMap = Ice.HashMap;
var RouterInfo = Ice.RouterInfo;
var RouterPrx = Ice.RouterPrx;

var RouterManager = function () {
    function RouterManager() {
        _classCallCheck(this, RouterManager);

        this._table = new HashMap(HashMap.compareEquals); // Map<Ice.RouterPrx, RouterInfo>
    }

    _createClass(RouterManager, [{
        key: "destroy",
        value: function destroy() {
            var _iteratorNormalCompletion = true;
            var _didIteratorError = false;
            var _iteratorError = undefined;

            try {
                for (var _iterator = this._table.values()[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                    var router = _step.value;

                    router.destroy();
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

            this._table.clear();
        }

        //
        // Returns router info for a given router. Automatically creates
        // the router info if it doesn't exist yet.
        //

    }, {
        key: "find",
        value: function find(rtr) {
            if (rtr === null) {
                return null;
            }

            //
            // The router cannot be routed.
            //
            var router = RouterPrx.uncheckedCast(rtr.ice_router(null));

            var info = this._table.get(router);
            if (info === undefined) {
                info = new RouterInfo(router);
                this._table.set(router, info);
            }

            return info;
        }
    }, {
        key: "erase",
        value: function erase(rtr) {
            var info = null;
            if (rtr !== null) {
                // The router cannot be routed.
                var router = RouterPrx.uncheckedCast(rtr.ice_router(null));

                info = this._table.get(router);
                this._table.delete(router);
            }
            return info;
        }
    }]);

    return RouterManager;
}();

Ice.RouterManager = RouterManager;
module.exports.Ice = Ice;