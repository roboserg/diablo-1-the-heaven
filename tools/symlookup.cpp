// symlookup.cpp — resolve RVAs (module-relative addresses) against a PDB without
// needing the matching exe. Uses DbgHelp's virtual-module load with a PDB handle.
//
// Build (x86, same as the game):
//   cd c:\th4\tools
//   call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
//   cl /nologo /EHsc /W4 /O2 symlookup.cpp dbghelp.lib /Fe:symlookup.exe
//
// Usage:
//   symlookup <path-to.pdb> <rva-hex> [rva-hex ...]
// Example:
//   symlookup c:\th4\Release\TH4.pdb 0x3A15B1 0x39FD1C

#include <windows.h>
#include <dbghelp.h>
#include <stdio.h>

#pragma comment(lib, "dbghelp.lib")

int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("usage: symlookup <pdb> <rva-hex> [rva-hex ...]\n");
		return 1;
	}

	HANDLE hProc = GetCurrentProcess();

	// Symbol search path = the PDB's own directory (dbghelp does lazy loads
	// relative to it), plus whatever the user has in _NT_SYMBOL_PATH.
	char symPath[MAX_PATH] = "";
	strncpy_s(symPath, argv[1], _TRUNCATE);
	if (char* slash = strrchr(symPath, '\\')) *slash = 0;
	{
		char env[MAX_PATH * 2] = "";
		GetEnvironmentVariableA("_NT_SYMBOL_PATH", env, sizeof(env));
		if (env[0]) { strncat_s(symPath, ";", _TRUNCATE); strncat_s(symPath, env, _TRUNCATE); }
	}

	SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_FAIL_CRITICAL_ERRORS);
	if (!SymInitialize(hProc, symPath, FALSE)) {
		printf("SymInitialize failed: %lu\n", GetLastError());
		return 1;
	}

	HANDLE hPdb = CreateFileA(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hPdb == INVALID_HANDLE_VALUE) {
		printf("cannot open pdb '%s': %lu\n", argv[1], GetLastError());
		return 1;
	}

	// Virtual module: no image file; the PDB handle supplies the symbols. A
	// non-zero size keeps DbgHelp from rejecting lookups that fall outside a
	// zero-sized module range.
	const DWORD64 base = 0x400000;
	const DWORD   fakeSize = 0x10000000;
	DWORD64 modBase = SymLoadModuleEx(hProc, hPdb, NULL, "TH4", base, fakeSize, NULL, SLMFLAG_VIRTUAL);
	if (!modBase) {
		printf("SymLoadModuleEx(virtual) failed: %lu — retrying with pdb path as image\n", GetLastError());
		modBase = SymLoadModuleEx(hProc, NULL, argv[1], "TH4", base, fakeSize, NULL, SLMFLAG_VIRTUAL);
	}
	if (!modBase) {
		printf("SymLoadModuleEx failed: %lu\n", GetLastError());
		return 1;
	}
	printf("module base = 0x%llX\n", (unsigned long long)modBase);
	{
		IMAGEHLP_MODULE64 mi;
		mi.SizeOfStruct = sizeof(mi);
		if (SymGetModuleInfo64(hProc, modBase, &mi)) {
			printf("module info: loaded=%d symType=%d name='%s'\n", (int)mi.SymType != SymNone, (int)mi.SymType, mi.ImageName);
		}
	}

	// Diagnostic: enumerate a few symbols to confirm the PDB table is really
	// available to DbgHelp (if this prints nothing, use the DIA path instead).
	if (argc > 2 && !strcmp(argv[2], "-enum")) {
		static int printed = 0;
		SymEnumSymbols(hProc, modBase, "*", [](PSYMBOL_INFO si, ULONG, PVOID) -> BOOL {
			printf("  %s @ 0x%llX\n", si->Name, (unsigned long long)si->Address);
			return (++printed < 25) ? TRUE : FALSE;
		}, NULL);
		printf("(enum done, %d shown)\n", printed);
		return 0;
	}

	for (int i = 2; i < argc; i++) {
		DWORD64 rva = _strtoui64(argv[i], NULL, 16);
		DWORD64 addr = base + rva;

		char symBuf[sizeof(SYMBOL_INFO) + 512];
		SYMBOL_INFO* sym = (SYMBOL_INFO*)symBuf;
		sym->SizeOfStruct = sizeof(SYMBOL_INFO);
		sym->MaxNameLen = 511;
		DWORD64 disp = 0;

		if (SymFromAddr(hProc, addr, &disp, sym)) {
			IMAGEHLP_LINE64 line;
			line.SizeOfStruct = sizeof(line);
			DWORD lineDisp = 0;
			if (SymGetLineFromAddr64(hProc, addr, &lineDisp, &line)) {
				printf("0x%X -> %s + 0x%llX   [%s:%lu]\n", (unsigned)rva, sym->Name,
					(unsigned long long)disp, line.FileName, line.LineNumber);
			} else {
				printf("0x%X -> %s + 0x%llX   [no line info]\n", (unsigned)rva, sym->Name,
					(unsigned long long)disp);
			}
		} else {
			printf("0x%X -> no symbol (err %lu)\n", (unsigned)rva, GetLastError());
		}
	}

	SymUnloadModule64(hProc, modBase);
	SymCleanup(hProc);
	CloseHandle(hPdb);
	return 0;
}
