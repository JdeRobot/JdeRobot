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

var Ice = require("../Ice/Debug").Ice;
var Debug = Ice.Debug;

Ice.StringUtil = function () {
    function _class() {
        _classCallCheck(this, _class);
    }

    _createClass(_class, null, [{
        key: "findFirstOf",

        //
        // Return the index of the first character in str to
        // appear in match, starting from start. Returns -1 if none is
        // found.
        //
        value: function findFirstOf(str, match, start) {
            start = start === undefined ? 0 : start;
            for (var i = start; i < str.length; i++) {
                var ch = str.charAt(i);
                if (match.indexOf(ch) != -1) {
                    return i;
                }
            }
            return -1;
        }
        //
        // Return the index of the first character in str which does
        // not appear in match, starting from start. Returns -1 if none is
        // found.
        //

    }, {
        key: "findFirstNotOf",
        value: function findFirstNotOf(str, match, start) {
            start = start === undefined ? 0 : start;
            for (var i = start; i < str.length; i++) {
                var ch = str.charAt(i);
                if (match.indexOf(ch) == -1) {
                    return i;
                }
            }
            return -1;
        }
        //
        // Add escape sequences (such as "\n", or "\123") to s
        //

    }, {
        key: "escapeString",
        value: function escapeString(s, special, toStringMode) {
            special = special === undefined ? null : special;
            if (special !== null) {
                for (var i = 0; i < special.length; ++i) {
                    if (special.charCodeAt(i) < 32 || special.charCodeAt(i) > 126) {
                        throw new Error("special characters must be in ASCII range 32-126");
                    }
                }
            }

            var result = [];

            if (toStringMode === Ice.ToStringMode.Compat) {
                // Encode UTF-8 bytes
                var bytes = unescape(encodeURIComponent(s));
                for (var _i = 0; _i < bytes.length; ++_i) {
                    var c = bytes.charCodeAt(_i);
                    encodeChar(c, result, special, toStringMode);
                }
            } else {
                for (var _i2 = 0; _i2 < s.length; ++_i2) {
                    var _c = s.charCodeAt(_i2);
                    if (toStringMode === Ice.ToStringMode.Unicode || _c < 0xD800 || _c > 0xDFFF) {
                        encodeChar(_c, result, special, toStringMode);
                    } else {
                        Debug.assert(toStringMode === Ice.ToStringMode.ASCII && _c >= 0xD800 && _c <= 0xDFFF);
                        if (_i2 + 1 === s.length) {
                            throw new Error("High surrogate without low surrogate");
                        } else {
                            var codePoint = s.codePointAt(_i2);
                            Debug.assert(codePoint > 0xFFFF);
                            _i2++;

                            // append \Unnnnnnnn
                            result.push("\\U");
                            var hex = codePoint.toString(16);
                            for (var j = hex.length; j < 8; j++) {
                                result.push('0');
                            }
                            result.push(hex);
                        }
                    }
                }
            }
            return result.join("");
        }
        //
        // Remove escape sequences added by escapeString. Throws Error
        // for an invalid input string.
        //

    }, {
        key: "unescapeString",
        value: function unescapeString(s, start, end, special) {
            start = start === undefined ? 0 : start;
            end = end === undefined ? s.length : end;
            special = special === undefined ? null : special;

            Debug.assert(start >= 0 && start <= end && end <= s.length);

            if (special !== null) {
                for (var i = 0; i < special.length; ++i) {
                    if (special.charCodeAt(i) < 32 || special.charCodeAt(i) > 126) {
                        throw new Error("special characters must be in ASCII range 32-126");
                    }
                }
            }

            // Optimization for strings without escapes
            var p = s.indexOf('\\', start);
            if (p == -1 || p >= end) {
                p = start;
                while (p < end) {
                    checkChar(s, p++);
                }
                return s.substring(start, end);
            } else {
                var arr = [];
                while (start < end) {
                    start = decodeChar(s, start, end, special, arr);
                }
                return arr.join("");
            }
        }
        //
        // Split string helper; returns null for unmatched quotes
        //

    }, {
        key: "splitString",
        value: function splitString(str, delim) {
            var v = [];
            var s = "";
            var pos = 0;
            var quoteChar = null;
            while (pos < str.length) {
                if (quoteChar === null && (str.charAt(pos) === '"' || str.charAt(pos) === '\'')) {
                    quoteChar = str.charAt(pos++);
                    continue; // Skip the quote.
                } else if (quoteChar === null && str.charAt(pos) === '\\' && pos + 1 < str.length && (str.charAt(pos + 1) === '"' || str.charAt(pos + 1) === '\'')) {
                    ++pos; // Skip the backslash
                } else if (quoteChar !== null && str.charAt(pos) === '\\' && pos + 1 < str.length && str.charAt(pos + 1) === quoteChar) {
                    ++pos; // Skip the backslash
                } else if (quoteChar !== null && str.charAt(pos) === quoteChar) {
                    ++pos;
                    quoteChar = null;
                    continue; // Skip the quote.
                } else if (delim.indexOf(str.charAt(pos)) !== -1) {
                    if (quoteChar === null) {
                        ++pos;
                        if (s.length > 0) {
                            v.push(s);
                            s = "";
                        }
                        continue;
                    }
                }

                if (pos < str.length) {
                    s += str.charAt(pos++);
                }
            }

            if (s.length > 0) {
                v.push(s);
            }
            if (quoteChar !== null) {
                return null; // Unmatched quote.
            }

            return v;
        }
        //
        // If a single or double quotation mark is found at the start position,
        // then the position of the matching closing quote is returned. If no
        // quotation mark is found at the start position, then 0 is returned.
        // If no matching closing quote is found, then -1 is returned.
        //

    }, {
        key: "checkQuote",
        value: function checkQuote(s, start) {
            start = start === undefined ? 0 : start;

            var quoteChar = s.charAt(start);
            if (quoteChar == '"' || quoteChar == '\'') {
                start++;
                var pos = void 0;
                while (start < s.length && (pos = s.indexOf(quoteChar, start)) != -1) {
                    if (s.charAt(pos - 1) != '\\') {
                        return pos;
                    }
                    start = pos + 1;
                }
                return -1; // Unmatched quote
            }
            return 0; // Not quoted
        }
    }, {
        key: "hashCode",
        value: function hashCode(s) {
            var hash = 0;
            for (var i = 0; i < s.length; i++) {
                hash = 31 * hash + s.charCodeAt(i);
            }
            return hash;
        }
    }, {
        key: "toInt",
        value: function toInt(s) {
            var n = parseInt(s, 10);
            if (isNaN(n)) {
                throw new Error("conversion of `" + s + "' to int failed");
            }
            return n;
        }
    }]);

    return _class;
}();
module.exports.Ice = Ice;

