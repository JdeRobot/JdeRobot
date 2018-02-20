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

var Ice = require("../Ice/StringUtil").Ice;
var StringUtil = Ice.StringUtil;

var HashUtil = function () {
    function HashUtil() {
        _classCallCheck(this, HashUtil);
    }

    _createClass(HashUtil, null, [{
        key: "addBoolean",
        value: function addBoolean(h, b) {
            return (h << 5) + h ^ (b ? 0 : 1);
        }
    }, {
        key: "addString",
        value: function addString(h, str) {
            if (str !== undefined && str !== null) {
                h = (h << 5) + h ^ StringUtil.hashCode(str);
            }
            return h;
        }
    }, {
        key: "addNumber",
        value: function addNumber(h, num) {
            return (h << 5) + h ^ num;
        }
    }, {
        key: "addHashable",
        value: function addHashable(h, obj) {
            if (obj !== undefined && obj !== null) {
                h = (h << 5) + h ^ obj.hashCode();
            }
            return h;
        }
    }, {
        key: "addArray",
        value: function addArray(h, arr, hashCode) {
            if (arr !== undefined && arr !== null) {
                for (var i = 0; i < arr.length; ++i) {
                    h = hashCode(h, arr[i]);
                }
            }
            return h;
        }
    }]);

    return HashUtil;
}();

Ice.HashUtil = HashUtil;
module.exports.Ice = Ice;