#include "header/notification.h"

#include <Windows.h>
#include <shellapi.h>

#define GET_ICON(name, id, wnd) NOTIFYICONDATA name = { sizeof(name), .uID = id, .hWnd = wnd, .uVersion = NOTIFYICON_VERSION_4 }
#define NEW_ICON(name) GET_ICON(name, rand(), NULL)
#define ICON(name, st) GET_ICON(name, st->id, st->wnd)

typedef struct tray_icon_st {
	uint id;
	HWND wnd;
} tray_icon;

static tray_icon* NewTrayIcon(uint id, HWND wnd) {
	tray_icon* result = (tray_icon*) malloc(sizeof(tray_icon));
	result->id = id;
	result->wnd = wnd;
	return result;
}

tray_icon* CreateTrayIcon() {
	NEW_ICON(icon);

	// Create a unique class name
	char buffer[512];
	itoa(icon.uID, buffer, 10);
	char className[1024] = "TrayIconWindowClass";
	strcat_s(className, sizeof(className), buffer);

	// Register window class
	WNDCLASSA wc = { sizeof(wc)};
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = className;
	wc.lpfnWndProc = DefWindowProcA;
	RegisterClassA(&wc);	

	// Register the tray icon
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
	icon.hIcon = LoadIcon(NULL, IDI_INFORMATION);

	Shell_NotifyIcon(NIM_ADD, &icon);
	Shell_NotifyIcon(NIM_SETVERSION, &icon);

	return NewTrayIcon(icon.uID, icon.hWnd);
}

void DeleteTrayIcon(tray_icon* handle) {
	ICON(icon, handle);
	Shell_NotifyIcon(NIM_DELETE, &icon);
	// TODO : DESTROY WINDOW
	free(handle);
}

void SetTip(tray_icon* handle, char* tip) {
	ICON(icon, handle);
	icon.uFlags = NIF_TIP | NIF_SHOWTIP;

	memset  (&icon.szTip, 0, sizeof(icon.szTip));
	memcpy_s(&icon.szTip,    sizeof(icon.szTip) - 1, tip, strlen(tip));

	Shell_NotifyIcon(NIM_MODIFY, &icon);
}

void SendNotification(tray_icon* handle, char* title, char* message) {
	ICON(icon, handle);
	icon.uFlags = NIF_INFO;
	icon.dwInfoFlags = NIIF_USER;

	memset(&icon.szInfoTitle, 0, sizeof(icon.szInfoTitle));
	memcpy_s(&icon.szInfoTitle, sizeof(icon.szInfoTitle) - 1, title, strlen(title));

	memset(&icon.szInfo, 0, sizeof(icon.szInfo));
	memcpy_s(&icon.szInfo, sizeof(icon.szInfo) - 1, message, strlen(message));

	Shell_NotifyIcon(NIM_MODIFY, &icon);
}

tray_icon* SendTransientNotification(char* title, char* message) {
	NEW_ICON(icon);
	icon.uFlags = NIF_ICON | NIF_INFO;
	icon.dwInfoFlags = NIIF_USER;
	icon.hIcon = LoadIcon(NULL, IDI_INFORMATION);

	memset  (&icon.szInfoTitle, 0, sizeof(icon.szInfoTitle));
	memcpy_s(&icon.szInfoTitle,    sizeof(icon.szInfoTitle) - 1, title, strlen(title));

	memset  (&icon.szInfo, 0, sizeof(icon.szInfo));
	memcpy_s(&icon.szInfo,    sizeof(icon.szInfo) - 1, message, strlen(message));

	Shell_NotifyIcon(NIM_ADD, &icon);
	Shell_NotifyIcon(NIM_SETVERSION, &icon);

	return NewTrayIcon(icon.uID, NULL);
}
