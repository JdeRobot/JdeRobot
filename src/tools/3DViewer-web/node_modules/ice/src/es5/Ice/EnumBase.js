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
//
// Ice.EnumBase
//

var EnumBase = function () {
    function EnumBase(name, value) {
        _classCallCheck(this, EnumBase);

        this._name = name;
        this._value = value;
    }

    _createClass(EnumBase, [{
        key: "equals",
        value: function equals(rhs) {
            if (this === rhs) {
                return true;
            }

            if (!(rhs instanceof Object.getPrototypeOf(this).constructor)) {
                return false;
            }

            return this._value == rhs._value;
        }
    }, {
        key: "hashCode",
        value: function hashCode() {
            return this._value;
        }
    }, {
        key: "toString",
        value: function toString() {
            return this._name;
        }
    }, {
        key: "name",
        get: function get() {
            return this._name;
        }
    }, {
        key: "value",
        get: function get() {
            return this._value;
        }
    }]);

    return EnumBase;
}();

Ice.EnumBase = EnumBase;

var EnumHelper = function () {
    function EnumHelper(enumType) {
        _classCallCheck(this, EnumHelper);

        this._enumType = enumType;
    }

    _createClass(EnumHelper, [{
        key: "write",
        value: function write(os, v) {
            this._enumType._write(os, v);
        }
    }, {
        key: "writeOptional",
        value: function writeOptional(os, tag, v) {
            this._enumType._writeOpt(os, tag, v);
        }
    }, {
        key: "read",
        value: function read(is) {
            return this._enumType._read(is);
        }
    }, {
        key: "readOptional",
        value: function readOptional(is, tag) {
            return this._enumType._readOpt(is, tag);
        }
    }]);

    return EnumHelper;
}();

Ice.EnumHelper = EnumHelper;

var Slice = Ice.Slice;
Slice.defineEnum = function (enumerators) {
    var type = function (_EnumBase) {
        _inherits(type, _EnumBase);

        function type(n, v) {
            _classCallCheck(this, type);

            return _possibleConstructorReturn(this, (type.__proto__ || Object.getPrototypeOf(type)).call(this, n, v));
        }

        return type;
    }(EnumBase);

    var enums = [];
    var maxValue = 0;
    var firstEnum = null;

    for (var idx in enumerators) {
        var e = enumerators[idx][0],
            value = enumerators[idx][1];
        var enumerator = new type(e, value);
        enums[value] = enumerator;
        if (!firstEnum) {
            firstEnum = enumerator;
        }
        Object.defineProperty(type, e, {
            enumerable: true,
            value: enumerator
        });
        if (value > maxValue) {
            maxValue = value;
        }
    }

    Object.defineProperty(type, "minWireSize", {
        get: function get() {
            return 1;
        }
    });

    type._write = function (os, v) {
        if (v) {
            os.writeEnum(v);
        } else {
            os.writeEnum(firstEnum);
        }
    };
    type._read = function (is) {
        return is.readEnum(type);
    };
    type._writeOpt = function (os, tag, v) {
        if (v !== undefined) {
            if (os.writeOptional(tag, Ice.OptionalFormat.Size)) {
                type._write(os, v);
            }
        }
    };
    type._readOpt = function (is, tag) {
        return is.readOptionalEnum(tag, type);
    };

    type._helper = new EnumHelper(type);

    Object.defineProperty(type, 'valueOf', {
        value: function value(v) {
            if (v === undefined) {
                return type;
            }
            return enums[v];
        }
    });

    Object.defineProperty(type, 'maxValue', {
        value: maxValue
    });

    Object.defineProperty(type.prototype, 'maxValue', {
        value: maxValue
    });

    return type;
};
module.exports.Ice = Ice;