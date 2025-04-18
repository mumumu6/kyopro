const getFlag = () => process.env.FLAG ?? "";

const app = new (require("hono").Hono)();

app.use("*", require("@hono/node-server/serve-static").serveStatic({ root: "./frontend/dist" }));

app.post("/api/calc", async (c) => {
  const input = await c.req.text();

  try {
    if (input.length > 64) throw new Error("Input too long");
    if (/[()\[\].=]/.test(input)) throw new Error("Invalid characters in input");
    if (/delete|Function|fetch|\+\+|--/.test(input)) throw new Error("Invalid keywords in input");

    const result = eval(`(${input})`);

    return c.json(result);
  } catch (error) {
    c.status(400);
    return c.text(`${error.name}: ${error.message}`);
  }
});

app.get("/api/flag", require("hono/bearer-auth").bearerAuth({ token: btoa(getFlag()) }), (c) => {
  return c.text(getFlag());
});

require("@hono/node-server").serve(app, () => {
  console.log("Server running on http://localhost:3000");
});
