var _get = function get(object, property, receiver) { if (object === null) object = Function.prototype; var desc = Object.getOwnPropertyDescriptor(object, property); if (desc === undefined) { var parent = Object.getPrototypeOf(object); if (parent === null) { return undefined; } else { return get(parent, property, receiver); } } else if ("value" in desc) { return desc.value; } else { var getter = desc.get; if (getter === undefined) { return undefined; } return getter.call(receiver); } };

var _createClass = function () { function defineProperties(target, props) { for (var i = 0; i < props.length; i++) { var descriptor = props[i]; descriptor.enumerable = descriptor.enumerable || false; descriptor.configurable = true; if ("value" in descriptor) descriptor.writable = true; Object.defineProperty(target, descriptor.key, descriptor); } } return function (Constructor, protoProps, staticProps) { if (protoProps) defineProperties(Constructor.prototype, protoProps); if (staticProps) defineProperties(Constructor, staticProps); return Constructor; }; }();

function _classCallCheck(instance, Constructor) { if (!(instance instanceof Constructor)) { throw new TypeError("Cannot call a class as a function"); } }

function _possibleConstructorReturn(self, call) { if (!self) { throw new ReferenceError("this hasn't been initialised - super() hasn't been called"); } return call && (typeof call === "object" || typeof call === "function") ? call : self; }

function _inherits(subClass, superClass) { if (typeof superClass !== "function" && superClass !== null) { throw new TypeError("Super expression must either be null or a function, not " + typeof superClass); } subClass.prototype = Object.create(superClass && superClass.prototype, { constructor: { value: subClass, enumerable: false, writable: true, configurable: true } }); if (superClass) Object.setPrototypeOf ? Object.setPrototypeOf(subClass, superClass) : subClass.__proto__ = superClass; }

// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_FOR_ACTIONSCRIPT_LICENSE file included in this distribution.
//
// **********************************************************************

var Ice = require("../Ice/ModuleRegistry").Ice;
Ice._ModuleRegistry.require(module, ["../Ice/AsyncStatus", "../Ice/AsyncResult", "../Ice/Stream", "../Ice/Debug", "../Ice/RetryException", "../Ice/Current", "../Ice/Protocol", "../Ice/BuiltinSequences", "../Ice/Exception", "../Ice/LocalException", "../Ice/Identity"]);

var AsyncStatus = Ice.AsyncStatus;
var AsyncResult = Ice.AsyncResult;
var InputStream = Ice.InputStream;
var OutputStream = Ice.OutputStream;
var Debug = Ice.Debug;
var RetryException = Ice.RetryException;
var OperationMode = Ice.OperationMode;
var Protocol = Ice.Protocol;
var Identity = Ice.Identity;

var OutgoingAsyncBase = function (_AsyncResult) {
    _inherits(OutgoingAsyncBase, _AsyncResult);

    function OutgoingAsyncBase(communicator, operation, connection, proxy, adapter) {
        _classCallCheck(this, OutgoingAsyncBase);

        var _this = _possibleConstructorReturn(this, (OutgoingAsyncBase.__proto__ || Object.getPrototypeOf(OutgoingAsyncBase)).call(this, communicator, operation, connection, proxy, adapter));

        _this._os = new OutputStream(_this._instance, Protocol.currentProtocolEncoding);
        return _this;
    }

    _createClass(OutgoingAsyncBase, [{
        key: "getOs",
        value: function getOs() {
            return this._os;
        }
    }, {
        key: "sent",
        value: function sent() {
            this.markSent(true);
        }
    }, {
        key: "completedEx",
        value: function completedEx(ex) {
            this.markFinishedEx(ex);
        }
    }]);

    return OutgoingAsyncBase;
}(AsyncResult);

