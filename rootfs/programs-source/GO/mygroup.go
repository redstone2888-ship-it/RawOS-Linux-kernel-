package main

import (
	"fmt"
	"os/user"
)

func main() {
	u, _ := user.Current()
	fmt.Println("UID: ", u.Uid, " GID: ", u.Gid, " Username: ", u.Username)
}
