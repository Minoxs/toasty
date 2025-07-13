#pragma once

typedef unsigned int uint;

uint CreateTrayIcon(char* tip);
void DeleteTrayIcon(uint id);

uint SendTransientNotification(char* title, char* message);
