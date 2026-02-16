package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: calc a+b")
		os.Exit(1)
	}

	expr := os.Args[1]
	
	if strings.Contains(expr, "+") {
		parts := strings.Split(expr, "+")
		a, _ := strconv.Atoi(strings.TrimSpace(parts[0]))
		b, _ := strconv.Atoi(strings.TrimSpace(parts[1]))
		fmt.Println(a + b)
	} else if strings.Contains(expr, "-") {
		parts := strings.Split(expr, "-")
		a, _ := strconv.Atoi(strings.TrimSpace(parts[0]))
		b, _ := strconv.Atoi(strings.TrimSpace(parts[1]))
		fmt.Println(a - b)
	} else if strings.Contains(expr, "*") {
		parts := strings.Split(expr, "*")
		a, _ := strconv.Atoi(strings.TrimSpace(parts[0]))
		b, _ := strconv.Atoi(strings.TrimSpace(parts[1]))
		fmt.Println(a * b)
	} else if strings.Contains(expr, "/") {
		parts := strings.Split(expr, "/")
		a, _ := strconv.Atoi(strings.TrimSpace(parts[0]))
		b, _ := strconv.Atoi(strings.TrimSpace(parts[1]))
		fmt.Println(a / b)
	}
}