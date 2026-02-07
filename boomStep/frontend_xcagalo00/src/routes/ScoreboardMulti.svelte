<script>
    /*
     * File: ScoreboardMulti.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Multiplayer scores page
     */
    import { onMount } from "svelte";
    import { push } from "svelte-spa-router";
    import { api } from "../api.js";
    import InGameButton from "../components/inGameButton.svelte";

    let history = [];
    //get the multiplayer data upon mount/enter
    onMount(async () => {
        try {
            const data = await api.getMultiplayerData();

            history = (data.history || []).reverse();
        } catch (e) {
            console.error("Chyba Scoreboard Multi:", e);
        }
    });
</script>

<div
    class="min-h-screen justify-center bg-dark font-sans flex flex-col items-center px-4"
>
    <div
        class="flex flex-col justify-center items-center rounded-lg p-6 shadow-lg bg-dark-200"
    >
        <div class="flex flex-col justify-center items-center mb-10">
            <div class="flex flex-col items-center">
                <h1 class="text-4xl font-bold text-white">Match History</h1>
                <p class="text-gray-400 text-sm mt-1">
                    History of multiplayer matches
                </p>
            </div>
            <div class="flex flex-row items-center gap-4">
                <InGameButton
                    text="Menu"
                    color="blue"
                    to="/"
                    icon="i-fa6-solid-house"
                />
                <InGameButton
                    text="Multiplayer"
                    color="yellow"
                    to="/multiplayer"
                    icon="i-fa6-solid-chess-board"
                />
            </div>
        </div>

        {#if history.length === 0}
            <div class="bg-dark rounded-lg p-10 text-center shadow-xl">
                <h1 class="text-2xl font-bold text-gray-200 mb-2">
                    No records yet
                </h1>
                <p class="text-gray-400 mb-6">
                    Finish a session to see your results here.
                </p>
            </div>
        {:else}
            <div class="flex flex-col gap-4">
                {#each history as session, index}
                    <div
                        class="bg-dark-400 rounded-lg p-6 shadow-lg flex flex-col md:flex-row items-center justify-between"
                    >
                        <div
                            class="flex flex-col items-center md:items-start md:mb-0 w-32"
                        >
                            <span class="text-xs text-gray-400 font-bold"
                                >RECORD</span
                            >
                            <span class="text-2xl text-white font-bold"
                                >#{history.length - index}</span
                            >
                        </div>

                        <div class="flex items-center gap-8 md:gap-16">
                            <div class="flex flex-col items-center">
                                <span
                                    class="text-blue-500 font-bold text-sm mb-1"
                                    >PLAYER 1</span
                                >
                                <span class="text-4xl font-bold text-blue-500">
                                    {session.player1}
                                </span>
                            </div>

                            <div class="flex flex-col items-center">
                                <span
                                    class="text-red-500 font-bold text-sm mb-1"
                                    >PLAYER 2</span
                                >
                                <span class="text-4xl font-bold text-red-500">
                                    {session.player2}
                                </span>
                            </div>
                        </div>

                        <div class="w-32 text-center md:text-right md:mt-0">
                            {#if session.player1 > session.player2}
                                <span
                                    class="bg-blue-950 text-blue-400 px-3 py-1 rounded-lg text-sm font-bold"
                                >
                                    P1 WON
                                </span>
                            {:else if session.player2 > session.player1}
                                <span
                                    class="bg-red-950 text-red-400 px-3 py-1 rounded-lg text-sm font-bold"
                                >
                                    P2 WON
                                </span>
                            {:else}
                                <span
                                    class="bg-gray-700 text-gray-300 px-3 py-1 rounded text-sm font-bold"
                                >
                                    DRAW
                                </span>
                            {/if}
                        </div>
                    </div>
                {/each}
            </div>
        {/if}
    </div>
</div>
