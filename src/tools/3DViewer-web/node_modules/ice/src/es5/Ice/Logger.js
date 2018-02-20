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

var Logger = function () {
    function Logger(prefix) {
        _classCallCheck(this, Logger);

        if (prefix !== undefined && prefix.length > 0) {
            this._prefix = prefix + ": ";
        } else {
            this._prefix = "";
        }

        this._dateformat = {
            year: 'numeric',
            month: 'numeric',
            day: 'numeric',
            hour: 'numeric',
            minute: 'numeric',
            second: 'numeric',
            hour12: false
        };
    }

    _createClass(Logger, [{
        key: "print",
        value: function print(message) {
            this.write(message, false);
        }
    }, {
        key: "trace",
        value: function trace(category, message) {
            var s = [];
            var d = new Date();
            s.push("-- ");
            s.push(this.timestamp());
            s.push(' ');
            s.push(this._prefix);
            s.push(category);
            s.push(": ");
            s.push(message);
            this.write(s.join(""), true);
        }
    }, {
        key: "warning",
        value: function warning(message) {
            var s = [];
            var d = new Date();
            s.push("-! ");
            s.push(this.timestamp());
            s.push(' ');
            s.push(this._prefix);
            s.push("warning: ");
            s.push(message);
            this.write(s.join(""), true);
        }
    }, {
        key: "error",
        value: function error(message) {
            var s = [];
            var d = new Date();
            s.push("!! ");
            s.push(this.timestamp());
            s.push(' ');
            s.push(this._prefix);
            s.push("error: ");
            s.push(message);
            this.write(s.join(""), true);
        }
    }, {
        key: "cloneWithPrefix",
        value: function cloneWithPrefix(prefix) {
            return new Logger(prefix);
        }
    }, {
        key: "write",
        value: function write(message, indent) {
            if (indent) {
                message = message.replace(/\n/g, "\n   ");
            }

            console.log(message);
        }
    }, {
        key: "timestamp",
        value: function timestamp() {
            var d = new Date();
            return d.toLocaleString("en-US", this._dateformat) + "." + d.getMilliseconds();
        }
    }]);

    return Logger;
}();

Ice.Logger = Logger;
module.exports.Ice = Ice;