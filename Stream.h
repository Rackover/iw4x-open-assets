#pragma once

namespace iw4oa
{
	namespace Utils
	{
		class Stream
		{
		private:
			std::string buffer;
			std::vector<void*> pointers;

		public:

			class Reader
			{
			public:
				Reader(Utils::MemoryManager* _allocator, const std::string& _buffer) : position(0), buffer(_buffer), allocator(_allocator) {}

				std::string readString();
				const char* readCString();

				char readByte();

				void* read(size_t size, size_t count = 1);
				template <typename T> inline T* readObject()
				{
					return readArray<T>(1);
				}
				template <typename T> inline T* readArray(size_t count = 1)
				{
					return reinterpret_cast<T*>(this->read(sizeof(T), count));
				}
				template <typename T> T read()
				{
					T obj;

					for (unsigned int i = 0; i < sizeof(T); ++i)
					{
						reinterpret_cast<char*>(&obj)[i] = this->readByte();
					}

					return obj;
				}

				bool end();
				void seek(unsigned int position);
				void seekRelative(unsigned int position);

				void* readPointer();
				void mapPointer(void* oldPointer, void* newPointer);
				bool hasPointer(void* pointer);

			private:
				unsigned int position;
				std::string buffer;
				std::map<void*, void*> pointerMap;
				Utils::MemoryManager* allocator;
			};

			
			Stream();
			Stream(size_t size);
			~Stream();

			class Offset
			{
			public:
				Offset(Stream* _stream, size_t _offset) : offset(_offset), stream(_stream) {}

				template <typename T> T* as()
				{
					return reinterpret_cast<T*>(this->stream->data() + offset);
				}

			private:
				size_t offset;
				Stream* stream;
			};

			size_t length();
			size_t capacity();

			char* save(const void* _str, size_t size, size_t count = 1);
			char* save(int value, size_t count = 1);
			template <typename T> inline char* saveObject(T value)
			{
				return saveArray(&value, 1);
			}
			template <typename T> inline char* saveArray(T* array, size_t count)
			{
				return save(array, sizeof(T), count);
			}

			char* saveString(std::string string);
			char* saveString(const char* string);
			char* saveString(const char* string, size_t len);
			char* saveByte(unsigned char byte, size_t count = 1);
			char* saveNull(size_t count = 1);
			char* saveMax(size_t count = 1);

			char* saveText(std::string string);

			Offset offset();
			char* data();
			char* at();
			template <typename T> T* Dest()
			{
				return reinterpret_cast<T*>(this->at());
			}

			void toBuffer(std::string& outBuffer);
			std::string toBuffer();

			void storePointer(void* pointer);
			bool hasPointer(void* pointer);

			template <typename T> static inline void ClearPointer(T** object)
			{
				*object = reinterpret_cast<T*>(-1);
			}
		};
	}
}