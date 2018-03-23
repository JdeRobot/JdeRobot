Roslibjs Client
====

This is a wrapper around the roslibjs package aiming to improve shortcomings of the original roslib package by introducing a fluent API as well as some useful error and fault handling mechanisms.

For the official roslibjs repository please head to [https://github.com/RobotWebTools/roslibjs](https://github.com/RobotWebTools/roslibjs)

## Main Features

* Fluent API
* Automatic connection recovery
* Reconnect support for topics

## Documentation

Establishing a reliable connection with ROS is one of the main objectives of this library. It is, also, quite simple and straightforward. Just instantiate the library like so:

	var RosClient = require("roslibjs-client"); // This is not necessary in the browser
	var client = new RosClient({
		url: "ws://192.168.0.11:9090"
	});

Once you instantiate the client, it will connect to the said URL and stay connected. If the connection drops, it will try to reconnect every 5 seconds. You can change this interval by passing an additional *reconnectInterval* option in milliseconds. It is set to 5000 by default.

### 1. Listening to Topics

Listening to ROS topics is much more reliable and efficient with this library because the handlers will tolerate connection drops. If your app gets disconnected from ROS, all the topics you have subscribed to are still stored by Roslibjs Client and once the connection is gained, it will instantly resubscribe to all the topics you want to listen.

You also don't have to worry about subscribing to a certain Topic multiple times. Behind the scenes, Roslibjs Client will always subscribe to a Topic once no matter how many handlers you attach to it.

To start listening to a topic simply do:

	var listener = client.topic.subscribe(topic_name, message_type, function(message) {
		// Here you can handle the message
	});
	
To stop listening to it, simply call `listener.dispose()`

And that's all there is to it, really. This handler will continue to work even after you lose and gain connectivity.

### 2. Publishing a Topic

To publish a message over a ROS Topic run simply:

	client.topic.publish(topic_name, message_type, payload);

### 3. Service Calls

To make a service call run the following:

	client.service.call(service_name, service_type, payload);
	
This returns a promise that will resolve when the service call is finished. Using that promise you can retrieve the response.

### 4. Events

RosClient extends EventEmitter2 and offers two simple events that you can listen to:

1. **connected** will be fired once when RosClient connects to the target.
2. **disconnected** will be fired once the connection is lost.

You can easily listen to these events like so:

	client.on("connected", function() {
		alert("Connection established!");
	});

### Use at your own risk

We neither encourage nor discourage the use of this package in your projects. However, this is just a simple wrapper we created for our own needs. Bug reports and suggestions pertaining to this particular client library are quite welcome, but we will not respond to feature requests for your own use case or bug reports that are related to the roslibjs itself. Bear in mind that you can always use Roslibjs without this package, since this package itself is a wrapper around it.