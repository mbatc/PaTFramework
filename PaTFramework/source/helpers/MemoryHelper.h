#pragma once

#include "../ErrorDefinitions.h"

namespace MEMORY
{
	template <class T> void SafeDelete(T*& t)
	{
		if(t)
			delete t;
		t = nullptr;
	}
	template <class T> void SafeDeleteArr(T*& t)
	{
		if(t)
			delete[] t;
		t = nullptr;
	}

	namespace LIST
	{
		template <class T> void InsertInList(T**& t, T* newT, int insert_index, int& nt)
		{
			if (!newT)
				return;

			T** temp = new T*[(nt)+1];
			
			for (int i = 0; i < nt + 1; i++)
			{
				if (i < insert_index)
				{
					temp[i] = t[i];
				}
				else if (i > insert_index)
				{
					temp[i] = t[i - 1];
				}
				else
				{
					temp[i] = newT;
				}
			}

			MEMORY::SafeDeleteArr(t);
			t = temp;
			nt++;
		}

		//Adds 'newT' to the end of the list
		template <class T> void AddToList(T*& t, T* newT, int& nt)
		{
			if(!newT)
				return;

			T* temp = new T[(nt) + 1];
			for(int i = 0; i < nt; i++)
			{
				temp[i] = t[i];
			}

			temp[nt] = *newT;

			SafeDeleteArr(t);
			nt++;

			t = new T[nt];

			for(int i = 0; i < nt; i++)
			{
				t[i] = temp[i];
			}

			SafeDeleteArr(temp);
		}
		//Adds 'newT' to the end of the list
		template <class T> void AddToListArr(T**& t, T* newT, int& nt)
		{
			if(!newT)
				return;

			T** temp = new T*[nt + 1];
			for(int i = 0; i < nt; i++)
			{
				temp[i] = t[i];
				t[i] = nullptr;
			}

			temp[nt] = newT;

			SafeDeleteArr(t);

			nt++;

			t = new T*[nt];

			for(int i = 0; i <  nt; i++)
			{
				t[i] = temp[i];
				temp[i] = nullptr;
			}

			SafeDeleteArr(temp);
		}
		//removes the item in the list and the index 'remove_i'
		template<class T> void RemoveItem(T**& t, int& nt, int remove_i)
		{
			T** temp = new T*[nt - 1];

			for (int i = 0; i < nt; i++)
			{
				if (i < remove_i)
					temp[i] = t[i];
				else if(i > remove_i)
					temp[i - 1] = t[i];
			}

			nt--;

			MEMORY::SafeDeleteArr(t);
			t = temp;
		}
		
		template<class T> void RemoveItem(T*& t, int& nt, int remove_i)
		{
			T* temp = new T[nt - 1];

			for (int i = 0; i < nt; i++)
			{
				if (i < remove_i)
					temp[i] = t[i];
				else if(i > remove_i)
					temp[i - 1] = t[i];
			}

			nt--;

			MEMORY::SafeDeleteArr(t);
			t = temp;
		}
	}

	namespace DX
	{
		template <class T> void SafeRelease(T*& t)
		{
			if(t)
				t->Release();
			t = nullptr;
		}
	}

	namespace SCRIPT
	{
		template <class T> unsigned int SafeAlloc(T*& in, const unsigned int type)
		{
			switch (type)
			{
			case SBOOL:
				MEMORY::SafeDelete((bool*)in);
				in = 0;
				in = (T*) new bool;
				if (in == 0)
					return ERROR_UNDEFINED_ERROR;

				return NO_ERROR;
			case SFLOAT:
				MEMORY::SafeDelete((float*)in);
				in = 0;
				in = (T*) new float;
				if (in == 0)
					return ERROR_UNDEFINED_ERROR;

				return NO_ERROR;
			case SINT:
				MEMORY::SafeDelete((int*)in);
				in = 0;
				in = (T*) new int;
				if (in == 0)
					return ERROR_UNDEFINED_ERROR;

				return NO_ERROR;
			case SSTR:
				MEMORY::SafeDelete((String*)in);
				in = 0;
				in = (T*) new String;
				if (in == 0)
					return ERROR_UNDEFINED_ERROR;

				return NO_ERROR;
			default:
				return ERROR_UNDEFINED_DATA_TYPE;
			}
		}
	}
}