var ProxyOutgoingAsyncBase = function (_OutgoingAsyncBase) {
    _inherits(ProxyOutgoingAsyncBase, _OutgoingAsyncBase);

    function ProxyOutgoingAsyncBase(prx, operation) {
        _classCallCheck(this, ProxyOutgoingAsyncBase);

        var _this2 = _possibleConstructorReturn(this, (ProxyOutgoingAsyncBase.__proto__ || Object.getPrototypeOf(ProxyOutgoingAsyncBase)).call(this, prx.ice_getCommunicator(), operation, null, prx, null));

        _this2._mode = null;
        _this2._cnt = 0;
        _this2._sent = false;
        _this2._handler = null;
        return _this2;
    }

    _createClass(ProxyOutgoingAsyncBase, [{
        key: "completedEx",
        value: function completedEx(ex) {
            try {
                this._instance.retryQueue().add(this, this.handleException(ex));
            } catch (ex) {
                this.markFinishedEx(ex);
            }
        }
    }, {
        key: "retryException",
        value: function retryException(ex) {
            try {
                this._proxy._updateRequestHandler(this._handler, null); // Clear request handler and always retry.
                this._instance.retryQueue().add(this, 0);
            } catch (ex) {
                this.completedEx(ex);
            }
        }
    }, {
        key: "retry",
        value: function retry() {
            this.invokeImpl(false);
        }
    }, {
        key: "abort",
        value: function abort(ex) {
            this.markFinishedEx(ex);
        }
    }, {
        key: "invokeImpl",
        value: function invokeImpl(userThread) {
            var _this3 = this;

            try {
                if (userThread) {
                    var invocationTimeout = this._proxy._getReference().getInvocationTimeout();
                    if (invocationTimeout > 0) {
                        this._timeoutToken = this._instance.timer().schedule(function () {
                            _this3.cancelWithException(new Ice.InvocationTimeoutException());
                        }, invocationTimeout);
                    }
                }

                while (true) {
                    try {
                        this._sent = false;
                        this._handler = this._proxy._getRequestHandler();
                        if ((this._handler.sendAsyncRequest(this) & AsyncStatus.Sent) > 0) {
                            if (userThread) {
                                this._sentSynchronously = true;
                            }
                        }
                        return; // We're done!
                    } catch (ex) {
                        if (ex instanceof RetryException) {
                            // Clear request handler and always retry
                            this._proxy._updateRequestHandler(this._handler, null);
                        } else {
                            var interval = this.handleException(ex);
                            if (interval > 0) {
                                this._instance.retryQueue().add(this, interval);
                                return;
                            }
                        }
                    }
                }
            } catch (ex) {
                this.markFinishedEx(ex);
            }
        }
    }, {
        key: "markSent",
        value: function markSent(done) {
            this._sent = true;
            if (done) {
                if (this._timeoutToken) {
                    this._instance.timer().cancel(this._timeoutToken);
                }
            }
            _get(ProxyOutgoingAsyncBase.prototype.__proto__ || Object.getPrototypeOf(ProxyOutgoingAsyncBase.prototype), "markSent", this).call(this, done);
        }
    }, {
        key: "markFinishedEx",
        value: function markFinishedEx(ex) {
            if (this._timeoutToken) {
                this._instance.timer().cancel(this._timeoutToken);
            }
            _get(ProxyOutgoingAsyncBase.prototype.__proto__ || Object.getPrototypeOf(ProxyOutgoingAsyncBase.prototype), "markFinishedEx", this).call(this, ex);
        }
    }, {
        key: "handleException",
        value: function handleException(ex) {
            var interval = { value: 0 };
            this._cnt = this._proxy._handleException(ex, this._handler, this._mode, this._sent, interval, this._cnt);
            return interval.value;
        }
    }]);

    return ProxyOutgoingAsyncBase;
}(OutgoingAsyncBase);

