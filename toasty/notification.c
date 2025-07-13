#include "header/notification.h"

#include <Windows.h>
#include <shellapi.h>

#define NEW_ICON(name) NOTIFYICONDATA name = { sizeof(name), .uID = rand() }

uint CreateTrayIcon(char* tip) {
	const char* className = "TrayIconWindowClass";
	WNDCLASSA wc = { sizeof(wc)};
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = className;
	wc.lpfnWndProc = DefWindowProcA;
	RegisterClassA(&wc);

	NEW_ICON(icon);

	icon.hWnd = CreateWindowExA(
		WS_EX_TRANSPARENT,
		className,
		className,
		0,
		0, 0, 0, 0,
		NULL, NULL,
		GetModuleHandle(NULL), NULL
	);

	icon.uFlags = NIF_ICON;
	icon.dwInfoFlags = NIIF_USER;
	icon.uVersion = NOTIFYICON_VERSION_4;
	icon.hIcon = LoadIcon(NULL, IDI_INFORMATION);

	if (strlen(tip) > 0) {
		icon.uFlags |= NIF_TIP | NIF_SHOWTIP;
		memset(&icon.szTip, 0, sizeof(icon.szTip));
		memcpy_s(&icon.szTip, sizeof(icon.szTip), tip, strlen(tip));
	}

	Shell_NotifyIcon(NIM_ADD, &icon);
	Shell_NotifyIcon(NIM_SETVERSION, &icon);
}

void DeleteTrayIcon(uint id) {
	NOTIFYICONDATA icon = { sizeof(icon), .uID = id };
	Shell_NotifyIcon(NIM_DELETE, &icon);
}

uint SendTransientNotification(char* title, char* message) {
	NEW_ICON(icon);

	icon.uFlags = NIF_ICON | NIF_INFO;
	icon.dwInfoFlags = NIIF_USER;
	icon.uVersion = NOTIFYICON_VERSION_4;
	icon.hIcon = LoadIcon(NULL, IDI_INFORMATION);

	memset  (&icon.szInfoTitle, 0, sizeof(icon.szInfoTitle));
	memcpy_s(&icon.szInfoTitle,    sizeof(icon.szInfoTitle), title, strlen(title));

	memset  (&icon.szInfo, 0, sizeof(icon.szInfo));
	memcpy_s(&icon.szInfo,    sizeof(icon.szInfo), message, strlen(message));

	Shell_NotifyIcon(NIM_ADD, &icon);
	Shell_NotifyIcon(NIM_SETVERSION, &icon);

	return icon.uID;
}
