// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <windows.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream> 
#include <iostream>

#include "Structs.h"
#include "lib/nlohmann/json.hpp"

#include "Utils.h"
#include "MemoryManager.h"
#include "IAssetHandler.h"

#include "API.h"
#include "SoundAliasList.h"

#endif //PCH_H