var OutgoingAsync = function (_ProxyOutgoingAsyncBa) {
    _inherits(OutgoingAsync, _ProxyOutgoingAsyncBa);

    function OutgoingAsync(prx, operation, completed) {
        _classCallCheck(this, OutgoingAsync);

        var _this4 = _possibleConstructorReturn(this, (OutgoingAsync.__proto__ || Object.getPrototypeOf(OutgoingAsync)).call(this, prx, operation));

        _this4._encoding = Protocol.getCompatibleEncoding(_this4._proxy._getReference().getEncoding());
        _this4._completed = completed;
        return _this4;
    }

    _createClass(OutgoingAsync, [{
        key: "prepare",
        value: function prepare(op, mode, ctx) {
            Protocol.checkSupportedProtocol(Protocol.getCompatibleProtocol(this._proxy._getReference().getProtocol()));

            this._mode = mode;
            if (ctx === null) {
                ctx = OutgoingAsync._emptyContext;
            }

            if (this._proxy.ice_isBatchOneway() || this._proxy.ice_isBatchDatagram()) {
                this._proxy._getBatchRequestQueue().prepareBatchRequest(this._os);
            } else {
                this._os.writeBlob(Protocol.requestHdr);
            }

            var ref = this._proxy._getReference();

            ref.getIdentity()._write(this._os);

            //
            // For compatibility with the old FacetPath.
            //
            var facet = ref.getFacet();
            if (facet === null || facet.length === 0) {
                Ice.StringSeqHelper.write(this._os, null);
            } else {
                Ice.StringSeqHelper.write(this._os, [facet]);
            }

            this._os.writeString(this._operation);

            this._os.writeByte(mode.value);

            if (ctx !== undefined) {
                if (ctx !== null && !(ctx instanceof Map)) {
                    throw new Error("illegal context value, expecting null or Map");
                }

                //
                // Explicit context
                //
                Ice.ContextHelper.write(this._os, ctx);
            } else {
                //
                // Implicit context
                //
                var implicitContext = ref.getInstance().getImplicitContext();
                var prxContext = ref.getContext();

                if (implicitContext === null) {
                    Ice.ContextHelper.write(this._os, prxContext);
                } else {
                    implicitContext.write(prxContext, this._os);
                }
            }
        }
    }, {
        key: "sent",
        value: function sent() {
            this.markSent(!this._proxy.ice_isTwoway());
        }
    }, {
        key: "invokeRemote",
        value: function invokeRemote(connection, response) {
            return connection.sendAsyncRequest(this, response, 0);
        }
    }, {
        key: "abort",
        value: function abort(ex) {
            if (this._proxy.ice_isBatchOneway() || this._proxy.ice_isBatchDatagram()) {
                this._proxy._getBatchRequestQueue().abortBatchRequest(this._os);
            }
            _get(OutgoingAsync.prototype.__proto__ || Object.getPrototypeOf(OutgoingAsync.prototype), "abort", this).call(this, ex);
        }
    }, {
        key: "invoke",
        value: function invoke() {
            if (this._proxy.ice_isBatchOneway() || this._proxy.ice_isBatchDatagram()) {
                this._sentSynchronously = true;
                this._proxy._getBatchRequestQueue().finishBatchRequest(this._os, this._proxy, this._operation);
                this.markFinished(true);
                return;
            }

            //
            // NOTE: invokeImpl doesn't throw so this can be called from the
            // try block with the catch block calling abort() in case of an
            // exception.
            //
            this.invokeImpl(true); // userThread = true
        }
    }, {
        key: "completed",
        value: function completed(istr) {
            Debug.assert(this._proxy.ice_isTwoway()); // Can only be called for twoways.

            var replyStatus = void 0;
            try {
                if (this._is === null) // _is can already be initialized if the invocation is retried
                    {
                        this._is = new InputStream(this._instance, Protocol.currentProtocolEncoding);
                    }
                this._is.swap(istr);
                replyStatus = this._is.readByte();

                switch (replyStatus) {
                    case Protocol.replyOK:
                    case Protocol.replyUserException:
                        {
                            break;
                        }

                    case Protocol.replyObjectNotExist:
                    case Protocol.replyFacetNotExist:
                    case Protocol.replyOperationNotExist:
                        {
                            var id = new Identity();
                            id._read(this._is);

                            //
                            // For compatibility with the old FacetPath.
                            //
                            var facetPath = Ice.StringSeqHelper.read(this._is);
                            var facet = void 0;
                            if (facetPath.length > 0) {
                                if (facetPath.length > 1) {
                                    throw new Ice.MarshalException();
                                }
                                facet = facetPath[0];
                            } else {
                                facet = "";
                            }

                            var operation = this._is.readString();

                            var rfe = null;
                            switch (replyStatus) {
                                case Protocol.replyObjectNotExist:
                                    {
                                        rfe = new Ice.ObjectNotExistException();
                                        break;
                                    }

                                case Protocol.replyFacetNotExist:
                                    {
                                        rfe = new Ice.FacetNotExistException();
                                        break;
                                    }

                                case Protocol.replyOperationNotExist:
                                    {
                                        rfe = new Ice.OperationNotExistException();
                                        break;
                                    }

                                default:
                                    {
                                        Debug.assert(false);
                                        break;
                                    }
                            }

                            rfe.id = id;
                            rfe.facet = facet;
                            rfe.operation = operation;
                            throw rfe;
                        }

                    case Protocol.replyUnknownException:
                    case Protocol.replyUnknownLocalException:
                    case Protocol.replyUnknownUserException:
                        {
                            var unknown = this._is.readString();

                            var ue = null;
                            switch (replyStatus) {
                                case Protocol.replyUnknownException:
                                    {
                                        ue = new Ice.UnknownException();
                                        break;
                                    }

                                case Protocol.replyUnknownLocalException:
                                    {
                                        ue = new Ice.UnknownLocalException();
                                        break;
                                    }

                                case Protocol.replyUnknownUserException:
                                    {
                                        ue = new Ice.UnknownUserException();
                                        break;
                                    }

                                default:
                                    {
                                        Debug.assert(false);
                                        break;
                                    }
                            }

                            ue.unknown = unknown;
                            throw ue;
                        }

                    default:
                        {
                            throw new Ice.UnknownReplyStatusException();
                        }
                }

                this.markFinished(replyStatus == Protocol.replyOK, this._completed);
            } catch (ex) {
                this.completedEx(ex);
            }
        }
    }, {
        key: "startWriteParams",
        value: function startWriteParams(format) {
            this._os.startEncapsulation(this._encoding, format);
            return this._os;
        }
    }, {
        key: "endWriteParams",
        value: function endWriteParams() {
            this._os.endEncapsulation();
        }
    }, {
        key: "writeEmptyParams",
        value: function writeEmptyParams() {
            this._os.writeEmptyEncapsulation(this._encoding);
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
                    if (ex instanceof Ice.UserException) {
                        this._is.endEncapsulation();
                    }
                    throw ex;
                }
            }
        }
    }]);

    return OutgoingAsync;
}(ProxyOutgoingAsyncBase);

