
#include "BasicRenderer.h"
#include "cstr.h"
#include "efiMemory.h"
#include <stdint.h>

struct BootInfo {
    Framebuffer *framebuffer;
    PSF1_FONT *psf1_Font;
    void *mMap;
    uint64_t mMapSize;
    uint64_t mMapDescSize;
};

extern "C" void _start(BootInfo *bootInfo) {

    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);
    newRenderer.Print(to_string((uint64_t)1234976));
    newRenderer.CursorPosition = {1000, 16};
    newRenderer.Print(to_string((int64_t)-1234976));
    newRenderer.CursorPosition = {1000, 32};
    newRenderer.Print(to_string((double)-13.14));
    newRenderer.CursorPosition = {1000, 48};
    newRenderer.Print(to_hstring((uint64_t)0xF0));
    newRenderer.CursorPosition = {1000, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint32_t)0xF0));
    newRenderer.CursorPosition = {1000, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint16_t)0xF0));
    newRenderer.CursorPosition = {1000, newRenderer.CursorPosition.Y + 16};
    newRenderer.Print(to_hstring((uint8_t)0xF0));

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

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

    return;
}