function encodeChar(c, sb, special, toStringMode) {
    switch (c) {
        case 92:
            // '\\'
            {
                sb.push("\\\\");
                break;
            }
        case 39:
            // '\''
            {
                sb.push("\\'");
                break;
            }
        case 34:
            // '"'
            {
                sb.push("\\\"");
                break;
            }
        case 7:
            // '\a'
            {
                if (toStringMode == Ice.ToStringMode.Compat) {
                    // Octal escape for compatibility with 3.6 and earlier
                    sb.push("\\007");
                } else {
                    sb.push("\\a");
                }
                break;
            }
        case 8:
            // '\b'
            {
                sb.push("\\b");
                break;
            }
        case 12:
            // '\f'
            {
                sb.push("\\f");
                break;
            }
        case 10:
            // '\n'
            {
                sb.push("\\n");
                break;
            }
        case 13:
            // '\r'
            {
                sb.push("\\r");
                break;
            }
        case 9:
            // '\t'
            {
                sb.push("\\t");
                break;
            }
        case 11:
            // '\v'
            {
                if (toStringMode == Ice.ToStringMode.Compat) {
                    // Octal escape for compatibility with 3.6 and earlier
                    sb.push("\\013");
                } else {
                    sb.push("\\v");
                }
                break;
            }
        default:
            {
                var s = String.fromCharCode(c);

                if (special !== null && special.indexOf(s) !== -1) {
                    sb.push('\\');
                    sb.push(s);
                } else {
                    if (c < 32 || c > 126) {
                        if (toStringMode === Ice.ToStringMode.Compat) {
                            //
                            // When ToStringMode=Compat, c is a UTF-8 byte
                            //
                            Debug.assert(c < 256);
                            sb.push('\\');
                            var octal = c.toString(8);
                            //
                            // Add leading zeroes so that we avoid problems during
                            // decoding. For example, consider the encoded string
                            // \0013 (i.e., a character with value 1 followed by
                            // the character '3'). If the leading zeroes were omitted,
                            // the result would be incorrectly interpreted by the
                            // decoder as a single character with value 11.
                            //
                            for (var j = octal.length; j < 3; j++) {
                                sb.push('0');
                            }
                            sb.push(octal);
                        } else if (c < 32 || c == 127 || toStringMode === Ice.ToStringMode.ASCII) {
                            // append \\unnnn
                            sb.push("\\u");
                            var hex = c.toString(16);
                            for (var _j = hex.length; _j < 4; _j++) {
                                sb.push('0');
                            }
                            sb.push(hex);
                        } else {
                            // keep as is
                            sb.push(s);
                        }
                    } else {
                        // printable ASCII character
                        sb.push(s);
                    }
                }
                break;
            }
    }
}

