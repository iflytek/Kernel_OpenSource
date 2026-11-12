# iFLYTEK Learning Tablet Kernel Open Source

This repository publishes the Linux kernel and U-Boot source code for iFLYTEK
learning tablets, in compliance with the GNU General Public License version 2
(GPL-2.0).

The source code is organized in per-device branches:

- **`android_9_kernel_*`** — Linux kernel 4.14.98 source (Android 9)
- **`android_9_uboot_*`** — U-Boot 2015.07 bootloader source

Platform: UNISOC UD710

## Branches & Supported Devices

| Branch | Device | Base Tag |
| :-: | :-: | :-: |
| [android_9_uboot_UD710](https://github.com/iflytek/Kernel_OpenSource/tree/android_9_uboot_UD710) / [android_9_kernel_UD710](https://github.com/iflytek/Kernel_OpenSource/tree/android_9_kernel_UD710) | A10, C10, C10 Pro, C10S, P30, Q20, Q30, S30, X2 Pro, X3 Pro | MOCORDROID9.0_Trunk_19B_ROC1_ORCA_W20.51.4 |
| [android_9_uboot_UD710_T10](https://github.com/iflytek/Kernel_OpenSource/tree/android_9_uboot_UD710_T10) / [android_9_kernel_UD710_T10](https://github.com/iflytek/Kernel_OpenSource/tree/android_9_kernel_UD710_T10) | T10 | MOCORDROID9.0_Trunk_19B_ROC1_ORCA_W20.51.4 |

## How to Get the Source Code

1. Check your device model on the tablet: **Settings → About Tablet**.
2. Find the matching row in the table above.
3. Download the source from the corresponding branch, e.g.:

   ```bash
   # Kernel source for T10
   git clone -b android_9_kernel_UD710_T10 https://github.com/iflytek/Kernel_OpenSource.git kernel

   # U-Boot source for T10
   git clone -b android_9_uboot_UD710_T10 https://github.com/iflytek/Kernel_OpenSource.git uboot
   ```

   Or download a branch archive directly from the branch page
   (**Code → Download ZIP**).

> **Note:** If your device model is not listed in the table above, please
> [open an issue](https://github.com/iflytek/Kernel_OpenSource/issues/new)
> and we will publish the corresponding source.

## Reporting Issues

If you find any problem with the published source code — for example missing
sources, a branch that does not match your device, build failures, or license
questions — please report it via
[GitHub Issues](https://github.com/iflytek/Kernel_OpenSource/issues).

Before opening a new issue, please search the existing ones to avoid
duplicates. When reporting, include as much of the following as possible:

- **Device model** (e.g. T10, X2 Pro)
- **Branch** (e.g. `android_9_kernel_UD710_T10`)
- **Base Tag** (see the table above)
- **Description of the problem** and steps to reproduce it
- **Build environment and error logs**, if applicable

We will follow up on reported issues as soon as possible.

## License

The kernel and U-Boot source code in this repository are distributed under the
[GNU General Public License v2.0](LICENSE.md).
