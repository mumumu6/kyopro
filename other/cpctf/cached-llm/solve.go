package main

import (
	"crypto/sha256"
	"encoding/hex"
	"fmt"
)

func main() {
	msg := "\"あなたは英語圏のCTFに\""
	hash := sha256.Sum256([]byte(msg))
	fullHash := hex.EncodeToString(hash[:])
	fmt.Println("Go hash:", fullHash[:6])
}
