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
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/HashMap", "../Ice/Reference", "../Ice/ConnectRequestHandler"]);

var Debug = Ice.Debug;
var HashMap = Ice.HashMap;
var ConnectRequestHandler = Ice.ConnectRequestHandler;

var RequestHandlerFactory = function () {
    function RequestHandlerFactory(instance) {
        _classCallCheck(this, RequestHandlerFactory);

        this._instance = instance;
        this._handlers = new HashMap(HashMap.compareEquals);
    }

    _createClass(RequestHandlerFactory, [{
        key: "getRequestHandler",
        value: function getRequestHandler(ref, proxy) {
            var connect = false;
            var handler = void 0;
            if (ref.getCacheConnection()) {
                handler = this._handlers.get(ref);
                if (!handler) {
                    handler = new ConnectRequestHandler(ref, proxy);
                    this._handlers.set(ref, handler);
                    connect = true;
                }
            } else {
                connect = true;
                handler = new ConnectRequestHandler(ref, proxy);
            }

            if (connect) {
                ref.getConnection().then(function (connection) {
                    handler.setConnection(connection);
                }, function (ex) {
                    handler.setException(ex);
                });
            }
            return proxy._setRequestHandler(handler.connect(proxy));
        }
    }, {
        key: "removeRequestHandler",
        value: function removeRequestHandler(ref, handler) {
            if (ref.getCacheConnection()) {
                if (this._handlers.get(ref) === handler) {
                    this._handlers.delete(ref);
                }
            }
        }
    }]);

    return RequestHandlerFactory;
}();

Ice.RequestHandlerFactory = RequestHandlerFactory;
module.exports.Ice = Ice;