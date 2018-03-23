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

Ice.TraceLevels = function (properties) {
    var networkCat = "Network";
    var protocolCat = "Protocol";
    var retryCat = "Retry";
    var locationCat = "Locator";
    var slicingCat = "Slicing";

    var keyBase = "Ice.Trace.";

    var network = properties.getPropertyAsInt(keyBase + networkCat);
    var protocol = properties.getPropertyAsInt(keyBase + protocolCat);
    var retry = properties.getPropertyAsInt(keyBase + retryCat);
    var location = properties.getPropertyAsInt(keyBase + locationCat);
    var slicing = properties.getPropertyAsInt(keyBase + slicingCat);

    properties.getPropertyAsInt(keyBase + "ThreadPool"); // Avoid an "unused property" warning.

    return function () {
        function _class() {
            _classCallCheck(this, _class);
        }

        _createClass(_class, null, [{
            key: "network",
            get: function get() {
                return network;
            }
        }, {
            key: "networkCat",
            get: function get() {
                return networkCat;
            }
        }, {
            key: "protocol",
            get: function get() {
                return protocol;
            }
        }, {
            key: "protocolCat",
            get: function get() {
                return protocolCat;
            }
        }, {
            key: "retry",
            get: function get() {
                return retry;
            }
        }, {
            key: "retryCat",
            get: function get() {
                return retryCat;
            }
        }, {
            key: "location",
            get: function get() {
                return location;
            }
        }, {
            key: "locationCat",
            get: function get() {
                return locationCat;
            }
        }, {
            key: "slicing",
            get: function get() {
                return slicing;
            }
        }, {
            key: "slicingCat",
            get: function get() {
                return slicingCat;
            }
        }]);

        return _class;
    }();
};
module.exports.Ice = Ice;