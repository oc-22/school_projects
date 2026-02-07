Author: Ondrej Cagalinec (xcagalo00), Jan Kostečka (xkostej00)
File: readme.txt
Brief: Instructions used to start the environment and description of file structure along with ownership

==============================================================================================
=======================USE AND SETUP OF frontend_xcagalo00====================================
==============================================================================================
Easiest way to test the application is use the provided script
run_xcagalo00.sh that will automatically download all needed packages and
run a backend server on port 8080 along with a frontend server.
use chmod +x run_xcagalo00.sh before running the script

If you do not wish to use the script, you can manually navigate to
boomStep_svelte/backend/cmd/minesweeper-api and type "go run main.go" to run the BE server

to run the frontend server naviagte to boomStep_svelte/frontend and type "nmp install"
followed by "npm run dev" to run the server locally.

the backend folder is the same for my teammate (xkostej00) and the frontend folder is unique
for both of us since the implementation was done in diffrent technologies.
=============================================================================================
=============================================================================================

boomStep_Svelte/
├── backend
│   ├── cmd
│   │   └── minesweeper-api
│   │       └── main.go
│   ├── go.mod
│   ├── go.sum
│   ├── internal
│   │   ├── api
│   │   │   ├── handler
│   │   │   │   └── game.go
│   │   │   └── router
│   │   │       ├── chi_router.go
│   │   │       └── router.go
│   │   ├── domain
│   │   │   └── game.go
│   │   ├── errors
│   │   │   └── service_error.go
│   │   └── services
│   │       ├── game_engine.go
│   │       ├── game_engine_test.go
│   │       └── game.go
│   ├── LICENSE
│   ├── Makefile
│   └── README.md
========================ALL FILES WITHIN frontend_xcagalo00 ARE THE PROJECT IMPLEMENTATION OF Ondrej Cagalinec======================
├── frontend_xcagalo00
│   ├── index.css
│   ├── index.html
│   ├── package.json
│   ├── package-lock.json
│   ├── src
│   │   ├── api.js <---------------------------API CALLS
│   │   ├── App.svelte
        ================================COMPONENTS======================================
│   │   ├── components
│   │   │   ├── board.svelte
│   │   │   ├── diffSelect.svelte
│   │   │   ├── homeButton.svelte
│   │   │   ├── inGameButton.svelte
│   │   │   ├── mineCount.svelte
│   │   │   └── scoreTable.svelte
│   │   ├── main.js
    	===============================MAIN PAGES======================================
│   │   ├── routes
│   │   │   ├── Home.svelte
│   │   │   ├── Multiplayer.svelte
│   │   │   ├── ScoreboardMulti.svelte
│   │   │   ├── ScoreboardSingle.svelte
│   │   │   └── Singleplayer.svelte
│   │   └── store.js <---------data transfer through diffrent routes
│   ├── uno.config.js
│   └── vite.config.js
================================================================================================================================
├── readme.txt
└── run_xcagalo00.sh
