# GRUB-Mod-Tracer
A small GDB extension to trace dynamically‑loaded GRUB modules (e.g. `linux.mod`) at runtime.

> [!WARNING]
> This script was tested on **Ubuntu 24.04** with **GRUB 2.12** (`grubx64.efi`).  
> Ubuntu 24.04 was run inside **QEMU**.

## Usage
1. Start QEMU and obtain GRUB’s **image_base**.  
2. In GDB, attach to QEMU with the `target remote` command.  
3. Load the script with `source mod_trace.py`.  
4. Use `modwatch` to trace the module you want to monitor.

### Expected output
When the script is working correctly, GDB prints a line in the following format:  
`name=linux: init=0x???????? base=0x????????`

--- 
# Caution
As mentioned above, `mod_trace.py` has only been tested on Ubuntu 24.04 with GRUB 2.12 (`grubx64.efi`).  
If you are using a different environment, the breakpoint offset may vary.  
Should you need to adjust it, set the offset to the first instruction executed **immediately after** the call to `grub_dl_load_core_noinit`.
