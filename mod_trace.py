# modtrace.py
import gdb, struct

OFF_NAME = 0x0      # char *name
OFF_INIT = 0x30     # void (*init)(mod)
OFF_BASE = 0x40     # void *base

def rd_qword(inf, addr):
    return struct.unpack("<Q", inf.read_memory(addr, 8))[0]

class ModPrint(gdb.Breakpoint):
    def __init__(self, addr, watch_name):
        super().__init__(f"*0x{addr:x}", internal=False)
        self.watch_name = watch_name

    def stop(self):
        mod = int(gdb.parse_and_eval("$rax"))
        if not mod:
            return False

        inf = gdb.selected_inferior()

        name_ptr = rd_qword(inf, mod + OFF_NAME)
        name = inf.read_memory(name_ptr, 64).tobytes().split(b'\0')[0].decode(errors="ignore")
        init_ptr = rd_qword(inf, mod + OFF_INIT)
        base     = rd_qword(inf, mod + OFF_BASE) + 0x20

        gdb.write(f"name={name:<13}: init=0x{init_ptr:X} base=0x{base:X}\n")

        return name == self.watch_name

class ModWatch(gdb.Command):
    """
    Usage:  modwatch <image_base(GRUB)> <module_name>
    Example: modwatch 0x7BB12000 linux
    """
    def __init__(self):
        super().__init__("modwatch", gdb.COMMAND_USER)

    def invoke(self, arg, from_tty):
        argv = gdb.string_to_argv(arg)
        if len(argv) != 2:
            gdb.write("usage: modwatch <image_base(GRUB)> <module_name>\n")
            return

        img_base = int(argv[0], 16)
        addr = img_base + 0x48C3 # breakpoint offset
        ModPrint(addr, argv[1])

ModWatch()
