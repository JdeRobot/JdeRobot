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

var Ice = require("../Ice/WSEndpoint").Ice;
var WSEndpoint = Ice.WSEndpoint;

var WSEndpointFactory = function (_WSEndpoint) {
    _inherits(WSEndpointFactory, _WSEndpoint);

    function WSEndpointFactory(instance, delegate) {
        _classCallCheck(this, WSEndpointFactory);

        var _this = _possibleConstructorReturn(this, (WSEndpointFactory.__proto__ || Object.getPrototypeOf(WSEndpointFactory)).call(this));

        _this._instance = instance;
        _this._delegate = delegate;
        return _this;
    }

    _createClass(WSEndpointFactory, [{
        key: "type",
        value: function type() {
            return this._instance.type();
        }
    }, {
        key: "protocol",
        value: function protocol() {
            return this._instance.protocol();
        }
    }, {
        key: "create",
        value: function create(args, oaEndpoint) {
            var e = new WSEndpoint(this._instance, this._delegate.create(args, oaEndpoint));
            e.initWithOptions(args);
            return e;
        }
    }, {
        key: "read",
        value: function read(s) {
            var e = new WSEndpoint(this._instance, this._delegate.read(s));
            e.initWithStream(s);
            return e;
        }
    }, {
        key: "destroy",
        value: function destroy() {
            this._delegate.destroy();
            this._instance = null;
        }
    }]);

    return WSEndpointFactory;
}(WSEndpoint);

Ice.WSEndpointFactory = WSEndpointFactory;
exports.Ice = Ice;