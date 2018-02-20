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
Ice._ModuleRegistry.require(module, ["../Ice/HashMap", "../Ice/LocatorInfo", "../Ice/LocatorTable", "../Ice/Locator"]);

var HashMap = Ice.HashMap;
var LocatorInfo = Ice.LocatorInfo;
var LocatorTable = Ice.LocatorTable;
var LocatorPrx = Ice.LocatorPrx;

var LocatorManager = function () {
    function LocatorManager(properties) {
        _classCallCheck(this, LocatorManager);

        this._background = properties.getPropertyAsInt("Ice.BackgroundLocatorCacheUpdates") > 0;
        this._table = new HashMap(HashMap.compareEquals); // Map<Ice.LocatorPrx, LocatorInfo>
        this._locatorTables = new HashMap(HashMap.compareEquals); // Map<Ice.Identity, LocatorTable>
    }

    _createClass(LocatorManager, [{
        key: "destroy",
        value: function destroy() {
            var _iteratorNormalCompletion = true;
            var _didIteratorError = false;
            var _iteratorError = undefined;

            try {
                for (var _iterator = this._table.values()[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                    var locator = _step.value;

                    locator.destroy();
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
            this._locatorTables.clear();
        }

        //
        // Returns locator info for a given locator. Automatically creates
        // the locator info if it doesn't exist yet.
        //

    }, {
        key: "find",
        value: function find(loc) {
            if (loc === null) {
                return null;
            }

            //
            // The locator can't be located.
            //
            var locator = LocatorPrx.uncheckedCast(loc.ice_locator(null));

            //
            // TODO: reap unused locator info objects?
            //
            var info = this._table.get(locator);
            if (info === undefined) {
                //
                // Rely on locator identity for the adapter table. We want to
                // have only one table per locator (not one per locator
                // proxy).
                //
                var table = this._locatorTables.get(locator.ice_getIdentity());
                if (table === undefined) {
                    table = new LocatorTable();
                    this._locatorTables.set(locator.ice_getIdentity(), table);
                }

                info = new LocatorInfo(locator, table, this._background);
                this._table.set(locator, info);
            }

            return info;
        }
    }]);

    return LocatorManager;
}();

Ice.LocatorManager = LocatorManager;
module.exports.Ice = Ice;