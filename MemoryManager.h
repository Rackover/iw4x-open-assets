#pragma once

namespace iw4oa {
    namespace Utils {
        class MemoryManager
        {
            class IDestructible
            {
            public:
                virtual ~IDestructible() = default;
            };

            template <class T>
            class Allocation final : public IDestructible
            {
            public:
                T entry;

                template <class... _Valty>
                explicit Allocation(_Valty&&... _Val)
                    : entry(std::forward<_Valty>(_Val)...)
                {
                }

                ~Allocation() override = default;

                Allocation(const Allocation& other) = delete;
                Allocation(Allocation&& other) noexcept = delete;
                Allocation& operator=(const Allocation& other) = delete;
                Allocation& operator=(Allocation&& other) noexcept = delete;
            };

            class AllocationInfo
            {
            public:
                IDestructible* data;
                void* dataPtr;

                AllocationInfo(IDestructible* data, void* dataPtr);
            };

            std::vector<void*> allocations{};
            std::vector<AllocationInfo> destructibles{};

        public:
            MemoryManager();
            virtual ~MemoryManager();

            void* Alloc(size_t size);

            template <class T, class... _Valty>
            T* Alloc()
            {
                return reinterpret_cast<T*>(Alloc(sizeof(T)));
            }

            template <class T, class... _Valty>
            T* AllocArray(size_t size)
            {
                return reinterpret_cast<T*>(Alloc(sizeof(T) * size));
            }

            char* Duplicate(const char* str);

            template <class T, class... _Valty>
            T* Create(_Valty&&... _Val)
            {
                Allocation<T>* allocation = new Allocation<T>(std::forward<_Valty>(_Val)...);
                destructibles.emplace_back(allocation, &allocation->entry);
                return &allocation->entry;
            }

            void Delete(void* data);
        };
    }
}