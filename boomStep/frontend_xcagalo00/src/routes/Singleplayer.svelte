<script>
    /*
     * File: Singleplayer.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Singleplayer game mode and viewing logic
     */
    import { push } from "svelte-spa-router";
    import { api } from "../api.js";
    import InGameButton from "../components/inGameButton.svelte";
    import Board from "../components/board.svelte";
    import DifficultySelector from "../components/diffSelect.svelte";
    import { gameSettings } from "../store.js";

    //difficulty presets
    const DIFF = {
        easy: { rows: 9, cols: 9, mines: 10 },
        medium: { rows: 16, cols: 16, mines: 40 },
        hard: { rows: 24, cols: 24, mines: 99 },
    };

    //gamename for the BE
    const GAME_NAME = "singleplayer";

    //default settings
    let difficulty = "easy";
    let grid = [];
    let isGameOver = false;
    let isGameWon = false;

    //time settings
    let timerEnabled = false;
    let timerInterval = null;
    let timeLeft = 0;
    let isTimeUp = false;

    //default custom size
    let customRows = 5;
    let customCols = 5;
    let customMines = 1;

    //Create the board on BE or write over the old one
    async function init() {
        try {
            //Initial data
            let rows, cols, mines;
            stopTimer();
            isGameOver = false;
            isGameWon = false;
            grid = [];

            //set the size to custom if selected
            if (difficulty === "custom") {
                rows = customRows;
                cols = customCols;
                mines = customMines;
            } else {
                rows = DIFF[difficulty].rows;
                cols = DIFF[difficulty].cols;
                mines = DIFF[difficulty].mines;
            }

            //Decide wheter the timer is to be used
            timerEnabled = $gameSettings.timerOn;
            if (timerEnabled) {
                timeLeft = $gameSettings.timerValue;
                startTimer();
            }

            //Create the board on BE or write over the old one
            await api.createGame(GAME_NAME, { rows, cols, mines, difficulty });
            await loadAndTransformBoard();
        } catch (e) {
            console.error("Initialization error:", e);
        }
    }

    //function initializing the timer if the game mode is selected
    function startTimer() {
        timerInterval = setInterval(() => {
            timeLeft--;
            if (timeLeft <= 0) {
                handleTimeUp();
            }
        }, 1000);
    }

    //function stopping the timer if the game mode is selected
    function stopTimer() {
        if (timerInterval) clearInterval(timerInterval);
    }

    //handle of the timer running out
    function handleTimeUp() {
        stopTimer();
        isTimeUp = true;
        loadAndTransformBoard();
    }

    //Deletion of timer upon the press of it on the board
    function handleTimerDelete() {
        isTimeUp = false;
        gameSettings.set({ timerOn: false });
        stopTimer();
        init();
    }

    //draw each cell of the board based on the game status
    async function loadAndTransformBoard() {
        try {
            //get the board from the API
            const rawBoard = await api.getBoard(GAME_NAME);

            //get all the information about the game
            const getStatus = await api.getGameStatus(GAME_NAME);

            //Set the gameover flags
            const hasExploded = getStatus.status === "over";
            isGameWon = getStatus.status === "won";

            //habdle gameover
            if (hasExploded || isTimeUp) {
                isGameOver = true;
                stopTimer();
                isTimeUp = false;
            } else {
                isGameOver = false;
            }

            //iterate through each cell
            grid = rawBoard.map((row, r) =>
                row.map((val, c) => {
                    //helper variables
                    let textToDisplay = "";
                    let iconToDisplay = null;
                    let isRevealed = false;
                    let isFlagged = false;

                    //flagged cells are marked by a lowercase letter from the BE
                    if (["e", "b", "m"].includes(val)) {
                        isFlagged = true;
                    } else {
                        isFlagged = false;
                    }

                    if (isGameOver) {
                        //reveal the cell if its not and empty one
                        isRevealed = val !== "E" && val !== "e";
                    } else if (getStatus.status === "won") {
                        //reveal all the cells sice only mines remain unrevealed
                        isRevealed = val !== "E" && val !== "e";
                    } else {
                        //reveal the cell if its not flagged and not empty/hidden
                        isRevealed =
                            !isFlagged &&
                            val !== "E" &&
                            val !== "M" &&
                            isFlagged === false;
                    }

                    if (["M", "X", "m"].includes(val)) {
                        //set the icon if we are revealing a mine
                        iconToDisplay = "i-fa6-solid-land-mine-on";
                        textToDisplay = "";
                    } else if (["B", "E"].includes(val)) {
                        //set the icon if we are revealing a bomb
                        textToDisplay = "";
                    } else {
                        //set the text if we are revealing a number
                        iconToDisplay = "";
                        textToDisplay = val;
                    }

                    if (
                        ["B", "E"].includes(val) ||
                        (!isGameOver && val === "M")
                    ) {
                        //dont display text if icon of the bomb should be displayed
                        textToDisplay = "";
                    }

                    //handle the styling based on the previous flags
                    let css =
                        "w-8 h-8 border-2 border-solid rounded text-sm font-bold flex items-center justify-center cursor-pointer ";

                    if (isRevealed) {
                        if (val === "X") {
                            css += "bg-red-600 border-red-600";
                        } else if (
                            (val === "M" || val === "m") &&
                            getStatus.status !== "won"
                        ) {
                            css +=
                                "bg-dark-800 border-red-600 text-white text-sm";
                        } else if (
                            (val === "M" || val === "m") &&
                            getStatus.status === "won"
                        ) {
                            css +=
                                "bg-green-600 text-white text-sm border-green-600";
                        } else {
                            switch (val) {
                                case "1":
                                    css +=
                                        "text-blue-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Blue 1
                                case "2":
                                    css +=
                                        "text-green-500 bg-gray-600 border-solid border-gray-600";
                                    break; // GReen 2
                                case "3":
                                    css +=
                                        "text-red-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Red 3
                                case "4":
                                    css +=
                                        "text-purple-500 bg-gray-600 border-solid border-gray-600";
                                    break; // Purple 4                                case "5":
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
                                    break; // light-gray 7
                                case "8":
                                    css +=
                                        "text-black bg-gray-600 border-solid border-black";
                                    break; // black 8
                                default:
                                    css += "bg-gray-600 border-gray-600 "; // Fallback(not gonna happen)
                            }
                        }
                    } else if (isFlagged) {
                        //style of the flagged cell
                        css +=
                            "bg-dark-400 hover:bg-dark-400 border-solid border-red-600 text-transparent";
                    } else {
                        //style of an unrevealed cell
                        css +=
                            "bg-dark-100 border-solid hover:bg-dark-400 border-t-gray-600 border-l-gray-600 border-r-gray-800 border-b-gray-800 text-transparent";
                    }

                    return {
                        text: textToDisplay,
                        cssClass: css,
                        icon: iconToDisplay,
                        r: r,
                        c: c,
                    };
                }),
            );
        } catch (e) {
            console.error(e);
        }
    }

    //difficulty change listener
    $: if (difficulty && difficulty !== "custom") init();

    //custom difficulty setting listener, the timer is to be stopped if the
    // difficulty is changed to custom
    $: if (difficulty === "custom") {
        stopTimer();
        timeLeft = $gameSettings.timerValue;
    }

    //handle a cell click on the board and send a request to the server
    async function onBoardAction(event) {
        if (isGameOver) return;
        const { row, col, kind } = event.detail;

        try {
            await api.clickCell(GAME_NAME, row, col, kind);

            await loadAndTransformBoard();
        } catch (e) {
            console.error("Click error:", e);
        }
    }
