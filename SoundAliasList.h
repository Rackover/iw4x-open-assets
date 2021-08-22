#pragma once

namespace iw4oa
{
	namespace AssetHandlers {

		class SoundAliasList : public IAssetHandler {

		public:
			virtual void serialize(void* asset, std::ostream& outputStream) override;
			virtual void* deserialize(
				std::istream& inputStream,
				const std::string assetName,
				MemoryManager& memoryManager,
				const std::function<Game::XAssetHeader* (uint8_t type, const char* name)>& findAssetFunction
			) override;

		};

	}
}