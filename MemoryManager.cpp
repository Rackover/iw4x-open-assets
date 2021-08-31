#include "pch.h"

namespace iw4oa 
{
    MemoryManager::AllocationInfo::AllocationInfo(IDestructible* data, void* dataPtr)
    {
        this->data = data;
        this->dataPtr = dataPtr;
    }

    MemoryManager::MemoryManager()
    = default;

    MemoryManager::~MemoryManager()
    {
        for (auto allocation : allocations)
        {
            free(allocation);
        }
        allocations.clear();

        for (auto destructible : destructibles)
        {
            delete destructible.data;
        }
        destructibles.clear();
    }

    void* MemoryManager::Alloc(const size_t size)
    {
        void* result = malloc(size);
        allocations.push_back(result);

        return result;
    }

    char* MemoryManager::Duplicate(const char* str)
    {
    #ifdef _MSC_VER
        auto* result = _strdup(str);
    #else
        auto* result = strdup(str);
    #endif
        allocations.push_back(result);

        return result;
    }

    void MemoryManager::Delete(void* data)
    {
        for (auto iAlloc = destructibles.begin(); iAlloc != destructibles.end(); ++iAlloc)
        {
            if (iAlloc->dataPtr == data)
            {
                delete iAlloc->data;
                destructibles.erase(iAlloc);
                return;
            }
        }
    }

}