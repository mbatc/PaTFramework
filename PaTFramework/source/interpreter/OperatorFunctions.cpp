#include "OperatorFunctions.h"
#include "String.h"

#include "../CScriptCOMObject.h"

        unsigned int SCRIPT::OPERATOR::add(void* a, unsigned int a_type,
                         const void* val_1, const unsigned int val_1_type,
                         const void* val_2, const unsigned int val_2_type)
        {
            if(a_type == SSTR)
            {
                if(val_1_type != SSTR)
                {
                    return ERROR_OPERATION_NOT_DEFINED;
                }
                switch(val_2_type)
                {
                case SSTR:
                    *(String*)a  = *(String*)val_1 + *(String*)val_2;
                    return NO_ERROR;
                    break;
                case SINT:
                    *(String*)a  = *(String*)val_1 + *(int*)val_2;
                    return NO_ERROR;
                    break;
                case SBOOL:
                    *(String*)a  = *(String*)val_1 + *(bool*)val_2;
                    return NO_ERROR;
                    break;
                case SFLOAT:
                    *(String*)a  = *(String*)val_1 + *(float*)val_2;
                    return NO_ERROR;
                }
            }
            else
            {
                if(val_1_type == SSTR || val_2_type == SSTR)
                    return ERROR_OPERATION_NOT_DEFINED;
            }
            if(a_type == SINT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(int*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(int*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(float*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(float*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SFLOAT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(int*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(int*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(float*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(float*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SBOOL)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(int*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(int*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(float*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(float*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            return NO_ERROR;
        }
        unsigned int SCRIPT::OPERATOR::subtract(void* a, unsigned int a_type,
                              const void* val_1, const unsigned int val_1_type,
                              const void* val_2, const unsigned int val_2_type)
        {
            if(a_type == SSTR || val_1_type == SSTR || val_2_type == SSTR)
                return ERROR_OPERATION_NOT_DEFINED;

            if(a_type == SINT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(int*)val_1 - *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(int*)val_1 - *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(float*)val_1 + *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(float*)val_1 + *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SFLOAT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(int*)val_1 - *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(int*)val_1 - *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(float*)val_1 - *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(float*)val_1 - *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SBOOL)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(int*)val_1 - *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(int*)val_1 - *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(float*)val_1 - *(int*)val_2;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(float*)val_1 - *(float*)val_2;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            return NO_ERROR;
        }
        unsigned int SCRIPT::OPERATOR::multiply(void* a, unsigned int a_type,
                              const void* val_1, const unsigned int val_1_type,
                              void* val_2, unsigned int val_2_type)
        {
            if(a_type == SSTR || val_1_type == SSTR || val_2_type == SSTR)
                return ERROR_OPERATION_NOT_DEFINED;

            if(a_type == SINT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(int*)val_1 * (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(int*)val_1 * (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(float*)val_1 * (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(float*)val_1 * (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SFLOAT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(int*)val_1 * (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(int*)val_1 * (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(float*)val_1 * (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(float*)val_1 * (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SBOOL)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(int*)val_1 * (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(int*)val_1 * (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(float*)val_1 * (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(float*)val_1 * (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            return NO_ERROR;
        }
        unsigned int SCRIPT::OPERATOR::divide(void* a, unsigned int a_type,
                            void* val_1, unsigned int val_1_type,
                            void* val_2, unsigned int val_2_type)
        {
            if(a_type == SSTR || val_1_type == SSTR || val_2_type == SSTR)
                return ERROR_OPERATION_NOT_DEFINED;

            if(a_type == SINT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(int*)val_1 / (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(int*)val_1 / (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(int*)a = *(float*)val_1 / (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(int*)a = *(float*)val_1 / (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SFLOAT)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(int*)val_1 / (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(int*)val_1 / (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(float*)a = *(float*)val_1 / (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(float*)a = *(float*)val_1 / (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            else if(a_type == SBOOL)
            {
                if(val_1_type == SINT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(int*)val_1 / (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(int*)val_1 / (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SFLOAT)
                {
                    if(val_2_type == SINT)
                    {
                        *(bool*)a = *(float*)val_1 / (*(int*)val_2);
                    }
                    else if(val_2_type == SFLOAT)
                    {
                        *(bool*)a = *(float*)val_1 / (*(float*)val_2);
                    }
                    else if(val_2_type == SBOOL)
                    {
                        return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
                else if(val_1_type == SBOOL)
                {
                    if(val_2_type == SINT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SFLOAT)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                    else if(val_2_type == SBOOL)
                    {
                         return ERROR_OPERATION_NOT_DEFINED;
                    }
                }
            }
            return NO_ERROR;
        }
        unsigned int SCRIPT::OPERATOR::assignVar(void* a, unsigned int a_type,
                               void* val, unsigned int val_type)
        {
			if (a_type == SOBJECT)
			{
				if (val_type != SOBJECT)
				{
					return ERROR_OPERATION_NOT_DEFINED;
				}
				CScriptCOMObject* _a = *(CScriptCOMObject**)a;
				CScriptCOMObject* _v = *(CScriptCOMObject**)val;
				if (!_a)
				{
					*(CScriptCOMObject**)a = *(CScriptCOMObject**)val;
				}
				else if (_a->COMGetType() == _v->COMGetType())
				{
					*(CScriptCOMObject**)a = *(CScriptCOMObject**)val;
				}
				else
				{
					return ERROR_OPERATION_NOT_DEFINED;
				}
			}
            if(a_type == SSTR)
            {
                if(val_type == SINT)
                {
					*(String*)a = *(int*)val;
                }
				else if (val_type == SFLOAT)
				{
					*(String*)a = *(float*)val;
				}
				else if (val_type == SBOOL)
				{
					*(String*)a = *(bool*)val;
				}
                else if(val_type = SSTR)
                {
                    *(String*)a = *(String*)val;
                }
            }

            if(a_type == SINT)
            {
                if(val_type == SINT)
                {
                    *(int*)a = *(int*)val;
                }
                else if(val_type == SFLOAT)
                {
                    *(int*)a = *(float*)val;
                }
                else if(val_type == SBOOL)
                {
                    *(int*)a = *(bool*)val;
                }
				else if (val_type == SSTR)
				{
					*(int*)a = (*(String*)val).toInt();
				}
            }
            else if(a_type == SFLOAT)
            {
                if(val_type == SINT)
                {
                    *(float*)a = *(int*)val;
                }
                else if(val_type == SFLOAT)
                {
                    *(float*)a = *(float*)val;
                }
                else if(val_type == SBOOL)
                {
                    *(float*)a = *(bool*)val;
                }
				else if (val_type == SSTR)
				{
					*(float*)a = (*(String*)val).toFloat();
				}
            }
            else if(a_type == SBOOL)
            {
                if(val_type == SINT)
                {
                    *(bool*)a = *(int*)val;
                }
                else if(val_type == SFLOAT)
                {
                    *(bool*)a = *(float*)val;
                }
                else if(val_type == SBOOL)
                {
                    *(bool*)a = *(bool*)val;
                }
				else if (val_type == SSTR)
				{
					*(bool*)a = (*(String*)val).toBool();
				}
            }

            return NO_ERROR;
        }
        unsigned int SCRIPT::LOGIC::lessThan(bool& result, void* val_1, unsigned int val_1_type,
                              void* val_2, unsigned int val_2_type)
        {
            if(val_1_type == SSTR || val_2_type == SSTR)
                return ERROR_OPERATION_NOT_DEFINED;
            if(val_1_type == SINT)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(int*)val_1) < *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(int*)val_1) < *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(int*)val_1) < *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }
            else if(val_1_type == SFLOAT)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(float*)val_1) < *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(float*)val_1) < *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(float*)val_1) < *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }
            else if(val_1_type == SBOOL)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(bool*)val_1) < *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(bool*)val_1) < *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(bool*)val_1) < *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }

            return NO_ERROR;
        }
        unsigned int SCRIPT::LOGIC::moreThan(bool& result, void* val_1, unsigned int val_1_type,
                              void* val_2, unsigned int val_2_type)
        {
            if(val_1_type == SSTR || val_2_type == SSTR)
                return ERROR_OPERATION_NOT_DEFINED;

            if(val_1_type == SINT)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(int*)val_1) > *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(int*)val_1) > *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(int*)val_1) > *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }
            else if(val_1_type == SFLOAT)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(float*)val_1) > *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(float*)val_1) > *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(float*)val_1) == *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }
            else if(val_1_type == SBOOL)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(bool*)val_1) > *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(bool*)val_1) > *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(bool*)val_1) > *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }

            return NO_ERROR;
        }
        unsigned int SCRIPT::LOGIC::equals(bool& result, void* val_1, unsigned int val_1_type,
                            void* val_2, unsigned int val_2_type)
        {
            if(val_1_type == SSTR)
            {
                if(val_2_type != SSTR)
                {

                }
                else
                {
                    result = false;
                    if(*(String*)val_1 == *(String*)val_2)
                    {
                        result = true;
                    }
                }
            }
            if(val_1_type == SINT)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(int*)val_1) == *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(int*)val_1) == *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(int*)val_1) == *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }
            else if(val_1_type == SFLOAT)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(float*)val_1) == *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(float*)val_1) == *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(float*)val_1) == *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }
            else if(val_1_type == SBOOL)
            {
                if(val_2_type == SINT)
                {
                    result = false;
                    if((*(bool*)val_1) == *(int*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SFLOAT)
                {
                    result = false;
                    if((*(bool*)val_1) == *(float*)val_2)
                    {
                        result = true;
                    }
                }
                else if(val_2_type == SBOOL)
                {
                    result = false;
                    if((*(bool*)val_1) == *(bool*)val_2)
                    {
                        result = true;
                    }
                }
            }

            return NO_ERROR;
        }
