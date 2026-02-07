/*
 * File: store.js
 * Author: Ondrej Cagalinec(xcagalo00)
 * Description: Used for passing data from main screen to game
 *
 */

import { writable } from "svelte/store";

export const gameSettings = writable({
  timerOn: false,
  timerValue: 60,
});
