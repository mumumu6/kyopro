package api

import (
	"context"
	"net/http"
	"os"
	"time"

	"github.com/labstack/echo/v4"
	"github.com/sashabaranov/go-openai"

	"cached-llm/cache"
	"cached-llm/logger"
	"cached-llm/ratelimit"
)

type Request struct {
	Message string `json:"message"`
}

// RegisterHandlers registers all API handlers to the Echo instance
func RegisterHandlers(e *echo.Echo) {
	// Health check endpoint for Docker
	e.GET("/health", HealthCheck)

	// Ping endpoint
	e.GET("/ping", Ping)

	// Chat endpoint
	e.POST("/chat", ChatHandler)
}

// HealthCheck handles health check requests
func HealthCheck(c echo.Context) error {
	return c.String(http.StatusOK, "healthy")
}

// Ping handles ping requests
func Ping(c echo.Context) error {
	return c.String(http.StatusOK, "pong")
}

// ChatHandler handles chat requests
func ChatHandler(c echo.Context) error {
	// IPアドレスを取得
	ip := c.RealIP()

	// レートリミットのチェック
	if ratelimit.IsRateLimited(ip) {
		return c.JSON(http.StatusTooManyRequests, map[string]string{
			"error": "Rate limit exceeded. Please try again later.",
		})
	}

	req := new(Request)
	if err := c.Bind(req); err != nil {
		return c.JSON(http.StatusBadRequest, err)
	}

	// Check cache first
	messageHash := cache.CalculateHash(req.Message)
	if cachedResponse, found := cache.GetFromCache(messageHash); found {
		// Log cache hit
		logger.WriteLog(logger.LogEntry{
			Timestamp:  time.Now(),
			Hash:       messageHash,
			Input:      req.Message,
			Output:     cachedResponse,
			CacheState: "HIT",
		})
		return c.JSON(http.StatusOK, cachedResponse)
	}

	// キャッシュがない場合はOpenAI APIを呼び出す
	response, err := CallOpenAI(req.Message)
	if err != nil {
		return c.JSON(http.StatusInternalServerError, map[string]string{
			"error": err.Error(),
		})
	}

	// レスポンスをキャッシュに追加
	cache.AddToCache(messageHash, response, false)

	// Log cache miss and new entry
	logger.WriteLog(logger.LogEntry{
		Timestamp:  time.Now(),
		Hash:       messageHash,
		Input:      req.Message,
		Output:     response,
		CacheState: "MISS",
	})

	return c.JSON(http.StatusOK, response)
}

// callOpenAI calls the OpenAI API
func CallOpenAI(message string) (string, error) {
	client := openai.NewClient(os.Getenv("OPENAI_API_KEY"))
	resp, err := client.CreateChatCompletion(
		context.Background(),
		openai.ChatCompletionRequest{
			Model:     openai.GPT3Dot5Turbo,
			MaxTokens: 150,
			Messages: []openai.ChatCompletionMessage{
				{
					Role:    openai.ChatMessageRoleUser,
					Content: message,
				},
			},
		},
	)

	if err != nil {
		return "", err
	}

	return resp.Choices[0].Message.Content, nil
}
