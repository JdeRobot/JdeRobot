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

var Ice = require("../Ice/ReferenceMode").Ice;
var ReferenceMode = Ice.ReferenceMode;

var ConnectionRequestHandler = function () {
    function ConnectionRequestHandler(ref, connection) {
        _classCallCheck(this, ConnectionRequestHandler);

        this._reference = ref;
        this._response = ref.getMode() == ReferenceMode.ModeTwoway;
        this._connection = connection;
    }

    _createClass(ConnectionRequestHandler, [{
        key: "update",
        value: function update(previousHandler, newHandler) {
            try {
                if (previousHandler === this) {
                    return newHandler;
                } else if (previousHandler.getConnection() === this._connection) {
                    //
                    // If both request handlers point to the same connection, we also
                    // update the request handler. See bug ICE-5489 for reasons why
                    // this can be useful.
                    //
                    return newHandler;
                }
            } catch (ex) {
                // Ignore
            }
            return this;
        }
    }, {
        key: "sendAsyncRequest",
        value: function sendAsyncRequest(out) {
            return out.invokeRemote(this._connection, this._response);
        }
    }, {
        key: "asyncRequestCanceled",
        value: function asyncRequestCanceled(out) {
            return this._connection.asyncRequestCanceled(out);
        }
    }, {
        key: "getReference",
        value: function getReference() {
            return this._reference;
        }
    }, {
        key: "getConnection",
        value: function getConnection() {
            return this._connection;
        }
    }]);

    return ConnectionRequestHandler;
}();

Ice.ConnectionRequestHandler = ConnectionRequestHandler;
module.exports.Ice = Ice;