var _slicedToArray = function () { function sliceIterator(arr, i) { var _arr = []; var _n = true; var _d = false; var _e = undefined; try { for (var _i = arr[Symbol.iterator](), _s; !(_n = (_s = _i.next()).done); _n = true) { _arr.push(_s.value); if (i && _arr.length === i) break; } } catch (err) { _d = true; _e = err; } finally { try { if (!_n && _i["return"]) _i["return"](); } finally { if (_d) throw _e; } } return _arr; } return function (arr, i) { if (Array.isArray(arr)) { return arr; } else if (Symbol.iterator in Object(arr)) { return sliceIterator(arr, i); } else { throw new TypeError("Invalid attempt to destructure non-iterable instance"); } }; }();

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

var Ice = require("../Ice/OptionalFormat").Ice;

var defineProperty = Object.defineProperty;
var OptionalFormat = Ice.OptionalFormat;

var StreamHelpers = {};

StreamHelpers.FSizeOptHelper = function () {
    this.writeOptional = function (os, tag, v) {
        if (v !== undefined && os.writeOptional(tag, OptionalFormat.FSize)) {
            var pos = os.startSize();
            this.write(os, v);
            os.endSize(pos);
        }
    };

    this.readOptional = function (is, tag) {
        var v = void 0;
        if (is.readOptional(tag, OptionalFormat.FSize)) {
            is.skip(4);
            v = this.read(is);
        }
        return v;
    };
};

StreamHelpers.VSizeOptHelper = function () {
    this.writeOptional = function (os, tag, v) {
        if (v !== undefined && os.writeOptional(tag, OptionalFormat.VSize)) {
            os.writeSize(this.minWireSize);
            this.write(os, v);
        }
    };

    this.readOptional = function (is, tag) {
        var v = void 0;
        if (is.readOptional(tag, OptionalFormat.VSize)) {
            is.skipSize();
            v = this.read(is);
        }
        return v;
    };
};

StreamHelpers.VSizeContainerOptHelper = function (elementSize) {
    this.writeOptional = function (os, tag, v) {
        if (v !== undefined && os.writeOptional(tag, OptionalFormat.VSize)) {
            var sz = this.size(v);
            os.writeSize(sz > 254 ? sz * elementSize + 5 : sz * elementSize + 1);
            this.write(os, v);
        }
    };

    this.readOptional = function (is, tag) {
        var v = void 0;
        if (is.readOptional(tag, OptionalFormat.VSize)) {
            is.skipSize();
            v = this.read(is);
        }
        return v;
    };
};

StreamHelpers.VSizeContainer1OptHelper = function () {
    this.writeOptional = function (os, tag, v) {
        if (v !== undefined && os.writeOptional(tag, OptionalFormat.VSize)) {
            this.write(os, v);
        }
    };

    this.readOptional = function (is, tag) {
        var v = void 0;
        if (is.readOptional(tag, OptionalFormat.VSize)) {
            v = this.read(is);
        }
        return v;
    };
};

//
// Sequence helper to write sequences
//

var SequenceHelper = function () {
    function SequenceHelper() {
        _classCallCheck(this, SequenceHelper);
    }

    _createClass(SequenceHelper, [{
        key: "write",
        value: function write(os, v) {
            if (v === null || v.length === 0) {
                os.writeSize(0);
            } else {
                var helper = this.elementHelper;
                os.writeSize(v.length);
                for (var i = 0; i < v.length; ++i) {
                    helper.write(os, v[i]);
                }
            }
        }
    }, {
        key: "read",
        value: function read(is) {
            var helper = this.elementHelper; // Cache the element helper.
            var sz = is.readAndCheckSeqSize(helper.minWireSize);
            var v = [];
            v.length = sz;
            for (var i = 0; i < sz; ++i) {
                v[i] = helper.read(is);
            }
            return v;
        }
    }, {
        key: "size",
        value: function size(v) {
            return v === null || v === undefined ? 0 : v.length;
        }
    }, {
        key: "minWireSize",
        get: function get() {
            return 1;
        }
    }]);

    return SequenceHelper;
}();

// Speacialization optimized for ByteSeq


var byteSeqHelper = new SequenceHelper();
byteSeqHelper.write = function (os, v) {
    return os.writeByteSeq(v);
};
byteSeqHelper.read = function (is) {
    return is.readByteSeq();
};
defineProperty(byteSeqHelper, "elementHelper", {
    get: function get() {
        return Ice.ByteHelper;
    }
});
StreamHelpers.VSizeContainer1OptHelper.call(byteSeqHelper);

