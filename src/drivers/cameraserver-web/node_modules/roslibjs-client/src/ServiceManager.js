var ROSLIB = require('roslib');
var Promise = require('bluebird');

module.exports = function(connection) {
	//
	this.call = function(name, serviceType, payload) {
		return connection.getInstance().then(function(ros) {
			return new Promise(function(resolve) {
				var service = new ROSLIB.Service({
					ros: ros,
					name: name,
					serviceType: serviceType
				});
				var request = new ROSLIB.ServiceRequest(payload);
				service.callService(request, function(response) {
					resolve(response);
				});
			});
		});
	};
};