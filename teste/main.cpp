#include <stdio.h>
#include "printH5.hpp"
#include <iostream>

using namespace std;

int main() {
    printGidH5("DamageTest.h5", Vector vector, GlobalModel.eqs.getTotalNumberDOFs() / GlobalModal.getDimensions(), SolvParam.nts());
}