#pragma once

#include "pch.h";

namespace iw4oa
{
	namespace AssetInterfaces {

		class SoundAliasList : IAsset {

		public:
			virtual void serialize(void* asset, std::ostream& outputStream) override;
			virtual void* deserialize(
				std::istream& inputStream,
				const std::string assetName,
				MemoryManager& memoryManager,
				const std::function<Game::XAssetHeader* (uint8_t type, const char* name)>& findAssetFunction = nullptr
			) override;

		};

	}
}