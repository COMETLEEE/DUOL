#pragma once
#include <typeinfo>

#if defined(_DEBUG) || defined(DEBUG)

template <typename DstT, typename SrcT>
DstT CheckedCast(SrcT* obj)
{
    try
    {
        DstT casted = dynamic_cast<DstT>(obj);
        if (!casted)
            throw std::bad_cast();
        return casted;
    }
    catch (const std::bad_cast&)
    {
#if defined _WIN32 && defined _DEBUG
        __debugbreak();
#endif
        throw;
    }
}

#define	TYPE_CAST(TYPE, OBJ) CheckedCast<TYPE>(OBJ) 

#else
#define	TYPE_CAST(TYPE, OBJ) static_cast<TYPE>(OBJ) 
#endif
