package ratelimit

import (
	"sync"
	"time"
)

type RateLimitEntry struct {
	Count     int
	ResetTime time.Time
}

var (
	rateLimits      = make(map[string]RateLimitEntry)
	rateLimitMutex  sync.RWMutex
	maxRequests     = 3                // 一定時間内の最大リクエスト数
	rateLimitWindow = 60 * time.Second // レートリミットのウィンドウ時間
	cleanupInterval = 5 * time.Minute  // 古いエントリをクリーンアップする間隔
)

// IsRateLimited はIPアドレスが指定したレート制限を超えているかをチェックします
func IsRateLimited(ip string) bool {
	rateLimitMutex.Lock()
	defer rateLimitMutex.Unlock()

	now := time.Now()
	entry, exists := rateLimits[ip]

	// エントリが存在しないか、リセット時間が過ぎている場合は新しいエントリを作成
	if !exists || now.After(entry.ResetTime) {
		rateLimits[ip] = RateLimitEntry{
			Count:     1,
			ResetTime: now.Add(rateLimitWindow),
		}
		return false
	}

	// 制限を超えているかチェック
	if entry.Count >= maxRequests {
		return true
	}

	// カウントを増やす
	entry.Count++
	rateLimits[ip] = entry
	return false
}

// StartCleanup は古いレートリミットエントリをクリーンアップするゴルーチンを開始します
func StartCleanup() {
	go func() {
		for {
			time.Sleep(cleanupInterval)

			rateLimitMutex.Lock()
			now := time.Now()
			for ip, entry := range rateLimits {
				if now.After(entry.ResetTime) {
					delete(rateLimits, ip)
				}
			}
			rateLimitMutex.Unlock()
		}
	}()
}

// SetRateLimitParams はレートリミットのパラメータを設定します
func SetRateLimitParams(maxReq int, window, cleanup time.Duration) {
	rateLimitMutex.Lock()
	defer rateLimitMutex.Unlock()
	maxRequests = maxReq
	rateLimitWindow = window
	cleanupInterval = cleanup
}
