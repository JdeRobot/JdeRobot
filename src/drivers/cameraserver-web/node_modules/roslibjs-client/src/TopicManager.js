var ROSLIB = require("roslib");
var constants = require("./Constants");

module.exports = function(client, connection) {

    var registeredTopics = {};

    // Internal signature for the topic
    var getSignature = function(name, messageType) {
        return messageType + '/' + name;
    }

    var listen = function(ros, name, messageType, signature) {
        var listener = new ROSLIB.Topic({
            ros: ros,
            name: name,
            messageType: messageType
        });
        registeredTopics[signature].listener = listener;
        registeredTopics[signature].listener.subscribe(function(message) {
            var numHandlers = registeredTopics[signature].handlers.length;
            for (var i = 0; i < numHandlers; i++) {
                // Actually invoke topic handlers
                registeredTopics[signature].handlers[i](message);
            }
        });
    };

    var connectAndListen = function(name, messageType, signature) {
        return connection.getInstance().then(function(ros) {
            listen(ros, name, messageType, signature);
        });
    };

    this.publish = function(name, messageType, payload) {
        return connection.getInstance().then(function(ros) {
            var topic = new ROSLIB.Topic({
                ros: ros,
                name: name,
                messageType: messageType
            });
            var message = new ROSLIB.Message(payload);
            topic.publish(message);
        });
    };

    this.subscribe = function(name, messageType, handler) {
        var signature = getSignature(name, messageType);
        if (signature in registeredTopics) {
            // Push to existing handlers
            registeredTopics[signature].handlers.push(handler);
        } else {
            // Create handler array and start topic subscription
            registeredTopics[signature] = {
                options: { name: name, messageType: messageType },
                listener: undefined,
                handlers: [handler]
            };
            connectAndListen(name, messageType, signature);
        }
        return {
            dispose: function() {
                var index = registeredTopics[signature].handlers.indexOf(handler);
                if (index !== -1) {
                    registeredTopics[signature].handlers.splice(index, 1);
                    // Close the topic, because no handlers are left
                    if (!registeredTopics[signature].handlers.length && registeredTopics[signature].listener) {
                        registeredTopics[signature].listener.unsubscribe();
                        registeredTopics[signature].listener = null;
                    }
                }
            }
        };
    };

    client.on(constants.EVENT_DISCONNECTED, function() {
        // Dispose all topic listeners (not handlers!)
        for (signature in registeredTopics) {
            var topic = registeredTopics[signature];
            if (topic.listener) {
                topic.listener.unsubscribe();
                topic.listener = null;
            }
        }
    });

    client.on(constants.EVENT_CONNECTED, function(ros) {
        // Reconnect disconnected handlers
        for (signature in registeredTopics) {
            var topic = registeredTopics[signature];
            if (topic.listener === null && topic.handlers.length) {
                listen(ros, topic.options.name, topic.options.messageType, signature);
                topic.listener = null;
            }
        }
    });
};
