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
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/LocalException", "../Ice/StringUtil", "../Ice/IdentityUtil", "../Ice/HashMap"]);

var Debug = Ice.Debug;
var StringUtil = Ice.StringUtil;
var HashMap = Ice.HashMap;

//
// Only for use by Ice.ObjectAdatperI.
//

var ServantManager = function () {
    function ServantManager(instance, adapterName) {
        _classCallCheck(this, ServantManager);

        this._instance = instance;
        this._adapterName = adapterName;
        this._servantMapMap = new HashMap(HashMap.compareEquals); // Map<Ice.Identity, Map<String, Ice.Object> >
        this._defaultServantMap = new Map(); // Map<String, Ice.Object>
        this._locatorMap = new Map(); // Map<String, Ice.ServantLocator>
    }

    _createClass(ServantManager, [{
        key: "addServant",
        value: function addServant(servant, ident, facet) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            if (facet === null) {
                facet = "";
            }

            var m = this._servantMapMap.get(ident);
            if (m === undefined) {
                m = new Map();
                this._servantMapMap.set(ident, m);
            } else {
                if (m.has(facet)) {
                    var ex = new Ice.AlreadyRegisteredException();
                    ex.id = Ice.identityToString(ident, this._instance.toStringMode());
                    ex.kindOfObject = "servant";
                    if (facet.length > 0) {
                        ex.id += " -f " + StringUtil.escapeString(facet, "", this._instance.toStringMode());
                    }
                    throw ex;
                }
            }

            m.set(facet, servant);
        }
    }, {
        key: "addDefaultServant",
        value: function addDefaultServant(servant, category) {
            Debug.assert(this._instance !== null); // Must not be called after destruction

            if (this._defaultServantMap.has(category)) {
                var ex = new Ice.AlreadyRegisteredException();
                ex.kindOfObject = "default servant";
                ex.id = category;
                throw ex;
            }

            this._defaultServantMap.set(category, servant);
        }
    }, {
        key: "removeServant",
        value: function removeServant(ident, facet) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            if (facet === null) {
                facet = "";
            }

            var m = this._servantMapMap.get(ident);
            if (m === undefined || !m.has(facet)) {
                var ex = new Ice.NotRegisteredException();
                ex.id = Ice.identityToString(ident, this._instance.toStringMode());
                ex.kindOfObject = "servant";
                if (facet.length > 0) {
                    ex.id += " -f " + StringUtil.escapeString(facet, "", this._instance.toStringMode());
                }
                throw ex;
            }

            var obj = m.get(facet);
            m.delete(facet);

            if (m.size === 0) {
                this._servantMapMap.delete(ident);
            }

            return obj;
        }
    }, {
        key: "removeDefaultServant",
        value: function removeDefaultServant(category) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            var obj = this._defaultServantMap.get(category);
            if (obj === undefined) {
                var ex = new Ice.NotRegisteredException();
                ex.kindOfObject = "default servant";
                ex.id = category;
                throw ex;
            }

            this._defaultServantMap.delete(category);
            return obj;
        }
    }, {
        key: "removeAllFacets",
        value: function removeAllFacets(ident) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            var m = this._servantMapMap.get(ident);
            if (m === undefined) {
                var ex = new Ice.NotRegisteredException();
                ex.id = Ice.identityToString(ident, this._instance.toStringMode());
                ex.kindOfObject = "servant";
                throw ex;
            }

            this._servantMapMap.delete(ident);

            return m;
        }
    }, {
        key: "findServant",
        value: function findServant(ident, facet) {
            //
            // This assert is not valid if the adapter dispatch incoming
            // requests from bidir connections. This method might be called if
            // requests are received over the bidir connection after the
            // adapter was deactivated.
            //
            //Debug.assert(this._instance !== null); // Must not be called after destruction.

            if (facet === null) {
                facet = "";
            }

            var m = this._servantMapMap.get(ident);
            var obj = null;
            if (m === undefined) {
                obj = this._defaultServantMap.get(ident.category);
                if (obj === undefined) {
                    obj = this._defaultServantMap.get("");
                }
            } else {
                obj = m.get(facet);
            }

            return obj === undefined ? null : obj;
        }
    }, {
        key: "findDefaultServant",
        value: function findDefaultServant(category) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            var ds = this._defaultServantMap.get(category);
            return ds === undefined ? null : ds;
        }
    }, {
        key: "findAllFacets",
        value: function findAllFacets(ident) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            var m = this._servantMapMap.get(ident);
            if (m !== undefined) {
                return new Map(m);
            }

            return new Map();
        }
    }, {
        key: "hasServant",
        value: function hasServant(ident) {
            //
            // This assert is not valid if the adapter dispatch incoming
            // requests from bidir connections. This method might be called if
            // requests are received over the bidir connection after the
            // adapter was deactivated.
            //
            //Debug.assert(this._instance !== null); // Must not be called after destruction.

            var m = this._servantMapMap.get(ident);
            if (m === undefined) {
                return false;
            } else {
                Debug.assert(m.size > 0);
                return true;
            }
        }
    }, {
        key: "addServantLocator",
        value: function addServantLocator(locator, category) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            if (this._locatorMap.has(category)) {
                var ex = new Ice.AlreadyRegisteredException();
                ex.id = StringUtil.escapeString(category, "", this._instance.toStringMode());
                ex.kindOfObject = "servant locator";
                throw ex;
            }

            this._locatorMap.set(category, locator);
        }
    }, {
        key: "removeServantLocator",
        value: function removeServantLocator(category) {
            Debug.assert(this._instance !== null); // Must not be called after destruction.

            var l = this._locatorMap.get(category);
            if (l === undefined) {
                var ex = new Ice.NotRegisteredException();
                ex.id = StringUtil.escapeString(category, "", this._instance.toStringMode());
                ex.kindOfObject = "servant locator";
                throw ex;
            }
            this._locatorMap.delete(category);
            return l;
        }
    }, {
        key: "findServantLocator",
        value: function findServantLocator(category) {
            //
            // This assert is not valid if the adapter dispatch incoming
            // requests from bidir connections. This method might be called if
            // requests are received over the bidir connection after the
            // adapter was deactivated.
            //
            //Debug.assert(this._instance !== null); // Must not be called after destruction.

            var l = this._locatorMap.get(category);
            return l === undefined ? null : l;
        }

        //
        // Only for use by Ice.ObjectAdapterI.
        //

    }, {
        key: "destroy",
        value: function destroy() {
            Debug.assert(this._instance !== null); // Must not be called after destruction.
            var logger = this._instance.initializationData().logger;
            this._servantMapMap.clear();

            this._defaultServantMap.clear();

            var locatorMap = new Map(this._locatorMap);
            this._locatorMap.clear();
            this._instance = null;

            var _iteratorNormalCompletion = true;
            var _didIteratorError = false;
            var _iteratorError = undefined;

            try {
                for (var _iterator = locatorMap[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                    var _step$value = _slicedToArray(_step.value, 2),
                        key = _step$value[0],
                        locator = _step$value[1];

                    try {
                        locator.deactivate(key);
                    } catch (ex) {
                        logger.error("exception during locator deactivation:\nobject adapter: `" + this._adapterName + "'\nlocator category: `" + key + "'\n" + ex.toString());
                    }
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
    }]);

    return ServantManager;
}();

Ice.ServantManager = ServantManager;
module.exports.Ice = Ice;