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
var fs = require("fs");

var Debug = function () {
    function Debug() {
        _classCallCheck(this, Debug);
    }

    _createClass(Debug, null, [{
        key: "assert",
        value: function assert(b, msg) {
            if (!b) {
                fs.writeSync(process.stderr.fd, msg === undefined ? "assertion failed" : msg);
                fs.writeSync(process.stderr.fd, new Error().stack);
                process.exit(1);
            }
        }
    }]);

    return Debug;
}();

Ice.Debug = Debug;
module.exports.Ice = Ice;