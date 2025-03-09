const wrapper = Deno.dlopen(`./lib/wrapper.dll`, {
  monitorOff: { parameters: [], result: "void" },
  monitorOn: { parameters: [], result: "void" },
  setVolume: { parameters: ["f32"], result: "i64" },
  getVolume: { parameters: [], result: "f32" },
});

export const monitorOff = wrapper.symbols.monitorOff;
export const monitorOn = wrapper.symbols.monitorOn;
export const setVolume = wrapper.symbols.setVolume;
export const getVolume = wrapper.symbols.getVolume;
