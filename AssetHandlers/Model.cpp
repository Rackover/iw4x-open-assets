#include "pch.h"

void iw4oa::AssetHandlers::Model::saveXSurfaceCollisionTree(Game::XSurfaceCollisionTree* entry, Utils::Stream* buffer)
{
	buffer->saveObject(*entry);

	if (entry->nodes)
	{
		buffer->saveArray(entry->nodes, entry->nodeCount);
	}

	if (entry->leafs)
	{
		buffer->saveArray(entry->leafs, entry->leafCount);
	}
}

void iw4oa::AssetHandlers::Model::saveXSurface(Game::XSurface* surf, Utils::Stream* buffer)
{
	if (surf->vertInfo.vertsBlend)
	{
		buffer->saveArray(surf->vertInfo.vertsBlend, surf->vertInfo.vertCount[0] + (surf->vertInfo.vertCount[1] * 3) + (surf->vertInfo.vertCount[2] * 5) + (surf->vertInfo.vertCount[3] * 7));
	}

	// Access vertex block
	if (surf->verts0)
	{
		buffer->saveArray(surf->verts0, surf->vertCount);
	}

	// Save_XRigidVertListArray
	if (surf->vertList)
	{
		buffer->saveArray(surf->vertList, surf->vertListCount);

		for (unsigned int i = 0; i < surf->vertListCount; ++i)
		{
			Game::XRigidVertList* rigidVertList = &surf->vertList[i];

			if (rigidVertList->collisionTree)
			{
				Model::saveXSurfaceCollisionTree(rigidVertList->collisionTree, buffer);
			}
		}
	}

	// Access index block
	if (surf->triIndices)
	{
		buffer->saveArray(surf->triIndices, surf->triCount * 3);
	}
}

void iw4oa::AssetHandlers::Model::saveXModelSurfs(Game::XModelSurfs* asset, Utils::Stream* buffer)
{
	buffer->saveObject(*asset);

	if (asset->name)
	{
		buffer->saveString(asset->name);
	}

	if (asset->surfs)
	{
		buffer->saveArray(asset->surfs, asset->numsurfs);

		for (int i = 0; i < asset->numsurfs; ++i)
		{
			Model::saveXSurface(&asset->surfs[i], buffer);
		}
	}
}

void iw4oa::AssetHandlers::Model::serialize(void* asset, const std::string& baseOutputPath)
{
	Game::XModel* model = reinterpret_cast<Game::XModel*>(asset);


	Utils::Stream buffer;
	buffer.saveText("IW4xModl" + std::to_string(IW4X_MODEL_VERSION) + "\n");

	buffer.saveObject(*model);

	if (model->name)
	{
		buffer.saveString(model->name);
	}

	if (model->boneNames)
	{
		for (char i = 0; i < model->numBones; ++i)
		{
			if (iw4oa::API::getStringFromKeyFunc != nullptr) 
			{
				buffer.saveString(iw4oa::API::getStringFromKeyFunc(model->boneNames[i]));
			}
			else {
				buffer.saveString("unknown");
			}
		}
	}

	if (model->parentList)
	{
		buffer.saveArray(model->parentList, model->numBones - model->numRootBones);
	}

	if (model->quats)
	{
		buffer.saveArray(model->quats, (model->numBones - model->numRootBones) * 4);
	}

	if (model->trans)
	{
		buffer.saveArray(model->trans, (model->numBones - model->numRootBones) * 3);
	}

	if (model->partClassification)
	{
		buffer.saveArray(model->partClassification, model->numBones);
	}

	if (model->baseMat)
	{
		buffer.saveArray(model->baseMat, model->numBones);
	}

	if (model->materialHandles)
	{
		buffer.saveArray(model->materialHandles, model->numsurfs);

		for (unsigned char i = 0; i < model->numsurfs; ++i)
		{
			if (model->materialHandles[i])
			{
				buffer.saveString(model->materialHandles[i]->info.name);
			}
		}
	}

	// Save_XModelLodInfoArray
	{
		for (int i = 0; i < 4; ++i)
		{
			if (model->lodInfo[i].modelSurfs)
			{
				Model::saveXModelSurfs(model->lodInfo[i].modelSurfs, &buffer);
			}
		}
	}

	// Save_XModelCollSurfArray
	if (model->collSurfs)
	{
		buffer.saveArray(model->collSurfs, model->numCollSurfs);

		for (int i = 0; i < model->numCollSurfs; ++i)
		{
			Game::XModelCollSurf_s* collSurf = &model->collSurfs[i];

			if (collSurf->collTris)
			{
				buffer.saveArray(collSurf->collTris, collSurf->numCollTris);
			}
		}
	}

	if (model->boneInfo)
	{
		buffer.saveArray(model->boneInfo, model->numBones);
	}

	if (model->physPreset)
	{
		buffer.saveObject(*model->physPreset);

		if (model->physPreset->name)
		{
			buffer.saveString(model->physPreset->name);
		}

		if (model->physPreset->sndAliasPrefix)
		{
			buffer.saveString(model->physPreset->sndAliasPrefix);
		}
	}

	if (model->physCollmap)
	{
		Game::PhysCollmap* collmap = model->physCollmap;
		buffer.saveObject(*collmap);

		if (collmap->name)
		{
			buffer.saveString(collmap->name);
		}

		if (collmap->geoms)
		{
			buffer.saveArray(collmap->geoms, collmap->count);

			for (unsigned int i = 0; i < collmap->count; ++i)
			{
				Game::PhysGeomInfo* geom = &collmap->geoms[i];

				if (geom->brushWrapper)
				{
					Game::BrushWrapper* brush = geom->brushWrapper;
					buffer.saveObject(*brush);
					{
						if (brush->brush.sides)
						{
							buffer.saveArray(brush->brush.sides, brush->brush.numsides);

							// Save_cbrushside_tArray
							for (unsigned short j = 0; j < brush->brush.numsides; ++j)
							{
								Game::cbrushside_t* side = &brush->brush.sides[j];

								// TODO: Add pointer support
								if (side->plane)
								{
									buffer.saveObject(*side->plane);
								}
							}
						}

						if (brush->brush.baseAdjacentSide)
						{
							buffer.saveArray(brush->brush.baseAdjacentSide, brush->totalEdgeCount);
						}
					}

					// TODO: Add pointer support
					if (brush->planes)
					{
						buffer.saveArray(brush->planes, brush->brush.numsides);
					}
				}
			}
		}
	}

	std::string outPath = Utils::String::VA("%s/%s", baseOutputPath.c_str(), getSerializedFilePath(model->name));
	auto dir = outPath.substr(0, outPath.find_last_of("/\\"));
	std::filesystem::create_directories(dir);
	std::ofstream destination(outPath, std::ios::out | std::ios::binary);

	if (destination.is_open()) 
	{
		destination << buffer.data();
	}
}

