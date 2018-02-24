var ROSLIB = require("roslib");
var Promise = require("bluebird");
var constants = require("./Constants");

module.exports = function(client, options) {
	
	var rosInstance;
	var connected = false;
	var connectScheduled = false;

	var onFail = function() {
		if(connected) {
			// Going from connected to disconnected, publish disconnected event
			client.emit(constants.EVENT_DISCONNECTED);
		}
		connected = false;

		if (!connectScheduled) {	
			connectScheduled = true;
			setTimeout(connect, options.reconnectInterval);
		}
	};

	var onSuccess = function() {
		if(connected) {
			// Already in connected state...
			return;
		}
		connected = true;
		client.emit(constants.EVENT_CONNECTED, rosInstance);
	};
	
	var connect = function() {
		connectScheduled = false;

		rosInstance = new ROSLIB.Ros({
			url: options.url
		});
		rosInstance.on("close", onFail);
		rosInstance.on("error", onFail);
		rosInstance.on("connection", onSuccess);
	};

	this.getInstance = function() {
		if(connected) {
			return Promise.resolve(rosInstance);
		}
		return new Promise(function(resolve) {
			client.once(constants.EVENT_CONNECTED, resolve);
		});
	};
	// Open the connection
	connect();
};