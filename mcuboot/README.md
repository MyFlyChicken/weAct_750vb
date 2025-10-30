# MCUBoot for WeAct STM32H750

This folder contains configuration overlays to build the upstream MCUBoot bootloader for the custom board `weact_stm32h750`.

What it does:
- Builds MCUBoot to run from internal flash (128KB @ 0x08000000)
- Defines external QSPI (W25Q64) partitions: image-0, image-1, and image-scratch
- Enables Direct-XIP so the application runs directly from external flash

## Build

Use the VS Code task "MCUBoot Build" or run (optional):

```sh
# Optional: from repo root
source $HOME/zephyrproject/zephyr/zephyr-env.sh
$HOME/zephyrproject/.venv/bin/west build -p auto -b weact_stm32h750 \
  $HOME/zephyrproject/bootloader/mcuboot/boot/zephyr -d build-mcuboot \
  -- -DBOARD_ROOT=$PWD -DOVERLAY_CONFIG=$PWD/mcuboot/prj.conf \
     -DDTC_OVERLAY_FILE=$PWD/mcuboot/boards/weact_stm32h750.overlay
```

Artifacts will be in `build-mcuboot/zephyr/` (e.g., `zephyr.hex`, `zephyr.bin`).

## Flashing

Use the VS Code task "MCUBoot Flash". You may switch runner with `-r openocd` or `-r jlink` depending on your setup.

Note: If the debugger cannot attach (RDP or wiring issues), use ST tools to mass-erase/unlock, ensure NRST is connected, and try connect-under-reset.

## App integration notes

To make your app bootable by MCUBoot:
- Ensure the app's devicetree chooses `zephyr,flash = &w25q64_qspi;` and `zephyr,code-partition = &slot0_partition;`
- In `prj-learn/prj.conf`, enable signing:
  - `CONFIG_BOOTLOADER_MCUBOOT=y`
  - Optional: `CONFIG_MCUBOOT_IMGTOOL_SIGN_VERSION="0.0.1"`
  - Optional: set a custom key via `CONFIG_BOOT_SIGNATURE_KEY_FILE="/path/to/key.pem"`

Then rebuild your app. The signed image will be `build-learn/zephyr/zephyr.signed.bin`/`.hex`. Flash MCUBoot first to internal flash, then program the signed app to external flash slot 0 (via your programmer or a small stage-1 loader).
