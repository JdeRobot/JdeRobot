protoclass is a thin class library that helps create, and extend prototypes.

### Example


```javascript

var protoclass = require("protoclass");


function Animal(name) {
  this.name = name;
}

protoclass(Animal);

function Cat(name) {
  Cat.parent.apply(this, arguments);
}

Animal.extend(Cat, {
  meow: function() {
    console.log(this.name + ": meow");
  }
});


console.log(Class.prototype.constructor == Class); // true
console.log(Class.parent == Animal); // true
consoele.log(Class.name); // Cat
```

## API

### fn protoclass([superclass,] subclass[, mixins])

### fn.superclass

### fn.__super__

super prototype

```javascript
function Vehicle() {
  
}

protoclass(Vehicle, {
  drive: function () {

  }
});


function Car () {
  Vehicle.superclass.apply(this, argumments);
}

Vehicle.extend(Car, {
  drive: function () {
    Car.__super__.drive.call(this);
  }
});
```

### fn.extend(subclass[, mixins])

### fn.mixin(mixins)

copies the the mixins over to the prototype
