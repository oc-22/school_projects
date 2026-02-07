# BoomStep

BoomStep is a full‑stack Minesweeper game consisting of a Go backend API and a Svelte frontend.  
It supports singleplayer and multiplayer modes, multiple difficulty levels, and scoreboards.  
The project was developed as a university assignment, with backend and frontend implemented separately and communicating via a REST API.

The backend is released under the MIT license.

---

## Features

- Classic Minesweeper gameplay  
- Multiple difficulty levels  
- Singleplayer mode  
- Multiplayer mode  
- Scoreboards for both modes  
- REST API backend written in Go  
- Frontend built with Svelte and Vite  

---

## Project Structure

---

## Backend (Go)

## Acknowledgements

The backend of this project is based on the open‑source repository  
https://github.com/arllanos/minesweeper-API  
created by GitHub user **arllanos**.

The original backend code is licensed under the MIT license, and the full license text is included in `backend/LICENSE`.
Modifications were made to integrate the backend with this project’s frontend and assignment requirements. 
also some features were added.


The backend is written in Go and follows a layered architecture:

- **API layer** – request handling and routing  
- **Service layer** – game logic and engine  
- **Domain layer** – core game structures  

### REST endpoints include

- Game creation  
- Game state updates  
- Score handling  

### Running the backend

- go run cmd/minesweeper-api/main.go

  
## Frontend (Svelte)

The frontend is built using Svelte and Vite and communicates with the backend through a REST API.  
Its structure separates UI components, application state, and network logic to keep the codebase maintainable and modular.

---

### Architecture Overview

#### Components (`src/components/`)
Contains reusable UI elements, including:

- Game board tiles  
- Timer and mine counter  
- Difficulty selector  
- Scoreboard tables  
- Multiplayer UI components  

Each component is implemented as a Svelte file, encapsulating markup, logic, and styling.

---

#### Routes (`src/routes/`)
Defines the main application views:

- Home screen  
- Singleplayer game  
- Multiplayer game  
- Scoreboard pages  

Routing is handled using lightweight Svelte routing.

---

#### API Communication (`src/api.js`)
A wrapper around `fetch` that provides functions for backend communication:

- Creating a new game  
- Revealing tiles  
- Flagging tiles  
- Fetching scoreboards  
- Joining multiplayer sessions  

This keeps network logic separate from UI components.

---

#### Global State (`src/store.js`)
Uses Svelte’s store system to maintain:

- Current game state  
- Multiplayer session state  
- Selected difficulty  
- User/session data  
- UI flags (loading, errors, etc.)  

Stores enable reactive updates across the entire application.

---

### Frontend Features

- Interactive Minesweeper board with reactive updates  
- Difficulty selection (beginner, intermediate, expert, custom)  
- Real‑time updates for multiplayer mode  
- Scoreboard pages for both game modes  
- Responsive layout  
- Fast development environment powered by Vite  

---


