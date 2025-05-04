# GRUB-Mod-Tracer
A small GDB extension for **module‑load tracing** of dynamically loaded GRUB modules (e.g. `linux.mod`).

> [!WARNING]
> This script was tested on **Ubuntu 24.04 (amd64)** with **GRUB 2.12** (`grubx64.efi`).  
> Ubuntu 24.04 was run inside **QEMU**.

## Usage
1. Start QEMU and obtain GRUB’s ImageBase.
2. In GDB, attach to QEMU with `target remote`.
3. Load the script: `source mod_trace.py`.
4. Trace a specific module with `modwatch`.

### Expected output
On a successful **module‑load tracing**, GDB prints:
`name=linux: init=0x???????? base=0x????????`

### How to get ImageBase of GRUB?
After building the ImgBaseDebug folder with EDK II, transfer ImgBaseDebug.efi to your Ubuntu VM in QEMU and run it from the UEFI Shell to obtain GRUB’s ImageBase.
- Copy your ImgBaseDebug folder into MdeModulePkg/Application/, then add its .inf to the [Components] section of MdeModulePkg.dsc and build.
- If the path to grubx64.efi is different, modify it accordingly.

--- 
# Caution
As mentioned above, `mod_trace.py` has only been tested on Ubuntu 24.04 with GRUB 2.12 (`grubx64.efi`).  
If you are using a different environment, the breakpoint offset may vary.  
Should you need to adjust it, set the offset to the first instruction executed **immediately after** the call to `grub_dl_load_core_noinit`.

# Reference
- Structure: https://elixir.bootlin.com/grub/grub-2.12/source/include/grub/dl.h#L174
- Load Core: https://elixir.bootlin.com/grub/grub-2.12/source/grub-core/kern/dl.c#L682
- EDK2 Build: https://github.com/tianocore/tianocore.github.io/wiki/Common-instructions
