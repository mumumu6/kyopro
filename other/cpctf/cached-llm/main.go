package main

import (
	"cached-llm/api"
	"cached-llm/ratelimit"
	"os"

	"github.com/labstack/echo/v4"
	"github.com/labstack/echo/v4/middleware"
)

// ログディレクトリを初期化する関数
func initLogger() error {
	// logsディレクトリが存在しない場合は作成
	if err := os.MkdirAll("logs", 0755); err != nil {
		return err
	}
	return nil
}

func main() {
	// ロガーの初期化
	if err := initLogger(); err != nil {
		panic("Failed to initialize logger: " + err.Error())
	}

	// Echo インスタンスの作成
	e := echo.New()
	e.Use(middleware.Logger())
	e.Use(middleware.Recover())

	// レートリミットのクリーンアップを開始
	ratelimit.StartCleanup()

	// APIハンドラーの登録
	api.RegisterHandlers(e)

	// サーバー起動
	e.Logger.Fatal(e.Start("0.0.0.0:8080"))
}
