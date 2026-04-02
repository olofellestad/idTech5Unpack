//--------------------------------------------------------------------------------------------------------------------------------
//  precompiled.h
//--------------------------------------------------------------------------------------------------------------------------------
#pragma once

// clang-format off
#include <initializer_list>
#include <utility>
#include <type_traits>
#include <cstring>

#include "lib/sys/defines.h"

#ifdef SYSTEM_WIN32
#    define WIN32_LEAN_AND_MEAN
#    include <Windows.h>
#endif

#include "lib/sys/system.h"
#include "lib/mem/memory.h"

#include "lib/math/hash.h"
#include "lib/va.h"
#include "lib/containers/search.h"
#include "lib/containers/sort.h"
#include "lib/containers/span.h"
#include "lib/containers/linklist.h"
#include "lib/containers/list.h"
#include "lib/containers/flatmap.h"
#include "lib/containers/hashmap.h"
#include "lib/containers/ref.h"
#include "lib/containers/str.h"
#include "lib/containers/staticlist.h"
#include "lib/containers/staticstr.h"
#include "lib/math/math.h"
#include "lib/math/rand.h"
// clang-format on