OutgoingAsync._emptyContext = new Map(); // Map<string, string>

var ProxyFlushBatch = function (_ProxyOutgoingAsyncBa2) {
    _inherits(ProxyFlushBatch, _ProxyOutgoingAsyncBa2);

    function ProxyFlushBatch(prx, operation) {
        _classCallCheck(this, ProxyFlushBatch);

        var _this5 = _possibleConstructorReturn(this, (ProxyFlushBatch.__proto__ || Object.getPrototypeOf(ProxyFlushBatch)).call(this, prx, operation));

        _this5._batchRequestNum = prx._getBatchRequestQueue().swap(_this5._os);
        return _this5;
    }

    _createClass(ProxyFlushBatch, [{
        key: "invokeRemote",
        value: function invokeRemote(connection, response) {
            if (this._batchRequestNum === 0) {
                this.sent();
                return AsyncStatus.Sent;
            }
            return connection.sendAsyncRequest(this, response, this._batchRequestNum);
        }
    }, {
        key: "invoke",
        value: function invoke() {
            Protocol.checkSupportedProtocol(Protocol.getCompatibleProtocol(this._proxy._getReference().getProtocol()));
            this.invokeImpl(true); // userThread = true
        }
    }]);

    return ProxyFlushBatch;
}(ProxyOutgoingAsyncBase);

