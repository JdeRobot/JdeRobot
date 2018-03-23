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

var Ice = require("../Ice/Value").Ice;

var SliceInfo = function SliceInfo() {
    _classCallCheck(this, SliceInfo);

    /**
    * The Slice type ID for this slice.
    **/
    this.typeId = "";

    /**
    * The Slice compact type ID for this slice.
    **/
    this.compactId = -1;

    /**
    * The encoded bytes for this slice, including the leading size integer.
    **/
    this.bytes = [];

    /**
    * The class instances referenced by this slice.
    **/
    this.instances = [];

    /**
    * Whether or not the slice contains optional members.
    **/
    this.hasOptionalMembers = false;

    /**
    * Whether or not this is the last slice.
    **/
    this.isLastSlice = false;
};

Ice.SliceInfo = SliceInfo;

var SlicedData = function SlicedData(slices) {
    _classCallCheck(this, SlicedData);

    this.slices = slices;
};

Ice.SlicedData = SlicedData;

var UnknownSlicedValue = function (_Ice$Value) {
    _inherits(UnknownSlicedValue, _Ice$Value);

    function UnknownSlicedValue(unknownTypeId) {
        _classCallCheck(this, UnknownSlicedValue);

        var _this = _possibleConstructorReturn(this, (UnknownSlicedValue.__proto__ || Object.getPrototypeOf(UnknownSlicedValue)).call(this));

        _this._unknownTypeId = unknownTypeId;
        return _this;
    }

    _createClass(UnknownSlicedValue, [{
        key: "ice_getSlicedData",
        value: function ice_getSlicedData() {
            return this._slicedData;
        }
    }, {
        key: "ice_id",
        value: function ice_id() {
            return this._unknownTypeId;
        }
    }, {
        key: "_iceWrite",
        value: function _iceWrite(os) {
            os.startValue(this._slicedData);
            os.endValue();
        }
    }, {
        key: "_iceRead",
        value: function _iceRead(is) {
            is.startValue();
            this._slicedData = is.endValue(true);
        }
    }]);

    return UnknownSlicedValue;
}(Ice.Value);

Ice.UnknownSlicedValue = UnknownSlicedValue;

module.exports.Ice = Ice;