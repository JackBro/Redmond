/* 
 * File:   redmond.cpp
 * Author: Ray
 *
 * Created on January 24, 2014, 11:04 PM
 */

#include <cstdlib>
#include <cstdio>

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <string>

// Fix the incompatibility between Windows sal.h and g++ compiler.
#define _Field_size_opt_(x)
#define _Out_writes_bytes_to_(x, y)
#define _In_reads_bytes_(x)
#define _Out_writes_bytes_(x)
#define _Inout_
#define _Out_writes_to_opt_(x, y)
#define _Out_writes_to_(x, y)

#define KDEXT_64BIT
#include <wdbgexts.h>

// Redefine the API by adding the export attribute.
#undef DECLARE_API
#define DECLARE_API(s)                             \
    CPPMOD __declspec(dllexport) VOID                                    \
    s(                                             \
        HANDLE                 hCurrentProcess,    \
        HANDLE                 hCurrentThread,     \
        ULONG                  dwCurrentPc,        \
        ULONG                  dwProcessor,        \
        PCSTR                  args                \
     )

#undef DECLARE_API32
#define DECLARE_API32(s)                           \
    CPPMOD __declspec(dllexport) VOID                                    \
    s(                                             \
        HANDLE                 hCurrentProcess,    \
        HANDLE                 hCurrentThread,     \
        ULONG                  dwCurrentPc,        \
        ULONG                  dwProcessor,        \
        PCSTR                  args                \
     )

#undef DECLARE_API64
#define DECLARE_API64(s)                           \
    CPPMOD __declspec(dllexport) VOID                                    \
    s(                                             \
        HANDLE                 hCurrentProcess,    \
        HANDLE                 hCurrentThread,     \
        ULONG64                dwCurrentPc,        \
        ULONG                  dwProcessor,        \
        PCSTR                  args                \
     )

#include <ntverp.h>

//
// globals
//
EXT_API_VERSION ApiVersion = {(VER_PRODUCTVERSION_W >> 8), (VER_PRODUCTVERSION_W & 0xff), EXT_API_VERSION_NUMBER64, 0};
WINDBG_EXTENSION_APIS ExtensionApis;
ULONG SavedMajorVersion;
ULONG SavedMinorVersion;

extern "C"
VOID
__declspec(dllexport)
WinDbgExtensionDllInit(
        PWINDBG_EXTENSION_APIS lpExtensionApis,
        USHORT MajorVersion,
        USHORT MinorVersion
        ) {
    ExtensionApis = *lpExtensionApis;

    SavedMajorVersion = MajorVersion;
    SavedMinorVersion = MinorVersion;

    return;
}

extern "C"
LPEXT_API_VERSION
__declspec(dllexport)
ExtensionApiVersion(
        VOID
        ) {
    //
    // ExtensionApiVersion should return EXT_API_VERSION_NUMBER64 in order for APIs
    // to recognize 64 bit addresses.  KDEXT_64BIT also has to be defined before including
    // wdbgexts.h to get 64 bit headers for WINDBG_EXTENSION_APIS
    //
    return &ApiVersion;
}

//
// Routine called by debugger after load
//

extern "C"
VOID
__declspec(dllexport)
CheckVersion(
        VOID
        ) {
    return;
}

BOOL WINAPI DllMain(
  _In_  HINSTANCE hinstDLL,
  _In_  DWORD fdwReason,
  _In_  LPVOID lpvReserved
)
{
    return TRUE;
}

DECLARE_API(help)
{
    dprintf("Redmond Debug Extension\n");
    dprintf("!libredmond.help - show this help message.\n");
    dprintf("!libredmond.genguid <address> - convert a 16-byte memory block to a GUID.\n");
    dprintf("                                To find a WPP GUID, run 'x <mod>!*ctlguid*'.\n");
    dprintf("!libredmond.maskirq <IRQ#> - mask an interrupt in IOAPIC.\n");
}

DECLARE_API(genguid)
{
    ULONG_PTR Address = 0;
    ULONG64 Value;
    PCSTR Remainder;

    ULONG Bytes;
    ULONG Data = 0;

    BOOL ret = GetExpressionEx(args, &Value, &Remainder);

    if (ret == TRUE && Value != 0) {
        Address = Value;
    }

    dprintf("GUID @ %p: {", Address);

    if (ReadMemory(Address, &Data, sizeof (DWORD), &Bytes) == TRUE) {
        dprintf("%08x-", Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    Address += 4;
    if (ReadMemory(Address, &Data, sizeof (WORD), &Bytes) == TRUE) {
        dprintf("%04x-", (WORD) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    Address += 2;
    if (ReadMemory(Address, &Data, sizeof (WORD), &Bytes) == TRUE) {
        dprintf("%04x-", (WORD) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    Address += 2;
    if (ReadMemory(Address, &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x-", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
        //guid.append(GUID);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    if (ReadMemory((++Address), &Data, sizeof (BYTE), &Bytes) == TRUE) {
        dprintf("%02x", (BYTE) Data);
    } else {
        dprintf("%p : Address Not Valid\n", Address);
    }

    dprintf("}\n");
}

DECLARE_API(maskirq)
{
    ULONG64 IRQ;
    PCSTR Remainder;

    BOOL ret = GetExpressionEx(args, &IRQ, &Remainder);
    if (ret == FALSE) {
        dprintf("Parameter is wrong: %s", args);
        return;
    }

    // TODO: How to get this value dynamically?
    ULONG64 IOAPICAddress = 0xFEC00000;
    ULONG32 Value = IRQ * 2 + 0x10;
    ULONG Size;
    
    WritePhysical(IOAPICAddress, &Value, sizeof(BYTE), &Size);
    if (Size != sizeof(BYTE)) {
        dprintf("Cannot write to IOREGSEL in IOAPIC @ %I64x", IOAPICAddress);
        return;
    }
    
    ReadPhysical(IOAPICAddress + 0x10, &Value, sizeof(Value), &Size);
    if (Size != sizeof(Value)) {
        dprintf("Cannot read from IOWIN in IOAPIC @ %I64x", IOAPICAddress + 0x10);
        return;
    }
    
    dprintf("Masking IRQ%I64x: %08x -> ", IRQ, Value);
    Value = (Value | (1 << 16));
    dprintf("%08x\n", Value);

    WritePhysical(IOAPICAddress + 0x10, &Value, sizeof(Value), &Size);
    if (Size != sizeof(Value)) {
        dprintf("Cannot write to IOWIN in IOAPIC @ %I64x", IOAPICAddress + 0x10);
        return;
    }
    
    dprintf("Done.");
}