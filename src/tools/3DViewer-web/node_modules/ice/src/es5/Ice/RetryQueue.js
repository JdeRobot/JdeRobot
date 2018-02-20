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

var RetryTask = function () {
    function RetryTask(instance, queue, outAsync, interval) {
        _classCallCheck(this, RetryTask);

        this._instance = instance;
        this._queue = queue;
        this._outAsync = outAsync;
    }

    _createClass(RetryTask, [{
        key: "run",
        value: function run() {
            this._outAsync.retry();
            this._queue.remove(this);
        }
    }, {
        key: "destroy",
        value: function destroy() {
            try {
                this._outAsync.abort(new Ice.CommunicatorDestroyedException());
            } catch (ex) {
                // Abort shouldn't throw if there's no callback, ignore.
            }
        }
    }, {
        key: "asyncRequestCanceled",
        value: function asyncRequestCanceled(outAsync, ex) {
            if (this._queue.cancel(this)) {
                if (this._instance.traceLevels().retry >= 1) {
                    this._instance.initializationData().logger.trace(this._instance.traceLevels().retryCat, "operation retry canceled\n" + ex.toString());
                }
                this._outAsync.completedEx(ex);
            }
        }
    }]);

    return RetryTask;
}();

var RetryQueue = function () {
    function RetryQueue(instance) {
        _classCallCheck(this, RetryQueue);

        this._instance = instance;
        this._requests = [];
    }

    _createClass(RetryQueue, [{
        key: "add",
        value: function add(outAsync, interval) {
            if (this._instance === null) {
                throw new Ice.CommunicatorDestroyedException();
            }
            var task = new RetryTask(this._instance, this, outAsync);
            outAsync.cancelable(task); // This will throw if the request is canceled
            task.token = this._instance.timer().schedule(function () {
                return task.run();
            }, interval);
            this._requests.push(task);
        }
    }, {
        key: "destroy",
        value: function destroy() {
            var _this = this;

            this._requests.forEach(function (request) {
                _this._instance.timer().cancel(request.token);
                request.destroy();
            });

            this._requests = [];
            this._instance = null;
        }
    }, {
        key: "remove",
        value: function remove(task) {
            var idx = this._requests.indexOf(task);
            if (idx >= 0) {
                this._requests.splice(idx, 1);
            }
        }
    }, {
        key: "cancel",
        value: function cancel(task) {
            var idx = this._requests.indexOf(task);
            if (idx >= 0) {
                this._requests.splice(idx, 1);
                return this._instance.timer().cancel(task.token);
            }
            return false;
        }
    }]);

    return RetryQueue;
}();

Ice.RetryQueue = RetryQueue;

module.exports.Ice = Ice;