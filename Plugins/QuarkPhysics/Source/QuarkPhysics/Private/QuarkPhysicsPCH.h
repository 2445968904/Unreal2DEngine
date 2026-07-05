#pragma once

// Precompiled header for QuarkPhysics UE module
// Includes UE core headers and suppresses warnings for third-party physics library

#include "CoreMinimal.h"

// Suppress strict UE warnings when compiling ported QuarkPhysics library code
PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4458 4456 4100 4701 4267 4244 4996 4005 4800 4018 4389 4702)
#endif
