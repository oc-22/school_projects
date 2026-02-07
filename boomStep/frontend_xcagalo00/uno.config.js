// uno.config.js
import { defineConfig, presetUno, presetWebFonts, presetIcons } from "unocss";

export default defineConfig({
  presets: [
    presetUno(),

    presetWebFonts({
      provider: "google",
      fonts: {
        sans: [
          {
            name: "Inter",
            weights: ["400", "500", "600", "700"],
          },
          {
            name: "sans-serif",
            provider: "none",
          },
        ],
      },
    }),

    presetIcons({
      scale: 1.2,
      extraProperties: {
        display: "inline-block",
        "vertical-align": "middle",
      },
    }),
  ],
});
