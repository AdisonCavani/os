
#include "BasicRenderer.h"
#include "Bitmap.h"
#include "cstr.h"
#include "efiMemory.h"
#include "memory.h"
#include "paging/PageFrameAllocator.h"
#include "paging/PageMapIndexer.h"
#include <stdint.h>

struct BootInfo {
    Framebuffer *framebuffer;
    PSF1_FONT *psf1_Font;
    EFI_MEMORY_DESCRIPTOR *mMap;
    uint64_t mMapSize;
    uint64_t mMapDescSize;
};

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

extern "C" void
_start(BootInfo *bootInfo) {

    BasicRenderer newRenderer = BasicRenderer(bootInfo->framebuffer, bootInfo->psf1_Font);

    uint64_t mMapEntries = bootInfo->mMapSize / bootInfo->mMapDescSize;

    PageFrameAllocator newAllocator;
    newAllocator.ReadEFIMemoryMap(bootInfo->mMap, bootInfo->mMapSize, bootInfo->mMapDescSize);

    uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    newAllocator.LockPages(&_KernelStart, kernelPages);

    PageMapIndexer pageIndexer = PageMapIndexer(0x1000 * 52 + 0x50000 * 7);

    newRenderer.Print(to_string(pageIndexer.P_i));
    newRenderer.Print(" - ");
    newRenderer.Print(to_string(pageIndexer.PT_i));
    newRenderer.Print(" - ");
    newRenderer.Print(to_string(pageIndexer.PD_i));
    newRenderer.Print(" - ");
    newRenderer.Print(to_string(pageIndexer.PDP_i));

    return;
}