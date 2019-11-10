var println = function(x) {
  print(x);
  print("\n");
};

var NewArray = function() {
  var arr = {};

  var checkIndex = function(index, callback) {
    if (notEqual(type(index), "integer")) return;
    if (lessOrEqual(length(arr), index)) return;

    return callback();
  };

  return {
    Get: function(index) {
      return checkIndex(index, function() {
        return get(arr, append("", index));
      });
    },

    Push: function(value) {
      var index = length(arr);
      set(arr, append("", index), value);
    },

    Set: function(index, value) {
      return checkIndex(index, function() {
        return set(arr, append("", index), value);
      });
    },

    ToString: function() {
      var i = 0;
      var ret = "[";

      while (less(i, length(arr))) {
        var item = get(arr, append("", i));

        ret = append(ret, item);

        if (less(i, subtract(length(arr), 1))) {
          ret = append(ret, ", ");
        }

        i = add(i, 1);
      }

      ret = append(ret, "]");

      return ret;
    },

    Length: function() {
      return length(arr);
    }
  };
};

var invoke = function(object, method, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) {
  var m = get(object, method);
  return m(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10);
};

var arr = NewArray();

var i = 0;

while (less(i, 10)) {
  var fn = read();

  if (equal(fn, "Get")) {
    println(invoke(arr, fn, int(read())));

  } else if (equal(fn, "Set")) {
    invoke(arr, fn, int(read()), read());

  } else if (equal(fn, "Length")) {
    invoke(arr, fn);

  } else if (equal(fn, "ToString")) {
    println(invoke(arr, fn));

  } else if (equal(fn, "Push")) {
    invoke(arr, fn, read());

  } else {
    println("Invalid fn!");
  }

  i = add(i, 1);
}

println(invoke(arr, "ToString"));