<script>
    /*
     * File: Home.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Landing page for the game, allows user to choose between singleplayer and multiplayer modes, also select timer mode
     */
    import { push } from "svelte-spa-router"; //used for route navigation
    import HomeButton from "../components/homeButton.svelte";
    import { gameSettings } from "../store.js"; //place to store the selected settings

    let active = "single"; //set the default selected mode
    let timerActive = false; //set the defaulut timer mode
    let timerValue = 60; //set the default timer value

    const handleClick = (mode) => {
        //switch modes based on which button is clicked
        active = mode;
    };

    //set the data to what the user has selected on the landing page
    function handleStart() {
        gameSettings.set({
            timerOn: timerActive,
            timerValue: timerValue,
        });
        //where to navigate based on the selected mode
        if (active === "single") {
            push("/singleplayer");
        } else {
            push("/multiplayer");
        }
    }
</script>

<div class="flex items-center justify-center min-h-screen bg-dark">
    <div class="flex flex-col items-center gap-5 p-12 bg-dark-200 rounded-lg">
        <h1 class="text-5xl font-sans text-gray-100">BOOMstep</h1>
        <p class="text-gray-100 font-sans">Choose your game mode:</p>

        <div class="flex gap-6">
            <HomeButton
                text="Singleplayer"
                isActive={active === "single"}
                on:click={() => handleClick("single")}
            />
            <HomeButton
                text="Multiplayer"
                isActive={active === "multi"}
                on:click={() => handleClick("multi")}
            />
        </div>

        <label class="text-gray-100 flex items-center gap-2 font-sans">
            Timer mode:
            <input
                type="checkbox"
                bind:checked={timerActive}
                class="w-5 h-5 accent-blue-500 font-sans"
            />
        </label>

        {#if timerActive}
            <div class="flex flex-col items-center gap-3 p-5 rounded-lg">
                <label for="timer-input" class="font-sans text-gray-100">
                    Set the timer (seconds)
                </label>
                <input
                    id="timer-input"
                    type="number"
                    min="1"
                    max="999"
                    bind:value={timerValue}
                    class="p-1 border rounded"
                />
            </div>
        {/if}

        <button
            class="p-4 text-gray-200 font-sans bg-green-500 rounded-lg border-solid border-0 hover:bg-green-400 transition"
            on:click={handleStart}
        >
            <span class="i-fa6-solid-play m-r-2"></span>
            Start game
        </button>
    </div>
</div>
