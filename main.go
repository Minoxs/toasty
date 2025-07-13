package main

import (
	"time"

	"github.com/minoxs/toasty/toasty"
)

func main() {
	defer toasty.SendTransientNotification("Info", "Body", 10*time.Second)()
	time.Sleep(5 * time.Second)
}
