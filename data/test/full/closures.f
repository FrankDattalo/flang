

var Constructor = function(name, age) {
    if (notEqual(type(name), "string")) {
        return undefined;
    }

    if (notEqual(type(age), "integer")) {
        return undefined;
    }

    var private = {
        name: name,
        age: age
    };

    return {
        GetName: function() {
            return get(private, "name");
        },
        GetAge: function() {
            return get(private, "age");
        },
        SetName: function(newName) {
            return set(private, "name", newName);
        },
        ToString: function() {
            return append("Person(name: ", append(get(private, "name"), append(", age: ", append(get(private, "age"), ")"))));
        }
    };
};

var myDude = Constructor("frank", 23);

var toString = get(myDude, "ToString");
var setName = get(myDude, "SetName");

print(toString());
print("\n");

# set(myDude, "private", "eric");
setName("eric");

print(toString());
print("\n");