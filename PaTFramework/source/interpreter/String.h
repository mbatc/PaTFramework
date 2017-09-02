#pragma once

#include "../helpers/MemoryHelper.h"
#include <string.h>
#include <stdio.h>

class String
{
public:
    String();
	String(const char* in);
    ~String();

    void	SetString(const char* str);
    char*	GetString();
    int		GetStringLen();

	int		toInt();
	float	toFloat();
	bool	toBool();

    String operator+(String in)
    {
        String out;
        int len_1 = this->GetStringLen();
        int len_2 = in.GetStringLen();

        char* newStr = new char[len_1 + len_2 + 1];

        strcpy(newStr, this->GetString());
        strcpy(&newStr[len_1], in.GetString());

        out.SetString(newStr);

        return out;
    }
	
	String operator+(char in)
	{		
		char temp[2] = { in, '\0' };
		String s_in;
		s_in.SetString(temp);

		return *this + temp;
	}

	String operator+(const char* in)
	{
		return *this + String(in);
	}

    void operator=(String in)
    {
        char* newStr = new char[in.GetStringLen() + 1];

        if(in.GetString())
            strcpy(newStr, in.GetString());
        newStr[in.GetStringLen()] = '\0';

        this->SetString(newStr);
    }
	void operator=(char* in)
	{
		if (in)
			this->SetString(in);
		else
			MEMORY::SafeDeleteArr(m_pStr);
	}

    bool operator == (String in)
    {
        return !strcmp(this->GetString(), in.GetString());
    }

    bool operator != (String in)
    {
        return (bool)strcmp(this->GetString(), in.GetString());
    }

    String operator += (String in)
    {
		*this = *this + in;
        return *this;
    }

	String operator += (const char in)
	{
		*this = *this + in;
		return *this;
	}

	String operator += (const char* in)
	{
		return *this += String(in);
	}

    String operator + (int in)
    {
        char temp[512];
        sprintf(temp,"%d", in);
        String tempString;
        tempString.SetString(temp);
        return *this + tempString;
    }

    String operator + (float in)
    {
        char temp[512];
        sprintf(temp,"%f", in);
        String tempString;
        tempString.SetString(temp);
        return *this + tempString;
    }

    String operator + (bool in)
    {
        String temp;
        switch(in)
        {
        case true:
            temp.SetString("true");
            break;
        case false:
            temp.SetString("false");
            break;
        }

		return *this + temp;
    }

	void operator = (int in)
	{
		*this = "";
		*this = *this + in;
	}

	void operator = (float in)
	{
		*this = "";
		*this = *this + in;
	}

	void operator = (bool in)
	{
		*this = "";
		*this = *this + in;
	}

private:
    char* m_pStr;
};