function checkChar(s, pos) {
    var c = s.charCodeAt(pos);
    if (c < 32 || c === 127) {
        var msg = void 0;
        if (pos > 0) {
            msg = "character after `" + s.substring(0, pos) + "'";
        } else {
            msg = "first character";
        }
        msg += " has invalid ordinal value" + c;
        throw new Error(msg);
    }
    return s.charAt(pos);
}
//
// Decode the character or escape sequence starting at start and appends it to result;
// returns the index of the first character following the decoded character
// or escape sequence.
//
function decodeChar(s, start, end, special, result) {
    Debug.assert(start >= 0);
    Debug.assert(start < end);
    Debug.assert(end <= s.length);

    if (s.charAt(start) != '\\') {
        result.push(checkChar(s, start++));
    } else if (start + 1 === end) {
        ++start;
        result.push("\\"); // trailing backslash
    } else {
        var c = s.charAt(++start);

        switch (c) {
            case '\\':
            case '\'':
            case '"':
            case '?':
                {
                    ++start;
                    result.push(c);
                    break;
                }
            case 'a':
                {
                    ++start;
                    result.append("\x07");
                    break;
                }
            case 'b':
                {
                    ++start;
                    result.push("\b");
                    break;
                }
            case 'f':
                {
                    ++start;
                    result.push("\f");
                    break;
                }
            case 'n':
                {
                    ++start;
                    result.push("\n");
                    break;
                }
            case 'r':
                {
                    ++start;
                    result.push("\r");
                    break;
                }
            case 't':
                {
                    ++start;
                    result.push("\t");
                    break;
                }
            case 'v':
                {
                    ++start;
                    result.push("\v");
                    break;
                }
            case 'u':
            case 'U':
                {
                    var codePoint = 0;
                    var inBMP = c === 'u';
                    var size = inBMP ? 4 : 8;
                    ++start;
                    while (size > 0 && start < end) {
                        var charVal = s.charCodeAt(start++);
                        if (charVal >= 0x30 && charVal <= 0x39) {
                            charVal -= 0x30;
                        } else if (charVal >= 0x61 && charVal <= 0x66) {
                            charVal += 10 - 0x61;
                        } else if (charVal >= 0x41 && charVal <= 0x46) {
                            charVal += 10 - 0x41;
                        } else {
                            break; // while
                        }
                        codePoint = codePoint * 16 + charVal;
                        --size;
                    }
                    if (size > 0) {
                        throw new Error("Invalid universal character name: too few hex digits");
                    }
                    if (codePoint >= 0xD800 && codePoint <= 0xDFFF) {
                        throw new Error("A universal character name cannot designate a surrogate");
                    }
                    if (inBMP || codePoint <= 0xFFFF) {
                        result.push(String.fromCharCode(codePoint));
                    } else {
                        result.push(String.fromCodePoint(codePoint));
                    }
                    break;
                }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case 'x':
                {
                    // UTF-8 byte sequence encoded with octal or hex escapes

                    var arr = [];
                    var more = true;
                    while (more) {
                        var val = 0;
                        if (c === 'x') {
                            var _size = 2;
                            ++start;
                            while (_size > 0 && start < end) {
                                var _charVal = s.charCodeAt(start++);
                                if (_charVal >= 0x30 && _charVal <= 0x39) {
                                    _charVal -= 0x30;
                                } else if (_charVal >= 0x61 && _charVal <= 0x66) {
                                    _charVal += 10 - 0x61;
                                } else if (_charVal >= 0x41 && _charVal <= 0x46) {
                                    _charVal += 10 - 0x41;
                                } else {
                                    break; // while
                                }
                                val = val * 16 + _charVal;
                                --_size;
                            }
                            if (_size === 2) {
                                throw new Error("Invalid \\x escape sequence: no hex digit");
                            }
                        } else {
                            for (var j = 0; j < 3 && start < end; ++j) {
                                var _charVal2 = s.charCodeAt(start++) - '0'.charCodeAt(0);
                                if (_charVal2 < 0 || _charVal2 > 7) {
                                    --start; // move back
                                    Debug.assert(j !== 0); // must be at least one digit
                                    break; // for
                                }
                                val = val * 8 + _charVal2;
                            }
                            if (val > 255) {
                                throw new Error("octal value \\" + val.toString(8) + " (" + val + ") is out of range");
                            }
                        }

                        arr.push(String.fromCharCode(val));

                        more = false;
                        if (start + 1 < end && s.charAt(start) === '\\') {
                            c = s.charAt(start + 1);
                            var _charVal3 = s.charCodeAt(start + 1);
                            if (c === 'x' || _charVal3 >= 0x30 && _charVal3 <= 0x39) {
                                start++;
                                more = true;
                            }
                        }
                    }

                    // Decode UTF-8 arr into string
                    result.push(decodeURIComponent(escape(arr.join(""))));
                    break;
                }
            default:
                {
                    if (special === null || special.length === 0 || special.indexOf(c) === -1) {
                        result.push("\\"); // not in special, so we keep the backslash
                    }
                    result.push(checkChar(s, start++));
                    break;
                }
        }
    }

    return start;
}