package cache

import (
	"crypto/sha256"
	"encoding/hex"
	"sync"
	"time"

	"cached-llm/logger"
)

type CacheEntry struct {
	Response   string
	Size       int
	Eternal    bool
	CreateTime time.Time
}

var (
	cache            = make(map[string]CacheEntry)
	cacheMutex       sync.RWMutex
	currentCacheSize int64 = 0
	maxCacheSize     int64 = 1 << 20 // 1MB in bytes
	cacheExpiration        = 10 * time.Minute
)

// CalculateHash はメッセージからハッシュ値を計算します
func CalculateHash(message string) string {
	hash := sha256.Sum256([]byte(message))
	fullHash := hex.EncodeToString(hash[:])
	return fullHash[:6] // Only use first 6 characters
}

// AddToCache はキャッシュにレスポンスを追加します
func AddToCache(hash string, response string, eternal bool) bool {
	size := len(response)
	if currentCacheSize+int64(size) > maxCacheSize {
		// キャッシュがいっぱいの場合はログを記録
		logger.WriteLog(logger.LogEntry{
			Timestamp:  time.Now(),
			Hash:       hash,
			Input:      "",
			Output:     "",
			CacheState: "FULL",
		})
		return false
	}
	cacheMutex.Lock()
	defer cacheMutex.Unlock()
	cache[hash] = CacheEntry{
		Response:   response,
		Size:       size,
		Eternal:    eternal,
		CreateTime: time.Now(),
	}
	currentCacheSize += int64(size)
	return true
}

// GetFromCache はキャッシュからレスポンスを取得します
func GetFromCache(hash string) (string, bool) {
	cacheMutex.RLock()
	defer cacheMutex.RUnlock()
	if entry, exists := cache[hash]; exists {
		if !entry.Eternal && time.Since(entry.CreateTime) > cacheExpiration {
			// Cache expired, remove it
			go func() {
				cacheMutex.Lock()
				defer cacheMutex.Unlock()
				if entry, stillExists := cache[hash]; stillExists {
					// 期限切れキャッシュのログを記録
					logger.WriteLog(logger.LogEntry{
						Timestamp:  time.Now(),
						Hash:       hash,
						Input:      "",
						Output:     entry.Response,
						CacheState: "EXPIRED",
					})
					currentCacheSize -= int64(entry.Size)
					delete(cache, hash)
				}
			}()
			return "", false
		}
		return entry.Response, true
	}
	return "", false
}

// SetCacheParams はキャッシュのパラメータを設定します
func SetCacheParams(maxSize int64, expiration time.Duration) {
	cacheMutex.Lock()
	defer cacheMutex.Unlock()
	maxCacheSize = maxSize
	cacheExpiration = expiration
}

// ClearCache はキャッシュをクリアします
func ClearCache() {
	cacheMutex.Lock()
	defer cacheMutex.Unlock()

	// クリア前にログを記録
	logger.WriteLog(logger.LogEntry{
		Timestamp:  time.Now(),
		Hash:       "",
		Input:      "",
		Output:     "",
		CacheState: "CLEARED",
	})

	cache = make(map[string]CacheEntry)
	currentCacheSize = 0
}
