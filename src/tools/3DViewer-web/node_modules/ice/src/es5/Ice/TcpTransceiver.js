var _typeof = typeof Symbol === "function" && typeof Symbol.iterator === "symbol" ? function (obj) { return typeof obj; } : function (obj) { return obj && typeof Symbol === "function" && obj.constructor === Symbol && obj !== Symbol.prototype ? "symbol" : typeof obj; };

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

var net = require("net");

var Ice = require("../Ice/ModuleRegistry").Ice;
Ice._ModuleRegistry.require(module, ["../Ice/Debug", "../Ice/ExUtil", "../Ice/SocketOperation", "../Ice/Connection", "../Ice/Exception", "../Ice/LocalException", "../Ice/Timer"]);

var Debug = Ice.Debug;
var ExUtil = Ice.ExUtil;
var Network = Ice.Network;
var SocketOperation = Ice.SocketOperation;
var LocalException = Ice.LocalException;
var SocketException = Ice.SocketException;
var Timer = Ice.Timer;

var StateNeedConnect = 0;
var StateConnectPending = 1;
var StateProxyConnectRequest = 2;
var StateProxyConnectRequestPending = 3;
var StateConnected = 4;
var StateClosed = 5;

var TcpTransceiver = function () {
    function TcpTransceiver(instance) {
        _classCallCheck(this, TcpTransceiver);

        this._logger = instance.logger();
        this._readBuffers = [];
        this._readPosition = 0;
        this._maxSendPacketSize = instance.properties().getPropertyAsIntWithDefault("Ice.TCP.SndSize", 512 * 1024);
    }

    _createClass(TcpTransceiver, [{
        key: "setCallbacks",
        value: function setCallbacks(connectedCallback, bytesAvailableCallback, bytesWrittenCallback) {
            this._connectedCallback = connectedCallback;
            this._bytesAvailableCallback = bytesAvailableCallback;
            this._bytesWrittenCallback = bytesWrittenCallback;
        }
        //
        // Returns SocketOperation.None when initialization is complete.
        //

    }, {
        key: "initialize",
        value: function initialize(readBuffer, writeBuffer) {
            var _this = this;

            try {
                if (this._exception) {
                    throw this._exception;
                }

                if (this._state === StateNeedConnect) {
                    this._state = StateConnectPending;
                    this._fd = net.createConnection({ port: this._addr.port,
                        host: this._addr.host,
                        localAddress: this._sourceAddr });

                    this._fd.on("connect", function () {
                        return _this.socketConnected();
                    });
                    this._fd.on("data", function (buf) {
                        return _this.socketBytesAvailable(buf);
                    });

                    //
                    // The error callback can be triggered from the socket
                    // write(). We don't want it to dispached right away
                    // from within the write() so we delay the call with
                    // setImmediate. We do the same for close as a
                    // precaution. See also issue #6226.
                    //
                    this._fd.on("close", function (err) {
                        return Timer.setImmediate(function () {
                            return _this.socketClosed(err);
                        });
                    });
                    this._fd.on("error", function (err) {
                        return Timer.setImmediate(function () {
                            return _this.socketError(err);
                        });
                    });

                    return SocketOperation.Connect; // Waiting for connect to complete.
                } else if (this._state === StateConnectPending) {
                    //
                    // Socket is connected.
                    //
                    this._desc = fdToString(this._fd, this._proxy, this._addr);
                    this._state = StateConnected;
                } else if (this._state === StateProxyConnectRequest) {
                    //
                    // Write completed.
                    //
                    this._proxy.endWriteConnectRequest(writeBuffer);
                    this._state = StateProxyConnectRequestPending; // Wait for proxy response
                    return SocketOperation.Read;
                } else if (this._state === StateProxyConnectRequestPending) {
                    //
                    // Read completed.
                    //
                    this._proxy.endReadConnectRequestResponse(readBuffer);
                    this._state = StateConnected;
                }
            } catch (err) {
                if (!this._exception) {
                    this._exception = translateError(this._state, err);
                }
                throw this._exception;
            }

            Debug.assert(this._state === StateConnected);
            return SocketOperation.None;
        }
    }, {
        key: "register",
        value: function register() {
            this._registered = true;
            this._fd.resume();
            if (this._exception) {
                this._bytesAvailableCallback();
            }
        }
    }, {
        key: "unregister",
        value: function unregister() {
            if (this._fd === null) {
                Debug.assert(this._exception); // Socket creation failed.
                return;
            }
            this._registered = false;
            this._fd.pause();
        }
    }, {
        key: "close",
        value: function close() {
            if (this._fd === null) {
                Debug.assert(this._exception); // Socket creation failed.
                return;
            }

            try {
                this._fd.destroy();
            } catch (ex) {
                throw translateError(this._state, ex);
            } finally {
                this._fd = null;
            }
        }
        //
        // Returns true if all of the data was flushed to the kernel buffer.
        //

    }, {
        key: "write",
        value: function write(byteBuffer) {
            var _this2 = this;

            if (this._exception) {
                throw this._exception;
            }

            var packetSize = byteBuffer.remaining;
            Debug.assert(packetSize > 0);

            if (this._maxSendPacketSize > 0 && packetSize > this._maxSendPacketSize) {
                packetSize = this._maxSendPacketSize;
            }

            var _loop = function _loop() {
                var slice = byteBuffer.b.slice(byteBuffer.position, byteBuffer.position + packetSize);

                var sync = true;
                /*jshint -W083 */
                sync = _this2._fd.write(new Buffer(slice), null, function () {
                    if (!sync) {
                        _this2._bytesWrittenCallback();
                    }
                });
                /*jshint +W083 */

                byteBuffer.position = byteBuffer.position + packetSize;
                if (!sync) {
                    return {
                        v: false
                    }; // Wait for callback to be called before sending more data.
                }

                if (_this2._maxSendPacketSize > 0 && byteBuffer.remaining > _this2._maxSendPacketSize) {
                    packetSize = _this2._maxSendPacketSize;
                } else {
                    packetSize = byteBuffer.remaining;
                }
            };

            while (packetSize > 0) {
                var _ret = _loop();

                if ((typeof _ret === "undefined" ? "undefined" : _typeof(_ret)) === "object") return _ret.v;
            }
            return true;
        }
    }, {
        key: "read",
        value: function read(byteBuffer, moreData) {
            if (this._exception) {
                throw this._exception;
            }

            moreData.value = false;

            if (this._readBuffers.length === 0) {
                return false; // No data available.
            }

            var avail = this._readBuffers[0].length - this._readPosition;
            Debug.assert(avail > 0);

            while (byteBuffer.remaining > 0) {
                if (avail > byteBuffer.remaining) {
                    avail = byteBuffer.remaining;
                }

                this._readBuffers[0].copy(new Buffer(byteBuffer.b), byteBuffer.position, this._readPosition, this._readPosition + avail);

                byteBuffer.position += avail;
                this._readPosition += avail;
                if (this._readPosition === this._readBuffers[0].length) {
                    //
                    // We've exhausted the current read buffer.
                    //
                    this._readPosition = 0;
                    this._readBuffers.shift();
                    if (this._readBuffers.length === 0) {
                        break; // No more data - we're done.
                    } else {
                        avail = this._readBuffers[0].length;
                    }
                }
            }
            moreData.value = this._readBuffers.length > 0;

            return byteBuffer.remaining === 0;
        }
    }, {
        key: "type",
        value: function type() {
            return "tcp";
        }
    }, {
        key: "getInfo",
        value: function getInfo() {
            Debug.assert(this._fd !== null);
            var info = new Ice.TCPConnectionInfo();
            info.localAddress = this._fd.localAddress;
            info.localPort = this._fd.localPort;
            info.remoteAddress = this._fd.remoteAddress;
            info.remotePort = this._fd.remotePort;
            info.rcvSize = -1;
            info.sndSize = this._maxSendPacketSize;
            return info;
        }
    }, {
        key: "checkSendSize",
        value: function checkSendSize(stream) {}
    }, {
        key: "setBufferSize",
        value: function setBufferSize(rcvSize, sndSize) {
            this._maxSendPacketSize = sndSize;
        }
    }, {
        key: "toString",
        value: function toString() {
            return this._desc;
        }
    }, {
        key: "socketConnected",
        value: function socketConnected() {
            Debug.assert(this._connectedCallback !== null);
            this._connectedCallback();
        }
    }, {
        key: "socketBytesAvailable",
        value: function socketBytesAvailable(buf) {
            Debug.assert(this._bytesAvailableCallback !== null);

            //
            // TODO: Should we set a limit on how much data we can read?
            // We can call _fd.pause() to temporarily stop reading.
            //
            if (buf.length > 0) {
                this._readBuffers.push(buf);
                this._bytesAvailableCallback();
            }
        }
    }, {
        key: "socketClosed",
        value: function socketClosed(err) {
            //
            // Don't call the closed callback if an error occurred; the error callback
            // will be called.
            //
            if (!err) {
                this.socketError(null);
            }
        }
    }, {
        key: "socketError",
        value: function socketError(err) {
            this._exception = translateError(this._state, err);
            if (this._state < StateConnected) {
                this._connectedCallback();
            } else if (this._registered) {
                this._bytesAvailableCallback();
            }
        }
    }], [{
        key: "createOutgoing",
        value: function createOutgoing(instance, addr, sourceAddr) {
            var transceiver = new TcpTransceiver(instance);

            transceiver._fd = null;
            transceiver._addr = addr;
            transceiver._sourceAddr = sourceAddr;
            transceiver._desc = "local address = <not connected>\nremote address = " + addr.host + ":" + addr.port;
            transceiver._state = StateNeedConnect;
            transceiver._registered = false;
            transceiver._exception = null;

            return transceiver;
        }
    }, {
        key: "createIncoming",
        value: function createIncoming(instance, fd) {
            var transceiver = new TcpTransceiver(instance);

            transceiver._fd = fd;
            transceiver._addr = null;
            transceiver._sourceAddr = null;
            transceiver._desc = fdToString(fd);
            transceiver._state = StateConnected;
            transceiver._registered = false;
            transceiver._exception = null;

            return transceiver;
        }
    }]);

    return TcpTransceiver;
}();

