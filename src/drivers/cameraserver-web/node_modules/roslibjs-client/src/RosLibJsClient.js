var ROSLIB = require("roslib");
var Promise = require("bluebird");
var ee2Lib = require('eventemitter2'), EventEmitter2 = ee2Lib.EventEmitter2 || ee2Lib;

// Internal libraries
var Connection = require('./Connection');
var TopicManager = require('./TopicManager');
var ServiceManager = require('./ServiceManager');

var defaultOptions = {
	url: "ws://localhost:9090",
	// Milliseconds before attempting another reconnect
	reconnectInterval: 5000
};

function getUserOptions(options) {
	// WARNING: This might not work on all browsers
	return Object.assign({}, defaultOptions, options);
}

function Client(options) {
	var _this = this;
	var options = getUserOptions(options);
	var connection = new Connection(this, options);
	// Public API
	this.service = new ServiceManager(connection);
	this.topic = new TopicManager(this, connection);
};

Client.prototype.__proto__ = EventEmitter2.prototype;
module.exports = Client;