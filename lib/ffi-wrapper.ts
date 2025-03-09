const HWND_BROADCAST = Deno.UnsafePointer.create(0xffffn);
const WM_SYSCOMMAND = 0x0112;
const SC_MONITORPOWER = 0xf170;
const MONITOR_OFF = 2n;
const MONITOR_ON = -1n;

const user32dll = Deno.dlopen("user32.dll", {
  SendMessageW: {
    parameters: ["pointer", "u32", "u32", "isize"],
    result: "isize",
  },
  mouse_event: {
    parameters: ["u32", "i32", "i32", "u32", "u32"],
    result: "void",
  },
});

export function monitorOff() {
  user32dll.symbols.SendMessageW(
    HWND_BROADCAST,
    WM_SYSCOMMAND,
    SC_MONITORPOWER,
    MONITOR_OFF
  );
}

export function monitorOn() {
  user32dll.symbols.SendMessageW(
    HWND_BROADCAST,
    WM_SYSCOMMAND,
    SC_MONITORPOWER,
    MONITOR_ON
  );
  user32dll.symbols.mouse_event(0x0001, 10, 0, 0, 0);
}