function fdToString(fd, targetAddr) {
    if (fd === null) {
        return "<closed>";
    }

    return addressesToString(fd.localAddress, fd.localPort, fd.remoteAddress, fd.remotePort, targetAddr);
}

function translateError(state, err) {
    if (!err) {
        return new Ice.ConnectionLostException();
    } else if (state < StateConnected) {
        if (connectionRefused(err.code)) {
            return new Ice.ConnectionRefusedException(err.code, err);
        } else if (connectionFailed(err.code)) {
            return new Ice.ConnectFailedException(err.code, err);
        }
    } else if (connectionLost(err.code)) {
        return new Ice.ConnectionLostException(err.code, err);
    }
    return new Ice.SocketException(err.code, err);
}

function addressesToString(localHost, localPort, remoteHost, remotePort, targetAddr) {
    remoteHost = remoteHost === undefined ? null : remoteHost;
    targetAddr = targetAddr === undefined ? null : targetAddr;

    var s = [];
    s.push("local address = ");
    s.push(localHost + ":" + localPort);

    if (remoteHost === null && targetAddr !== null) {
        remoteHost = targetAddr.host;
        remotePort = targetAddr.port;
    }

    if (remoteHost === null) {
        s.push("\nremote address = <not connected>");
    } else {
        s.push("\nremote address = ");
        s.push(remoteHost + ":" + remotePort);
    }

    return s.join("");
}

var ECONNABORTED = "ECONNABORTED";
var ECONNREFUSED = "ECONNREFUSED";
var ECONNRESET = "ECONNRESET";
var EHOSTUNREACH = "EHOSTUNREACH";
var ENETUNREACH = "ENETUNREACH";
var ENOTCONN = "ENOTCONN";
var EPIPE = "EPIPE";
var ESHUTDOWN = "ESHUTDOWN";
var ETIMEDOUT = "ETIMEDOUT";

function connectionRefused(err) {
    return err == ECONNREFUSED;
}

function connectionFailed(err) {
    return err == ECONNREFUSED || err == ETIMEDOUT || err == ENETUNREACH || err == EHOSTUNREACH || err == ECONNRESET || err == ESHUTDOWN || err == ECONNABORTED;
}

function connectionLost(err) {
    return err == ECONNRESET || err == ENOTCONN || err == ESHUTDOWN || err == ECONNABORTED || err == EPIPE;
}

Ice.TcpTransceiver = TcpTransceiver;
module.exports.Ice = Ice;