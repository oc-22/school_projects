package services

import (
	"errors"
	"math/rand"
	"strconv"
	"time"
	"unicode"

	"github.com/arllanos/minesweeper-API/internal/domain"
)

// create local random number generator
var randg = rand.New(rand.NewSource(time.Now().UnixNano()))

// generateBoard creates empty grid and randomly plants mines
func generateBoard(game *domain.Game) {
	game.Board = make([][]string, game.Rows)
	for i := range game.Board {
		game.Board[i] = make([]string, game.Cols)
		for j := 0; j < game.Cols; j++ {
			game.Board[i][j] = "E" // E = unrevealed empty
		}
	}

	// randomly plant mines
	i := 0
	for i < game.Mines {
		x := randg.Intn(game.Rows)
		y := randg.Intn(game.Cols)
		if game.Board[x][y] != "M" {
			game.Board[x][y] = "M"
			i++
		}
	}
}

func clickCell(game *domain.Game, i int, j int) error {
	// directions: NW, N, NE, SE, S, SW, W, E
	dirVector := [8][2]int{{-1, -1}, {-1, 0}, {-1, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {0, 1}}

	if !(i >= 0 && i < game.Rows && j >= 0 && j < game.Cols) {
		return errors.New("clicked cell out of bounds")
	}

	// if flagged cell
	if game.Board[i][j] == "m" || game.Board[i][j] == "e" {
		return errors.New("clicked cell is flagged")
	}

	// increment click count
	if game.Board[i][j] == "M" || game.Board[i][j] == "E" {
		game.Clicks++
	}

	if game.Board[i][j] == "M" {
		game.Board[i][j] = "X" // boom
		game.Status = "over"
		return nil
	}

	// recursive reveal function
	var solve func(board [][]string, r int, c int)
	solve = func(board [][]string, r int, c int) {
		mineCount := 0
		for k := 0; k < 8; k++ {
			x, y := r+dirVector[k][0], c+dirVector[k][1]
			if x >= 0 && x < len(board) && y >= 0 && y < len(board[0]) &&
				(board[x][y] == "M" || board[x][y] == "m") {
				mineCount++
			}
		}

		if mineCount > 0 {
			board[r][c] = strconv.Itoa(mineCount)
			return
		}

		board[r][c] = "B" // blank revealed

		for k := 0; k < 8; k++ {
			x, y := r+dirVector[k][0], c+dirVector[k][1]
			if x >= 0 && x < len(board) && y >= 0 && y < len(board[0]) &&
				(board[x][y] == "E" || board[x][y] == "e") {
				solve(board, x, y)
			}
		}
	}

	solve(game.Board, i, j)
	return nil
}

func flagCell(game *domain.Game, i int, j int) error {
	if !(i >= 0 && i < game.Rows && j >= 0 && j < game.Cols) {
		return errors.New("flagged cell out of bounds")
	}

	// toggle flags on unrevealed cells
	value := game.Board[i][j]
	if value == "M" || value == "E" {
		game.Board[i][j] = string(unicode.ToLower(rune(value[0])))
	} else if value == "m" || value == "e" {
		game.Board[i][j] = string(unicode.ToUpper(rune(value[0])))
	}

	return nil
}

func weHaveWinner(game *domain.Game) bool {
	for i := range game.Board {
		for j := range game.Board[i] {
			if game.Board[i][j] == "E" || game.Board[i][j] == "e" || game.Board[i][j] == "X" {
				return false
			}
		}
	}
	return true
}
