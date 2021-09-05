#pragma once

namespace iw4oa
{
	namespace AssetHandlers
	{

#define IW4X_MODEL_VERSION 6

		class Model : public IAssetHandler {

		public:
			virtual void serialize(void* asset, const std::string& baseOutputPath) override;
			virtual void* deserialize(
				const char* assetName,
				const std::string& buffer,
				Utils::MemoryManager& memoryManager,
				const std::function<Game::XAssetHeader(uint8_t type, const char* name)>& findAssetFunction
			) override;

			virtual const char* getSerializedFilePath(const char* assetName) override;
			virtual const char* geSerializedBasePath() override;

		private:
			void saveXSurfaceCollisionTree(Game::XSurfaceCollisionTree* entry, Utils::Stream* buffer);
			void saveXSurface(Game::XSurface* surf, Utils::Stream* buffer);
			void saveXModelSurfs(Game::XModelSurfs* asset, Utils::Stream* buffer);
		};
	}
}
