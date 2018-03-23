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

var Ice = require("../Ice/LocalException").Ice;

var AlreadyRegisteredException = Ice.AlreadyRegisteredException;
var NotRegisteredException = Ice.NotRegisteredException;

//
// Only for use by Instance
//

var ValueFactoryManagerI = function () {
    function ValueFactoryManagerI() {
        _classCallCheck(this, ValueFactoryManagerI);

        this._factoryMap = new Map(); // Map<String, ValueFactory>
    }

    _createClass(ValueFactoryManagerI, [{
        key: "add",
        value: function add(factory, id) {
            if (this._factoryMap.has(id)) {
                throw new AlreadyRegisteredException("value factory", id);
            }
            this._factoryMap.set(id, factory);
        }
    }, {
        key: "find",
        value: function find(id) {
            return this._factoryMap.get(id);
        }
    }, {
        key: "destroy",
        value: function destroy() {
            this._factoryMap = new Map(); // Map<String, ValueFactory>
        }
    }]);

    return ValueFactoryManagerI;
}();

Ice.ValueFactoryManagerI = ValueFactoryManagerI;
module.exports.Ice = Ice;