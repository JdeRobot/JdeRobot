var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

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
var _ModuleRegistry = Ice._ModuleRegistry;
_ModuleRegistry.require(module, ["../Ice/StringUtil", "../Ice/UUID"]);
var StringUtil = Ice.StringUtil;

function setInternal(map, key, value, hash, index) {
    //
    // Search for an entry with the same key.
    //
    for (var e = map._table[index]; e !== null; e = e._nextInBucket) {
        if (e._hash === hash && map.keysEqual(key, e._key)) {
            //
            // Found a match, update the value.
            //
            e._value = value;
            return undefined;
        }
    }

    //
    // No match found, add a new entry.
    //
    map.add(key, value, hash, index);
    return undefined;
}

function compareEquals(v1, v2) {
    if (v1 === v2) {
        return true;
    }
    if (v1 === undefined || v1 === null || v2 === undefined || v2 === null) {
        return false;
    }
    return v1.equals(v2);
}

function compareIdentity(v1, v2) {
    return v1 === v2;
}

var HashMap = function () {
    function HashMap(arg1, arg2) {
        _classCallCheck(this, HashMap);

        //
        // The first argument can be a HashMap or the keyComparator, the second
        // argument if present is always the value comparator.
        //
        var h = void 0,
            keyComparator = void 0,
            valueComparator = void 0;

        if (typeof arg1 == "function") {
            keyComparator = arg1;
            valueComparator = arg2;
        } else if (arg1 instanceof HashMap) {
            h = arg1;
            keyComparator = h.keyComparator;
            valueComparator = h.valueComparator;
        }

        this._size = 0;
        this._head = null;
        this._initialCapacity = 32;
        this._loadFactor = 0.75;
        this._table = [];

        this._keyComparator = typeof keyComparator == "function" ? keyComparator : compareIdentity;
        this._valueComparator = typeof valueComparator == "function" ? valueComparator : compareIdentity;

        if (h instanceof HashMap && h._size > 0) {
            this._threshold = h._threshold;
            this._table.length = h._table.length;
            for (var i = 0; i < h._table.length; i++) {
                this._table[i] = null;
            }
            this.merge(h);
        } else {
            this._threshold = this._initialCapacity * this._loadFactor;
            for (var _i = 0; _i < this._initialCapacity; _i++) {
                this._table[_i] = null;
            }
        }
    }

    _createClass(HashMap, [{
        key: "set",
        value: function set(key, value) {
            var r = this.computeHash(key); // Returns an object with key,hash members.

            var index = this.hashIndex(r.hash, this._table.length);

            return setInternal(this, r.key, value, r.hash, index);
        }
    }, {
        key: "get",
        value: function get(key) {
            var r = this.computeHash(key); // Returns an object with key,hash members.
            var e = this.findEntry(r.key, r.hash);
            return e !== undefined ? e._value : undefined;
        }
    }, {
        key: "has",
        value: function has(key) {
            var r = this.computeHash(key); // Returns an object with key,hash members.
            return this.findEntry(r.key, r.hash) !== undefined;
        }
    }, {
        key: "delete",
        value: function _delete(key) {
            var r = this.computeHash(key); // Returns an object with key,hash members.

            var index = this.hashIndex(r.hash, this._table.length);

            //
            // Search for an entry with the same key.
            //
            var prev = null;
            for (var e = this._table[index]; e !== null; e = e._nextInBucket) {
                if (e._hash === r.hash && this.keysEqual(r.key, e._key)) {
                    //
                    // Found a match.
                    //
                    this._size--;

                    //
                    // Remove from bucket.
                    //
                    if (prev !== null) {
                        prev._nextInBucket = e._nextInBucket;
                    } else {
                        this._table[index] = e._nextInBucket;
                    }

                    //
                    // Unlink the entry.
                    //
                    if (e._prev !== null) {
                        e._prev._next = e._next;
                    }
                    if (e._next !== null) {
                        e._next._prev = e._prev;
                    }

                    if (this._head === e) {
                        this._head = e._next;
                    }

                    return e._value;
                }

                prev = e;
            }

            return undefined;
        }
    }, {
        key: "clear",
        value: function clear() {
            for (var i = 0; i < this._table.length; ++i) {
                this._table[i] = null;
            }
            this._head = null;
            this._size = 0;
        }
    }, {
        key: "forEach",
        value: function forEach(fn, obj) {
            obj = obj === undefined ? fn : obj;
            for (var e = this._head; e !== null; e = e._next) {
                fn.call(obj, e._value, e._key);
            }
        }
    }, {
        key: "entries",
        value: regeneratorRuntime.mark(function entries() {
            var e;
            return regeneratorRuntime.wrap(function entries$(_context) {
                while (1) {
                    switch (_context.prev = _context.next) {
                        case 0:
                            e = this._head;

                        case 1:
                            if (!(e !== null)) {
                                _context.next = 7;
                                break;
                            }

                            _context.next = 4;
                            return [e._key, e._value];

                        case 4:
                            e = e._next;
                            _context.next = 1;
                            break;

                        case 7:
                        case "end":
                            return _context.stop();
                    }
                }
            }, entries, this);
        })
    }, {
        key: "keys",
        value: regeneratorRuntime.mark(function keys() {
            var e;
            return regeneratorRuntime.wrap(function keys$(_context2) {
                while (1) {
                    switch (_context2.prev = _context2.next) {
                        case 0:
                            e = this._head;

                        case 1:
                            if (!(e !== null)) {
                                _context2.next = 7;
                                break;
                            }

                            _context2.next = 4;
                            return e._key;

                        case 4:
                            e = e._next;
                            _context2.next = 1;
                            break;

                        case 7:
                        case "end":
                            return _context2.stop();
                    }
                }
            }, keys, this);
        })
    }, {
        key: "values",
        value: regeneratorRuntime.mark(function values() {
            var e;
            return regeneratorRuntime.wrap(function values$(_context3) {
                while (1) {
                    switch (_context3.prev = _context3.next) {
                        case 0:
                            e = this._head;

                        case 1:
                            if (!(e !== null)) {
                                _context3.next = 7;
                                break;
                            }

                            _context3.next = 4;
                            return e._value;

                        case 4:
                            e = e._next;
                            _context3.next = 1;
                            break;

                        case 7:
                        case "end":
                            return _context3.stop();
                    }
                }
            }, values, this);
        })
    }, {
        key: "equals",
        value: function equals(other, valuesEqual) {
            var _this = this;

            if (other === null || !(other instanceof HashMap) || this._size !== other._size) {
                return false;
            }

            var eq = valuesEqual || function (v1, v2) {
                return _this._valueComparator.call(_this._valueComparator, v1, v2);
            };

            for (var e = this._head; e !== null; e = e._next) {
                var oe = other.findEntry(e._key, e._hash);
                if (oe === undefined || !eq(e._value, oe._value)) {
                    return false;
                }
            }
            return true;
        }
    }, {
        key: "merge",
        value: function merge(from) {
            for (var e = from._head; e !== null; e = e._next) {
                setInternal(this, e._key, e._value, e._hash, this.hashIndex(e._hash, this._table.length));
            }
        }
    }, {
        key: "add",
        value: function add(key, value, hash, index) {
            //
            // Create a new table entry.
            //
            var e = Object.create(null, {
                "key": {
                    enumerable: true,
                    get: function get() {
                        return this._key;
                    }
                },
                "value": {
                    enumerable: true,
                    get: function get() {
                        return this._value;
                    }
                },
                "next": {
                    enumerable: true,
                    get: function get() {
                        return this._next;
                    }
                },
                "_key": {
                    enumerable: false,
                    writable: true,
                    value: key
                },
                "_value": {
                    enumerable: false,
                    writable: true,
                    value: value
                },
                "_prev": {
                    enumerable: false,
                    writable: true,
                    value: null
                },
                "_next": {
                    enumerable: false,
                    writable: true,
                    value: null
                },
                "_nextInBucket": {
                    enumerable: false,
                    writable: true,
                    value: null
                },
                "_hash": {
                    enumerable: false,
                    writable: true,
                    value: hash
                }
            });
            e._nextInBucket = this._table[index];
            this._table[index] = e;

            e._next = this._head;
            if (this._head !== null) {
                this._head._prev = e;
            }
            this._head = e;

            this._size++;
            if (this._size >= this._threshold) {
                this.resize(this._table.length * 2);
            }
        }
    }, {
        key: "resize",
        value: function resize(capacity) {
            var newTable = new Array(capacity).fill(null);

            //
            // Re-assign all entries to buckets.
            //
            for (var e = this._head; e !== null; e = e._next) {
                var index = this.hashIndex(e._hash, capacity);
                e._nextInBucket = newTable[index];
                newTable[index] = e;
            }

            this._table = newTable;
            this._threshold = capacity * this._loadFactor;
        }
    }, {
        key: "findEntry",
        value: function findEntry(key, hash) {
            var index = this.hashIndex(hash, this._table.length);
            //
            // Search for an entry with the same key.
            //
            for (var e = this._table[index]; e !== null; e = e._nextInBucket) {
                if (e._hash === hash && this.keysEqual(key, e._key)) {
                    return e;
                }
            }

            return undefined;
        }
    }, {
        key: "hashIndex",
        value: function hashIndex(hash, len) {
            return hash & len - 1;
        }
    }, {
        key: "computeHash",
        value: function computeHash(v) {
            if (v === 0 || v === -0) {
                return { key: 0, hash: 0 };
            }

            if (v === null) {
                if (HashMap._null === null) {
                    var uuid = Ice.generateUUID();
                    HashMap._null = { key: uuid, hash: StringUtil.hashCode(uuid) };
                }
                return HashMap._null;
            }

            if (v === undefined) {
                throw new Error("cannot compute hash for undefined value");
            }

            if (typeof v.hashCode === "function") {
                return { key: v, hash: v.hashCode() };
            }

            var type = typeof v === "undefined" ? "undefined" : _typeof(v);
            if (type === "string" || v instanceof String) {
                return { key: v, hash: StringUtil.hashCode(v) };
            } else if (type === "number" || v instanceof Number) {
                if (isNaN(v)) {
                    if (HashMap._nan === null) {
                        var _uuid = Ice.generateUUID();
                        HashMap._nan = { key: _uuid, hash: StringUtil.hashCode(_uuid) };
                    }
                    return HashMap._nan;
                }
                return { key: v, hash: v.toFixed(0) };
            } else if (type === "boolean" || v instanceof Boolean) {
                return { key: v, hash: v ? 1 : 0 };
            }

            throw new Error("cannot compute hash for value of type " + type);
        }
    }, {
        key: "keysEqual",
        value: function keysEqual(k1, k2) {
            return this._keyComparator.call(this._keyComparator, k1, k2);
        }
    }, {
        key: "size",
        get: function get() {
            return this._size;
        }
    }]);

    return HashMap;
}();

HashMap.prototype[Symbol.iterator] = HashMap.prototype.entries;

Ice.HashMap = HashMap;

HashMap.compareEquals = compareEquals;
HashMap.compareIdentity = compareIdentity;
HashMap._null = null;
HashMap._nan = null;

var Slice = Ice.Slice;

Slice.defineDictionary = function (module, name, helperName, keyHelper, valueHelper, fixed, keysEqual, valueType) {
    if (keysEqual === undefined) {
        module[name] = Map;
    } else {
        //
        // Define a constructor function for a dictionary whose key type requires
        // comparison using an equals() method instead of the native comparison
        // operators.
        //
        module[name] = function (h) {
            return new HashMap(h || keysEqual);
        };
    }

    var helper = null;
    Object.defineProperty(module, helperName, {
        get: function get() {
            if (helper === null) {
                helper = Ice.StreamHelpers.generateDictHelper(_ModuleRegistry.type(keyHelper), _ModuleRegistry.type(valueHelper), fixed, _ModuleRegistry.type(valueType), module[name]);
            }
            return helper;
        }
    });
};
module.exports.Ice = Ice;