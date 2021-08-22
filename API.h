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

				std::string toString() const throw()
				{
					return reason;
				}

			private:
				const char* reason;
			};

		static IAssetHandler* get_handler_for_type(uint8_t uiType);

	private:

		static std::map<uint8_t, IAssetHandler*> instantiatedHandlers;
	};
}