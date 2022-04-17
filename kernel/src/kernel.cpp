
#include "BasicRenderer.h"
#include "Bitmap.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include <stdint.h>

struct BootInfo {
    Framebuffer *framebuffer;
    PSF1_FONT *psf1_Font;
    EFI_MEMORY_DESCRIPTOR *mMap;
    uint64_t mMapSize;
    uint64_t mMapDescSize;
};

uint8_t testbuffer[20];

extern "C" void
_start(BootInfo *bootInfo) {

    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);

    // Test displaying numbers with to_string method
    newRenderer.CursorPosition = {1000, 0};
    newRenderer.Print(to_string((int64_t)-1234976));
    newRenderer.CursorPosition = {1000, 16};
    newRenderer.Print(to_string((double)-13.14));

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    // List all memory descriptors with size
    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)bootInfo->mMap + (i * bootInfo->mMapDescSize));
        newRenderer.CursorPosition = {0, newRenderer.CursorPosition.Y + 16};
        newRenderer.Print(EFI_MEMORY_TYPE_STRINGS[desc->type]);
        newRenderer.Colour = 0xffff00ff;
        newRenderer.Print(" ");
        newRenderer.Print(to_string(desc->numPages * 4096 / 1024));
        newRenderer.Print(" KB");
        newRenderer.Colour = 0xffffffff;
    }

    Bitmap testBitmap;
    testBitmap.Buffer = &testbuffer[0];
    testBitmap.Set(0, false);
    testBitmap.Set(1, true);
    testBitmap.Set(2, false);
    testBitmap.Set(3, false);
    testBitmap.Set(4, true);
    testBitmap.Set(16, true);

    newRenderer.CursorPosition = {1400, 0};

    for (int i = 0; i < 20; i++) {
        newRenderer.CursorPosition = {1400, newRenderer.CursorPosition.Y + 16};
        newRenderer.Print(testBitmap[i] ? "true" : "false");
    }

    newRenderer.CursorPosition = {1000, 500};
    newRenderer.Print(to_string(GetMemorySize(bootInfo->mMap, mMapEntries, bootInfo->mMapDescSize)));

    return;
}