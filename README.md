# ChenOS

![banner](https://img.shields.io/badge/version-v1.0.0-brightgreen)

ChenOS is an UNIX-like operating system kernel.

## Table of Contents

- [Completed](#completed)
- [Setup Env](#setup)
- [Usage](#usage)
- [License](#license)

## Completed

| **Module**         | **Status**         | **Description** |
| -------------- | ----------| ----------------|
| Multi Task | Compeleted | Preemptive multitasking(time-slice) |
| Memory Management | Compeleted | Memory paging |
| Filesystem | Compeleted |Implemeted basic syscall|
| Coming... |  | |

## Setup

You can setup the enviroment by Docker.

```
docker pull noterpopo/ubuntu-gcc-nasm
```

## Usage

Use this to run the kernel!

```
bochs -f bochs

```

Note: You shounld use this command under the default dir.

## License

[MIT © Noterpopo.](../LICENSE)
