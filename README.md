# phantom_thread

phantom_thread is a POC kernel driver designed for manual mapping. It spoofs the thread's start address and uses `KeEnterGuardedRegion()` to avoid asynchronous procedure calls (APCs). This subverts bad actors that could be looking to find the spoofed system thread. this was a driver i wrote when i was bored for fun not for bypassing any specific anticheats/services.

## features

- **manual map support**: designed to be manually mapped into the kernel space.
- **spoofs thread start address**: changes the start address of threads to enhance security and obfuscation through the usage of a gadget in a signed module.
- **apc avoidance**: utilizes `KeEnterGuardedRegion()` to prevent APCs from being queued upon the thread finding that it links back to an unsigned module.
