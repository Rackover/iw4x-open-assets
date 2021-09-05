#pragma once

#define JSON_INDENT 4


#define WRITE_STR_IN_JSON(instance_member, json_member) if (instance_member) json_member = instance_member; else json_member = nullptr; 


namespace iw4oa 
{
	class IAssetHandler {

	public:
		class MissingFileException : public std::exception {};

		class DeserializationException : public std::exception{
			public:
				DeserializationException(const char* reason) {
					this->reason = reason;
				}

				DeserializationException(const char* fmt, const char* args, ...) {
					this->reason = iw4oa::Utils::String::VA(fmt, args);
				}

				std::string toString() const throw()
				{
					return reason;
				}

			private:
				const char* reason;
		};
		
		/// <summary>
		/// Takes a pointer to an asset, and a file output path and serializes the asset to the disk, in a format that can then be deserialized.
		/// </summary>
		/// <param name="asset">Pointer to an XAsset member (for example, a Game::snd_alias_list_t*)</param>
		/// <param name="baseZonePath">The base path where you want to dump your files (for example: /raw/your_zone_name)</param>
		/// <returns></returns>
		virtual void serialize(void* asset, const std::string& baseZonePath) = 0;

		/// <summary>
		/// Reads a serialized file to disk and makes a proper asset* from it.
		///  you hav
		/// </summary>
		/// <param name="baseZonePath">An asset name (example: ambient_mp_storm). NO EXTENSIONS!</param>
		/// <param name="assetName">A base zone path (for example: /raw/your_zone_name)</param>
		/// <param name="memoryManager">A memory manager instance (create it on the heap before deserializing, no worries)</param>
		/// <param name="findAssetFunction">Facultative but recommended: a lambda that can return other assets this asset might be referencing.</param>
		/// <returns>Pointer to the newly created asset (for example, a Game::snd_alias_list_t*)</returns>
		virtual void* deserialize(
			const char* assetName,
			const std::string& buffer, 
			Utils::MemoryManager &memoryManager,
			const std::function<Game::XAssetHeader(uint8_t type, const char* name)> &findAssetFunction = nullptr
		) = 0;

		/// <summary>
		/// Formats an asset name to an expected file path to use with the serializer (example: abc => sounds/abc.json)
		/// </summary>
		/// <param name="assetName">Name of the asset (asset->name, for most assets)</param>
		/// <returns></returns>
		virtual const char* getSerializedFilePath(const char* assetName) = 0;

		/// <summary>
		/// Returns the base folder name in which this asset should be dumped (example, for a Material => materials)
		/// </summary>
		/// <returns></returns>
		virtual const char* geSerializedBasePath() = 0;
	};

}