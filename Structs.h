#pragma once

namespace iw4oa {
	namespace Game {

		////////////////////////////////////////////////
		//
		// Material


		struct __declspec(align(4)) GfxImageLoadDef
		{
			char levelCount;
			char pad[3];
			int flags;
			int format;
			int resourceSize;
			char data[1];
		};

		union GfxTexture
		{
			GfxImageLoadDef* loadDef;
		};

		struct Picmip
		{
			char platform[2];
		};

		struct CardMemory
		{
			int platform[2];
		};

		struct GfxImage
		{
			GfxTexture texture;
			char mapType;
			char semantic;
			char category;
			bool useSrgbReads;
			Picmip picmip;
			bool noPicmip;
			char track;
			CardMemory cardMemory;
			unsigned __int16 width;
			unsigned __int16 height;
			unsigned __int16 depth;
			bool delayLoadPixels;
			const char* name;
		};

		struct GfxDrawSurfFields
		{
			unsigned __int64 objectId : 16;
			unsigned __int64 reflectionProbeIndex : 8;
			unsigned __int64 hasGfxEntIndex : 1;
			unsigned __int64 customIndex : 5;
			unsigned __int64 materialSortedIndex : 12;
			unsigned __int64 prepass : 2;
			unsigned __int64 useHeroLighting : 1;
			unsigned __int64 sceneLightIndex : 8;
			unsigned __int64 surfType : 4;
			unsigned __int64 primarySortKey : 6;
			unsigned __int64 unused : 1;
		};

		union GfxDrawSurf
		{
			GfxDrawSurfFields fields;
			unsigned __int64 packed;
		};

		struct MaterialInfo
		{
			const char* name;
			char gameFlags;
			char sortKey;
			char textureAtlasRowCount;
			char textureAtlasColumnCount;
			GfxDrawSurf drawSurf;
			unsigned int surfaceTypeBits;
			unsigned __int16 hashIndex;
		};


		struct MaterialStreamRouting
		{
			char source;
			char dest;
		};

		struct MaterialVertexStreamRouting
		{
			MaterialStreamRouting data[13];
			/*IDirect3DVertexDeclaration9*/void* decl[16];
		};

		struct MaterialVertexDeclaration
		{
			const char* name;
			char streamCount;
			bool hasOptionalSource;
			MaterialVertexStreamRouting routing;
		};

		struct GfxVertexShaderLoadDef
		{
			unsigned int* program;
			unsigned __int16 programSize;
			unsigned __int16 loadForRenderer;
		};

		struct MaterialVertexShaderProgram
		{
			/*IDirect3DVertexShader9*/void* vs;
			GfxVertexShaderLoadDef loadDef;
		};

		struct MaterialVertexShader
		{
			const char* name;
			MaterialVertexShaderProgram prog;
		};

		struct GfxPixelShaderLoadDef
		{
			unsigned int* program;
			unsigned __int16 programSize;
			unsigned __int16 loadForRenderer;
		};

		struct MaterialPixelShaderProgram
		{
			/*IDirect3DPixelShader9*/void* ps;
			GfxPixelShaderLoadDef loadDef;
		};

		struct MaterialPixelShader
		{
			const char* name;
			MaterialPixelShaderProgram prog;
		};

		struct MaterialArgumentCodeConst
		{
			unsigned __int16 index;
			char firstRow;
			char rowCount;
		};

		union MaterialArgumentDef
		{
			const float* literalConst;
			MaterialArgumentCodeConst codeConst;
			unsigned int codeSampler;
			unsigned int nameHash;
		};

		struct MaterialShaderArgument
		{
			unsigned __int16 type;
			unsigned __int16 dest;
			MaterialArgumentDef u;
		};

		struct MaterialPass
		{
			MaterialVertexDeclaration* vertexDecl;
			MaterialVertexShader* vertexShader;
			MaterialPixelShader* pixelShader;
			char perPrimArgCount;
			char perObjArgCount;
			char stableArgCount;
			char customSamplerFlags;
			MaterialShaderArgument* args;
		};

