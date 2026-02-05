# Boot Loader Entry Manager

A Bash utility for managing **systemd‑boot** loader entry `.conf` files.  
It provides commands for listing, filtering, duplicating, modifying, and removing bootloader entries stored in:
(or a custom directory via `-b`).

---

## Features

### List Entries (`list`)
Display boot entries with optional sorting and filtering.

**Options:**
- `-f` — sort alphabetically  
- `-s` — sort by `sort-key` (adds missing keys automatically)  
- `-t <pattern>` — filter by `title`  
- `-k <pattern>` — filter by `linux` kernel path  

**Example:**
```bash
./manager.sh list -s -t linux
./manager.sh remove "Arch Linux"
vutfit_default y
vutfit_default n
./manager.sh make-default /boot/loader/entries/arch.conf
./manager.sh duplicate -k /vmlinuz-linux -i /initramfs-linux.img \
    -t "Custom Linux" -a "quiet splash" --make-default
./manager.sh -b /my/custom/dir list
