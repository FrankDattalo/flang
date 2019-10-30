#include "Runtime.hpp"

Variable Variable::createUndefined() {
  Variable v;
  v.type = ObjectType::Undefined;
  v.integerValue = 0;
  return v;
}

Variable Variable::createBoolean(bool val) {
  Variable v;
  v.type = ObjectType::Boolean;
  v.boolValue = val;
  return v;
}

Variable Variable::createInteger(std::int64_t val) {
  Variable v;
  v.type = ObjectType::Integer;
  v.integerValue = val;
  return v;
}

Variable Variable::createObject(Object* val) {
  Variable v;
  v.type = ObjectType::Object;
  v.objectValue = val;
  return v;
}

Variable Variable::createFloat(double val) {
  Variable v;
  v.type = ObjectType::Float;
  v.doubleValue = val;
  return v;
}

bool Variable::equals(const Variable& other) {
  if (this->type != other.type) {
    return false;
  }

  switch (this->type) {
    case ObjectType::Undefined: return true;
    case ObjectType::Integer: return this->integerValue == other.integerValue;
    case ObjectType::Boolean: return this->boolValue == other.boolValue;
    case ObjectType::Float: return this->doubleValue == other.doubleValue;
    case ObjectType::Object: return this->objectValue == other.objectValue;
    case ObjectType::Function: return this->closureValue->function == other.closureValue->function;
    default: {
      Runtime::panic("Encountered unknown object type in Variable::equals");
      return false;
    }
  }
}