		struct MaterialTechnique
		{
			const char* name;
			unsigned __int16 flags;
			unsigned __int16 passCount;
			MaterialPass passArray[1];
		};

		struct MaterialTechniqueSet
		{
			const char* name;
			char worldVertFormat;
			bool hasBeenUploaded;
			char unused[1];
			MaterialTechniqueSet* remappedTechniqueSet;
			MaterialTechnique* techniques[48];
		};

		struct WaterWritable
		{
			float floatTime;
		};

		struct complex_s
		{
			float real;
			float imag;
		};

		struct water_t
		{
			WaterWritable writable;
			complex_s* H0;
			float* wTerm;
			int M;
			int N;
			float Lx;
			float Lz;
			float gravity;
			float windvel;
			float winddir[2];
			float amplitude;
			float codeConstant[4];
			GfxImage* image;
		};

		union MaterialTextureDefInfo
		{
			GfxImage* image;
			water_t* water;
		};

		struct MaterialTextureDef
		{
			unsigned int nameHash;
			char nameStart;
			char nameEnd;
			char samplerState;
			char semantic;
			MaterialTextureDefInfo u;
		};

		struct MaterialConstantDef
		{
			unsigned int nameHash;
			char name[12];
			float literal[4];
		};

		struct GfxStateBits
		{
			unsigned int loadBits[2];
		};

		struct Material
		{
			MaterialInfo info;
			char stateBitsEntry[48];
			char textureCount;
			char constantCount;
			char stateBitsCount;
			char stateFlags;
			char cameraRegion;
			MaterialTechniqueSet* techniqueSet;
			MaterialTextureDef* textureTable;
			MaterialConstantDef* constantTable;
			GfxStateBits* stateBitsTable;
		};

		////////////////////////////////////////////////
		//
		// Sound
		struct _AILSOUNDINFO
		{
			int format;
			const void* data_ptr;
			unsigned int data_len;
			unsigned int rate;
			int bits;
			int channels;
			unsigned int samples;
			unsigned int block_size;
			const void* initial_ptr;
		};

		struct MssSound
		{
			_AILSOUNDINFO info;
			char* data;
		};

		struct LoadedSound
		{
			const char* name;
			MssSound sound;
		};

		struct StreamFileNameRaw
		{
			const char* dir;
			const char* name;
		};

		union StreamFileInfo
		{
			StreamFileNameRaw raw;
		};

		struct StreamFileName
		{
			StreamFileInfo info;
		};

		struct StreamedSound
		{
			StreamFileName filename;
		};

		union SoundFileRef
		{
			LoadedSound* loadSnd;
			StreamedSound streamSnd;
		};

		enum snd_alias_type_t
		{
			SAT_UNKNOWN = 0x0,
			SAT_LOADED = 0x1,
			SAT_STREAMED = 0x2,
			SAT_VOICED = 0x3,
			SAT_COUNT = 0x4,
		};

		struct SoundFile
		{
			char type;
			char exists;
			SoundFileRef u;
		};

		union $C8D87EB0090687D323381DFB7A82089C
		{
			float slavePercentage;
			float masterPercentage;
		};

		struct SndCurve
		{
			const char* filename;
			unsigned __int16 knotCount;
			float knots[16][2];
		};

		struct MSSSpeakerLevels
		{
			int speaker;
			int numLevels;
			float levels[2];
		};

		struct MSSChannelMap
		{
			unsigned int speakerCount;
			MSSSpeakerLevels speakers[6];
		};

		struct SpeakerMap
		{
			bool isDefault;
			const char* name;
			MSSChannelMap channelMaps[2][2];
		};

		const struct snd_alias_t
		{
			const char* aliasName;
			const char* subtitle;
			const char* secondaryAliasName;
			const char* chainAliasName;
			const char* mixerGroup;
			SoundFile* soundFile;
			int sequence;
			float volMin;
			float volMax;
			float pitchMin;
			float pitchMax;
			float distMin;
			float distMax;
			float velocityMin;
			int flags;
			$C8D87EB0090687D323381DFB7A82089C ___u15;
			float probability;
			float lfePercentage;
			float centerPercentage;
			int startDelay;
			SndCurve* volumeFalloffCurve;
			float envelopMin;
			float envelopMax;
			float envelopPercentage;
			SpeakerMap* speakerMap;
		};

