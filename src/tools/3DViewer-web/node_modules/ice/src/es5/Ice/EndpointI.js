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

var Ice = require("../Ice/Endpoint").Ice;

var EndpointI = function () {
    function EndpointI() {
        _classCallCheck(this, EndpointI);
    }

    _createClass(EndpointI, [{
        key: "toString",
        value: function toString() {
            //
            // WARNING: Certain features, such as proxy validation in Glacier2,
            // depend on the format of proxy strings. Changes to toString() and
            // methods called to generate parts of the reference string could break
            // these features. Please review for all features that depend on the
            // format of proxyToString() before changing this and related code.
            //
            return this.protocol() + this.options();
        }
    }, {
        key: "initWithOptions",
        value: function initWithOptions(args) {
            var unknown = [];

            var str = "`" + this.protocol();
            for (var i = 0; i < args.length; ++i) {
                if (args[i].search(/[ \t\n\r]+/) !== -1) {
                    str += " \"" + args[i] + "\"";
                } else {
                    str += " " + args[i];
                }
            }
            str += "'";

            for (var _i = 0; _i < args.length;) {
                var option = args[_i++];
                if (option.length < 2 || option.charAt(0) != '-') {
                    unknown.push(option);
                    continue;
                }

                var argument = null;
                if (_i < args.length && args[_i].charAt(0) != '-') {
                    argument = args[_i++];
                }

                if (!this.checkOption(option, argument, str)) {
                    unknown.push(option);
                    if (argument !== null) {
                        unknown.push(argument);
                    }
                }
            }

            args.length = 0;
            for (var _i2 = 0; _i2 < unknown.length; _i2++) {
                args.push(unknown[_i2]);
            }
        }
        //
        // Compare endpoints for sorting purposes
        //

    }, {
        key: "equals",
        value: function equals(p) {
            if (!(p instanceof EndpointI)) {
                return false;
            }
            return this.compareTo(p) === 0;
        }
    }, {
        key: "checkOption",
        value: function checkOption() {
            return false;
        }
    }]);

    return EndpointI;
}();

Ice.EndpointI = EndpointI;
module.exports.Ice = Ice;