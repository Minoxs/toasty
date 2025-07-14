#pragma once

typedef unsigned int uint;
typedef struct tray_icon_st tray_icon;

tray_icon* CreateTrayIcon();
void DeleteTrayIcon(tray_icon* handle);

void SetTip(tray_icon* handle, char* tip);
void SendNotification(tray_icon* handle, char* title, char* message);

tray_icon* SendTransientNotification(char* title, char* message);
