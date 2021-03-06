# DinoOL

## 使用说明
[前往查看](https://brownlzy.github.io/2021/07/02/DinoOL/)

# 关于DinoOL

相信不少人曾经在断网时偶然发现了谷歌浏览器自带的小游戏，如果你正在使用谷歌浏览器，可以[右键复制链接前往游戏](chrome://dino)。此游戏是使用C++语言编写的的复刻增强版，增加了横向走位、双人对战、局域网联机功能。游戏中大部分图片资源来自原版游戏，有小部分是我用PS原创的。

本文是DinoOL的使用说明书，当然，在不看说明书的情况下自己摸索也很容易上手。
<!--more-->

# DinoOL使用说明

## 系统要求

Windows 10及以上 **64位**，100MB以上硬盘空间，100MB以上运行内存，需包含基本vc运行时。

## 安装

注：本游戏提供完整安装包（约37MB），请到github项目仓库下载~~`Release v1.0.0`~~`Release v2.0.0`中提供的安装包`install.exe`
运行安装程序后按如下步骤操作：

### 初始页面

单击`下一步`即可

### 安装指定文件夹

默认安装位置为`D:\Brownlzy\DinoOL`，如若有需要切换，`单击浏览`可选择文件夹或`直接修改文本框内容`（不推荐）。
修改好后单击`下一步`

注意：此时若提示`您选择的目录已存在且包含安装程序。请选择其他目标进行安装`，则表示您可能曾经安装过此程序，或者所选路径下有其他文件。请更换目录或手动卸载旧版程序。

### 选择组件

此处请**务必勾选**`DinoOL Server`组件！！！由于当前版本为非广泛传播版，每次游戏均需要联络服务器鉴权，此组件是与服务器沟通的必要组件，若选择不安装，可能导致无许可权限报错。
勾选后再次单击`下一步`。

### 开始菜单快捷方式

默认即可，单击`下一步`。

### 准备安装

单击`安装`后，安装向导会自动安装DinoOL，请等待安装过程结束。

## 运行

此安装程序不会在桌面创建快捷方式，您需要打开开始菜单->找到并打开`Brownlzy`文件夹->单击`DinoOL`运行。
由于安装包仅提供`v1.0.0`版本主程序，第一次需要联网更新才可正常游戏，请保证良好的网络环境。请手动前往`关于`页更新（帮助->关于->更新），更新完成后程序将自动重启完成更新。
注意：若在升级过程中出现`校验失败`的错误字样时，请先检查网络是否正常；若您的网络状态良好，可以正常访问本页面，且当前版本低于
`v1.1.3`，请打开程序安装目录手动将文件夹内的~~`DinoOLNew.exe`或`DinoOLNew`~~`DinoOL.bin`文件重命名替换`DinoOL.exe`。

## 开始游戏

自v1.1.4以后，P1的控制键不再限定使用WASD，您可以提供↑开始游戏，并提供↑↓←→对P1进行控制，相应的，P2将使用WASD操控。
因此，您可以使用W、Space、↑开始游戏。


未完待续...