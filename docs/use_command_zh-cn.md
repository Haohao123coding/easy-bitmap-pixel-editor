| 中文 | [English](./use_command_en-us.md) |

# 如何使用命令

## 使用方法

运行可执行文件后，在窗口输入命令即可。左边的文件是当前打开的文件，没有文件时显示 `(No File)`。如果输入命令未知，则输出 `Unknown Command!`；如果输入命令格式有误，则输出 `Wrong Format!`。

## 具体命令

### gen

用法：
- `gen <filename> <width> <height>`
- `gen <filename> <width> <height> <fillingColor>`

生成一个宽为 `width`、高为 `height`、填充 `fillingColor` 的 BMP 文件 `filename`。不填写 `fillingColor` 时，默认为 `#ffffffff`。

### open

用法：
- `open <filename>`

打开 `filename` 这一文件，不会保存原文件。

### save

用法：
- `save`

保存当前文件至磁盘。

### exit

用法：
- `exit`

保存当前文件至磁盘后退出进程。

### set

用法：
- `set <item> <value>`

将 `item` 的值设为 `value`。

> `item` 的可用项：
> - `width`
> - `height`

### get

用法：
- `get <item>`

获取 `item` 的值。

> `item` 的可用项：
> - `width`
> - `height`

### draw

坐标从 0 开始。如果坐标越界，则输出 `Pixel coordinates out of bounds!`。

-   **pixel**
    
    用法：
    - `draw pixel <x> <y> <color>`
    
    用 `color` 绘制像素或矩形。

-   **rect**
    
    用法：
    - `draw rect <xFirst> <yFirst> <xLast> <yLast> <color>`
    - `draw rect <xFirst> <yFirst> <xLast> <yLast> <color> <borderPixelCount>`
    
    用 `color` 绘制矩形。`borderPixelCount` 表示线段宽度，边框的最外层在原本（四个角）的位置。若无 `borderPixelCount` 则绘制填充矩形。

### help

用法：
- `help`
- `help <command>`

输出所有命令或 `<command>` 的用法。

### echo

用法：
- `echo <message>`

输出单词（不含空格）`<message>`。
