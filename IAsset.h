#pragma once

#include "pch.h";

#define JSON_INDENT 4

namespace iw4oa 
{
	class IAsset {

	public:
		class DeserializationException : public std::exception{
			public:
				DeserializationException(const char* reason) {
					this->reason = reason;
				}

				DeserializationException(const char* fmt, const char* args, ...) {
					this->reason = Utils::String::VA(fmt, args);
				}

				std::string toString() const throw()
				{
					return reason;
				}

			private:
				const char* reason;
		};
		
		virtual void serialize(void* asset, std::ostream& outputStream) = 0;
		virtual void* deserialize(
			std::istream& inputStream,
			const std::string assetName, 
			MemoryManager &memoryManager,
			const std::function<Game::XAssetHeader*(uint8_t type, const char* name)> &findAssetFunction = nullptr
		) = 0;
	};

}