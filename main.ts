import { Application, Router } from "@oak/oak";
import { monitorOff, monitorOn, setVolumeToZero } from "./util/library-wrapper.ts";

const router = new Router();

router.post("/monitor-off", () => {
  monitorOff();
});

router.post("/monitor-on", () => {
  monitorOn();
});

router.post("/volume-zero", () => {
  setVolumeToZero();
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

app.listen({ port: 8080 });
