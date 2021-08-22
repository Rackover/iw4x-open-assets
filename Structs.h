#pragma once

namespace iw4oa {
	namespace Game {

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
			snd_alias_list_t* sound;
			SndCurve* sndCurve;
			LoadedSound* loadSnd;
			void* data;
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

	}
}