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
Ice._ModuleRegistry.require(module, ["../Ice/Stream", "../Ice/Debug", "../Ice/ExUtil", "../Ice/Protocol"]);

var OutputStream = Ice.OutputStream;
var Debug = Ice.Debug;
var ExUtil = Ice.ExUtil;
var Protocol = Ice.Protocol;

var udpOverhead = 20 + 8;

var BatchRequestQueue = function () {
    function BatchRequestQueue(instance, datagram) {
        _classCallCheck(this, BatchRequestQueue);

        this._batchStreamInUse = false;
        this._batchRequestNum = 0;
        this._batchStream = new OutputStream(instance, Protocol.currentProtocolEncoding);
        this._batchStream.writeBlob(Protocol.requestBatchHdr);
        this._batchMarker = this._batchStream.size;
        this._exception = null;

        this._maxSize = instance.batchAutoFlushSize();
        if (this._maxSize > 0 && datagram) {
            var udpSndSize = instance.initializationData().properties.getPropertyAsIntWithDefault("Ice.UDP.SndSize", 65535 - udpOverhead);
            if (udpSndSize < this._maxSize) {
                this._maxSize = udpSndSize;
            }
        }
    }

    _createClass(BatchRequestQueue, [{
        key: "prepareBatchRequest",
        value: function prepareBatchRequest(os) {
            if (this._exception) {
                throw this._exception;
            }
            this._batchStream.swap(os);
        }
    }, {
        key: "finishBatchRequest",
        value: function finishBatchRequest(os, proxy, operation) {
            //
            // No need for synchronization, no other threads are supposed
            // to modify the queue since we set this._batchStreamInUse to true.
            //
            this._batchStream.swap(os);

            try {
                if (this._maxSize > 0 && this._batchStream.size >= this._maxSize) {
                    proxy.ice_flushBatchRequests(); // Auto flush
                }

                Debug.assert(this._batchMarker < this._batchStream.size);
                this._batchMarker = this._batchStream.size;
                ++this._batchRequestNum;
            } finally {
                this._batchStream.resize(this._batchMarker);
            }
        }
    }, {
        key: "abortBatchRequest",
        value: function abortBatchRequest(os) {
            this._batchStream.swap(os);
            this._batchStream.resize(this._batchMarker);
        }
    }, {
        key: "swap",
        value: function swap(os) {
            if (this._batchRequestNum === 0) {
                return 0;
            }

            var lastRequest = null;
            if (this._batchMarker < this._batchStream.size) {
                var length = this._batchStream.size - this._batchMarker;
                this._batchStream.pos = this._batchMarker;
                lastRequest = this._batchStream.buffer.getArray(length);
                this._batchStream.resize(this._batchMarker);
            }

            var requestNum = this._batchRequestNum;
            this._batchStream.swap(os);

            //
            // Reset the batch.
            //
            this._batchRequestNum = 0;
            this._batchStream.writeBlob(Protocol.requestBatchHdr);
            this._batchMarker = this._batchStream.size;
            if (lastRequest !== null) {
                this._batchStream.writeBlob(lastRequest);
            }
            return requestNum;
        }
    }, {
        key: "destroy",
        value: function destroy(ex) {
            this._exception = ex;
        }
    }, {
        key: "isEmpty",
        value: function isEmpty() {
            return this._batchStream.size === Protocol.requestBatchHdr.length;
        }
    }]);

    return BatchRequestQueue;
}();

Ice.BatchRequestQueue = BatchRequestQueue;
module.exports.Ice = Ice;