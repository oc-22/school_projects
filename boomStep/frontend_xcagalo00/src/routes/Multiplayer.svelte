<script>
    /*
     * File: Multiplayer.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Multiplayer game mode and viewing logic
     */
    import { push } from "svelte-spa-router";
    import { onDestroy } from "svelte";
    import { api } from "../api.js";
    import InGameButton from "../components/inGameButton.svelte";
    import Board from "../components/board.svelte";
    import DifficultySelector from "../components/diffSelect.svelte";
    import MineCount from "../components/mineCount.svelte";
    import { gameSettings } from "../store.js";

    //constant game names for multiplayer boards(required from BE for some reason)
    const GAME_P1 = "multi_p1";
    const GAME_P2 = "multi_p2";

    //difficulty array to select from
    const DIFF = {
        easy: { rows: 9, cols: 9, mines: 10 },
        medium: { rows: 16, cols: 16, mines: 40 },
        hard: { rows: 24, cols: 24, mines: 99 },
    };

    //player objects, each containing data from their own board
    let p1 = {
        grid: [],
        score: 0,
        isGameWon: false,
        isGameOver: false,
        name: "Player 1",
    };
    let p2 = {
        grid: [],
        score: 0,
        isGameWon: false,
        isGameOver: false,
        name: "Player 2",
    };

    let activePlayer = 1; //the default active board is number 1
    let winner = null;
    let difficulty = "easy";

    //default custom board settings(small for testing reasons)
    let customRows = 5;
    let customCols = 5;
    let customMines = 1;

    //Used to display the number of wins for each player
    let sessionWinsP1 = 0;
    let sessionWinsP2 = 0;

    //default timer settings
    let timerEnabled = false;
    let turnDuration = 10;
    let timeLeft = 10;
    let timerInterval = null;

    onDestroy(() => stopTimer());

    async function init() {
        try {
            stopTimer(); //stop the timer when initializing
            winner = null;
            activePlayer = activePlayer === 1 ? 2 : 1; //switch turns sfter each match
            let rows, cols, mines;

            //Object reset upon initialization
            p1 = {
                grid: [],
                score: 0,
                isGameWon: false,
                isGameOver: false,
                name: "Player 1",
            };
            p2 = {
                grid: [],
                score: 0,
                isGameWon: false,
                isGameOver: false,
                name: "Player 2",
            };

            //Load settings from store.js, passed from home
            timerEnabled = $gameSettings.timerOn;
            turnDuration = timerEnabled
                ? Number($gameSettings.timerValue) || 10
                : 0;
            timeLeft = turnDuration;

            if (difficulty === "custom") {
                rows = customRows;
                cols = customCols;
                mines = customMines;
            } else {
                rows = DIFF[difficulty].rows;
                cols = DIFF[difficulty].cols;
                mines = DIFF[difficulty].mines;
            }
            //Backend game creation for each player
            // We are providing: name, rows, cols, mines, mode
            await api.createGame(GAME_P1, { rows, cols, mines, difficulty });
            await api.createGame(GAME_P2, { rows, cols, mines, difficulty });

            //Display the boards for each player
            await updateBoard(1);
            await updateBoard(2);

            if (timerEnabled) startTurnTimer(); //start the timer for the first player
        } catch (e) {
            console.error("Multiplayer Init Error", e);
        }
    }

    //function to setup the game timer if timerEnabled
    function startTurnTimer() {
        stopTimer();
        timeLeft = turnDuration;
        timerInterval = setInterval(() => {
            timeLeft--;
            if (timeLeft <= 0) {
                //if time has expired, determinte the winner
                winner = activePlayer === 1 ? "PLAYER 2" : "PLAYER 1";
                stopTimer();
                updateBoard(activePlayer);
            }
        }, 1000);
    }

    //Timer stop
    function stopTimer() {
        if (timerInterval) clearInterval(timerInterval);
    }

    //switch turn for players if no winner yet
    function switchTurn() {
        if (winner) return;
        activePlayer = activePlayer === 1 ? 2 : 1;
        if (timerEnabled) startTurnTimer(); //reset the timer upon switching turns
    }

    //update board for players to determine winner or continue game
    async function updateBoard(playerNum) {
        const gameName = playerNum === 1 ? GAME_P1 : GAME_P2; //determine which game to gather data from

        try {
            //get all the current data from the API about the current game
            const data = await api.getGameStatus(gameName);
            const rawBoard = data.board;
            const status = data.status;

            // lose/win logic implementation
            const hasExploded = status === "over" || winner !== null;
            const hasWon = status === "won";

            let currentScore = 0;

            // transform each cell in the board based on the current game status
            const newGrid = rawBoard.map((row, r) =>
                row.map((val, c) => {
                    let isRevealed = false;
                    let textToDisplay = "";
                    let iconToDisplay = null;

                    let isFlagged = false;

                    //flagged cells have lowercase letters instead of uppercase
                    isFlagged = ["m", "e", "b"].includes(val) ? true : false;

                    if (hasExploded) {
                        //if mine was clicked, reveal all the mines
                        isRevealed = val !== "E" && val !== "e";
                    } else if (hasWon) {
                        //if game has ended in success, reveal all the mines
                        isRevealed = true;
                    } else {
                        //if the game has not ended, diplay only empty or value-weighted cells
                        isRevealed = val !== "E" && val !== "M" && !isFlagged;
                    }
                    //set the icon for the mine
                    if (["M", "X", "m"].includes(val) && isRevealed) {
                        iconToDisplay = "i-fa6-solid-land-mine-on";
                        textToDisplay = "";
                    } else {
                        textToDisplay = val;
                    }
                    //dont display any text if icon should be displayed
                    if (
                        ["B", "E"].includes(val) ||
                        (!hasExploded && val === "M")
                    ) {
                        textToDisplay = "";
                    }
                    //increase score
                    if (isRevealed && !["X", "M", "E"].includes(val)) {
                        currentScore++;
                    }
                    //Handle styiling based on previously set parameters
                    let css =
                        "w-8 h-8 border-2 border-solid rounded text-sm font-bold flex items-center justify-center cursor-pointer ";
                    if (isRevealed) {
                        if (val === "X") {
                            css += "bg-red-600 border-red-600";
                        } else if ((val === "M" || val === "m") && !hasWon) {
                            css +=
                                "bg-dark-800 border-dark-800 border-red-600 text-white";
                        } else if ((val === "M" || val === "m") && hasWon) {
                            css += "bg-green-600 border-green-600";
                        } else {
                            switch (val) {
                                case "1":
                                    css +=
                                        "text-blue-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Blue 1
                                case "2":
                                    css +=
                                        "text-green-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Green 2
                                case "3":
                                    css +=
                                        "text-red-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Red 3
                                case "4":
                                    css +=
                                        "text-purple-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Purple 4
                                case "5":
                                    css +=
                                        "text-orange-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Orange 5
                                case "6":
                                    css +=
                                        "text-teal-300 bg-gray-600 border-solid border-gray-600";
                                    break; // Cyan 6
                                case "7":
                                    css +=
                                        "text-gray-200 bg-gray-600 border-solid border-gray-600";
                                    break; // lignt gray 7
                                case "8":
                                    css +=
                                        "text-black bg-gray-600 border-solid border-black";
                                    break; // gray 8
                                default:
                                    css += "bg-gray-600 border-gray-600 "; // Fallback(not gonna happen)
                            }
                        }
                    } else if (isFlagged) {
                        //style the flagged cell
                        css +=
                            "bg-dark-400 hover:bg-dark-400 border-solid border-red-600 text-transparent";
                    } else {
                        //style the cell that is not flagged or revealed
                        css +=
                            "bg-dark-100 hover:bg-dark-400 border-t-gray-600 border-l-gray-600 border-r-gray-800 border-b-gray-800 text-transparent cursor-pointer";
                    }

                    return {
                        text: textToDisplay,
                        icon: iconToDisplay,
                        cssClass: css,
                        r,
                        c,
                    };
                }),
            );

            //update local data
            if (playerNum === 1) {
                p1.grid = newGrid;
                p1.score = currentScore;
                p1.isGameOver = hasExploded;
                p1.isGameWon = hasWon;
                p1 = p1; // Trigger update
            } else {
                p2.grid = newGrid;
                p2.score = currentScore;
                p2.isGameOver = hasExploded;
                p2.isGameWon = hasWon;
                p2 = p2; // Trigger update
            }
            if (hasExploded || hasWon) {
                // refresh to gather scores
                await refreshSessionData();
            }
            //Winner/Loser check
            if (hasExploded && !winner) {
                stopTimer();
                winner = playerNum === 1 ? "PLAYER 2" : "PLAYER 1";
            }
            if (hasWon && !winner) {
                stopTimer();
                winner = playerNum === 1 ? "PLAYER 1" : "PLAYER 2";
            }
        } catch (e) {
            console.error(`Update Error pre ${gameName}:`, e);
        }
    }
    //Function to handle each click event
    async function handleAction(playerNum, event) {
        if (winner) return;
        if (activePlayer !== playerNum) return;

        //gather game data and the current player
        const { row, col, kind } = event.detail;
        const gameName = playerNum === 1 ? GAME_P1 : GAME_P2;

        try {
            //we need a score(out internal score based on clicks)
            //to evaluate when the turn should be switched
            // this score only increases when a player clicks on a non-revealed cell
            const scoreBefore = playerNum === 1 ? p1.score : p2.score;

            //get this internal score afrer clicking
            await api.clickCell(gameName, row, col, kind);
            await updateBoard(playerNum);
            const scoreAfter = playerNum === 1 ? p1.score : p2.score;

            //only if we dont have a winner and the score has changed can we actually switch turns
            if (!winner && scoreAfter > scoreBefore) {
                switchTurn();
            }
        } catch (e) {
            console.error("Action error:", e);
        }
    }
    //Change listener for difficulty changes
    $: if (difficulty && difficulty !== "custom") init();

    //stop the timer and dont initialize another game until the player has
    // set the custom game parameters
    $: if (difficulty === "custom") {
        //dont time the game before its created
        stopTimer();
        timeLeft = $gameSettings.timerValue;
    }

    //gather session scores
    async function refreshSessionData() {
        try {
            const data = await api.getMultiplayerData();
            sessionWinsP1 = data.current.player1;
            sessionWinsP2 = data.current.player2;
        } catch (e) {
            console.error("Session Error:", e);
        }
    }

    //reset the scores and start counting them from 0
    // the BE saves the scores that is being reset
    async function handleEndSession() {
        await api.resetMultiplayerScore();
        await refreshSessionData();
        await init();
    }

    //the timer can be deleted right from the game itself
    function handleTimerDelete() {
        gameSettings.set({ timerOn: false });
        stopTimer();
        init();
    }
