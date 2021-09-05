#pragma once

namespace iw4oa {
	class API {

	public:
		class MissingAssetHandlerException : public std::exception {
			public:
				MissingAssetHandlerException(const char* reason) {
					this->reason = reason;
				}

				MissingAssetHandlerException(const char* fmt, const char* args, ...) {
					this->reason = iw4oa::Utils::String::VA(fmt, args);
				}

				std::string to_string() const throw()
				{
					return reason;
				}

			private:
				const char* reason;
			};

		static IAssetHandler* getHandlerForType(uint8_t uiType);

		static std::function<char* (unsigned short key)> getStringFromKeyFunc;
		static std::function<unsigned short(char* string)> storeStringFunc;

	private:

		static std::map<uint8_t, IAssetHandler*> instantiatedHandlers;
	};
}