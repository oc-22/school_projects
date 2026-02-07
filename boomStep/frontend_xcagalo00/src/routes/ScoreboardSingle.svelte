<script>
    /*
     * File: ScoreboardSingle.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Singleplayer scores page
     */
    import { onMount } from "svelte";
    import { push } from "svelte-spa-router";
    import { api } from "../api.js";
    import InGameButton from "../components/inGameButton.svelte";
    import ScoreTable from "../components/scoreTable.svelte";

    //data for all difficulties
    let easyScores = [];
    let mediumScores = [];
    let hardScores = [];
    let customScores = [];

    onMount(async () => {
        try {
            const data = await api.getScores();
            const allScores = data || [];

            // sort by time
            allScores.sort((a, b) => a.time_spent - b.time_spent);

            // categorize all the scores
            // BE return the game difficulty as well
            easyScores = allScores.filter((s) => s.game_mode === "easy");
            mediumScores = allScores.filter((s) => s.game_mode === "medium");
            hardScores = allScores.filter((s) => s.game_mode === "hard");
            customScores = allScores.filter((s) => s.game_mode === "custom");
        } catch (e) {
            console.error("ERROR Scoreboard:", e);
        }
    });
</script>

<div
    class="min-h-screen bg-dark font-sans flex items-center justify-center px-4"
>
    <div
        class="bg-dark-300 rounded-lg p-16 flex flex-col items-center gap-6 shadow-xl"
    >
        <div class="flex flex-col w-full items-center mb-5">
            <h1 class="text-4xl font-sans text-white tracking-wider">
                Hall of Fame
            </h1>
            <div class="flex flex-row items-center gap-4 justify-center">
                <InGameButton
                    text="Menu"
                    color="blue"
                    to="/"
                    icon="i-fa6-solid-house"
                />
                <InGameButton
                    text="Game"
                    color="yellow"
                    to="/singleplayer"
                    icon="i-fa6-solid-chess-board"
                />
            </div>
        </div>
        <div class="flex flex-row w-full gap-4">
            <ScoreTable title="Easy" scores={easyScores} color="green" />

            <ScoreTable title="Medium" scores={mediumScores} color="yellow" />

            <ScoreTable title="Hard" scores={hardScores} color="red" />

            <ScoreTable title="Custom" scores={customScores} color="blue" />
        </div>
    </div>
</div>
