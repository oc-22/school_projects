<script>
    /*
     * File: board.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Game board component used in single and multiplayer modes
     *
     */
    import { createEventDispatcher } from "svelte";

    export let grid = []; // Array of arrays representing the game board

    //Dispatcher for calling a clickCell function within the actual file where board was used
    const dispatch = createEventDispatcher();
    function handleAction(r, c, kind) {
        dispatch("cellAction", { row: r, col: c, kind });
    }
</script>

{#if grid.length > 0}
    <div
        class="grid bg-dark-400 p-3 rounded border border-gray-400 select-none"
        style="grid-template-columns: repeat({grid[0].length}, 32px); gap: 2px;"
    >
        {#each grid as row}
            {#each row as cell}
                <button
                    on:click={() => handleAction(cell.r, cell.c, "click")}
                    on:contextmenu|preventDefault={() =>
                        handleAction(cell.r, cell.c, "flag")}
                    class={cell.cssClass}
                >
                    {#if cell.icon}
                        <span class="{cell.icon} text-lg"></span>
                    {:else}
                        {cell.text}
                    {/if}
                </button>
            {/each}
        {/each}
    </div>
{/if}
