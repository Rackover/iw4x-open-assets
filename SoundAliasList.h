#pragma once

namespace iw4oa
{
	namespace AssetHandlers {

		class SoundAliasList : public IAssetHandler {

		public:
			virtual void serialize(void* asset, const std::string& baseOutputPath) override;
			virtual void* deserialize(
				const char* assetName,
				const std::string& buffer,
				MemoryManager& memoryManager,
				const std::function<Game::XAssetHeader (uint8_t type, const char* name)>& findAssetFunction
			) override;

			virtual const char* get_serialized_file_path(const char* assetName) override;
		};

	}
}