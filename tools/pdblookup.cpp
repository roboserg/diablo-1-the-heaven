// pdblookup.cpp — read a PDB directly through the DIA SDK and resolve
// module-relative addresses (RVAs) to function + source line. Needs no exe,
// so it can map the "TH4.exe+0x..." values logged by D3dDiagAddr even when
// the game folder is not accessible.
//
// Build (x86):
//   cd c:\th4\tools
//   call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvarsall.bat" x86
//   cl /nologo /EHsc /O2 /I"C:\Program Files\Microsoft Visual Studio\18\Community\DIA SDK\include" ^
//      pdblookup.cpp ole32.lib oleaut32.lib /Fe:pdblookup.exe
//
// Usage:
//   pdblookup <path-to.pdb> <rva-hex> [rva-hex ...]

#include <windows.h>
#include <dia2.h>
#include <stdio.h>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "oleaut32.lib")

// {E6756135-1E65-4D17-8576-610761398C3C} — DiaSource (msdia140)
static const GUID kCLSID_DiaSource = { 0xE6756135, 0x1E65, 0x4D17, { 0x85, 0x76, 0x61, 0x07, 0x61, 0x39, 0x8C, 0x3C } };

static IDiaDataSource* CreateDia()
{
	IDiaDataSource* src = nullptr;

	// Path 1: registered COM server.
	if (SUCCEEDED(CoCreateInstance(kCLSID_DiaSource, NULL, CLSCTX_INPROC_SERVER,
	                               __uuidof(IDiaDataSource), (void**)&src)))
		return src;

	// Path 2: load msdia140.dll directly (not registered).
	const char* candidates[] = {
		"C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\DIA SDK\\bin\\msdia140.dll",
		"C:\\Program Files\\Microsoft Visual Studio\\18\\Community\\Common7\\IDE\\msdia140.dll",
	};
	for (const char* path : candidates) {
		HMODULE h = LoadLibraryA(path);
		if (!h) continue;
		typedef HRESULT (STDAPICALLTYPE *DllGetClassObjectFn)(REFCLSID, REFIID, void**);
		DllGetClassObjectFn fn = (DllGetClassObjectFn)GetProcAddress(h, "DllGetClassObject");
		if (!fn) continue;
		IClassFactory* cf = nullptr;
		if (FAILED(fn(kCLSID_DiaSource, __uuidof(IClassFactory), (void**)&cf)) || !cf) continue;
		HRESULT hr = cf->CreateInstance(NULL, __uuidof(IDiaDataSource), (void**)&src);
		cf->Release();
		if (SUCCEEDED(hr)) return src;
	}

	printf("cannot create DIA data source (msdia140 not registered and not loadable)\n");
	return nullptr;
}

static void BstrToAnsi(BSTR b, char* out, size_t outSize)
{
	out[0] = 0;
	if (b) WideCharToMultiByte(CP_ACP, 0, b, -1, out, (int)outSize, NULL, NULL);
	if (b) SysFreeString(b);
}

int main(int argc, char** argv)
{
	if (argc < 3) {
		printf("usage: pdblookup <pdb> <rva-hex> [rva-hex ...]\n");
		return 1;
	}
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	IDiaDataSource* src = CreateDia();
	if (!src) return 2;

	wchar_t wpath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, argv[1], -1, wpath, MAX_PATH);
	HRESULT hr = src->loadDataFromPdb(wpath);
	if (FAILED(hr)) {
		printf("loadDataFromPdb('%s') failed: 0x%08X\n", argv[1], hr);
		return 3;
	}

	IDiaSession* sess = nullptr;
	hr = src->openSession(&sess);
	if (FAILED(hr) || !sess) {
		printf("openSession failed: 0x%08X\n", hr);
		return 4;
	}

	for (int i = 2; i < argc; i++) {
		DWORD rva = (DWORD)_strtoui64(argv[i], NULL, 16);

		char symName[512] = "";
		DWORD symRva = 0;
		ULONGLONG symLen = 0;
		IDiaSymbol* fn = nullptr;
		if (SUCCEEDED(sess->findSymbolByRVA(rva, SymTagFunction, &fn)) && fn) {
			BSTR b = nullptr;
			if (SUCCEEDED(fn->get_name(&b))) BstrToAnsi(b, symName, sizeof(symName));
			fn->get_relativeVirtualAddress(&symRva);
			fn->get_length(&symLen);
			fn->Release();
		}

		char fileName[512] = "";
		DWORD line = 0;
		IDiaEnumLineNumbers* lines = nullptr;
		if (SUCCEEDED(sess->findLinesByRVA(rva, 1, &lines)) && lines) {
			IDiaLineNumber* ln = nullptr;
			ULONG fetched = 0;
			if (SUCCEEDED(lines->Next(1, &ln, &fetched)) && ln) {
				ln->get_lineNumber(&line);
				IDiaSourceFile* sf = nullptr;
				if (SUCCEEDED(ln->get_sourceFile(&sf)) && sf) {
					BSTR b = nullptr;
					if (SUCCEEDED(sf->get_fileName(&b))) BstrToAnsi(b, fileName, sizeof(fileName));
					sf->Release();
				}
				ln->Release();
			}
			lines->Release();
		}

		if (symName[0]) {
			printf("0x%X -> %s + 0x%X (+%llu bytes fn)   [%s:%lu]\n",
				(unsigned)rva, symName, (unsigned)(rva - symRva),
				(unsigned long long)(rva - symRva), fileName[0] ? fileName : "?", (unsigned long)line);
		} else {
			printf("0x%X -> no function symbol   [%s:%lu]\n",
				(unsigned)rva, fileName[0] ? fileName : "?", (unsigned long)line);
		}
	}

	sess->Release();
	src->Release();
	CoUninitialize();
	return 0;
}
