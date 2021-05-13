# ChenOS

[中文](./README-zh.md) | [English](./README.md)

![banner](https://img.shields.io/badge/version-v1.0.0-brightgreen)

ChenOS 是一个类UNIX操作系统内核。

## 目录

- [已完成](#已完成)
- [配置运行环境](#配置运行环境)
- [使用](#使用)
- [许可](#许可)

## 已完成

| **模块**         | **状态**         | **描述** |
| -------------- | ----------| ----------------|
| 多任务 | 已完成 | 基于时间片的抢占式调度 |
| 内存管理 | 已完成 | 分页内存管理 |
| 文件系统 | 已完成 |实现基本系统调用|
| Coming... |  | |

## 配置运行环境

通过docker部署环境

```
docker pull noterpopo/ubuntu-gcc-nasm
```

## 使用

使用下面命令运行内核

```
bochs -f bochs
```

Note: 在根目录下使用该命令

## 许可

[MIT © Noterpopo.](../LICENSE)
