#include "../dependencies/includes.h"

void NTAPI test_thread(void*) {
	DBGLOG("[ * ] hi from test_thread!");

	const auto info = PsGetCurrentThread();

	PETHREAD_META_2009 p_thread_meta_data = ((PETHREAD_META_2009)((uintptr_t)info));

	DBGLOG("[ * ] checking thread info: [start_addr: %p]", p_thread_meta_data->StartAddress);

	DBGLOG("[ + ] entering guarded region to disable apc");
	KeEnterGuardedRegion();

	for (;;) {
		DBGLOG("[ * ] new_thread: i'm alive..");

		utils::sleep(5000);
	}
}

NTSTATUS driver_entry(const PMDL mdl, const uint64_t base, const uint64_t size) {
    DBGLOG("[ + ] hi from driver_entry");

	const auto kernel_base = utils::get_kernel_base();

	if (kernel_base) {
		DBGLOG("[ * ] kernel_base: %p", kernel_base);

		const auto thread_gadget = utils::find_pattern_image(reinterpret_cast<PCHAR>(kernel_base), "\xFF\xE1", "xx");

		DBGLOG("[ + ] thread_gadget: %p", thread_gadget);

		if (thread_gadget) {
			HANDLE thread_handle;

			NTSTATUS status = PsCreateSystemThread(
				&thread_handle,
				GENERIC_ALL,
				nullptr,
				nullptr,
				nullptr,
				(PKSTART_ROUTINE)thread_gadget,
				(void*)&test_thread
			);

			if (!NT_SUCCESS(status)) {
				DBGLOG("[ ! ] failed to create thread");
				return STATUS_FAILED_DRIVER_ENTRY;
			}

			ZwClose(thread_handle);
		}
	}

    return STATUS_SUCCESS;
}