		struct snd_alias_list_t
		{
			const char* aliasName;
			snd_alias_t* head;
			unsigned int count;
		};

		////////////////////////////////////////////
		// Models

		struct DObjAnimMat
		{
			float quat[4];
			float trans[3];
			float transWeight;
		};

		struct __declspec(align(4)) PhysPreset
		{
			const char* name;
			int type;
			float mass;
			float bounce;
			float friction;
			float bulletForceScale;
			float explosiveForceScale;
			const char* sndAliasPrefix;
			float piecesSpreadFraction;
			float piecesUpwardVelocity;
			bool tempDefaultToCylinder;
			bool perSurfaceSndAlias;
		};

		struct Bounds
		{
			float midPoint[3];
			float halfSize[3];
		};

		struct cplane_s
		{
			float normal[3];
			float dist;
			char type;
			char pad[3];
		};

		struct cbrushside_t
		{
			cplane_s* plane;
			unsigned __int16 materialNum;
			char firstAdjacentSideOffset;
			char edgeCount;
		};

		struct cbrush_t
		{
			unsigned __int16 numsides;
			unsigned __int16 glassPieceIndex;
			cbrushside_t* sides;
			char* baseAdjacentSide;
			__int16 axialMaterialNum[2][3];
			char firstAdjacentSideOffsets[2][3];
			char edgeCount[2][3];
		};

		struct BrushWrapper
		{
			Bounds bounds;
			cbrush_t brush;
			int totalEdgeCount;
			cplane_s* planes;
		};

		struct PhysGeomInfo
		{
			BrushWrapper* brushWrapper;
			int type;
			float orientation[3][3];
			Bounds bounds;
		};

		struct PhysMass
		{
			float centerOfMass[3];
			float momentsOfInertia[3];
			float productsOfInertia[3];
		};

		struct PhysCollmap
		{
			const char* name;
			unsigned int count;
			PhysGeomInfo* geoms;
			PhysMass mass;
			Bounds bounds;
		};

		union XAnimIndices
		{
			char* _1;
			unsigned __int16* _2;
			void* data;
		};

		struct XAnimNotifyInfo
		{
			unsigned __int16 name;
			float time;
		};

		union XAnimDynamicFrames
		{
			char(*_1)[3];
			unsigned __int16(*_2)[3];
		};

		union XAnimDynamicIndices
		{
			char _1[1];
			unsigned __int16 _2[1];
		};

		struct __declspec(align(4)) XAnimPartTransFrames
		{
			float mins[3];
			float size[3];
			XAnimDynamicFrames frames;
			XAnimDynamicIndices indices;
		};

		union XAnimPartTransData
		{
			XAnimPartTransFrames frames;
			float frame0[3];
		};

		struct XAnimPartTrans
		{
			unsigned __int16 size;
			char smallTrans;
			XAnimPartTransData u;
		};

		struct __declspec(align(4)) XAnimDeltaPartQuatDataFrames2
		{
			__int16(*frames)[2];
			XAnimDynamicIndices indices;
		};

		union XAnimDeltaPartQuatData2
		{
			XAnimDeltaPartQuatDataFrames2 frames;
			__int16 frame0[2];
		};

		struct XAnimDeltaPartQuat2
		{
			unsigned __int16 size;
			XAnimDeltaPartQuatData2 u;
		};

		struct __declspec(align(4)) XAnimDeltaPartQuatDataFrames
		{
			__int16(*frames)[4];
			XAnimDynamicIndices indices;
		};

		union XAnimDeltaPartQuatData
		{
			XAnimDeltaPartQuatDataFrames frames;
			__int16 frame0[4];
		};

		struct XAnimDeltaPartQuat
		{
			unsigned __int16 size;
			XAnimDeltaPartQuatData u;
		};

