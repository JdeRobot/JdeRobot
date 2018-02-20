var RosClient = require('../src/RosLibJsClient');

var instance = new RosClient({
	url: "ws://192.168.0.12:9090",
});

var listener1 = instance.topic.subscribe("/robots_status", "std_msgs/String", function() {
	console.log("Message received");
});

instance.on("connected", function() {
	console.log("Connected!");
});