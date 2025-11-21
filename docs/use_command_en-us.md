| [中文](./use_command_zh-cn.md) | English |

# How to Use Commands

Note: English translate may not 100% accurate.

## Usage

After running the executable, enter commands in the window. The text on the left shows the currently opened file, or displays `(No File)` if no file is open. If an unknown command is entered, it will output `Unknown Command!`. If the command format is incorrect, it will output `Wrong Format!`.

## Detailed Commands

### gen

Usage:
- `gen <filename> <width> <height>`
- `gen <filename> <width> <height> <fillingColor>`

Generates a BMP file named `filename` with width `width`, height `height`, and filled with `fillingColor`. If `fillingColor` is not provided, it defaults to `#ffffffff`.

### open

Usage:
- `open <filename>`

Opens the file `filename`. The original file will not be saved automatically.

### save

Usage:
- `save`

Saves the current file to disk.

### exit

Usage:
- `exit`

Saves the current file to disk and exits the process.

### set

Usage:
- `set <item> <value>`

Sets the value of `item` to `value`.

> Available options for `item`:
> - `width`
> - `height`

### get

Usage:
- `get <item>`

Gets the value of `item`.

> Available options for `item`:
> - `width`
> - `height`

### draw

Coordinates start from 0. If coordinates are out of bounds, output `Pixel coordinates out of bounds!`.

-   **pixel**
    
    Usage:
    - `draw pixel <x> <y> <color>`
    
    Draw a pixel at the specified coordinates using `color`.

-   **rect**
    
    Usage:
    - `draw rect <xFirst> <yFirst> <xLast> <yLast> <color>`
    - `draw rect <xFirst> <yFirst> <xLast> <yLast> <color> <borderPixelCount>`
    
    Draw a rectangle using `color`. `borderPixelCount` means the line width, the outermost edge remains at the original position (four angles). If no `borderPixelCount`, draw filled rectangle.

### help

Usage:
- `help`
- `help <command>`

Output all commands or the usage of `<command>`.

### echo

Usage:
- `echo <message>`

Output word (no spaces) `<message>`。
