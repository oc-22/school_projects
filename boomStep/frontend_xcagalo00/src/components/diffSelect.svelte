<script>
    /*
     * File: diffSelect.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: difficulty selector used in single and multiplayer modes
     *
     */
    export let value = "easy";
</script>

<label class="rounded-lg">
    <select
        bind:value
        class="p-2 rounded-lg border-2 border-gray-600 font-sans bg-gray-600 text-white h-full"
    >
        <option value="easy" class="text-green-400 font-sans bg-gray-600">
            Easy
        </option>
        <option value="medium" class="text-yellow-400 font-sans bg-gray-600">
            Medium
        </option>
        <option value="hard" class="text-red-400 font-sans bg-gray-600">
            Hard
        </option>
        <option value="custom" class="text-blue-400 font-sans bg-gray-600">
            Custom
        </option>
    </select>
</label>
