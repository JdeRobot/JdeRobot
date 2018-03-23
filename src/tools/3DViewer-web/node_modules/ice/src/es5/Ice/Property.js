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

Ice.Property = function () {
    function _class(pattern, deprecated, deprecatedBy) {
        _classCallCheck(this, _class);

        this._pattern = pattern;
        this._deprecated = deprecated;
        this._deprecatedBy = deprecatedBy;
    }

    _createClass(_class, [{
        key: "pattern",
        get: function get() {
            return this._pattern;
        }
    }, {
        key: "deprecated",
        get: function get() {
            return this._deprecated;
        }
    }, {
        key: "deprecatedBy",
        get: function get() {
            return this._deprecatedBy;
        }
    }]);

    return _class;
}();

module.exports.Ice = Ice;