</script>

<div
    class="bg-dark flex flex-col items-center justify-center min-h-screen font-sans"
>
    <div
        class="bg-dark-200 rounded-lg p-16 flex flex-col items-center gap-6 shadow-xl"
    >
        <h1 class="text-gray-100 text-3xl font-bold">BOOMstep</h1>

        {#if isGameOver}
            <p class="text-red-500 font-bold">GAME OVER</p>
        {:else if isGameWon}
            <p class="text-green-600 font-bold">YOU WON!</p>
        {:else}
            <p class="text-gray-300 mb-1">
                Left click to reveal, Right click to flag
            </p>
            <div
                class="mb-1 p-3 bg-red-950 text-white rounded-lg border-solid border-2 border-red-600"
            >
                <p
                    class="m-0 opacity-100 text-white flex items-center justify-center"
                >
                    <span class="i-fa6-solid-flag text-gray-200 m-r-2"> </span>
                    {difficulty === "custom"
                        ? customMines
                        : DIFF[difficulty].mines}
                </p>
            </div>
        {/if}

        <div class="flex gap-5 text-center items-center justify-center">
            <DifficultySelector bind:value={difficulty} />
            <InGameButton
                text="Menu"
                color="blue"
                to="/"
                icon="i-fa6-solid-house"
            />
            <InGameButton
                text="Scoreboard"
                color="yellow"
                to="/scoreboard"
                icon="i-fa6-solid-trophy"
            />
            {#if timerEnabled}
                <InGameButton
                    text="{timeLeft}s"
                    color="timer"
                    icon="i-fa-solid-clock"
                    on:click={handleTimerDelete}
                />
            {/if}
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
        <Board {grid} on:cellAction={onBoardAction} />
        {#if isGameOver || timerEnabled || isGameWon}
            <InGameButton
                text="Retry"
                color="gray"
                icon="i-fa6-solid-arrow-rotate-right"
                gameOver={true}
                on:newGame={init}
            />
        {/if}
    </div>
</div>
