/*
 * File: api.js
 * Author: Ondrej Cagalinec(xcagalo00)
 * Description: Api call functions for clearer code within each route/page
 *
 */

/*
 * Helper function for making API requests (to avoid repetition)
 * @param {string} path - The API endpoint path
 * @param {string} method - The HTTP method (e.g., "GET", "POST", "PUT")
 * @param {object} body - The request body (optional)
 */
async function request(path, method, body = null) {
  const options = {
    method: method,
    headers: { "Content-Type": "application/json" },
  };

  if (body) {
    options.body = JSON.stringify(body);
  }

  const response = await fetch("/api" + path, options);

  if (!response.ok) {
    throw new Error(`API Error: ${response.status} ${response.statusText}`);
  }

  return response;
}

// Export exact data for the game
export const api = {
  // Create the game
  createGame: async (name, difficultyConfig) => {
    return request("/games", "PUT", {
      name: name,
      rows: difficultyConfig.rows,
      cols: difficultyConfig.cols,
      mines: difficultyConfig.mines,
      mode: difficultyConfig.difficulty,
    });
  },

  // Click on a cell
  clickCell: async (gameName, row, col, kind) => {
    return request(`/games/${gameName}/click`, "POST", {
      row,
      col,
      kind,
    });
  },

  // Load the board
  getBoard: async (gameName) => {
    const response = await request(`/games/${gameName}/board`, "GET");
    const text = await response.text();
    return text ? JSON.parse(text) : [];
  },

  //Get all the information within the Game data structure
  getGameStatus: async (gameName) => {
    const response = await request(`/games/${gameName}`, "GET");

    return response.json();
  },
  //Get singleplayer score data from BE
  getScores: async () => {
    // global endpoint call /scores
    const response = await request("/scores", "GET");
    return response.json();
  },
  //Get multiplayer leaderboard data
  getMultiplayerData: async () => {
    const response = await request("/multiplayer/data", "GET");
    return response.json();
  },

  //Start a new multiplayer score count
  resetMultiplayerScore: async () => {
    const response = await request("/multiplayer/reset", "POST");
    return response.json();
  },
};
