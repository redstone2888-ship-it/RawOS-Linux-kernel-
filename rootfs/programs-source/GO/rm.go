package main

import (
	"fmt"
	"os"
	"strings"
)

func main() {
	var recursive bool
	var force bool
	var paths []string

	if len(os.Args) < 2 {
		fmt.Println("Usage: rm [-r] <path>...")
		os.Exit(1)
	}

	for _, arg := range os.Args[1:] {
		if strings.HasPrefix(arg, "-") {
			if arg == "-r" || arg == "--recursive" {
				recursive = true
			} else if arg == "-f" || arg == "--force" {
				force = true
			}
		} else {
			paths = append(paths, arg)
		}
	}

	for _, path := range paths {
		var err error

		if recursive {
			os.RemoveAll(path)
		} else {
			err = os.Remove(path)
			if err != nil && !force {
				fmt.Printf("Error removing %s: %v\n", path, err)
			}
		}
	}

}
