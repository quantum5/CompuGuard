#pragma once
#ifndef idF81614FF_98D7_44CA_AA835CD21C5A4330
#define idF81614FF_98D7_44CA_AA835CD21C5A4330

#define SelfAlloc(size) HeapAlloc(g_hHeap, 0, size)
#define SelfAllocZero(size) HeapAlloc(g_hHeap, HEAP_ZERO_MEMORY, size)
#define SelfFree(ram) HeapFree(g_hHeap, 0, ram)

extern HANDLE g_hHeap;

#endif // header

