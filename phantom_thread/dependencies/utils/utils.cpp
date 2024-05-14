#include "../includes.h"

PVOID utils::get_kernel_base() {
	ULONG modules_size = 0;
	PVOID kernel_base = 0;

	ZwQuerySystemInformation(SystemModuleInformation, 0, modules_size, &modules_size);

	PRTL_PROCESS_MODULES modules = reinterpret_cast<PRTL_PROCESS_MODULES>(ExAllocatePool(NonPagedPool, modules_size));

	if (!modules)
		return kernel_base;

	RtlSecureZeroMemory(modules, modules_size);

	if (!NT_SUCCESS(ZwQuerySystemInformation(SystemModuleInformation, modules, modules_size, &modules_size))) {
		ExFreePool(modules);
		return kernel_base;
	}

	kernel_base = modules->Modules[0].ImageBase;

	ExFreePool(modules);

	return kernel_base;
}

// shii tbh idr where this sig scanning code is from but s/o whoever i stole it from :p
BOOL utils::correct_mask(PCHAR base, PCHAR pattern, PCHAR mask) {
	for (; *mask; ++base, ++pattern, ++mask)
		if (*mask == 'x' && *base != *pattern)
			return FALSE;

	return TRUE;
}

PVOID utils::find_pattern(PCHAR base, DWORD length, PCHAR pattern, PCHAR mask) {
	length -= (DWORD)strlen(mask);

	for (DWORD i = 0; i <= length; ++i) {
		PVOID addr = &base[i];

		if (correct_mask(reinterpret_cast<PCHAR>(addr), pattern, mask))
			return addr;
	}

	return 0;
}

PVOID utils::find_pattern_image(PCHAR base, PCHAR pattern, PCHAR mask) {
	PVOID return_address = 0;

	PIMAGE_NT_HEADERS headers = (PIMAGE_NT_HEADERS)(base + ((PIMAGE_DOS_HEADER)base)->e_lfanew);
	PIMAGE_SECTION_HEADER sections = IMAGE_FIRST_SECTION(headers);

	for (DWORD i = 0; i < headers->FileHeader.NumberOfSections; ++i) {
		PIMAGE_SECTION_HEADER section = &sections[i];

		if (*(PINT)section->Name == 'EGAP' || memcmp(section->Name, ".text", 5) == 0) {
			return_address = find_pattern(base + section->VirtualAddress, section->Misc.VirtualSize, pattern, mask);

			if (return_address)
				break;
		}
	}

	return return_address;
}

// https://github.com/vRare/AutoSpitta-x64/blob/5f5a3c5306f0606190c10a9b3743278c6b14932c/hacks.c#L9
void utils::sleep(ULONGLONG ms) {
	LARGE_INTEGER delay;
	ULONG* split;

	ms *= 1000000;

	ms /= 100;

	ms = -ms;

	split = (ULONG*)&ms;

	delay.LowPart = *split;

	split++;

	delay.HighPart = *split;

	KeDelayExecutionThread(KernelMode, 0, &delay);
}