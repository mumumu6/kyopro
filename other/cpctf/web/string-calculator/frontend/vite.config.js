import { defineConfig } from "vite";
import UnoCSS from "unocss/vite";

export default defineConfig({
  build: {
    minify: false,
    sourcemap: true,
  },
  plugins: [
    UnoCSS({
      safelist: ["text-emerald-300", "text-yellow-500", "text-rose-500"],
    }),
  ],
});
