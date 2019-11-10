var println = function(x) {
  print(x);
  print("\n");
};

var NewArray = function() {
  var arr = {};

  var checkIndex = function(index, callback) {
    if (notEqual(type(index), "integer")) {
      println("Expected index type of integer, but was not");
      return;
    }

    if (lessOrEqual(length(arr), index)) {
      println("Index out of bounds for array");
      return;
    }

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

println(invoke(arr, "Length"));
println(invoke(arr, "Push", 1));
println(invoke(arr, "Push", "Dog"));
println(invoke(arr, "Length"));
println(invoke(arr, "Get", 1));
println(invoke(arr, "Set", 2, "Cat"));
println(invoke(arr, "Push", "Cat"));
println(invoke(arr, "ToString"));