package toasty

/*
#include <stdlib.h>
#cgo nocallback free

#include "header/notification.h"
#cgo nocallback CreateTrayIcon
#cgo nocallback SendTransientNotification
#cgo nocallback DeleteTrayIcon
*/
import "C"
import (
	"sync"
	"time"
	"unsafe"
)

func free(str *C.char) {
	C.free(unsafe.Pointer(str))
}

type TrayIcon struct {
	id uint
}

func CreateTrayIcon(tip string) TrayIcon {
	var cTip = C.CString(tip)
	defer free(cTip)

	return TrayIcon{
		id: uint(C.CreateTrayIcon(cTip)),
	}
}

// SendTransientNotification sends a notification to the notification area which disappears after the specified timeout.
// This type of notification will disappear on mouseover and a click will do nothing.
// Call the returned cancel function to remove the notification early and ensure the icon won't stay in the tray
func SendTransientNotification(title string, message string, timeout time.Duration) (cancel func()) {
	var (
		cTitle   = C.CString(title)
		cMessage = C.CString(message)
	)
	defer free(cTitle)
	defer free(cMessage)

	var id = C.SendTransientNotification(cTitle, cMessage)
	cancel = sync.OnceFunc(
		func() {
			C.DeleteTrayIcon(id)
		},
	)

	go func() {
		time.Sleep(timeout)
		cancel()
	}()

	return
}
