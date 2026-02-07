package handler

import (
	"encoding/json"
	"net/http"

	"github.com/arllanos/minesweeper-API/internal/domain"
	"github.com/arllanos/minesweeper-API/internal/errors"
	"github.com/arllanos/minesweeper-API/internal/services"
)

type handler struct {
	gameService services.GameService
}

type GameHandler interface {
	CreateGame(response http.ResponseWriter, request *http.Request)
	ClickCell(response http.ResponseWriter, request *http.Request)
	GetBoard(response http.ResponseWriter, request *http.Request)
	GetGameStatus(response http.ResponseWriter, request *http.Request)
	GetScores(response http.ResponseWriter, request *http.Request)
	GetMultiplayerData(w http.ResponseWriter, r *http.Request)
	ResetMultiplayerScore(w http.ResponseWriter, r *http.Request)
}

func NewGameHandler(service services.GameService) GameHandler {
	return &handler{
		gameService: service,
	}
}

func (h *handler) CreateGame(response http.ResponseWriter, request *http.Request) {
	response.Header().Set("Content-Type", "application/json")

	var game domain.Game
	err := json.NewDecoder(request.Body).Decode(&game)
	if err != nil {
		response.WriteHeader(http.StatusBadRequest)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: "Invalid request body"})
		return
	}

	result, err1 := h.gameService.CreateGame(&game)
	if err1 != nil {
		response.WriteHeader(http.StatusInternalServerError)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: err1.Error()})
		return
	}

	response.WriteHeader(http.StatusCreated)
	json.NewEncoder(response).Encode(result)
}

// -----------------------------
// Spracovanie kliknutia
// -----------------------------
func (h *handler) ClickCell(response http.ResponseWriter, request *http.Request) {
	response.Header().Set("Content-Type", "application/json")

	var click domain.ClickData
	if err := json.NewDecoder(request.Body).Decode(&click); err != nil {
		response.WriteHeader(http.StatusBadRequest)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: "Invalid click data"})
		return
	}

	gameName, ok := request.Context().Value("gameName").(string)
	if !ok || gameName == "" {
		response.WriteHeader(http.StatusBadRequest)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: "Missing game name"})
		return
	}

	result, err1 := h.gameService.Click(gameName, &click)
	if err1 != nil {
		switch err1.Error() {
		case "bad_click_kind":
			response.WriteHeader(http.StatusBadRequest)
		case "game_not_found":
			response.WriteHeader(http.StatusNotFound)
		case "game_over_or_won":
			response.WriteHeader(http.StatusConflict)
		default:
			response.WriteHeader(http.StatusInternalServerError)
		}
		json.NewEncoder(response).Encode(errors.ServiceError{Message: err1.Error()})
		return
	}

	response.WriteHeader(http.StatusOK)
	json.NewEncoder(response).Encode(result)
}

// -----------------------------
// Načítanie hracej dosky
// -----------------------------
func (h *handler) GetBoard(response http.ResponseWriter, request *http.Request) {
	response.Header().Set("Content-Type", "application/json")

	gameName, ok := request.Context().Value("gameName").(string)
	if !ok || gameName == "" {
		response.WriteHeader(http.StatusBadRequest)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: "Missing game name"})
		return
	}

	board, err := h.gameService.Board(gameName)
	if err != nil {
		switch err.Error() {
		case "game_not_found":
			response.WriteHeader(http.StatusNotFound)
		default:
			response.WriteHeader(http.StatusInternalServerError)
		}
		json.NewEncoder(response).Encode(errors.ServiceError{Message: err.Error()})
		return
	}

	response.WriteHeader(http.StatusOK)
	json.NewEncoder(response).Encode(board)
}
func (h *handler) GetGameStatus(response http.ResponseWriter, request *http.Request) {
	response.Header().Set("Content-Type", "application/json")

	gameName, ok := request.Context().Value("gameName").(string)
	if !ok || gameName == "" {
		response.WriteHeader(http.StatusBadRequest)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: "Missing game name"})
		return
	}

	// Volať novú službu, ktorá vráti celý Game objekt
	game, err := h.gameService.GetGameStatus(gameName) //
	if err != nil {
		switch err.Error() {
		case "game_not_found":
			response.WriteHeader(http.StatusNotFound)
		default:
			response.WriteHeader(http.StatusInternalServerError)
		}
		json.NewEncoder(response).Encode(errors.ServiceError{Message: err.Error()})
		return
	}

	// Kódujeme celý objekt Game (vrátane statusu, času, atď.)
	response.WriteHeader(http.StatusOK)
	json.NewEncoder(response).Encode(game)
}
func (h *handler) GetScores(response http.ResponseWriter, request *http.Request) {
	response.Header().Set("Content-Type", "application/json")

	// Žiadna extrakcia username nie je potrebná!
	scores, err := h.gameService.GetScores()
	if err != nil {
		response.WriteHeader(http.StatusInternalServerError)
		json.NewEncoder(response).Encode(errors.ServiceError{Message: err.Error()})
		return
	}

	response.WriteHeader(http.StatusOK)
	json.NewEncoder(response).Encode(scores)
}
func (h *handler) GetMultiplayerData(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	data, _ := h.gameService.GetMultiplayerData()
	json.NewEncoder(w).Encode(data)
}

func (h *handler) ResetMultiplayerScore(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "application/json")
	data, _ := h.gameService.ResetMultiplayerScore()
	w.WriteHeader(http.StatusOK)
	json.NewEncoder(w).Encode(data)
}
