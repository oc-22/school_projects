package domain

import "time"

type Game struct {
	Name      string        `json:"name"`
	Username  string        `json:"username"`
	Rows      int           `json:"rows"`
	Cols      int           `json:"cols"`
	Mines     int           `json:"mines"`
	Status    string        `json:"status"`
	Board     [][]string    `json:"board"`
	Mode      string        `json:"mode"`
	Clicks    int           `json:"clicks"`
	CreatedAt time.Time     `json:"created_at,omitempty"`
	StartedAt time.Time     `json:"started_at"`
	TimeSpent time.Duration `json:"time_spent"`
}

type ClickData struct {
	Row  int    `json:"row"`
	Col  int    `json:"col"`
	Kind string `json:"kind"`
}
type ScoreEntry struct {
	GameMode  string        `json:"game_mode"`
	TimeSpent time.Duration `json:"time_spent"`
}
type MultiplayerScore struct {
	Player1 int `json:"player1"`
	Player2 int `json:"player2"`
}
type MultiplayerData struct {
	Current MultiplayerScore   `json:"current"`
	History []MultiplayerScore `json:"history"`
}
