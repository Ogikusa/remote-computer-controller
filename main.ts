import { Application, Router } from "@oak/oak";
import {
  getVolume,
  monitorOff,
  monitorOn,
  setVolume,
} from "./util/library-wrapper.ts";

const router = new Router();

router.post("/monitor-off", () => {
  monitorOff();
});

router.post("/monitor-on", () => {
  monitorOn();
});

router.post("/volume", (ctx) => {
  const volume = ctx.request.url.searchParams.get("volume") ?? "0.0";
  const volumeNumber = Number.parseFloat(volume);
  setVolume(volumeNumber / 100);
});

router.get("/volume", (ctx) => {
  ctx.response.body = getVolume() * 100;
});

const app = new Application();
app.use(router.routes());
app.use(router.allowedMethods());

app.use(async (ctx) => {
  try {
    await ctx.send({
      root: `${Deno.cwd()}/assets`,
      index: "index.html",
    });
  } catch (e) {
    if (e instanceof Deno.errors.NotFound) {
      ctx.throw(404);
      console.warn("Not found file was requested");
    }
  }
});

app.addEventListener("listen", ({ port, secure }) => {
  const protocol = secure ? "https://" : "http://";
  console.log(`The server started: ${protocol}localhost:${port}`);
});

app.listen({ port: 8080 });
