#include "pch.h"

void iw4oa::AssetHandlers::SoundAliasList::serialize(void* asset, const std::string& baseOutputPath)
{
	auto ents = reinterpret_cast<Game::snd_alias_list_t*>(asset);

	nlohmann::json::array_t head{}; 

	for (size_t i = 0; i < ents->count; i++) {

		Game::snd_alias_t alias = ents->head[i];

		nlohmann::json::array_t channelMaps{};

		for (size_t j = 0; j < 2; j++)
		{
			for (size_t k = 0; k < 2; k++)
			{
				auto channelMap = alias.speakerMap->channelMaps[j][k];
				nlohmann::json::array_t speakers{};

				for (size_t speakerIndex = 0; speakerIndex < channelMap.speakerCount; speakerIndex++)
				{
					auto speaker = channelMap.speakers[speakerIndex];
					speakers.emplace_back(nlohmann::json::object_t{
						{"levels0", speaker.numLevels > 0 ? speaker.levels[0] : 0 },
						{"levels1", speaker.numLevels > 1 ? speaker.levels[1] : 0 },
						{"numLevels", speaker.numLevels },
						{"speaker", speaker.speaker }
						});
				}

				channelMaps.emplace_back(nlohmann::json::object_t{
					{"entryCount", channelMap.speakerCount},
					{"speakers", speakers}
					});
			}
		}

		std::string soundFile("");
		auto type = 1;

		if (alias.soundFile)
		{
			type = alias.soundFile->type;

			switch (type)
			{
				// LOADED
			case Game::snd_alias_type_t::SAT_LOADED:
				// Save the LoadedSound subasset
				soundFile = alias.soundFile->u.loadSnd->name;
				break;

				// STREAMED
			case Game::snd_alias_type_t::SAT_STREAMED:
				{
					soundFile = alias.soundFile->u.streamSnd.filename.info.raw.name;

					if (alias.soundFile->u.streamSnd.filename.info.raw.dir)
					{
						soundFile = Utils::String::VA("%s/%s", alias.soundFile->u.streamSnd.filename.info.raw.dir, soundFile.c_str());
					}

					break;
				}
			}
		}

		auto jsonAlias = nlohmann::json::object_t{
			{"centerPercentage",alias.centerPercentage},
			{"distMax", alias.distMax},
			{"distMin", alias.distMin},
			{"envelopMax", alias.envelopMax},
			{"envelopMin", alias.envelopMin},
			{"envelopPercentage", alias.envelopPercentage},
			{"flags", alias.flags},
			{"lfePercentage", alias.lfePercentage},
			{"mixerGroup", nlohmann::json()},
			{"pitchMax", alias.pitchMax},
			{"pitchMin", alias.pitchMin},
			{"probability", alias.probability},
			{"sequence", alias.sequence},
			{"slavePercentage", alias.___u15.slavePercentage},
			{"speakerMap",  nlohmann::json::object_t{
				{"channelMaps", channelMaps},
				{"isDefault", alias.speakerMap->isDefault},
				{"name", alias.speakerMap->name}
			}},
			{"soundFile", soundFile.c_str()},
			{"startDelay", alias.startDelay},
			{"type", type},
			{"volMax", alias.volMax},
			{"volMin", alias.volMin},
			{"volumeFalloffCurve", alias.volumeFalloffCurve->filename}
		};

		WRITE_STR_IN_JSON(alias.aliasName, jsonAlias["aliasName"]);
		WRITE_STR_IN_JSON(alias.chainAliasName, jsonAlias["chainAliasName"]);
		WRITE_STR_IN_JSON(alias.secondaryAliasName, jsonAlias["secondaryAliasName"]);
		WRITE_STR_IN_JSON(alias.subtitle, jsonAlias["subtitle"]);

		head.emplace_back(jsonAlias);
	}

	nlohmann::json aliasList = nlohmann::json::object_t{
		{"aliasName", ents->aliasName},
		{"count", ents->count},
		{"head", head}
	};

	auto buffer = aliasList.dump(JSON_INDENT);

	auto outPath = Utils::String::VA("%s", baseOutputPath.c_str(), get_serialized_file_path(ents->aliasName));
	std::filesystem::create_directories(Utils::String::VA("%s/%s", baseOutputPath.c_str(), get_serialized_base_path()));

	std::ofstream destination(outPath, std::ios::binary);
	destination.write(buffer.data(), buffer.length());
}

