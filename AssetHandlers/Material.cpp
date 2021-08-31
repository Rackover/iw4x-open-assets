#include "pch.h"

void iw4oa::AssetHandlers::Material::serialize(void* asset, const std::string& baseOutputPath)
{
	Game::Material* material = reinterpret_cast<Game::Material*>(asset);

	std::string buffer;

	auto constantTable = nlohmann::json::array_t(material->constantCount);
	for (char i = 0; i < material->constantCount; i++)
	{
		auto literals = nlohmann::json::array_t(4);
		for (char literalIndex = 0; literalIndex < 4; literalIndex++)
		{
			literals[literalIndex] = material->constantTable[i].literal[literalIndex];
		}

		constantTable.push_back(nlohmann::json::object_t{
			{"literals", literals},
			{"name", material->constantTable[i].name},
			{"nameHash", static_cast<int>(material->constantTable[i].nameHash)}
			});
	}

	auto textureTable = nlohmann::json::array_t(material->textureCount);
	for (char i = 0; i < material->textureCount; i++)
	{
		auto texture = material->textureTable[i];
		auto imageName = texture.u.image->name;
		bool hasWater = false;
		nlohmann::json::object_t jsonWater{};

		if (texture.semantic == SEMANTIC_WATER_MAP)
		{
			if (texture.u.water)
			{
				Game::water_t* water = texture.u.water;
				hasWater = true;

				imageName = water->image->name;

				jsonWater["floatTime"] = water->writable.floatTime;
				auto codeConstants = nlohmann::json::array_t{};
				for (size_t j = 0; j < 4; j++)
				{
					codeConstants.push_back(water->codeConstant[j]);
				}

				jsonWater["codeConstant"] = codeConstants;

				jsonWater["M"] = water->M;
				jsonWater["N"] = water->N;
				jsonWater["Lx"] = water->Lx;
				jsonWater["Lz"] = water->Lz;
				jsonWater["gravity"] = water->gravity;
				jsonWater["windvel"] = water->windvel;

				auto windDirection = nlohmann::json::array_t{ water->winddir[0],  water->winddir[1] };

				jsonWater["winddir"] = windDirection;
				jsonWater["amplitude"] = water->amplitude;

				nlohmann::json::array_t waterComplexData{};
				nlohmann::json::array_t wTerm{};

				for (int j = 0; j < water->M * water->N; j++)
				{
					nlohmann::json::object_t complexdata{};
					nlohmann::json::array_t curWTerm{};

					complexdata["real"] = water->H0[j].real;
					complexdata["imag"] = water->H0[j].imag;

					curWTerm[j] = water->wTerm[j];

					waterComplexData[j] = complexdata;
				}

				jsonWater["complex"] = waterComplexData;
				jsonWater["wTerm"] = wTerm;
			}
		}

		auto jsonTexture = nlohmann::json::object_t{
			{ "image", imageName },
			{ "firstCharacter", texture.nameStart },
			{ "lastCharacter", texture.nameEnd },
			{ "firstCharacter", texture.u.image->name[0] },
			{ "sampleState", texture.samplerState },
			{ "semantic", texture.semantic },
			{ "nameHash", texture.nameHash }
		};

		if (hasWater) {
			jsonTexture["waterinfo"] = jsonWater;
		}

		textureTable.push_back(jsonTexture);
	}


	auto stateMap = nlohmann::json::array_t{};
	for (char i = 0; i < material->stateBitsCount; i++)
	{
		stateMap.push_back(
			nlohmann::json::array_t{
				material->stateBitsTable[i].loadBits[0],
				material->stateBitsTable[i].loadBits[1]
			}
		);
	}

	nlohmann::json matData = nlohmann::json::object_t{
		{ "name",  material->info.name },
		{ "techniqueSet->name", material->techniqueSet->name },
		{ "gameFlags", material->info.gameFlags },
		{ "sortKey", material->info.sortKey },
		{ "animationX", material->info.textureAtlasColumnCount },
		{ "animationY", material->info.textureAtlasRowCount },
		{ "cameraRegion", material->cameraRegion },
		{ "constantTable", constantTable},
		{ "maps", textureTable},
		{ "stateMap", stateMap},
		{ "stateFlags", material->stateFlags },
		{ "surfaceTypeBits", material->info.surfaceTypeBits },
		{ "unknown", material->info.drawSurf.packed }
	};

	auto buffer = matData.dump(JSON_INDENT);


	auto outPath = Utils::String::VA("%s", baseOutputPath.c_str(), get_serialized_file_path(material->info.name));
	std::filesystem::create_directories(Utils::String::VA("%s/%s", baseOutputPath.c_str(), get_serialized_base_path()));
	std::ofstream destination(outPath, std::ios::binary);
	destination.write(buffer.data(), buffer.length());
}