		struct XAnimDeltaPart
		{
			XAnimPartTrans* trans;
			XAnimDeltaPartQuat2* quat2;
			XAnimDeltaPartQuat* quat;
		};

		struct XAnimParts
		{
			const char* name;
			unsigned __int16 dataByteCount;
			unsigned __int16 dataShortCount;
			unsigned __int16 dataIntCount;
			unsigned __int16 randomDataByteCount;
			unsigned __int16 randomDataIntCount;
			unsigned __int16 numframes;
			char flags;
			char boneCount[10];
			char notifyCount;
			char assetType;
			bool isDefault;
			unsigned int randomDataShortCount;
			unsigned int indexCount;
			float framerate;
			float frequency;
			unsigned __int16* names;
			char* dataByte;
			__int16* dataShort;
			int* dataInt;
			__int16* randomDataShort;
			char* randomDataByte;
			int* randomDataInt;
			XAnimIndices indices;
			XAnimNotifyInfo* notify;
			XAnimDeltaPart* deltaPart;
		};

		struct XSurfaceVertexInfo
		{
			__int16 vertCount[4];
			unsigned __int16* vertsBlend;
		};

		union GfxColor
		{
			unsigned int packed;
			char array[4];
		};

		union PackedTexCoords
		{
			unsigned int packed;
		};

		union PackedUnitVec
		{
			unsigned int packed;
			char array[4];
		};

		struct GfxPackedVertex
		{
			float xyz[3];
			float binormalSign;
			GfxColor color;
			PackedTexCoords texCoord;
			PackedUnitVec normal;
			PackedUnitVec tangent;
		};

		struct XSurfaceCollisionAabb
		{
			unsigned __int16 mins[3];
			unsigned __int16 maxs[3];
		};

		struct XSurfaceCollisionNode
		{
			XSurfaceCollisionAabb aabb;
			unsigned __int16 childBeginIndex;
			unsigned __int16 childCount;
		};

		struct XSurfaceCollisionLeaf
		{
			unsigned __int16 triangleBeginIndex;
		};

		struct XSurfaceCollisionTree
		{
			float trans[3];
			float scale[3];
			unsigned int nodeCount;
			XSurfaceCollisionNode* nodes;
			unsigned int leafCount;
			XSurfaceCollisionLeaf* leafs;
		};

		struct XRigidVertList
		{
			unsigned __int16 boneOffset;
			unsigned __int16 vertCount;
			unsigned __int16 triOffset;
			unsigned __int16 triCount;
			XSurfaceCollisionTree* collisionTree;
		};

		struct XSurface
		{
			char tileMode;
			bool deformed;
			unsigned __int16 vertCount;
			unsigned __int16 triCount;
			char zoneHandle;
			unsigned __int16 baseTriIndex;
			unsigned __int16 baseVertIndex;
			unsigned __int16* triIndices;
			XSurfaceVertexInfo vertInfo;
			GfxPackedVertex* verts0;
			unsigned int vertListCount;
			XRigidVertList* vertList;
			int partBits[6];
		};

		struct XModelSurfs
		{
			const char* name;
			XSurface* surfs;
			unsigned __int16 numsurfs;
			int partBits[6];
		};

		struct XModelLodInfo
		{
			float dist;
			unsigned __int16 numsurfs;
			unsigned __int16 surfIndex;
			XModelSurfs* modelSurfs;
			int partBits[6];
			XSurface* surfs;
			char lod;
			char smcBaseIndexPlusOne;
			char smcSubIndexMask;
			char smcBucket;
		};

		struct XModelCollTri_s
		{
			float plane[4];
			float svec[4];
			float tvec[4];
		};

		struct XModelCollSurf_s
		{
			XModelCollTri_s* collTris;
			int numCollTris;
			Bounds bounds;
			int boneIdx;
			int contents;
			int surfFlags;
		};

		struct XBoneInfo
		{
			Bounds bounds;
			float radiusSquared;
		};