void* iw4oa::AssetHandlers::SoundAliasList::deserialize(
	const char* assetName,
	const std::string& buffer,
	MemoryManager& memoryManager, 
	const std::function<Game::XAssetHeader (uint8_t type, const char* name)>& findAssetFunction
)
{
	Game::snd_alias_list_t* aliasList = memoryManager.Alloc<Game::snd_alias_list_t>();

	if (!aliasList)
	{
		throw DeserializationException("Error allocating memory for sound alias structure!\n");
	}

	if (buffer.empty())
	{
		throw MissingFileException();
	}

	nlohmann::json::object_t jsonAliasesObj = nlohmann::json::parse(buffer);
	nlohmann::json::array_t jsonAliases = jsonAliasesObj["head"];


	aliasList->count = jsonAliases.size();

	// Allocate

	aliasList->head = memoryManager.AllocArray<Game::snd_alias_t>(aliasList->count);
	if (!aliasList->head)
	{
		throw DeserializationException("Error allocating memory for sound alias structure!\n");
	}

	auto name = memoryManager.Duplicate(assetName);
	aliasList->aliasName = name;

	for (size_t i = 0; i < aliasList->count; i++)
	{
		nlohmann::json head = jsonAliases[i];

		if (!head.is_object())
		{
			throw DeserializationException("Failed to load sound %s!", name);
		}

		aliasList->head->soundFile = memoryManager.AllocArray<Game::SoundFile>(aliasList->count);
		if (!aliasList->head->soundFile)
		{
			throw DeserializationException("Error allocating memory for sound alias structure!\n");
		}

		Game::snd_alias_t* alias = aliasList->head;

		// try and parse everything and if it fails then fail for the whole file
		auto type = head["type"];
		auto subtitle = head["subtitle"];
		auto secondaryAliasName = head["secondaryAliasName"];
		auto chainAliasName = head["chainAliasName"];
		auto soundFile = head["soundFile"];
		auto sequence = head["sequence"];
		auto volMin = head["volMin"];
		auto volMax = head["volMax"];
		auto pitchMin = head["pitchMin"];
		auto pitchMax = head["pitchMax"];
		auto distMin = head["distMin"];
		auto distMax = head["distMax"];
		auto flags = head["flags"];
		auto slavePercentage = head["slavePercentage"];
		auto probability = head["probability"];
		auto lfePercentage = head["lfePercentage"];
		auto centerPercentage = head["centerPercentage"];
		auto startDelay = head["startDelay"];
		auto volumeFalloffCurve = head["volumeFalloffCurve"];
		auto envelopMin = head["envelopMin"];
		auto envelopMax = head["envelopMax"];
		auto envelopPercentage = head["envelopPercentage"];
		auto speakerMap = head["speakerMap"];
		auto aliasName = head["aliasName"];

		// Fix casing
		if (soundFile.is_null())
		{
			soundFile = head["soundfile"];

			//Components::Logger::Print("Fixed casing on %s\n", name.c_str());
		}

		if (type.is_null() || soundFile.is_null())
		{
			//Components::Logger::Print("Type is %s\n", type.dump().c_str());
			//Components::Logger::Print("SoundFile is %s\n", soundFile.dump().c_str());
			throw DeserializationException("Failed to parse sound %s! Each alias must have at least a type and a soundFile\n", name);
		}

#define CHECK(x, type) (x.is_##type##() || x.is_null())
#define CHECK_NUMBER(x)\
if (!CHECK(x, number))\
	throw DeserializationException("%s is not number but %d (%s)\n", "type", x.type(), x.dump().c_str());\

#define CHECK_STR(x)\
if (!CHECK(x, string))\
	throw DeserializationException("%s is not string but %d (%s)\n", "type", x.type(), x.dump().c_str());\

		// TODO: actually support all of those properties
		CHECK_NUMBER(type);
		CHECK_NUMBER(sequence);
		CHECK_NUMBER(volMin);
		CHECK_NUMBER(volMax);
		CHECK_STR(subtitle);
		CHECK_STR(aliasName);
		CHECK_STR(secondaryAliasName);
		CHECK_STR(chainAliasName);
		CHECK_STR(soundFile);
		CHECK_NUMBER(volMax);
		CHECK_NUMBER(volMin);
		CHECK_STR(soundFile);
		CHECK_NUMBER(pitchMin);
		CHECK_NUMBER(pitchMax);
		CHECK_NUMBER(probability);

		CHECK_NUMBER(probability);
		CHECK_NUMBER(lfePercentage);
		CHECK_NUMBER(centerPercentage);
		CHECK_NUMBER(startDelay);
		CHECK_STR(volumeFalloffCurve);
		CHECK_NUMBER(envelopMin);
		CHECK_NUMBER(envelopMax);
		CHECK_NUMBER(envelopPercentage);
		CHECK_NUMBER(probability);

		if (!CHECK(speakerMap, object))
		{
			throw DeserializationException("%s is not object but %d (%s)\n", "speakerMap", speakerMap.type(), speakerMap.dump().c_str());
		}

		alias->soundFile->exists = true;
		alias->aliasName = memoryManager.Duplicate(aliasName.get<nlohmann::json::string_t>().c_str());

		if (subtitle.is_string())
		{
			alias->subtitle = memoryManager.Duplicate(subtitle.get<nlohmann::json::string_t>().c_str());
		}
		if (secondaryAliasName.is_string())
		{
			alias->secondaryAliasName = memoryManager.Duplicate(secondaryAliasName.get<nlohmann::json::string_t>().c_str());
		}
		if (chainAliasName.is_string())
		{
			alias->chainAliasName = memoryManager.Duplicate(chainAliasName.get<nlohmann::json::string_t>().c_str());
		}

		alias->sequence = sequence.get<nlohmann::json::number_integer_t>();
		alias->volMin = volMin.get<nlohmann::json::number_float_t>();
		alias->volMax = volMax.get<nlohmann::json::number_float_t>();
		alias->pitchMin = pitchMin.get<nlohmann::json::number_float_t>();
		alias->pitchMax = pitchMax.get<nlohmann::json::number_float_t>();
		alias->distMin = distMin.get<nlohmann::json::number_float_t>();
		alias->distMax = distMax.get<nlohmann::json::number_float_t>();
		alias->flags = flags.get<nlohmann::json::number_integer_t>();
		alias->___u15.slavePercentage = slavePercentage.get<nlohmann::json::number_float_t>();
		alias->probability = probability.get<nlohmann::json::number_float_t>();
		alias->lfePercentage = lfePercentage.get<nlohmann::json::number_float_t>();
		alias->centerPercentage = centerPercentage.get<nlohmann::json::number_float_t>();
		alias->startDelay = startDelay.get<nlohmann::json::number_integer_t>();
		alias->envelopMin = envelopMin.get<nlohmann::json::number_float_t>();
		alias->envelopMax = envelopMax.get<nlohmann::json::number_float_t>();
		alias->envelopPercentage = envelopPercentage.get<nlohmann::json::number_float_t>();

		// Speaker map object
		if (!speakerMap.is_null())
		{
			alias->speakerMap = memoryManager.Alloc<Game::SpeakerMap>();
			if (!alias->speakerMap)
			{
				throw DeserializationException("Error allocating memory for speakermap in sound alias%s!\n", alias->aliasName);
			}

			alias->speakerMap->name = memoryManager.Duplicate(speakerMap["name"].get<nlohmann::json::string_t>().c_str());
			alias->speakerMap->isDefault = speakerMap["isDefault"].get<nlohmann::json::boolean_t>();

			if (speakerMap["channelMaps"].is_array())
			{
				nlohmann::json::array_t channelMaps = speakerMap["channelMaps"];

				assert(channelMaps.size() <= 4);

				// channelMapIndex should never exceed 1
				for (size_t channelMapIndex = 0; channelMapIndex < 2; channelMapIndex++)
				{
					// subChannelIndex should never exceed 1
					for (size_t subChannelIndex = 0; subChannelIndex < 2; subChannelIndex++)
					{
						nlohmann::json channelMap = channelMaps[channelMapIndex * 2 + subChannelIndex]; // 0-3

						nlohmann::json::array_t  speakers = channelMap["speakers"];

						alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakerCount = speakers.size();

						for (size_t speakerIndex = 0; speakerIndex < alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakerCount; speakerIndex++)
						{
							auto speaker = speakers[speakerIndex];
							alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].levels[0] = speaker["levels0"].get<nlohmann::json::number_float_t>();
							alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].levels[1] = speaker["levels1"].get<nlohmann::json::number_float_t>();
							alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].numLevels = speaker["numLevels"].get<nlohmann::json::number_integer_t>();
							alias->speakerMap->channelMaps[channelMapIndex][subChannelIndex].speakers[speakerIndex].speaker = speaker["speaker"].get<nlohmann::json::number_integer_t>();
						}
					}
				}
			}
			}

			if (volumeFalloffCurve.is_string())
			{
				std::string fallOffCurve = volumeFalloffCurve.get<nlohmann::json::string_t>();

				if (fallOffCurve.size() == 0)
				{
					fallOffCurve = "$default";
				}

				if (findAssetFunction) {
					auto curve = findAssetFunction(
						Game::XAssetType::ASSET_TYPE_SOUND_CURVE,
						fallOffCurve.c_str()
					).sndCurve;

					alias->volumeFalloffCurve = curve;
				}
			}

			if (static_cast<Game::snd_alias_type_t>(type.get<nlohmann::json::number_unsigned_t>()) == Game::snd_alias_type_t::SAT_LOADED) // Loaded
			{
				alias->soundFile->type = Game::SAT_LOADED;

				if (findAssetFunction) {
					alias->soundFile->u.loadSnd = findAssetFunction(Game::XAssetType::ASSET_TYPE_LOADED_SOUND, soundFile.get<nlohmann::json::string_t>().c_str()).loadSnd;
				}
			}
			else if (static_cast<Game::snd_alias_type_t>(type.get<nlohmann::json::number_unsigned_t>()) == Game::snd_alias_type_t::SAT_STREAMED) // Streamed 
			{
				alias->soundFile->type = Game::SAT_STREAMED;

				std::string streamedFile = soundFile.get<nlohmann::json::string_t>();
				std::string directory = "";
				int split = streamedFile.find_last_of('/');

				if (split >= 0)
				{
					directory = streamedFile.substr(0, split);
					streamedFile = streamedFile.substr(split + 1);
				}

				alias->soundFile->u.streamSnd.filename.info.raw.dir = memoryManager.Duplicate(directory.c_str());
				alias->soundFile->u.streamSnd.filename.info.raw.name = memoryManager.Duplicate(streamedFile.c_str());
			}
			else
			{
				throw DeserializationException("Failed to parse sound %s! Invalid sound type %s\n", assetName, type.get<nlohmann::json::string_t>().c_str());
			}

			aliasList->head[i] = *alias;
		}

#undef CHECK
#undef CHECK_NUMBER
#undef CHECK_STR

		return aliasList;
	}

	const char* iw4oa::AssetHandlers::SoundAliasList::get_serialized_file_path(const char* assetName)
	{
		return Utils::String::VA("%s/%s.json", get_serialized_base_path(), assetName);
	}

	const char* iw4oa::AssetHandlers::SoundAliasList::get_serialized_base_path()
	{
		return Utils::String::VA("sounds");
	}