</script>

<div
    class="min-h-screen bg-dark font-sans flex flex-col items-center justify-center"
>
    <div class="bg-dark-200 p-6 rounded-lg shadow-lg">
        <div class="text-center mb-10">
            <h1 class="text-4xl font-sans text-gray-200 mb-4">BOOMstep</h1>
            <h2 class="text-xl font-sans text-gray-200 mb-4">Multiplayer</h2>

            {#if winner}
                <div class="text-green-500 text-3xl font-bold mb-4">
                    {winner} WINS!
                </div>
            {:else if timerEnabled}
                <p class="text-gray-500 text-sm mt-1">
                    Time limited mode, turn changes automatically
                </p>
            {:else}
                <p class="text-gray-400 text-lg font-sans">Free Play Mode</p>
            {/if}

            <div class="mt-6 flex flex-row justify-center gap-4">
                <InGameButton
                    text="Scoreboard"
                    color="yellow"
                    to="/ScoreboardMulti"
                    icon="i-fa6-solid-trophy"
                />
                <InGameButton
                    text="Finish match"
                    color="red"
                    icon="i-fa6-solid-square"
                    on:click={() => handleEndSession()}
                />
                <InGameButton
                    text="Menu"
                    color="blue"
                    to="/"
                    icon="i-fa6-solid-house"
                />
                <MineCount
                    mines={difficulty === "custom"
                        ? customMines
                        : DIFF[difficulty].mines}
                />
                <DifficultySelector bind:value={difficulty} />
            </div>

            {#if difficulty === "custom"}
                <div class="flex flex-col items-center gap-2">
                    <p class="text-gray-200 font-bold text-sm mb-1">
                        Custom Settings:
                    </p>

                    <div class="flex gap-2 w-full justify-center">
                        <input
                            type="number"
                            min="5"
                            bind:value={customRows}
                            class="bg-dark-400 text-gray-200 border-0 rounded-lg p-2 w-1/3 text-center outline-none focus:border-blue-400"
                        />
                        <input
                            type="number"
                            min="5"
                            bind:value={customCols}
                            class="bg-dark-400 text-gray-200 border-0 rounded-lg p-2 w-1/3 text-center outline-none focus:border-blue-400"
                        />
                    </div>

                    <label for="customSett" class="text-gray-200 text-sm mt-2">
                        Mines (Max: {customRows * customCols - 1})
                    </label>
                    <input
                        type="number"
                        min="1"
                        id="customSett"
                        max={customRows * customCols - 1}
                        bind:value={customMines}
                        class="bg-dark-400 text-gray-200 border border-0 rounded-lg p-2 w-1/3 text-center outline-none focus:border-blue-400"
                    />
                    <InGameButton
                        text="Create"
                        color="green"
                        on:click={() => init()}
                    />
                </div>
            {/if}
        </div>

        <div class="flex flex-row gap-7 items-center">
            <div
                class="flex flex-col items-center transition-all duration-300 {activePlayer ===
                1
                    ? 'scale-105 opacity-100'
                    : 'opacity-40 grayscale pointer-events-none'}"
            >
                <h2 class="text-2xl font-bold text-blue-500">PLAYER 1</h2>
                <div
                    class="bg-dark px-6 py-2 rounded-lg border-2 border-solid border-gray-500 text-gray-200 text-l font-mono"
                >
                    Score: {sessionWinsP1}
                </div>

                {#if p1.grid.length > 0}
                    <div class="p-2 rounded-lg">
                        <Board
                            grid={p1.grid}
                            on:cellAction={(e) => handleAction(1, e)}
                        />
                    </div>
                {/if}
            </div>
            {#if timerEnabled}
                <InGameButton
                    text="{timeLeft}s"
                    color="timer"
                    icon="i-fa-solid-clock"
                    on:click={handleTimerDelete}
                />
            {/if}
            <div
                class="flex flex-col items-center transition-all duration-300 {activePlayer ===
                2
                    ? 'scale-105 opacity-100'
                    : 'opacity-40 grayscale pointer-events-none'}"
            >
                <h2 class="text-2xl font-bold text-red-500">PLAYER 2</h2>
                <div
                    class="bg-dark px-6 py-2 rounded-lg border-2 border-solid border-gray-500 text-gray-200 text-l font-mono"
                >
                    Score: {sessionWinsP2}
                </div>

                {#if p2.grid.length > 0}
                    <div class="p-2 rounded-lg">
                        <Board
                            grid={p2.grid}
                            on:cellAction={(e) => handleAction(2, e)}
                        />
                    </div>
                {/if}
            </div>
        </div>
        {#if winner}
            <div class="flex justify-center m-t-4">
                <InGameButton
                    text="New Game"
                    color="gray"
                    on:click={init}
                    icon="i-fa6-solid-arrow-rotate-right"
                />
            </div>
        {/if}
    </div>
</div>