		struct XModel
		{
			const char* name;
			char numBones;
			char numRootBones;
			unsigned char numsurfs;
			char lodRampType;
			float scale;
			unsigned int noScalePartBits[6];
			unsigned __int16* boneNames;
			char* parentList;
			__int16* quats;
			float* trans;
			char* partClassification;
			DObjAnimMat* baseMat;
			Material** materialHandles;
			XModelLodInfo lodInfo[4];
			char maxLoadedLod;
			char numLods;
			char collLod;
			char flags;
			XModelCollSurf_s* collSurfs;
			int numCollSurfs;
			int contents;
			XBoneInfo* boneInfo;
			float radius;
			Bounds bounds;
			int memUsage;
			bool bad;
			PhysPreset* physPreset;
			PhysCollmap* physCollmap;
		};

		// XAsset
		enum XAssetType : uint8_t
		{
			ASSET_TYPE_PHYSPRESET = 0x0,
			ASSET_TYPE_PHYSCOLLMAP = 0x1,
			ASSET_TYPE_XANIMPARTS = 0x2,
			ASSET_TYPE_XMODEL_SURFS = 0x3,
			ASSET_TYPE_XMODEL = 0x4,
			ASSET_TYPE_MATERIAL = 0x5,
			ASSET_TYPE_PIXELSHADER = 0x6,
			ASSET_TYPE_VERTEXSHADER = 0x7,
			ASSET_TYPE_VERTEXDECL = 0x8,
			ASSET_TYPE_TECHNIQUE_SET = 0x9,
			ASSET_TYPE_IMAGE = 0xA,
			ASSET_TYPE_SOUND = 0xB,
			ASSET_TYPE_SOUND_CURVE = 0xC,
			ASSET_TYPE_LOADED_SOUND = 0xD,
			ASSET_TYPE_CLIPMAP_SP = 0xE,
			ASSET_TYPE_CLIPMAP_MP = 0xF,
			ASSET_TYPE_COMWORLD = 0x10,
			ASSET_TYPE_GAMEWORLD_SP = 0x11,
			ASSET_TYPE_GAMEWORLD_MP = 0x12,
			ASSET_TYPE_MAP_ENTS = 0x13,
			ASSET_TYPE_FXWORLD = 0x14,
			ASSET_TYPE_GFXWORLD = 0x15,
			ASSET_TYPE_LIGHT_DEF = 0x16,
			ASSET_TYPE_UI_MAP = 0x17,
			ASSET_TYPE_FONT = 0x18,
			ASSET_TYPE_MENULIST = 0x19,
			ASSET_TYPE_MENU = 0x1A,
			ASSET_TYPE_LOCALIZE_ENTRY = 0x1B,
			ASSET_TYPE_WEAPON = 0x1C,
			ASSET_TYPE_SNDDRIVER_GLOBALS = 0x1D,
			ASSET_TYPE_FX = 0x1E,
			ASSET_TYPE_IMPACT_FX = 0x1F,
			ASSET_TYPE_AITYPE = 0x20,
			ASSET_TYPE_MPTYPE = 0x21,
			ASSET_TYPE_CHARACTER = 0x22,
			ASSET_TYPE_XMODELALIAS = 0x23,
			ASSET_TYPE_RAWFILE = 0x24,
			ASSET_TYPE_STRINGTABLE = 0x25,
			ASSET_TYPE_LEADERBOARD = 0x26,
			ASSET_TYPE_STRUCTURED_DATA_DEF = 0x27,
			ASSET_TYPE_TRACER = 0x28,
			ASSET_TYPE_VEHICLE = 0x29,
			ASSET_TYPE_ADDON_MAP_ENTS = 0x2A,
			ASSET_TYPE_COUNT = 0x2B,
			ASSET_TYPE_STRING = 0x2B,
			ASSET_TYPE_ASSETLIST = 0x2C,
		};

		union XAssetHeader
		{
			MaterialInfo* material;
			snd_alias_list_t* sound;
			SndCurve* sndCurve;
			LoadedSound* loadSnd;
			GfxImage* image;
			MaterialTechniqueSet* techniqueSet;

			PhysPreset* physPreset;

			void* data;
		};
	}
}