void* iw4oa::AssetHandlers::Material::deserialize(
	const char* assetName,
	const std::string& buffer,
	MemoryManager& memoryManager,
	const std::function<Game::XAssetHeader(uint8_t type, const char* name)>& findAssetFunction
)
{
	Game::Material* material = memoryManager.Alloc<Game::Material>();

	if (!material)
	{
		throw DeserializationException("Error allocating memory for material structure!\n");
	}

	if (buffer.empty())
	{
		throw MissingFileException();
	}



	std::string errors;
	nlohmann::json jsonMat = nlohmann::json::parse(buffer);

	if (!jsonMat.is_object())
	{
		throw new DeserializationException("Failed to load material information for %s!", assetName);
		return;
	}

	material->info.textureAtlasColumnCount = jsonMat["animationX"].get<nlohmann::json::number_unsigned_t>();
	material->info.textureAtlasRowCount = jsonMat["animationY"].get<nlohmann::json::number_unsigned_t>();
	material->cameraRegion = jsonMat["cameraRegion"].get<nlohmann::json::number_unsigned_t>();

	nlohmann::json::array_t jsonConstantTable = jsonMat["constantTable"];
	material->constantCount = static_cast<char>(jsonConstantTable.size());
	material->constantTable = memoryManager.AllocArray<Game::MaterialConstantDef>(material->constantCount);

	for (char i = 0; i < material->constantCount; i++)
	{
		for (size_t litteralIndex = 0; litteralIndex < 4; litteralIndex++)
		{
			material->constantTable[i].literal[litteralIndex] = jsonConstantTable[i]["literal"][litteralIndex].get<nlohmann::json::number_float_t>();
		}

		strncpy(material->constantTable[i].name, jsonConstantTable[i]["name"].get<std::string>().c_str(), sizeof(Game::MaterialConstantDef::name));
		material->constantTable[i].nameHash = jsonConstantTable[i]["nameHash"].get<unsigned int>();
	}

	material->info.gameFlags = jsonMat["gameFlags"].get<nlohmann::json::number_unsigned_t>();

	nlohmann::json::array_t jsonMaps = jsonMat["maps"];
	material->textureCount = static_cast<char>(jsonMaps.size());
	material->textureTable = memoryManager.AllocArray<Game::MaterialTextureDef>(material->textureCount);

	for (char i = 0; i < material->textureCount; i++)
	{
		auto imageName = jsonMaps[i]["image"].get<nlohmann::json::string_t>().c_str();
		auto texture = &material->textureTable[i];
		texture->nameStart = static_cast<char>(jsonMaps[i]["firstCharacter"].get<nlohmann::json::number_integer_t>());
		texture->nameEnd = static_cast<char>(jsonMaps[i]["lastCharacter"].get<nlohmann::json::number_integer_t>());
		texture->u.image = findAssetFunction(Game::XAssetType::ASSET_TYPE_IMAGE, imageName).image;
		texture->samplerState = static_cast<char>(jsonMaps[i]["sampleState"].get<nlohmann::json::number_integer_t>());
		texture->semantic = static_cast<char>(jsonMaps[i]["semantic"].get<nlohmann::json::number_integer_t>());
		texture->nameHash = jsonMaps[i]["typeHash"].get<nlohmann::json::number_unsigned_t>();

		if (texture->semantic == SEMANTIC_WATER_MAP)
		{
			if (jsonMaps[i]["waterinfo"].is_object())
			{
				auto jsonWater = jsonMaps[i]["waterinfo"];
				auto water = memoryManager.Alloc<Game::water_t>();

				water->image->name = imageName;

				water->writable.floatTime = static_cast<float>(jsonWater["floatTime"].get<nlohmann::json::number_float_t>());
				auto codeConstants = nlohmann::json::array_t{};
				for (size_t j = 0; j < 4; j++)
				{
					water->codeConstant[j] = static_cast<float>(jsonWater["codeConstant"][j].get<nlohmann::json::number_float_t>());
				}

				water->M = jsonWater["M"].get<nlohmann::json::number_integer_t>();
				water->N = jsonWater["N"].get<nlohmann::json::number_integer_t>();
				water->Lx = jsonWater["Lx"].get<nlohmann::json::number_float_t>();
				water->Lz = jsonWater["Lz"].get<nlohmann::json::number_float_t>();
				water->gravity = jsonWater["gravity"].get<nlohmann::json::number_float_t>();
				water->windvel = jsonWater["windvel"].get<nlohmann::json::number_float_t>();

				water->winddir[0] = jsonWater["winddir"][0].get<nlohmann::json::number_float_t>();
				water->winddir[1] = jsonWater["winddir"][1].get<nlohmann::json::number_float_t>();

				water->amplitude = jsonWater["amplitude"].get<nlohmann::json::number_float_t>();

				auto count = jsonWater["M"].get<nlohmann::json::number_integer_t>() * jsonWater["N"].get<nlohmann::json::number_integer_t>();

				water->H0 = memoryManager.AllocArray<Game::complex_s>(count);

				for (int j = 0; j < count; j++)
				{
					water->wTerm[j] = jsonWater["wTerm"][j].get<nlohmann::json::number_float_t>();

					water->H0[j].real = jsonWater["complex"][j]["real"].get<nlohmann::json::number_float_t>();
					water->H0[j].imag = jsonWater["complex"][j]["complex"].get<nlohmann::json::number_float_t>();
				}


				texture->u.water = water;
			}
			else
			{
				throw new DeserializationException("Invalid material! %s is expected to have water info for texture %s, but no 'waterinfo' object is described.", material->info.name, imageName);
			}
		}
	}

	material->info.name = memoryManager.Duplicate(jsonMat["name"].get<nlohmann::json::string_t>().c_str());
	material->info.sortKey = static_cast<char>(jsonMat["sortKey"].get<nlohmann::json::number_integer_t>());
	material->stateFlags = static_cast<char>(jsonMat["stateFlags"].get<nlohmann::json::number_integer_t>());

	nlohmann::json::array_t jsonStateMap = jsonMat["stateMap"];
	material->stateBitsCount = static_cast<char>(jsonStateMap.size());
	material->stateBitsTable = memoryManager.AllocArray<Game::GfxStateBits>(material->stateBitsCount);

	for (char i = 0; i < material->stateBitsCount; i++)
	{
		material->stateBitsTable[i].loadBits[0] = jsonStateMap[i][0].get<nlohmann::json::number_integer_t>();
		material->stateBitsTable[i].loadBits[1] = jsonStateMap[i][1].get<nlohmann::json::number_integer_t>();
	}

	material->info.surfaceTypeBits = jsonMat["surfaceTypeBits"].get<nlohmann::json::number_unsigned_t>();

	auto tsName = jsonMat["techniqueSet->name"].get<nlohmann::json::string_t>();
	material->techniqueSet = findAssetFunction(Game::XAssetType::ASSET_TYPE_TECHNIQUE_SET, tsName.data()).techniqueSet;

	material->info.drawSurf = Game::GfxDrawSurf{ jsonMat["unknown"].get<nlohmann::json::number_unsigned_t>() };

	return material;

}

const char* iw4oa::AssetHandlers::Material::get_serialized_file_path(const char* assetName)
{
	return Utils::String::VA("%s/%s.json", get_serialized_base_path(), assetName);
}

const char* iw4oa::AssetHandlers::Material::get_serialized_base_path()
{
	return "materials";
}
