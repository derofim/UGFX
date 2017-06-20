// This file is part of the "UGFX".
// For conditions of distribution and use, see copyright notice in LICENSE.txt

#pragma once

// @note "new (std::nothrow)" will return a null pointer instead of throwing an exception.
//		 You can handle the failure locally; 
//		 perhaps by requesting to free some other memory and then retrying, 
//		 or by trying to allocate something smaller, 
//		 or using an alternative algorithm that doesn't need extra memory.
#define exanew		new (std::nothrow)

#define exadel		delete

template< class T > void SafeDelete(T*& p)
{
	if (p != nullptr) {
		exadel p;
		p = nullptr;
	}
}

template< class T > void SafeDeleteArray(T*& p)
{
	if (p != nullptr) {
		exadel[] p;
		p = nullptr;
	}
}

template< class T > void SafeRelease(T*& p)
{
	if (p) {
		p->Release();
		p = nullptr;
	}
}