void* iw4oa::AssetHandlers::Model::deserialize(
	const char* assetName,
	const std::string& buffer,
	Utils::MemoryManager& memoryManager,
	const std::function<Game::XAssetHeader(uint8_t type, const char* name)>& findAssetFunction
)
{
	Game::XModel* model = memoryManager.Alloc<Game::XModel>();

	if (!model)
	{
		throw DeserializationException("Error allocating memory for material structure!\n");
	}

	if (buffer.empty())
	{
		throw MissingFileException();
	}

	Utils::Stream::Reader reader(&memoryManager, buffer);

	__int64 magic = reader.read<__int64>();
	if (std::memcmp(&magic, "IW4xModl", 8))
	{
		throw new DeserializationException("Reading model '%s' failed, header is invalid!", assetName);
	}

	int version = reader.read<int>();
	if (version != IW4X_MODEL_VERSION)
	{
		throw new DeserializationException("Reading model '%s' failed, expected version is %d, but it was %d!", assetName, IW4X_MODEL_VERSION, version);
	}

	Game::XModel* asset = reader.readObject<Game::XModel>();

	if (asset->name)
	{
		asset->name = reader.readCString();
	}

	if (asset->boneNames)
	{
		asset->boneNames = memoryManager.AllocArray<unsigned short>(asset->numBones);

		if (API::storeStringFunc == nullptr) {
			throw new DeserializationException("No store string function (SL_GetString) provided to iw4oa, but is necessary to deserialize this type of asset.");
		}

		for (char i = 0; i < asset->numBones; ++i)
		{
			asset->boneNames[i] = API::storeStringFunc(reader.readCString());
		}
	}

	if (asset->parentList)
	{
		asset->parentList = reader.readArray<char>(asset->numBones - asset->numRootBones);
	}

	if (asset->quats)
	{
		asset->quats = reader.readArray<short>((asset->numBones - asset->numRootBones) * 4);
	}

	if (asset->trans)
	{
		asset->trans = reader.readArray<float>((asset->numBones - asset->numRootBones) * 3);
	}

	if (asset->partClassification)
	{
		asset->partClassification = reader.readArray<char>(asset->numBones);
	}

	if (asset->baseMat)
	{
		asset->baseMat = reader.readArray<Game::DObjAnimMat>(asset->numBones);
	}

	if (asset->materialHandles)
	{
		asset->materialHandles = reader.readArray<Game::Material*>(asset->numsurfs);

		for (unsigned char i = 0; i < asset->numsurfs; ++i)
		{
			if (asset->materialHandles[i])
			{
				asset->materialHandles[i] = Components::AssetHandler::FindAssetForZone(Game::XAssetType::ASSET_TYPE_MATERIAL, reader.readString(), builder).material;
			}
		}
	}

	// Save_XModelLodInfoArray
	{
		for (int i = 0; i < 4; ++i)
		{
			if (asset->lodInfo[i].modelSurfs)
			{
				asset->lodInfo[i].modelSurfs = reader.readObject<Game::XModelSurfs>();
				this->loadXModelSurfs(asset->lodInfo[i].modelSurfs, &reader, builder);
				Components::AssetHandler::StoreTemporaryAsset(Game::XAssetType::ASSET_TYPE_XMODEL_SURFS, { asset->lodInfo[i].modelSurfs });

				asset->lodInfo[i].surfs = asset->lodInfo[i].modelSurfs->surfs;

				// Zero that for now, it breaks the models.
				// TODO: Figure out how that can be converted
				asset->lodInfo[i].smcBaseIndexPlusOne = 0;
				asset->lodInfo[i].smcSubIndexMask = 0;
				asset->lodInfo[i].smcBucket = 0;
			}
		}
	}

	// Save_XModelCollSurfArray
	if (asset->collSurfs)
	{
		asset->collSurfs = reader.readArray<Game::XModelCollSurf_s>(asset->numCollSurfs);

		for (int i = 0; i < asset->numCollSurfs; ++i)
		{
			Game::XModelCollSurf_s* collSurf = &asset->collSurfs[i];

			if (collSurf->collTris)
			{
				collSurf->collTris = reader.readArray<Game::XModelCollTri_s>(collSurf->numCollTris);
			}
		}
	}

	if (asset->boneInfo)
	{
		asset->boneInfo = reader.readArray<Game::XBoneInfo>(asset->numBones);
	}

	if (asset->physPreset)
	{
		asset->physPreset = reader.readObject<Game::PhysPreset>();

		if (asset->physPreset->name)
		{
			asset->physPreset->name = reader.readCString();
		}

		if (asset->physPreset->sndAliasPrefix)
		{
			asset->physPreset->sndAliasPrefix = reader.readCString();
		}

		// This is an experiment, ak74 fails though
		if (std::strncmp(asset->name, "weapon_ak74u", strlen(asset->name)))
		{
			asset->physPreset = nullptr;
		}
		else
		{
			if (findAssetFunction) {
				Game::PhysPreset* preset = findAssetFunction(static_cast<uint8_t>(Game::XAssetType::ASSET_TYPE_PHYSPRESET), asset->physPreset->name).physPreset;
				if (preset)
				{
					asset->physPreset = preset;
				}
			}
			//else
			//{
			//	Components::AssetHandler::StoreTemporaryAsset(Game::XAssetType::ASSET_TYPE_PHYSPRESET, { asset->physPreset });
			//}
		}
	}

	if (asset->physCollmap)
	{
		if (version == 4)
		{
			asset->physCollmap = nullptr;
		}
		else
		{
			Game::PhysCollmap* collmap = reader.readObject<Game::PhysCollmap>();
			asset->physCollmap = collmap;

			if (collmap->name)
			{
				collmap->name = reader.readCString();
			}

			if (collmap->geoms)
			{
				collmap->geoms = reader.readArray<Game::PhysGeomInfo>(collmap->count);

				for (unsigned int i = 0; i < collmap->count; ++i)
				{
					Game::PhysGeomInfo* geom = &collmap->geoms[i];

					if (geom->brushWrapper)
					{
						Game::BrushWrapper* brush = reader.readObject<Game::BrushWrapper>();
						geom->brushWrapper = brush;
						{
							if (brush->brush.sides)
							{
								brush->brush.sides = reader.readArray<Game::cbrushside_t>(brush->brush.numsides);
								for (unsigned short j = 0; j < brush->brush.numsides; ++j)
								{
									Game::cbrushside_t* side = &brush->brush.sides[j];

									// TODO: Add pointer support
									if (side->plane)
									{
										side->plane = reader.readObject<Game::cplane_s>();
									}
								}
							}

							if (brush->brush.baseAdjacentSide)
							{
								brush->brush.baseAdjacentSide = reader.readArray<char>(brush->totalEdgeCount);
							}
						}

						// TODO: Add pointer support
						if (brush->planes)
						{
							brush->planes = reader.readArray<Game::cplane_s>(brush->brush.numsides);
						}
					}
				}
			}

			//Components::AssetHandler::StoreTemporaryAsset(Game::XAssetType::ASSET_TYPE_PHYSCOLLMAP, { asset->physCollmap });
			 asset->physCollmap = nullptr;
		}
	}

	if (!reader.end())
	{
		throw new DeserializationException("Reading model '%s' failed, remaining raw data found!", assetName);
	}



	return model;

}

const char* iw4oa::AssetHandlers::Model::getSerializedFilePath(const char* assetName)
{
	return Utils::String::VA("%s/%s.iw4oa.model", geSerializedBasePath(), assetName);
}

const char* iw4oa::AssetHandlers::Model::geSerializedBasePath()
{
	return "models";
}
