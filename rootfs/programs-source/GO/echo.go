package main

// ECHO COMMAND

import (
	"fmt"
	"os"
)

func main() {
	var ln bool

	for _, arg := range os.Args[1:] {
		if arg == "-ln" {
			ln = true
			break
		} else if arg == "-h" {
			fmt.Println("ECHO: Usage: echo <text>")
			fmt.Println("\t-ln = Add newline at the end")
			fmt.Println("\t-h  = Show this message")
			fmt.Println()
			os.Exit(0)
		}
	}

	for i := 1; i < len(os.Args); i++ {
		if os.Args[i] == "-ln" {
			continue
		} else if i > 1 {
			fmt.Print(" ")
		}
		fmt.Print(os.Args[i])

	}
	if ln {
		fmt.Println()
	}
	printf("NOT BUSYBOX!")
}
