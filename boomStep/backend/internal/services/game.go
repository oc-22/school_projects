package services

import (
	"errors"
	"log"
	"time"

	"github.com/arllanos/minesweeper-API/internal/domain"
	"github.com/segmentio/ksuid"
)

var games = make(map[string]*domain.Game)
var highScores = []domain.ScoreEntry{}
var mpScore = domain.MultiplayerScore{Player1: 0, Player2: 0}
var mpHistory = []domain.MultiplayerScore{}

const (
	defaultRows  = 10
	defaultCols  = 10
	defaultMines = 15
	maxRows      = 30
	maxCols      = 30
	minRows      = 2
	minCols      = 2
)

// rozhranie služby – bez používateľov
type GameService interface {
	CreateGame(game *domain.Game) (*domain.Game, error)
	Exists(key string) bool
	Click(gameName string, data *domain.ClickData) (*domain.Game, error)
	Board(gameName string) ([][]string, error)
	GetGameStatus(gameName string) (*domain.Game, error)
	GetScores() ([]domain.ScoreEntry, error)
	GetMultiplayerData() (*domain.MultiplayerData, error)
	ResetMultiplayerScore() (*domain.MultiplayerData, error)
}

type service struct{}

// konštruktor – bez repozitára, všetko v pamäti
func NewGameService() GameService {
	return &service{}
}

// vytvorenie novej hry
func (s *service) CreateGame(game *domain.Game) (*domain.Game, error) {
	// nastavenie rozmerov a limitov
	if game.Rows == 0 {
		game.Rows = defaultRows
	}
	if game.Cols == 0 {
		game.Cols = defaultCols
	}
	if game.Mines == 0 {
		game.Mines = defaultMines
	}

	if game.Rows > maxRows {
		game.Rows = maxRows
	}
	if game.Cols > maxCols {
		game.Cols = maxCols
	}
	if game.Rows < minRows {
		game.Rows = minRows
	}
	if game.Cols < minCols {
		game.Cols = minCols
	}

	if game.Mines > (game.Cols * game.Rows) {
		game.Mines = (game.Cols * game.Rows)
	}
	if game.Mode == "" {
		game.Mode = "CUSTOM"
	}

	// ak nemá meno, pridelíme ID
	if game.Name == "" {
		game.Name = ksuid.New().String()
	}

	game.Board = nil
	game.CreatedAt = time.Now()
	game.Status = "ready"

	// vygenerujeme hraciu plochu
	generateBoard(game)
	games[game.Name] = game

	return game, nil
}

// overenie existencie hry
func (s *service) Exists(key string) bool {
	_, ok := games[key]
	return ok
}

// spracovanie kliknutia
func (s *service) Click(gameName string, click *domain.ClickData) (*domain.Game, error) {
	game, ok := games[gameName]
	if !ok {
		return nil, errors.New("game_not_found")
	}

	log.Printf("Click type [%s] request at (%d, %d) for game [%s] with status [%s]",
		click.Kind, click.Row, click.Col, game.Name, game.Status)

	if click.Kind != "click" && click.Kind != "flag" {
		return nil, errors.New("bad_click_kind")
	}

	// prvý klik spustí hru
	if game.Status == "ready" {
		game.Status = "in_progress"
		game.StartedAt = time.Now()
	}

	// po výhre alebo prehre už neklikáme
	if game.Status == "over" || game.Status == "won" {
		return nil, errors.New("game_over_or_won")
	}

	// spracovanie kliknutia alebo vlajky
	if click.Kind == "click" {
		if err := clickCell(game, click.Row, click.Col); err != nil {
			return nil, err
		}
		if game.Status == "over" {
			if gameName == "multi_p1" {
				mpScore.Player2++ // P1 vybuchol -> P2 má bod
			} else if gameName == "multi_p2" {
				mpScore.Player1++ // P2 vybuchol -> P1 má bod
			}
		}
	} else if click.Kind == "flag" {
		if err := flagCell(game, click.Row, click.Col); err != nil {
			return nil, err
		}
	}

	game.TimeSpent = time.Since(game.StartedAt)
	if weHaveWinner(game) {
		game.Status = "won"

		newScore := domain.ScoreEntry{
			GameMode:  game.Mode, // Používame režim poslaný z frontendu
			TimeSpent: game.TimeSpent,
		}
		highScores = append(highScores, newScore)

		if gameName == "multi_p1" {
			mpScore.Player1++
		} else if gameName == "multi_p2" {
			mpScore.Player2++
		}
	}

	games[gameName] = game
	return game, nil
}

// načítanie hracej plochy
func (s *service) Board(gameName string) ([][]string, error) {
	game, ok := games[gameName]
	if !ok {
		return nil, errors.New("game_not_found")
	}
	if game.Board == nil {
		return nil, errors.New("this game has no board")
	}
	return game.Board, nil
}
func (s *service) GetGameStatus(gameName string) (*domain.Game, error) {
	game, ok := games[gameName]
	if !ok {
		return nil, errors.New("game_not_found")
	}
	return game, nil
}
func (s *service) GetScores() ([]domain.ScoreEntry, error) {
	return highScores, nil
}

func (s *service) GetMultiplayerData() (*domain.MultiplayerData, error) {
	return &domain.MultiplayerData{
		Current: mpScore,
		History: mpHistory,
	}, nil
}

func (s *service) ResetMultiplayerScore() (*domain.MultiplayerData, error) {
	// Uložíme do histórie len ak sa reálne niečo hralo (nie 0:0)
	if mpScore.Player1 > 0 || mpScore.Player2 > 0 {
		mpHistory = append(mpHistory, mpScore)
	}

	// Reset na nuly
	mpScore = domain.MultiplayerScore{Player1: 0, Player2: 0}

	return &domain.MultiplayerData{
		Current: mpScore,
		History: mpHistory,
	}, nil
}
