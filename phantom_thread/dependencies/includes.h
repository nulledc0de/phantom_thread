#pragma once

#include <ntddk.h>
#include <ntdef.h>
#include <minwindef.h>
#include <ntimage.h>
#include <cstdint>

#define DBGLOG(...) DbgPrint(__VA_ARGS__);

#include "extern/extern.h"
#include "utils/utils.h"