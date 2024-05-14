#pragma once

namespace utils {
	PVOID get_kernel_base();

	// shii tbh idr where this sig scanning code is from but s/o whoever i stole it from :p
	BOOL  correct_mask(PCHAR base, PCHAR pattern, PCHAR mask);
	PVOID find_pattern(PCHAR base, DWORD length, PCHAR pattern, PCHAR mask);
	PVOID find_pattern_image(PCHAR base, PCHAR pattern, PCHAR mask);

	// https://github.com/vRare/AutoSpitta-x64/blob/5f5a3c5306f0606190c10a9b3743278c6b14932c/hacks.c#L9
	void sleep(ULONGLONG milliseconds);
}