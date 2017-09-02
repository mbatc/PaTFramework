#pragma once

#include "ErrorDefinitions.h"
#include "DataTypeDefinitions.h"

namespace SCRIPT
{
    namespace OPERATOR
    {
        unsigned int add(void* a, unsigned int a_type,
                         const void* val_1, const unsigned int val_1_type,
                         const void* val_2, const unsigned int val_2_type);
        unsigned int subtract(void* a, unsigned int a_type,
                              const void* val_1, const unsigned int val_1_type,
                              const void* val_2, const unsigned int val_2_type);
        unsigned int multiply(void* a, unsigned int a_type,
                              const void* val_1, const unsigned int val_1_type,
                              void* val_2, unsigned int val_2_type);
        unsigned int divide(void* a, unsigned int a_type,
                            void* val_1, unsigned int val_1_type,
                            void* val_2, unsigned int val_2_type);
        unsigned int assignVar(void* a, unsigned int a_type,
                               void* val, unsigned int val_type);
    }
    namespace LOGIC
    {
        unsigned int lessThan(bool& result, void* val_1, unsigned int val_1_type,
                              void* val_2, unsigned int val_2_type);
        unsigned int moreThan(bool& result, void* val_1, unsigned int val_1_type,
                              void* val_2, unsigned int val_2_type);
        unsigned int equals(bool& result, void* val_1, unsigned int val_1_type,
                            void* val_2, unsigned int val_2_type);
    }
}
