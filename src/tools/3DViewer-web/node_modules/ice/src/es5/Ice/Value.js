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

//
// Ice.Value
//
var Ice = require("../Ice/ModuleRegistry").Ice;
Ice._ModuleRegistry.require(module, ["../Ice/Exception", "../Ice/FormatType", "../Ice/StreamHelpers", "../Ice/OptionalFormat"]);

var ids = ["::Ice::Object"];

Ice.Value = function () {
    function _class() {
        _classCallCheck(this, _class);
    }

    _createClass(_class, [{
        key: "ice_preMarshal",
        value: function ice_preMarshal() {}
    }, {
        key: "ice_postUnmarshal",
        value: function ice_postUnmarshal() {}
    }, {
        key: "ice_getSlicedData",
        value: function ice_getSlicedData() {
            return null;
        }
    }, {
        key: "_iceWrite",
        value: function _iceWrite(os) {
            os.startValue(null);
            writeImpl(this, os, this._iceMostDerivedType());
            os.endValue();
        }
    }, {
        key: "_iceRead",
        value: function _iceRead(is) {
            is.startValue();
            readImpl(this, is, this._iceMostDerivedType());
            is.endValue(false);
        }

        //
        // These methods are used for object parameters.
        //

    }], [{
        key: "write",
        value: function write(os, v) {
            os.writeValue(v);
        }
    }, {
        key: "writeOptional",
        value: function writeOptional(os, tag, v) {
            os.writeOptionalValue(tag, v);
        }
    }, {
        key: "read",
        value: function read(is) {
            var v = { value: null };
            is.readValue(function (o) {
                return v.value = o;
            }, this);
            return v;
        }
    }, {
        key: "readOptional",
        value: function readOptional(is, tag) {
            var v = { value: undefined };
            is.readOptionalValue(tag, function (o) {
                return v.value = o;
            }, this);
            return v;
        }
    }]);

    return _class;
}();

Ice.InterfaceByValue = function (_Ice$Value) {
    _inherits(_class2, _Ice$Value);

    function _class2(id) {
        _classCallCheck(this, _class2);

        var _this = _possibleConstructorReturn(this, (_class2.__proto__ || Object.getPrototypeOf(_class2)).call(this));

        _this._id = id;
        return _this;
    }

    _createClass(_class2, [{
        key: "ice_id",
        value: function ice_id() {
            return this._id;
        }
    }, {
        key: "_iceWrite",
        value: function _iceWrite(os) {
            os.startValue(null);
            os.startSlice(this.ice_id(), -1, true);
            os.endSlice();
            os.endValue();
        }
    }, {
        key: "_iceRead",
        value: function _iceRead(is) {
            is.startValue();
            is.startSlice();
            is.endSlice();
            is.endValue(false);
        }
    }]);

    return _class2;
}(Ice.Value);

//
// Private methods
//
var writeImpl = function writeImpl(obj, os, type) {
    //
    // The writeImpl method is a recursive method that goes down the
    // class hierarchy to marshal each slice of the class using the
    // generated _iceWriteMemberImpl method.
    //

    if (type === undefined || type === Ice.Value) {
        return; // Don't marshal anything for Ice.Value
    }

    os.startSlice(type.ice_staticId(), Object.prototype.hasOwnProperty.call(type, '_iceCompactId') ? type._iceCompactId : -1, Object.getPrototypeOf(type) === Ice.Value);
    if (type.prototype._iceWriteMemberImpl) {
        type.prototype._iceWriteMemberImpl.call(obj, os);
    }
    os.endSlice();
    writeImpl(obj, os, Object.getPrototypeOf(type));
};

var readImpl = function readImpl(obj, is, type) {
    //
    // The readImpl method is a recursive method that goes down the
    // class hierarchy to unmarshal each slice of the class using the
    // generated _iceReadMemberImpl method.
    //

    if (type === undefined || type === Ice.Value) {
        return; // Don't unmarshal anything for Ice.Value
    }

    is.startSlice();
    if (type.prototype._iceReadMemberImpl) {
        type.prototype._iceReadMemberImpl.call(obj, is);
    }
    is.endSlice();
    readImpl(obj, is, Object.getPrototypeOf(type));
};

function writePreserved(os) {
    //
    // For Slice classes which are marked "preserved", the implementation of this method
    // replaces the Ice.Value.prototype._iceWrite method.
    //
    os.startValue(this._iceSlicedData);
    writeImpl(this, os, this._iceMostDerivedType());
    os.endValue();
}

function readPreserved(is) {
    //
    // For Slice classes which are marked "preserved", the implementation of this method
    // replaces the Ice.Value.prototype._iceRead method.
    //
    is.startValue();
    readImpl(this, is, this._iceMostDerivedType());
    this._iceSlicedData = is.endValue(true);
}

function ice_getSlicedData() {
    return this._iceSlicedData;
}

var Slice = Ice.Slice;

Slice.defineValue = function (valueType, id, preserved) {
    var compactId = arguments.length > 3 && arguments[3] !== undefined ? arguments[3] : 0;

    valueType.prototype.ice_id = function () {
        return id;
    };

    valueType.prototype._iceMostDerivedType = function () {
        return valueType;
    };

    valueType.ice_staticId = function () {
        return id;
    };

    if (preserved) {
        valueType.prototype.ice_getSlicedData = ice_getSlicedData;
        valueType.prototype._iceWrite = writePreserved;
        valueType.prototype._iceRead = readPreserved;
    }

    if (compactId > 0) {
        Ice.CompactIdRegistry.set(compactId, id);
    }
};
Slice.defineValue(Ice.Value, "::Ice::Object");

module.exports.Ice = Ice;