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

var Timer = function Timer() {
  _classCallCheck(this, Timer);
};

Timer.setTimeout = setTimeout;
Timer.clearTimeout = clearTimeout;
Timer.setInterval = setInterval;
Timer.clearInterval = clearInterval;
Timer.setImmediate = setImmediate;

Ice.Timer = Timer;
module.exports.Ice = Ice;