package main

import (
	"bufio"
	"fmt"
	"os"
	"os/exec"
	"os/user"
	"strings"
)

func main() {
	os.Setenv("PATH", "/home/user/bin:/bin:/home/user/sbin:/sbin")

	reader := bufio.NewReader(os.Stdin)

	for {
		cwd, _ := os.Getwd()
		home := "/home/user"

		// ~ = /home/user
		if strings.HasPrefix(cwd, home) {
			cwd = "~" + strings.TrimPrefix(cwd, home)
		}

		// определить символ prompt
		prompt := "$ "
		currentUser, err := user.Current()
		if err == nil && currentUser.Uid == "0" {
			prompt = "# "
		}

		fmt.Print(cwd, " ", prompt)

		line, _ := reader.ReadString('\n')
		line = strings.TrimSpace(line)

		if line == "" {
			continue
		}

		args := strings.Fields(line)

		// built-in commands
		switch args[0] {

		case "cd":
			dir := home
			if len(args) > 1 {
				dir = args[1]

				if dir == "~" {
					dir = home
				} else if strings.HasPrefix(dir, "~/") {
					dir = home + dir[1:]
				}
			}

			if err := os.Chdir(dir); err != nil {
				fmt.Println("cd:", err)
			}
			continue

		case "echo":
			if len(args) > 1 && strings.HasPrefix(args[1], "$") {
				fmt.Println(os.Getenv(args[1][1:]))
			} else {
				fmt.Println(strings.Join(args[1:], " "))
			}
			continue
		}

		// execution if not built-in
		cmd := exec.Command(args[0], args[1:]...)
		cmd.Stdin = os.Stdin
		cmd.Stdout = os.Stdout
		cmd.Stderr = os.Stderr

		if err := cmd.Run(); err != nil {
			fmt.Println("error:", err)
		}
	}
}
