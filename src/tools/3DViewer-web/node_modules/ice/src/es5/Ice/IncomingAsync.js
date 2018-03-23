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
Ice._ModuleRegistry.require(module, ["../Ice/Stream", "../Ice/BuiltinSequences", "../Ice/Connection", "../Ice/Current", "../Ice/Debug", "../Ice/Exception", "../Ice/Identity", "../Ice/LocalException", "../Ice/Protocol", "../Ice/StringUtil"]);

var OutputStream = Ice.OutputStream;
var Current = Ice.Current;
var Debug = Ice.Debug;
var FormatType = Ice.FormatType;
var Context = Ice.Context;
var Identity = Ice.Identity;
var Protocol = Ice.Protocol;
var StringUtil = Ice.StringUtil;

var IncomingAsync = function () {
    function IncomingAsync(instance, connection, adapter, response, requestId) {
        _classCallCheck(this, IncomingAsync);

        this._instance = instance;
        this._response = response;
        this._connection = connection;
        this._format = Ice.FormatType.DefaultFormat;

        this._current = new Current();
        this._current.id = new Identity();
        this._current.adapter = adapter;
        this._current.con = this._connection;
        this._current.requestId = requestId;

        this._servant = null;
        this._locator = null;
        this._cookie = { value: null };

        this._os = null;
        this._is = null;
    }

    _createClass(IncomingAsync, [{
        key: "startWriteParams",
        value: function startWriteParams() {
            if (!this._response) {
                throw new Ice.MarshalException("can't marshal out parameters for oneway dispatch");
            }

            Debug.assert(this._current.encoding !== null); // Encoding for reply is known.
            this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
            this._os.writeBlob(Protocol.replyHdr);
            this._os.writeInt(this._current.requestId);
            this._os.writeByte(0);
            this._os.startEncapsulation(this._current.encoding, this._format);
            return this._os;
        }
    }, {
        key: "endWriteParams",
        value: function endWriteParams() {
            if (this._response) {
                this._os.endEncapsulation();
            }
        }
    }, {
        key: "writeEmptyParams",
        value: function writeEmptyParams() {
            if (this._response) {
                Debug.assert(this._current.encoding !== null); // Encoding for reply is known.
                this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                this._os.writeBlob(Protocol.replyHdr);
                this._os.writeInt(this._current.requestId);
                this._os.writeByte(Protocol.replyOK);
                this._os.writeEmptyEncapsulation(this._current.encoding);
            }
        }
    }, {
        key: "writeParamEncaps",
        value: function writeParamEncaps(v, ok) {
            if (this._response) {
                Debug.assert(this._current.encoding !== null); // Encoding for reply is known.
                this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                this._os.writeBlob(Protocol.replyHdr);
                this._os.writeInt(this._current.requestId);
                this._os.writeByte(ok ? Protocol.replyOK : Protocol.replyUserException);
                if (v === null || v.length === 0) {
                    this._os.writeEmptyEncapsulation(this._current.encoding);
                } else {
                    this._os.writeEncapsulation(v);
                }
            }
        }
    }, {
        key: "setFormat",
        value: function setFormat(format) {
            this._format = format;
        }
    }, {
        key: "warning",
        value: function warning(ex) {
            Debug.assert(this._instance !== null);

            var s = [];
            s.push("dispatch exception:");
            s.push("\nidentity: " + Ice.identityToString(this._current.id, this._instance.toStringMode()));
            s.push("\nfacet: " + StringUtil.escapeString(this._current.facet, "", this._instance.toStringMode()));
            s.push("\noperation: " + this._current.operation);
            if (this._connection !== null) {
                try {
                    for (var p = this._connection.getInfo(); p; p = p.underlying) {
                        if (p instanceof Ice.IPConnectionInfo) {
                            s.push("\nremote host: " + p.remoteAddress + " remote port: " + p.remotePort);
                        }
                    }
                } catch (exc) {
                    // Ignore.
                }
            }
            if (ex.stack) {
                s.push("\n");
                s.push(ex.stack);
            }
            this._instance.initializationData().logger.warning(s.join(""));
        }
    }, {
        key: "handleException",
        value: function handleException(ex, amd) {
            Debug.assert(this._connection !== null);

            var props = this._instance.initializationData().properties;
            if (ex instanceof Ice.RequestFailedException) {
                if (ex.id === null) {
                    ex.id = this._current.id;
                }

                if (ex.facet === null) {
                    ex.facet = this._current.facet;
                }

                if (ex.operation === null || ex.operation.length === 0) {
                    ex.operation = this._current.operation;
                }

                if (props.getPropertyAsIntWithDefault("Ice.Warn.Dispatch", 1) > 1) {
                    this.warning(ex);
                }

                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    if (ex instanceof Ice.ObjectNotExistException) {
                        this._os.writeByte(Protocol.replyObjectNotExist);
                    } else if (ex instanceof Ice.FacetNotExistException) {
                        this._os.writeByte(Protocol.replyFacetNotExist);
                    } else if (ex instanceof Ice.OperationNotExistException) {
                        this._os.writeByte(Protocol.replyOperationNotExist);
                    } else {
                        Debug.assert(false);
                    }
                    ex.id._write(this._os);

                    //
                    // For compatibility with the old FacetPath.
                    //
                    if (ex.facet === null || ex.facet.length === 0) {
                        Ice.StringSeqHelper.write(this._os, null);
                    } else {
                        Ice.StringSeqHelper.write(this._os, [ex.facet]);
                    }

                    this._os.writeString(ex.operation);

                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } else if (ex instanceof Ice.UnknownLocalException) {
                if (props.getPropertyAsIntWithDefault("Ice.Warn.Dispatch", 1) > 0) {
                    this.warning(ex);
                }

                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    this._os.writeByte(Protocol.replyUnknownLocalException);
                    this._os.writeString(ex.unknown);
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } else if (ex instanceof Ice.UnknownUserException) {
                if (props.getPropertyAsIntWithDefault("Ice.Warn.Dispatch", 1) > 0) {
                    this.warning(ex);
                }

                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    this._os.writeByte(Protocol.replyUnknownUserException);
                    this._os.writeString(ex.unknown);
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } else if (ex instanceof Ice.UnknownException) {
                if (props.getPropertyAsIntWithDefault("Ice.Warn.Dispatch", 1) > 0) {
                    this.warning(ex);
                }

                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    this._os.writeByte(Protocol.replyUnknownException);
                    this._os.writeString(ex.unknown);
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } else if (ex instanceof Ice.LocalException) {
                if (props.getPropertyAsIntWithDefault("Ice.Warn.Dispatch", 1) > 0) {
                    this.warning(ex);
                }

                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    this._os.writeByte(Protocol.replyUnknownLocalException);
                    //this._os.writeString(ex.toString());
                    var s = [ex.ice_id()];
                    if (ex.stack) {
                        s.push("\n");
                        s.push(ex.stack);
                    }
                    this._os.writeString(s.join(""));
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } else if (ex instanceof Ice.UserException) {
                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    this._os.writeByte(Protocol.replyUserException);
                    this._os.startEncapsulation(this._current.encoding, this._format);
                    this._os.writeUserException(ex);
                    this._os.endEncapsulation();
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } else {
                if (props.getPropertyAsIntWithDefault("Ice.Warn.Dispatch", 1) > 0) {
                    this.warning(ex);
                }

                if (this._response) {
                    this._os = new OutputStream(this._instance, Protocol.currentProtocolEncoding);
                    this._os.writeBlob(Protocol.replyHdr);
                    this._os.writeInt(this._current.requestId);
                    this._os.writeByte(Protocol.replyUnknownException);
                    //this._os.writeString(ex.toString());
                    this._os.writeString(ex.toString() + (ex.stack ? "\n" + ex.stack : ""));
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }

                if (!amd) {
                    throw new Ice.ServantError(ex);
                }
            }

            this._connection = null;
        }
    }, {
        key: "invoke",
        value: function invoke(servantManager, stream) {
            var _this = this;

            this._is = stream;

            //
            // Read the current.
            //
            this._current.id._read(this._is);

            //
            // For compatibility with the old FacetPath.
            //
            var facetPath = Ice.StringSeqHelper.read(this._is);
            if (facetPath.length > 0) {
                if (facetPath.length > 1) {
                    throw new Ice.MarshalException();
                }
                this._current.facet = facetPath[0];
            } else {
                this._current.facet = "";
            }

            this._current.operation = this._is.readString();
            this._current.mode = Ice.OperationMode.valueOf(this._is.readByte());
            this._current.ctx = new Context();
            var sz = this._is.readSize();
            while (sz-- > 0) {
                this._current.ctx.set(this._is.readString(), this._is.readString());
            }

            //
            // Don't put the code above into the try block below. Exceptions
            // in the code above are considered fatal, and must propagate to
            // the caller of this operation.
            //
            if (servantManager !== null) {
                this._servant = servantManager.findServant(this._current.id, this._current.facet);
                if (this._servant === null) {
                    this._locator = servantManager.findServantLocator(this._current.id.category);
                    if (this._locator === null && this._current.id.category.length > 0) {
                        this._locator = servantManager.findServantLocator("");
                    }

                    if (this._locator !== null) {
                        try {
                            this._servant = this._locator.locate(this._current, this._cookie);
                        } catch (ex) {
                            this.skipReadParams(); // Required for batch requests.
                            this.handleException(ex, false);
                            return;
                        }
                    }
                }
            }

            if (this._servant === null) {
                try {
                    if (servantManager !== null && servantManager.hasServant(this._current.id)) {
                        throw new Ice.FacetNotExistException(this._current.id, this._current.facet, this._current.operation);
                    } else {
                        throw new Ice.ObjectNotExistException(this._current.id, this._current.facet, this._current.operation);
                    }
                } catch (ex) {
                    this.skipReadParams(); // Required for batch requests.
                    this.handleException(ex, false);
                    return;
                }
            }

            try {
                Debug.assert(this._servant !== null);
                var promise = this._servant._iceDispatch(this, this._current);
                if (promise !== null) {
                    promise.then(function () {
                        return _this.completed(null, true);
                    }, function (ex) {
                        return _this.completed(ex, true);
                    });
                    return;
                }

                Debug.assert(!this._response || this._os !== null);
                this.completed(null, false);
            } catch (ex) {
                this.completed(ex, false);
            }
        }
    }, {
        key: "startReadParams",
        value: function startReadParams() {
            //
            // Remember the encoding used by the input parameters, we'll
            // encode the response parameters with the same encoding.
            //
            this._current.encoding = this._is.startEncapsulation();
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
            this._current.encoding = this._is.skipEmptyEncapsulation();
        }
    }, {
        key: "readParamEncaps",
        value: function readParamEncaps() {
            this._current.encoding = new Ice.EncodingVersion();
            return this._is.readEncapsulation(this._current.encoding);
        }
    }, {
        key: "skipReadParams",
        value: function skipReadParams() {
            this._current.encoding = this._is.skipEncapsulation();
        }
    }, {
        key: "completed",
        value: function completed(exc, amd) {
            try {
                if (this._locator !== null && !this.servantLocatorFinished(amd)) {
                    Debug.assert(this._locator !== null && this._servant !== null);
                    try {
                        this._locator.finished(this._current, this._servant, this._cookie.value);
                    } catch (ex) {
                        this.handleException(ex, amd);
                        return;
                    }
                }

                Debug.assert(this._connection !== null);

                if (exc !== null) {
                    this.handleException(exc, amd);
                } else if (this._response) {
                    this._connection.sendResponse(this._os);
                } else {
                    this._connection.sendNoResponse();
                }
            } catch (ex) {
                if (ex instanceof Ice.LocalException) {
                    this._connection.invokeException(ex, 1);
                } else {
                    throw ex;
                }
            }
            this._connection = null;
        }
    }]);

    return IncomingAsync;
}();

Ice.IncomingAsync = IncomingAsync;
module.exports.Ice = Ice;