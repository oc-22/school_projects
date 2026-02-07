/*
 * File: main.js
 * Author: Ondrej Cagalinec(xcagalo00)
 * Description: svelte entry point
 *
 */
import "virtual:uno.css";
import App from "./App.svelte";
import "../index.css";

const app = new App({
  target: document.getElementById("app"),
});

export default app;
