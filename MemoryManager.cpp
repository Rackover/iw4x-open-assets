#include "MemoryManager.h"

namespace iw4oa 
{
    MemoryManager::AllocationInfo::AllocationInfo(IDestructible* data, void* dataPtr)
    {
        data = data;
        dataPtr = dataPtr;
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

        for (auto destructible : destructible)
        {
            delete destructible.data;
        }
        destructible.clear();
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
        for (auto iAlloc = destructible.begin(); iAlloc != destructible.end(); ++iAlloc)
        {
            if (iAlloc->dataPtr == data)
            {
                delete iAlloc->data;
                destructible.erase(iAlloc);
                return;
            }
        }
    }

}