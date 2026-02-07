<script>
    /*
     * File: mineCount.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: Displays the number of hidden mines within the game
     */
    export let mines = 0;
</script>

<div
    class="mb-1 p-3 bg-red-950 text-white rounded-lg border-solid border-2 border-red-600"
>
    <p class="m-0 opacity-100 text-white flex items-center justify-center">
        <span class="i-fa6-solid-flag text-gray-200 m-r-2"> </span>
        {mines}
    </p>
</div>
