<script>
    /*
     * File: scoreTable.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Table component for displaying singleplayer scores in various difficulties
     *
     */
    export let title = ""; //difficulty
    export let scores = []; //the actual scores/times to be displayed
    export let color = "blue"; //default text color of the title

    //funtion for conevring the time from nanoSeconds to regular seconds
    function formatDuration(nanoseconds) {
        if (!nanoseconds) return "N/A";
        const seconds = nanoseconds / 1000000000;
        return `${seconds.toFixed(1)} s`;
    }
</script>

<div
    class="bg-dark-200 rounded-lg shadow-lg w-auto flex flex-col h-auto self-start"
>
    <div
        class="p-4 flex flex-col items-center border-b border-gray-700 flex-shrink-0"
    >
        <h2 class="text-xl font-bold font-sans uppercase text-{color}-400">
            {title}
        </h2>
        <span class="text-xs text-gray-400 font-sans mt-1">
            Number of records: {scores.length}
        </span>
    </div>

    <div class="overflow-y-auto max-h-96 pb-2 custom-scrollbar">
        {#if scores.length === 0}
            <div class="p-6 text-center text-gray-500 text-sm italic">
                No games yet.
            </div>
        {:else}
            <table class="w-full text-left border-collapse">
                <tbody class="text-gray-300 text-sm">
                    {#each scores as score, index}
                        <tr
                            class="border-b border-gray-700 last:border-0 hover:bg-white/5 transition"
                        >
                            <td class="px-4 py-2 font-sans text-gray-400 w-10">
                                {index + 1}.
                            </td>
                            <td
                                class="py-2 px-4 text-right font-sans font-bold text-{color}-200"
                            >
                                {formatDuration(score.time_spent)}
                            </td>
                        </tr>
                    {/each}
                </tbody>
            </table>
        {/if}
    </div>
</div>
