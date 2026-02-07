<script>
    /*
     * File: board.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Home button for selecting game mode(single/multi)
     */
    export let text = ""; //button text content
    export let isActive = false; //used for diffrent styling if the button is selected
</script>

<button
    on:click
    class="font-sans border-3 border-solid p-8 rounded-lg bg-dark-300 transition-all duration-200 {isActive
        ? 'border-green-500 bg-blue-50 text-white scale-105 '
        : 'border-gray-400 hover:border-white text-white '}"
>
    {text}
</button>
