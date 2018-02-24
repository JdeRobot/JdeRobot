[![Build Status](https://travis-ci.org/mojo-js/obj-stream.svg)](https://travis-ci.org/mojo-js/obj-stream) [![Coverage Status](https://coveralls.io/repos/mojo-js/obj-stream/badge.svg?branch=master)](https://coveralls.io/r/mojo-js/obj-stream?branch=master) [![Dependency Status](https://david-dm.org/mojo-js/obj-stream.svg)](https://david-dm.org/mojo-js/obj-stream)

A lightweight object stream library for NodeJS and the browser.

```javascript
var stream = require("obj-stream");

var stream = new stream.Stream();
stream.write({ name: "obj" }); // valid
stream.write("blah"); // invalid
stream.end();

var readable = new stream.Readable([{ name: "obj" }]);
readable.pipe(new stream.Stream());
```

#### writable stream.writable()

creates a new writable stream

```javascript
var writable = stream.writable();
```

#### writable.write(object)

Writes a new object

```javascript
writable.write({ name: "Jeff Gordon" });
```

#### writable.end([object])

Ends the stream

```javascript
writable.write({ name: "Donkay" });
writable.write({ name: "Shrek" });
writable.end(); // done
```

#### writable.once(event, callback)

listens to one event then disposes

#### reader writable.reader

```javascript
Returns the readable object

var writer = stream.writable();
var reader = writer.reader;

reader.on("data", function() {

});

reader.on("end", function() {

});

writer.end({ name: "Oprah" }); // write & end
```

#### reader stream.readable()

creates a new readable object

#### reader.on(event, callback)

listens for an event.

- `event`
  - `error` - emitted on error
  - `data` - emitted on data
  - `end` - emitted when the stream closes
  - `drain` - emitted when the writer is resumed

#### reader.once(event, callback)

listens to one event then disposes

#### reader.removeListener(event, callback)

removes a listener

#### reader.pause()

pauses the stream.

```javascript
var writer = stream.writable();
var reader = writer.reader;

reader.pause();
writer.write({ name: "bob" }); // buffered, doesn't get emitted as data
```

#### reader.resume()

resumes the stream.

#### reader.isPaused()

returns true if the stream is paused.

#### writable.pipe(writable)

pipes data to a writable

#### stream stream.through(fn)

helper for transforming data

```javascript
var writable = stream.writable();
writable.reader.pipe(stream.though(function(data, next) {
  this.push("blah");
  next(); // done
})).on("data", function(data) {
  // blah is always emitted here
});

writable.write({ name: "org" });
writable.end();
```
