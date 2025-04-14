import { dlopen, FFIType, ptr, CString } from "bun:ffi";

const omenalib = dlopen("libomena.so", {
    seed_virtual_ips: {
        args: [FFIType.i64],
        returns: FFIType.void,
    },
    // TODO! is there a way for bun:ffi to handle structs?
});

const seedCount = 100;
omenalib.symbols.seed_virtual_ips(seedCount);

for (let i = 0; i < seedCount; i++) {}
