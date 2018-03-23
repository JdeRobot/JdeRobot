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
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/Protocol", "../Ice/StringUtil", "../Ice/Current", "../Ice/Identity"]);

//
// Local aliases.
//
var Debug = Ice.Debug;
var Protocol = Ice.Protocol;
var StringUtil = Ice.StringUtil;
var OperationMode = Ice.OperationMode;
var Identity = Ice.Identity;
var slicingIds = new Map();

function printIdentityFacetOperation(s, stream) {
    var toStringMode = Ice.ToStringMode.Unicode;
    if (stream.instance !== null) {
        toStringMode = stream.instance.toStringMode();
    }

    var identity = new Identity();
    identity._read(stream);
    s.push("\nidentity = " + Ice.identityToString(identity, toStringMode));

    var facet = Ice.StringSeqHelper.read(stream);
    s.push("\nfacet = ");
    if (facet.length > 0) {
        s.push(StringUtil.escapeString(facet[0], "", toStringMode));
    }

    var operation = stream.readString();
    s.push("\noperation = " + operation);
}

function printRequest(s, stream) {
    var requestId = stream.readInt();
    s.push("\nrequest id = " + requestId);
    if (requestId === 0) {
        s.push(" (oneway)");
    }

    printRequestHeader(s, stream);
}

function printBatchRequest(s, stream) {
    var batchRequestNum = stream.readInt();
    s.push("\nnumber of requests = " + batchRequestNum);

    for (var i = 0; i < batchRequestNum; ++i) {
        s.push("\nrequest #" + i + ':');
        printRequestHeader(s, stream);
    }
}

function printReply(s, stream) {
    var requestId = stream.readInt();
    s.push("\nrequest id = " + requestId);

    var replyStatus = stream.readByte();
    s.push("\nreply status = " + replyStatus + ' ');

    switch (replyStatus) {
        case Protocol.replyOK:
            {
                s.push("(ok)");
                break;
            }

        case Protocol.replyUserException:
            {
                s.push("(user exception)");
                break;
            }

        case Protocol.replyObjectNotExist:
        case Protocol.replyFacetNotExist:
        case Protocol.replyOperationNotExist:
            {
                switch (replyStatus) {
                    case Protocol.replyObjectNotExist:
                        {
                            s.push("(object not exist)");
                            break;
                        }

                    case Protocol.replyFacetNotExist:
                        {
                            s.push("(facet not exist)");
                            break;
                        }

                    case Protocol.replyOperationNotExist:
                        {
                            s.push("(operation not exist)");
                            break;
                        }

                    default:
                        {
                            Debug.assert(false);
                            break;
                        }
                }

                printIdentityFacetOperation(s, stream);
                break;
            }

        case Protocol.replyUnknownException:
        case Protocol.replyUnknownLocalException:
        case Protocol.replyUnknownUserException:
            {
                switch (replyStatus) {
                    case Protocol.replyUnknownException:
                        {
                            s.push("(unknown exception)");
                            break;
                        }

                    case Protocol.replyUnknownLocalException:
                        {
                            s.push("(unknown local exception)");
                            break;
                        }

                    case Protocol.replyUnknownUserException:
                        {
                            s.push("(unknown user exception)");
                            break;
                        }

                    default:
                        {
                            Debug.assert(false);
                            break;
                        }
                }

                var unknown = stream.readString();
                s.push("\nunknown = " + unknown);
                break;
            }

        default:
            {
                s.push("(unknown)");
                break;
            }
    }

    if (replyStatus === Protocol.replyOK || replyStatus === Protocol.replyUserException) {
        var ver = stream.skipEncapsulation();
        if (!ver.equals(Ice.Encoding_1_0)) {
            s.push("\nencoding = ");
            s.push(Ice.encodingVersionToString(ver));
        }
    }
}

function printRequestHeader(s, stream) {
    printIdentityFacetOperation(s, stream);

    var mode = stream.readByte();
    s.push("\nmode = " + mode + ' ');
    switch (OperationMode.valueOf(mode)) {
        case OperationMode.Normal:
            {
                s.push("(normal)");
                break;
            }

        case OperationMode.Nonmutating:
            {
                s.push("(nonmutating)");
                break;
            }

        case OperationMode.Idempotent:
            {
                s.push("(idempotent)");
                break;
            }

        default:
            {
                s.push("(unknown)");
                break;
            }
    }

    var sz = stream.readSize();
    s.push("\ncontext = ");
    while (sz-- > 0) {
        var key = stream.readString();
        var value = stream.readString();
        s.push(key + '/' + value);
        if (sz > 0) {
            s.push(", ");
        }
    }

    var ver = stream.skipEncapsulation();
    if (!ver.equals(Ice.Encoding_1_0)) {
        s.push("\nencoding = ");
        s.push(Ice.encodingVersionToString(ver));
    }
}