// Read method for value sequences
var valueSequenceHelperRead = function valueSequenceHelperRead(is) {
    var sz = is.readAndCheckSeqSize(1);
    var v = [];
    v.length = sz;
    var elementType = this.elementType;
    var readValueAtIndex = function readValueAtIndex(idx) {
        is.readValue(function (obj) {
            return v[idx] = obj;
        }, elementType);
    };

    for (var i = 0; i < sz; ++i) {
        readValueAtIndex(i);
    }
    return v;
};

StreamHelpers.generateSeqHelper = function (elementHelper, fixed, elementType) {
    if (elementHelper === Ice.ByteHelper) {
        return byteSeqHelper;
    }

    var helper = new SequenceHelper();
    if (fixed) {
        if (elementHelper.minWireSize === 1) {
            StreamHelpers.VSizeContainer1OptHelper.call(helper);
        } else {
            StreamHelpers.VSizeContainerOptHelper.call(helper, elementHelper.minWireSize);
        }
    } else {
        StreamHelpers.FSizeOptHelper.call(helper);
    }

    defineProperty(helper, "elementHelper", {
        get: function get() {
            return elementHelper;
        }
    });

    if (elementHelper == Ice.ObjectHelper) {
        defineProperty(helper, "elementType", {
            get: function get() {
                return elementType;
            }
        });
        helper.read = valueSequenceHelperRead;
    }

    return helper;
};

//
// Dictionary helper to write dictionaries
//

var DictionaryHelper = function () {
    function DictionaryHelper() {
        _classCallCheck(this, DictionaryHelper);
    }

    _createClass(DictionaryHelper, [{
        key: "write",
        value: function write(os, v) {
            if (v === null || v.size === 0) {
                os.writeSize(0);
            } else {
                var keyHelper = this.keyHelper;
                var valueHelper = this.valueHelper;
                os.writeSize(v.size);
                var _iteratorNormalCompletion = true;
                var _didIteratorError = false;
                var _iteratorError = undefined;

                try {
                    for (var _iterator = v[Symbol.iterator](), _step; !(_iteratorNormalCompletion = (_step = _iterator.next()).done); _iteratorNormalCompletion = true) {
                        var _step$value = _slicedToArray(_step.value, 2),
                            key = _step$value[0],
                            value = _step$value[1];

                        keyHelper.write(os, key);
                        valueHelper.write(os, value);
                    }
                } catch (err) {
                    _didIteratorError = true;
                    _iteratorError = err;
                } finally {
                    try {
                        if (!_iteratorNormalCompletion && _iterator.return) {
                            _iterator.return();
                        }
                    } finally {
                        if (_didIteratorError) {
                            throw _iteratorError;
                        }
                    }
                }
            }
        }
    }, {
        key: "read",
        value: function read(is) {
            var mapType = this.mapType;
            var v = new mapType();
            var sz = is.readSize();
            var keyHelper = this.keyHelper;
            var valueHelper = this.valueHelper;
            for (var i = 0; i < sz; ++i) {
                v.set(keyHelper.read(is), valueHelper.read(is));
            }
            return v;
        }
    }, {
        key: "size",
        value: function size(v) {
            return v === null || v === undefined ? 0 : v.size;
        }
    }, {
        key: "minWireSize",
        get: function get() {
            return 1;
        }
    }]);

    return DictionaryHelper;
}();

// Read method for dictionaries of values


var valueDictionaryHelperRead = function valueDictionaryHelperRead(is) {
    var sz = is.readSize();
    var mapType = this.mapType;
    var v = new mapType();
    var valueType = this.valueType;

    var readValueForKey = function readValueForKey(key) {
        is.readValue(function (obj) {
            v.set(key, obj);
        }, valueType);
    };

    var keyHelper = this.keyHelper;
    for (var i = 0; i < sz; ++i) {
        readValueForKey(keyHelper.read(is));
    }
    return v;
};

StreamHelpers.generateDictHelper = function (keyHelper, valueHelper, fixed, valueType, mapType) {
    var helper = new DictionaryHelper();
    if (fixed) {
        StreamHelpers.VSizeContainerOptHelper.call(helper, keyHelper.minWireSize + valueHelper.minWireSize);
    } else {
        StreamHelpers.FSizeOptHelper.call(helper);
    }

    defineProperty(helper, "mapType", {
        get: function get() {
            return mapType;
        }
    });

    defineProperty(helper, "keyHelper", {
        get: function get() {
            return keyHelper;
        }
    });

    defineProperty(helper, "valueHelper", {
        get: function get() {
            return valueHelper;
        }
    });

    if (valueHelper == Ice.ObjectHelper) {
        defineProperty(helper, "valueType", {
            get: function get() {
                return valueType;
            }
        });

        helper.read = valueDictionaryHelperRead;
    }

    return helper;
};

Ice.StreamHelpers = StreamHelpers;
module.exports.Ice = Ice;