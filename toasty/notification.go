package toasty

/*
#include <stdlib.h>
#cgo nocallback free

#include "header/notification.h"
#cgo nocallback CreateTrayIcon
#cgo nocallback DeleteTrayIcon
#cgo nocallback SetTip
#cgo nocallback SendNotification
#cgo nocallback SendTransientNotification
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
	handle *C.tray_icon
}

func CreateTrayIcon() TrayIcon {
	return TrayIcon{C.CreateTrayIcon()}
}

func (t *TrayIcon) Delete() {
	C.DeleteTrayIcon(t.handle)
	t.handle = nil
}

func (t *TrayIcon) SetTip(tip string) {
	var cTip = C.CString(tip)
	defer free(cTip)

	C.SetTip(t.handle, cTip)
}

func (t *TrayIcon) SendNotification(title string, message string) {
	var (
		cTitle   = C.CString(title)
		cMessage = C.CString(message)
	)
	defer free(cTitle)
	defer free(cMessage)

	C.SendNotification(t.handle, cTitle, cMessage)
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

	var handle = C.SendTransientNotification(cTitle, cMessage)
	cancel = sync.OnceFunc(
		func() {
			C.DeleteTrayIcon(handle)
		},
	)

	go func() {
		time.Sleep(timeout)
		cancel()
	}()

	return
}
