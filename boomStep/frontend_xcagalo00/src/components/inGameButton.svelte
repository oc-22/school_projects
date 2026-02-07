<script>
    /*
     * File: inGameButton.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Buttons used on the game page, used mainly for navigation and actions during the game.
     *
     */
    import { createEventDispatcher } from "svelte";
    import { push } from "svelte-spa-router";

    export let text = "";
    export let color = "blue";
    export let to = "";
    export let gameOver = false;

    export let icon = "";

    const dispatch = createEventDispatcher();

    //Uno/svalte has issues with passing style from the caller, so we just make some constant ones that will be used
    const colors = {
        yellow: "bg-yellow-500 border-2 border-solid border-yellow-500 hover:bg-yellow-400 text-white hover:border-yellow-400",
        blue: "bg-blue-600 border-2 border-solid border-blue-600 hover:bg-blue-500 text-white hover:border-blue-500",
        timer: "bg-dark-400 border-2 border-solid border-dark-400 hover:border-red-700 text-white",
        gray: "bg-gray-600 border-2 border-solid border-gray-600 hover:bg-gray-500 hover:border-gray-500 text-white",
        red: "bg-red-600 border-2 border-solid border-red-600 hover:bg-red-500 text-white",
        green: "bg-green-600 border-2 border-solid border-green-600 hover:bg-green-500 text-white",
    };

    $: colorClasses = colors[color] || colors.blue;

    //Special case when no new page is loaded, just a generation of a new game
    function handleClick() {
        if (gameOver) {
            dispatch("newGame");
        }
        if (to) push(to);
    }
</script>

<button
    on:click={handleClick}
    on:click
    class="
    {colorClasses}
    flex items-center justify-center gap-2
    px-3 py-3 rounded-lg font-sans shadow-md
  "
>
    {#if icon}
        <span class={icon}></span>
    {/if}

    <span>{text}</span>
</button>
