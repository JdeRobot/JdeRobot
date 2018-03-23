var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

var Ice = require("../Ice/Promise").Ice;

var AsyncResultBase = function (_Ice$Promise) {
    _inherits(AsyncResultBase, _Ice$Promise);

    function AsyncResultBase(communicator, op, connection, proxy, adapter) {
        _classCallCheck(this, AsyncResultBase);

        var _this = _possibleConstructorReturn(this, (AsyncResultBase.__proto__ || Object.getPrototypeOf(AsyncResultBase)).call(this));

        _this._communicator = communicator;
        _this._instance = communicator !== null ? communicator.instance : null;
        _this._operation = op;
        _this._connection = connection;
        _this._proxy = proxy;
        _this._adapter = adapter;
        return _this;
    }

    _createClass(AsyncResultBase, [{
        key: "communicator",
        get: function get() {
            return this._communicator;
        }
    }, {
        key: "connection",
        get: function get() {
            return this._connection;
        }
    }, {
        key: "proxy",
        get: function get() {
            return this._proxy;
        }
    }, {
        key: "adapter",
        get: function get() {
            return this._adapter;
        }
    }, {
        key: "operation",
        get: function get() {
            return this._operation;
        }
    }]);

    return AsyncResultBase;
}(Ice.Promise);

Ice.AsyncResultBase = AsyncResultBase;

module.exports.Ice = Ice;