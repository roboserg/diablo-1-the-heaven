#ifndef _XBOX_CONTROLLER_H_
#define _XBOX_CONTROLLER_H_

// No MFC
#define WIN32_LEAN_AND_MEAN

// We need the Windows Header and the XInput Header
#include <windows.h>
#include <XInput.h>

// Now, the XInput Library
// NOTE: COMMENT THIS OUT IF YOU ARE NOT USING A COMPILER THAT SUPPORTS THIS METHOD OF LINKING LIBRARIES
// (th4) xinput9_1_0.lib, NOT XInput.lib. XInput.lib is an SDK-dependent stub: under the
// old SDK 7.0 it resolved to xinput9_1_0.dll, but under a modern SDK it resolves to
// xinput1_4.dll, which only exists on Windows 8+. That would make TH4.exe fail to LOAD
// on Windows 7 -- before main, for everyone, gamepad or not -- and it silently broke the
// /DELAYLOAD:xinput9_1_0.dll in the vcxproj (LNK4199), which is what lets the exe start
// at all when the DLL is absent. Pinning 9.1.0 keeps Vista+ support.
// Safe because we only call XInputGetState/XInputSetState, which 9.1.0 exports.
// GetXInputGetKeystroke below is OUR wrapper over prev/cur bitmasks, not the real
// XInputGetKeystroke -- which 9.1.0 does NOT export.
#pragma comment(lib, "xinput9_1_0.lib")

#define XINPUT_KEYSTROKE_KEYDOWN        0x0001
#define XINPUT_KEYSTROKE_KEYUP          0x0002
#define XINPUT_KEYSTROKE_REPEAT         0x0004

// XBOX Controller Class Definition
class CXBOXController
{
private:
	XINPUT_STATE _controllerState;
	int _controllerNum;
	WORD prevStatus;
	WORD curStatus;
public:
	CXBOXController(int playerNumber);
	XINPUT_STATE GetState();
	bool IsConnected();
	void Vibrate(int leftVal = 0, int rightVal = 0);
	void UpdateButtonsStatus();
	WORD GetXInputGetKeystroke(const WORD key);
};

#endif