<script>
    /*
     * File: App.svelte
     * Author: Ondrej Cagalinec(xcagalo00)
     * Description: integrator of routes
     *
     */
    //use router for navigation within modules
    import Router from "svelte-spa-router";

    // Import each route/component
    import Home from "./routes/Home.svelte";
    import SinglePlayer from "./routes/Singleplayer.svelte";
    import ScoreBoardSingle from "./routes/ScoreboardSingle.svelte";
    import Multiplayer from "./routes/Multiplayer.svelte";
    import ScoreBoardMultiplayer from "./routes/ScoreboardMulti.svelte";

    // Append each component to a route
    const routes = {
        "/": Home,
        "/singleplayer": SinglePlayer,
        "/scoreboard": ScoreBoardSingle,
        "/multiplayer": Multiplayer,
        "/scoreboardMulti": ScoreBoardMultiplayer,
    };
</script>

<Router {routes} />
