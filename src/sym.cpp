#include <math.h>
#include "sym.h"

// To access something in sym.h, start with sym:: for the namespace, then the class name, then the method / value.

// Constructors
sym::sym::sym()
  : rawEquation_(nullptr) {}

sym::sym::sym(std::string equation)
  : rawEquation_(equation) {}

sym::sym::~sym() {}

std::string sym::sym::getRawEquation() {
  return rawEquation_;
}
