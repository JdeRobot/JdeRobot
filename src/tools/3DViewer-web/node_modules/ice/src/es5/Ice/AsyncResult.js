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

var Ice = require("../Ice/ModuleRegistry").Ice;
Ice._ModuleRegistry.require(module, ["../Ice/AsyncResultBase", "../Ice/Debug", "../Ice/Protocol", "../Ice/Exception", "../Ice/Stream"]);

var AsyncResultBase = Ice.AsyncResultBase;
var Debug = Ice.Debug;
var Protocol = Ice.Protocol;
var UserException = Ice.UserException;
var OutputStream = Ice.OutputStream;

var AsyncResult = function (_AsyncResultBase) {
    _inherits(AsyncResult, _AsyncResultBase);

    function AsyncResult(com, op, connection, proxy, adapter, completedFn) {
        _classCallCheck(this, AsyncResult);

        var _this = _possibleConstructorReturn(this, (AsyncResult.__proto__ || Object.getPrototypeOf(AsyncResult)).call(this, com, op, connection, proxy, adapter));

        _this._completed = completedFn;
        _this._is = null;
        _this._os = com !== null ? new OutputStream(_this._instance, Protocol.currentProtocolEncoding) : null;
        _this._state = 0;
        _this._exception = null;
        _this._sentSynchronously = false;
        return _this;
    }

    _createClass(AsyncResult, [{
        key: "cancel",
        value: function cancel() {
            this.cancelWithException(new Ice.InvocationCanceledException());
        }
    }, {
        key: "isCompleted",
        value: function isCompleted() {
            return (this._state & AsyncResult.Done) > 0;
        }
    }, {
        key: "isSent",
        value: function isSent() {
            return (this._state & AsyncResult.Sent) > 0;
        }
    }, {
        key: "throwLocalException",
        value: function throwLocalException() {
            if (this._exception !== null) {
                throw this._exception;
            }
        }
    }, {
        key: "sentSynchronously",
        value: function sentSynchronously() {
            return this._sentSynchronously;
        }
    }, {
        key: "markSent",
        value: function markSent(done) {
            Debug.assert((this._state & AsyncResult.Done) === 0);
            this._state |= AsyncResult.Sent;
            if (done) {
                this._state |= AsyncResult.Done | AsyncResult.OK;
                this._cancellationHandler = null;
                this.resolve();
            }
        }
    }, {
        key: "markFinished",
        value: function markFinished(ok, completed) {
            Debug.assert((this._state & AsyncResult.Done) === 0);
            this._state |= AsyncResult.Done;
            if (ok) {
                this._state |= AsyncResult.OK;
            }
            this._cancellationHandler = null;
            if (completed) {
                completed(this);
            } else {
                this.resolve();
            }
        }
    }, {
        key: "markFinishedEx",
        value: function markFinishedEx(ex) {
            Debug.assert((this._state & AsyncResult.Done) === 0);
            this._exception = ex;
            this._state |= AsyncResult.Done;
            this._cancellationHandler = null;
            this.reject(ex);
        }
    }, {
        key: "cancelWithException",
        value: function cancelWithException(ex) {
            this._cancellationException = ex;
            if (this._cancellationHandler) {
                this._cancellationHandler.asyncRequestCanceled(this, ex);
            }
        }
    }, {
        key: "cancelable",
        value: function cancelable(handler) {
            if (this._cancellationException) {
                try {
                    throw this._cancellationException;
                } finally {
                    this._cancellationException = null;
                }
            }
            this._cancellationHandler = handler;
        }
    }, {
        key: "getOs",
        value: function getOs() {
            return this._os;
        }
    }, {
        key: "startReadParams",
        value: function startReadParams() {
            this._is.startEncapsulation();
            return this._is;
        }
    }, {
        key: "endReadParams",
        value: function endReadParams() {
            this._is.endEncapsulation();
        }
    }, {
        key: "readEmptyParams",
        value: function readEmptyParams() {
            this._is.skipEmptyEncapsulation();
        }
    }, {
        key: "throwUserException",
        value: function throwUserException() {
            Debug.assert((this._state & AsyncResult.Done) !== 0);
            if ((this._state & AsyncResult.OK) === 0) {
                try {
                    this._is.startEncapsulation();
                    this._is.throwException();
                } catch (ex) {
                    if (ex instanceof UserException) {
                        this._is.endEncapsulation();
                    }
                    throw ex;
                }
            }
        }
    }]);

    return AsyncResult;
}(AsyncResultBase);

AsyncResult.OK = 0x1;
AsyncResult.Done = 0x2;
AsyncResult.Sent = 0x4;

Ice.AsyncResult = AsyncResult;
module.exports.Ice = Ice;