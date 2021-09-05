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
				Utils::MemoryManager& memoryManager,
				const std::function<Game::XAssetHeader (uint8_t type, const char* name)>& findAssetFunction
			) override;

			virtual const char* getSerializedFilePath(const char* assetName) override;
			virtual const char* geSerializedBasePath() override;
		};

	}
}