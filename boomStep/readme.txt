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

==============================================================================================
=======================USE AND SETUP OF frontend_xkostej00====================================
==============================================================================================
The frontend part of the application is implemented using the React library and can be run
locally using the Node.js environment. After cloning the repository, dependencies must first be
installed in the root directory of the frontend part using the npm install command. The frontend
can then be started with the npm run dev command, which launches a development server accessible
in the browser (typically at http://localhost:5173/).

The backend part of the application must be started separately in a second terminal. In the backend
directory, the server can be launched using the go run cmd/minesweeper-api/main.go command.
The frontend then communicates with the running backend via a REST API, assuming the appropriate
backend address and port are configured in the API communication layer.
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
========================ALL FILES WITHIN frontend_xkostej00 ARE THE PROJECT IMPLEMENTATION OF Jan Kostečka======================
├── frontend_xkostej00
│   ├── eslint.config.js
│   ├── index.html
│   ├── package.json
│   ├── package-lock.json
│   ├── postcss.config.js
│   ├── public
│   │   └── vite.svg
│   ├── src
│   │   ├── api <-------------API CALLS
│   │   │   ├── client.ts
│   │   │   └── mines.ts
│   │   ├── App.css
│   │   ├── App.tsx
│   │   ├── assets
│   │   │   └── react.svg
        ================================COMPONENTS======================================
│   │   ├── components
│   │   │   ├── ConfirmModal.tsx
│   │   │   ├── home_buttons.tsx
│   │   │   ├── ScoresMulti.tsx
│   │   │   └── ScoresSingle.tsx
│   │   ├── index.css
│   │   ├── main.tsx
        ===============================MAIN PAGES======================================
│   │   └── pages
│   │       ├── Home.tsx
│   │       ├── MultiPlayer.tsx
│   │       └── SinglePlayer.tsx
│   ├── tailwind.config.js
│   ├── tsconfig.app.json
│   ├── tsconfig.json
│   ├── tsconfig.node.json
│   └── vite.config.ts
========================================================================================================================
├── readme.txt
└── run_xcagalo00.sh
