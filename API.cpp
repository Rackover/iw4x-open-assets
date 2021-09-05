#include "pch.h"

namespace iw4oa {

	std::function<char* (unsigned short key)> API::getStringFromKeyFunc = nullptr;
	std::function<unsigned short(char* string)> API::storeStringFunc = nullptr;

	std::map<uint8_t, IAssetHandler*> API::instantiatedHandlers {
		{static_cast<uint8_t>(Game::XAssetType::ASSET_TYPE_SOUND), new AssetHandlers::SoundAliasList()},
		{static_cast<uint8_t>(Game::XAssetType::ASSET_TYPE_XMODEL), new AssetHandlers::Model()},
		{static_cast<uint8_t>(Game::XAssetType::ASSET_TYPE_MATERIAL), new AssetHandlers::Material()}
	};

	IAssetHandler* API::getHandlerForType(uint8_t uiType){

		auto type = static_cast<Game::XAssetType>(uiType);

		if (type < 0 || type >= Game::XAssetType::ASSET_TYPE_COUNT) {
			throw MissingAssetHandlerException("Wrong asset type %d", std::to_string(type).c_str());
		}

		for (auto handlerKv : instantiatedHandlers) {
			if (handlerKv.first == uiType) {
				return handlerKv.second;
			}
		}

		return nullptr;
	}
}