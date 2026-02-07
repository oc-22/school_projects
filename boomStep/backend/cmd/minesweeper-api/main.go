package main

import (
	"log"
	"os"

	"github.com/arllanos/minesweeper-API/internal/api/handler"
	"github.com/arllanos/minesweeper-API/internal/api/router"
	"github.com/arllanos/minesweeper-API/internal/services"
)

const defaultPort = "8080"

func main() {
	gameService := services.NewGameService()
	gameHandler := handler.NewGameHandler(gameService)
	httpRouter := router.NewChiRouter()

	// register routes
	httpRouter.PUT("/games", gameHandler.CreateGame)
	httpRouter.POST("/games/{gamename}/click", gameHandler.ClickCell)
	httpRouter.GET("/games/{gamename}/board", gameHandler.GetBoard)
	httpRouter.GET("/games/{gamename}", gameHandler.GetGameStatus)
	httpRouter.GET("/scores", gameHandler.GetScores)
	httpRouter.GET("/multiplayer/data", gameHandler.GetMultiplayerData)

	httpRouter.POST("/multiplayer/reset", gameHandler.ResetMultiplayerScore)

	// start the server
	port := os.Getenv("PORT")
	if port == "" {
		port = defaultPort
	}
	log.Printf("Server running on port %s", port)
	if err := httpRouter.SERVE(port); err != nil {
		log.Fatalf("Failed to start server %v", err)
	}
}