var ProxyGetConnection = function (_ProxyOutgoingAsyncBa3) {
    _inherits(ProxyGetConnection, _ProxyOutgoingAsyncBa3);

    function ProxyGetConnection(prx, operation) {
        _classCallCheck(this, ProxyGetConnection);

        return _possibleConstructorReturn(this, (ProxyGetConnection.__proto__ || Object.getPrototypeOf(ProxyGetConnection)).call(this, prx, operation));
    }

    _createClass(ProxyGetConnection, [{
        key: "invokeRemote",
        value: function invokeRemote(connection, response) {
            this.markFinished(true, function (r) {
                return r.resolve(connection);
            });
            return AsyncStatus.Sent;
        }
    }, {
        key: "invoke",
        value: function invoke() {
            this.invokeImpl(true); // userThread = true
        }
    }]);

    return ProxyGetConnection;
}(ProxyOutgoingAsyncBase);

var ConnectionFlushBatch = function (_OutgoingAsyncBase2) {
    _inherits(ConnectionFlushBatch, _OutgoingAsyncBase2);

    function ConnectionFlushBatch(con, communicator, operation) {
        _classCallCheck(this, ConnectionFlushBatch);

        return _possibleConstructorReturn(this, (ConnectionFlushBatch.__proto__ || Object.getPrototypeOf(ConnectionFlushBatch)).call(this, communicator, operation, con, null, null));
    }

    _createClass(ConnectionFlushBatch, [{
        key: "invoke",
        value: function invoke() {
            try {
                var batchRequestNum = this._connection.getBatchRequestQueue().swap(this._os);
                var status = void 0;
                if (batchRequestNum === 0) {
                    this.sent();
                    status = AsyncStatus.Sent;
                } else {
                    status = this._connection.sendAsyncRequest(this, false, batchRequestNum);
                }

                if ((status & AsyncStatus.Sent) > 0) {
                    this._sentSynchronously = true;
                }
            } catch (ex) {
                this.completedEx(ex);
            }
        }
    }]);

    return ConnectionFlushBatch;
}(OutgoingAsyncBase);

var HeartbeatAsync = function (_OutgoingAsyncBase3) {
    _inherits(HeartbeatAsync, _OutgoingAsyncBase3);

    function HeartbeatAsync(con, communicator) {
        _classCallCheck(this, HeartbeatAsync);

        return _possibleConstructorReturn(this, (HeartbeatAsync.__proto__ || Object.getPrototypeOf(HeartbeatAsync)).call(this, communicator, "heartbeat", con, null, null));
    }

    _createClass(HeartbeatAsync, [{
        key: "invoke",
        value: function invoke() {
            try {
                this._os.writeBlob(Protocol.magic);
                Protocol.currentProtocol._write(this._os);
                Protocol.currentProtocolEncoding._write(this._os);
                this._os.writeByte(Protocol.validateConnectionMsg);
                this._os.writeByte(0);
                this._os.writeInt(Protocol.headerSize); // Message size.

                var status = this._connection.sendAsyncRequest(this, false, 0);

                if ((status & AsyncStatus.Sent) > 0) {
                    this._sentSynchronously = true;
                }
            } catch (ex) {
                this.completedEx(ex);
            }
        }
    }]);

    return HeartbeatAsync;
}(OutgoingAsyncBase);

Ice.OutgoingAsync = OutgoingAsync;
Ice.ProxyFlushBatch = ProxyFlushBatch;
Ice.ProxyGetConnection = ProxyGetConnection;
Ice.ConnectionFlushBatch = ConnectionFlushBatch;
Ice.HeartbeatAsync = HeartbeatAsync;

module.exports.Ice = Ice;