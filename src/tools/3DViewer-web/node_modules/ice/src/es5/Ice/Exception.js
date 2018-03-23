var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

function _extendableBuiltin3(cls) {
    function ExtendableBuiltin() {
        var instance = Reflect.construct(cls, Array.from(arguments));
        Object.setPrototypeOf(instance, Object.getPrototypeOf(this));
        return instance;
    }

    ExtendableBuiltin.prototype = Object.create(cls.prototype, {
        constructor: {
            value: cls,
            enumerable: false,
            writable: true,
            configurable: true
        }
    });

    if (Object.setPrototypeOf) {
        Object.setPrototypeOf(ExtendableBuiltin, cls);
    } else {
        ExtendableBuiltin.__proto__ = cls;
    }

    return ExtendableBuiltin;
}

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

function _extendableBuiltin(cls) {
    function ExtendableBuiltin() {
        var instance = Reflect.construct(cls, Array.from(arguments));
        Object.setPrototypeOf(instance, Object.getPrototypeOf(this));
        return instance;
    }

    ExtendableBuiltin.prototype = Object.create(cls.prototype, {
        constructor: {
            value: cls,
            enumerable: false,
            writable: true,
            configurable: true
        }
    });

    if (Object.setPrototypeOf) {
        Object.setPrototypeOf(ExtendableBuiltin, cls);
    } else {
        ExtendableBuiltin.__proto__ = cls;
    }

    return ExtendableBuiltin;
}

// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************

var Ice = require("../Ice/ModuleRegistry").Ice;

var _toString = function _toString(key, object, objectTable, ident) {
    ident += "  ";
    if (object === null) {
        return "\n" + ident + key + ": (null)";
    }
    if (object === undefined) {
        return "\n" + ident + key + ": (undefined)";
    }
    if (key == "stack" || typeof object == "function") {
        return "";
    }
    if ((typeof object === "undefined" ? "undefined" : _typeof(object)) != "object") {
        return "\n" + ident + key + ": \"" + object + "\"";
    }
    if (objectTable.indexOf(object) != -1) {
        return "\n" + ident + key + ": (recursive)";
    }

    objectTable.push(object);
    var s = "\n" + ident + key + ":";
    for (var k in object) {
        if (key.indexOf("_") === 0) {
            continue;
        }

        if (typeof object[k] == "function") {
            continue;
        }
        s += ident + _toString(k, object[k], objectTable, ident);
    }
    return s;
};

var ServantError = function (_extendableBuiltin2) {
    _inherits(ServantError, _extendableBuiltin2);

    function ServantError(cause) {
        _classCallCheck(this, ServantError);

        var _this = _possibleConstructorReturn(this, (ServantError.__proto__ || Object.getPrototypeOf(ServantError)).call(this));

        _this.cause = cause;
        return _this;
    }

    return ServantError;
}(_extendableBuiltin(Error));

Ice.ServantError = ServantError;

//
// Ice.Exception
//

var Exception = function (_extendableBuiltin4) {
    _inherits(Exception, _extendableBuiltin4);

    function Exception(cause) {
        _classCallCheck(this, Exception);

        var _this2 = _possibleConstructorReturn(this, (Exception.__proto__ || Object.getPrototypeOf(Exception)).call(this));

        if (cause) {
            _this2.ice_cause = cause;
        }
        return _this2;
    }

    _createClass(Exception, [{
        key: "ice_name",
        value: function ice_name() {
            return this.constructor._id.substr(2);
        }
    }, {
        key: "ice_id",
        value: function ice_id() {
            return this.constructor._id;
        }
    }, {
        key: "toString",
        value: function toString() {
            //
            // We have a guard here to prevent being re-entered. With some browsers (IE), accessing
            // the stack property ends up calling toString on the exception to print it out with the
            // stack.
            //
            if (this._inToStringAlready) {
                return "";
            }

            this._inToStringAlready = true;
            var s = this.ice_id();
            for (var key in this) {
                if (key != "_inToStringAlready") {
                    s += _toString(key, this[key], [], "");
                }
            }

            if (Ice._printStackTraces === true && this.stack) {
                s += "\n" + this.stack;
            }
            this._inToStringAlready = false;
            return s;
        }
    }], [{
        key: "captureStackTrace",
        value: function captureStackTrace(object) {
            var stack = new Error().stack;
            //
            // In IE 10 and greater the stack will be filled once the Error is throw
            // we don't need to do anything.
            //
            if (stack !== undefined) {
                Object.defineProperty(object, "stack", {
                    get: function get() {
                        return stack;
                    }
                });
            }
        }
    }, {
        key: "_id",
        get: function get() {
            return "::Ice::Exception";
        }
    }]);

    return Exception;
}(_extendableBuiltin3(Error));