function printHeader(s, stream) {
    stream.readByte(); // Don't bother printing the magic number
    stream.readByte();
    stream.readByte();
    stream.readByte();

    //        const pMajor = stream.readByte();
    //        const pMinor = stream.readByte();
    //        s.push("\nprotocol version = " + pMajor + "." + pMinor);
    stream.readByte(); // major
    stream.readByte(); // minor

    //        const eMajor = stream.readByte();
    //        const eMinor = stream.readByte();
    //        s.push("\nencoding version = " + eMajor + "." + eMinor);
    stream.readByte(); // major
    stream.readByte(); // minor

    var type = stream.readByte();

    s.push("\nmessage type = " + type + " (" + getMessageTypeAsString(type) + ')');
    var compress = stream.readByte();
    s.push("\ncompression status = " + compress + ' ');
    switch (compress) {
        case 0:
            {
                s.push("(not compressed; do not compress response, if any)");
                break;
            }

        case 1:
            {
                s.push("(not compressed; compress response, if any)");
                break;
            }

        case 2:
            {
                s.push("(compressed; compress response, if any)");
                break;
            }

        default:
            {
                s.push("(unknown)");
                break;
            }
    }

    var size = stream.readInt();
    s.push("\nmessage size = " + size);
    return type;
}

function printMessage(s, stream) {
    var type = printHeader(s, stream);

    switch (type) {
        case Protocol.closeConnectionMsg:
        case Protocol.validateConnectionMsg:
            {
                // We're done.
                break;
            }

        case Protocol.requestMsg:
            {
                printRequest(s, stream);
                break;
            }

        case Protocol.requestBatchMsg:
            {
                printBatchRequest(s, stream);
                break;
            }

        case Protocol.replyMsg:
            {
                printReply(s, stream);
                break;
            }

        default:
            {
                break;
            }
    }

    return type;
}

function getMessageTypeAsString(type) {
    switch (type) {
        case Protocol.requestMsg:
            return "request";
        case Protocol.requestBatchMsg:
            return "batch request";
        case Protocol.replyMsg:
            return "reply";
        case Protocol.closeConnectionMsg:
            return "close connection";
        case Protocol.validateConnectionMsg:
            return "validate connection";
        default:
            return "unknown";
    }
}

var TraceUtil = function () {
    function TraceUtil() {
        _classCallCheck(this, TraceUtil);
    }

    _createClass(TraceUtil, null, [{
        key: "traceSlicing",
        value: function traceSlicing(kind, typeId, slicingCat, logger) {
            if (!slicingIds.has(typeId)) {
                logger.trace(slicingCat, "unknown " + kind + " type `" + typeId + "'");
                slicingIds.set(typeId, 1);
            }
        }
    }, {
        key: "traceSend",
        value: function traceSend(stream, logger, traceLevels) {
            if (traceLevels.protocol >= 1) {
                var p = stream.pos;
                var is = new Ice.InputStream(stream.instance, stream.getEncoding(), stream.buffer);
                is.pos = 0;

                var s = [];
                var type = printMessage(s, is);

                logger.trace(traceLevels.protocolCat, "sending " + getMessageTypeAsString(type) + " " + s.join(""));

                stream.pos = p;
            }
        }
    }, {
        key: "traceRecv",
        value: function traceRecv(stream, logger, traceLevels) {
            if (traceLevels.protocol >= 1) {
                var p = stream.pos;
                stream.pos = 0;

                var s = [];
                var type = printMessage(s, stream);

                logger.trace(traceLevels.protocolCat, "received " + getMessageTypeAsString(type) + " " + s.join(""));

                stream.pos = p;
            }
        }
    }, {
        key: "traceOut",
        value: function traceOut(heading, stream, logger, traceLevels) {
            if (traceLevels.protocol >= 1) {
                var p = stream.pos;
                var is = new Ice.InputStream(stream.instance, stream.getEncoding(), stream.buffer);
                is.pos = 0;

                var s = [];
                s.push(heading);
                printMessage(s, is);

                logger.trace(traceLevels.protocolCat, s.join(""));
                stream.pos = p;
            }
        }
    }, {
        key: "traceIn",
        value: function traceIn(heading, stream, logger, traceLevels) {
            if (traceLevels.protocol >= 1) {
                var p = stream.pos;
                stream.pos = 0;

                var s = [];
                s.push(heading);
                printMessage(s, stream);

                logger.trace(traceLevels.protocolCat, s.join(""));
                stream.pos = p;
            }
        }
    }, {
        key: "dumpStream",
        value: function dumpStream(stream) {
            var pos = stream.pos;
            stream.pos = 0;

            var data = stream.readBlob(stream.size());
            TraceUtil.dumpOctets(data);

            stream.pos = pos;
        }
    }, {
        key: "dumpOctets",
        value: function dumpOctets(data) {
            var inc = 8;
            var buf = [];

            for (var i = 0; i < data.length; i += inc) {
                for (var j = i; j - i < inc; j++) {
                    if (j < data.length) {
                        var n = data[j];
                        if (n < 0) {
                            n += 256;
                        }
                        var s = void 0;
                        if (n < 10) {
                            s = "  " + n;
                        } else if (n < 100) {
                            s = " " + n;
                        } else {
                            s = "" + n;
                        }
                        buf.push(s + " ");
                    } else {
                        buf.push("    ");
                    }
                }

                buf.push('"');

                for (var _j = i; _j < data.length && _j - i < inc; _j++) {
                    if (data[_j] >= 32 && data[_j] < 127) {
                        buf.push(String.fromCharCode(data[_j]));
                    } else {
                        buf.push('.');
                    }
                }

                buf.push("\"\n");
            }

            console.log(buf.join(""));
        }
    }]);

    return TraceUtil;
}();

Ice.TraceUtil = TraceUtil;
module.exports.Ice = Ice;