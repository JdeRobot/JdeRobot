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

//
// Ice.Object
//
// Using IceObject in this file to avoid collisions with the native Object.
//
var Ice = require("../Ice/ModuleRegistry").Ice;
Ice._ModuleRegistry.require(module, ["../Ice/Exception", "../Ice/FormatType", "../Ice/StreamHelpers", "../Ice/OptionalFormat"]);

var ids = ["::Ice::Object"];

Ice.Object = function () {
    function _class() {
        _classCallCheck(this, _class);
    }

    _createClass(_class, [{
        key: "ice_isA",
        value: function ice_isA(s, current) {
            return this._iceMostDerivedType()._iceIds.indexOf(s) >= 0;
        }
    }, {
        key: "ice_ping",
        value: function ice_ping(current) {}
    }, {
        key: "ice_ids",
        value: function ice_ids(current) {
            return this._iceMostDerivedType()._iceIds;
        }
    }, {
        key: "ice_id",
        value: function ice_id(current) {
            return this._iceMostDerivedType()._iceId;
        }
    }, {
        key: "toString",
        value: function toString() {
            return "[object " + this.ice_id() + "]";
        }

        //
        // _iceMostDerivedType returns the the most derived Ice generated class. This is
        // necessary because the user might extend Slice generated classes. The user
        // class extensions don't have _iceId, _iceIds, etc static members so the implementation
        // of ice_id and ice_ids would fail trying to access those members of the user
        // defined class. Instead, ice_id, ice_ids and ice_instanceof call _iceMostDerivedType
        // to get the most derived Ice class.
        //

    }, {
        key: "_iceMostDerivedType",
        value: function _iceMostDerivedType() {
            return Ice.Object;
        }

        //
        // The default implementation of equals compare references.
        //

    }, {
        key: "equals",
        value: function equals(other) {
            return this === other;
        }
    }], [{
        key: "_iceImplements",
        get: function get() {
            return [];
        }
    }]);

    return _class;
}();

module.exports.Ice = Ice;