Ice.Exception = Exception;

//
// Ice.LocalException
//

var LocalException = function (_Exception) {
    _inherits(LocalException, _Exception);

    function LocalException(cause) {
        _classCallCheck(this, LocalException);

        var _this3 = _possibleConstructorReturn(this, (LocalException.__proto__ || Object.getPrototypeOf(LocalException)).call(this, cause));

        Exception.captureStackTrace(_this3);
        return _this3;
    }

    _createClass(LocalException, null, [{
        key: "_id",
        get: function get() {
            return "::Ice::LocalException";
        }
    }]);

    return LocalException;
}(Exception);

Ice.LocalException = LocalException;

//
// Ice.UserException
//

var UserException = function (_Exception2) {
    _inherits(UserException, _Exception2);

    function UserException(cause) {
        _classCallCheck(this, UserException);

        var _this4 = _possibleConstructorReturn(this, (UserException.__proto__ || Object.getPrototypeOf(UserException)).call(this, cause));

        Exception.captureStackTrace(_this4);
        return _this4;
    }

    _createClass(UserException, [{
        key: "ice_getSlicedData",
        value: function ice_getSlicedData() {
            return null;
        }
    }, {
        key: "_write",
        value: function _write(os) {
            os.startException(null);
            writeImpl(this, os, this._mostDerivedType());
            os.endException();
        }
    }, {
        key: "_read",
        value: function _read(is) {
            is.startException();
            readImpl(this, is, this._mostDerivedType());
            is.endException(false);
        }
    }, {
        key: "_usesClasses",
        value: function _usesClasses() {
            return false;
        }
    }, {
        key: "_mostDerivedType",
        value: function _mostDerivedType() {
            return Ice.UserException;
        }
    }], [{
        key: "_id",
        get: function get() {
            return "::Ice::UserException";
        }
    }]);

    return UserException;
}(Exception);

Ice.UserException = UserException;

//
// Private methods
//

var writeImpl = function writeImpl(obj, os, type) {
    //
    // The writeImpl method is a recursive method that goes down the
    // class hierarchy to marshal each slice of the class using the
    // generated _writeMemberImpl method.
    //

    if (type === undefined || type === UserException) {
        return; // Don't marshal anything for Ice.UserException
    }

    os.startSlice(type._id, -1, type._parent === UserException);
    if (type.prototype._writeMemberImpl) {
        type.prototype._writeMemberImpl.call(obj, os);
    }
    os.endSlice();
    writeImpl(obj, os, type._parent);
};

var readImpl = function readImpl(obj, is, type) {
    //
    // The readImpl method is a recursive method that goes down the
    // class hierarchy to marshal each slice of the class using the
    // generated _readMemberImpl method.
    //

    if (type === undefined || type === UserException) {
        return; // Don't marshal anything for UserException
    }

    is.startSlice();
    if (type.prototype._readMemberImpl) {
        type.prototype._readMemberImpl.call(obj, is);
    }
    is.endSlice();
    readImpl(obj, is, type._parent);
};

var writePreserved = function writePreserved(os) {
    //
    // For Slice exceptions which are marked "preserved", the implementation of this method
    // replaces the Ice.UserException.prototype._write method.
    //
    os.startException(this._slicedData);
    writeImpl(this, os, this._mostDerivedType());
    os.endException();
};

var readPreserved = function readPreserved(is) {
    //
    // For Slice exceptions which are marked "preserved", the implementation of this method
    // replaces the Ice.UserException.prototype._read method.
    //
    is.startException();
    readImpl(this, is, this._mostDerivedType());
    this._slicedData = is.endException(true);
};

var ice_getSlicedData = function ice_getSlicedData() {
    return this._slicedData;
};

Ice.Slice.PreservedUserException = function (ex) {
    ex.prototype.ice_getSlicedData = ice_getSlicedData;
    ex.prototype._write = writePreserved;
    ex.prototype._read = readPreserved;
};

module.exports.Ice = Ice;