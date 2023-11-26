#ifndef SSD1306_DEFS_H
#define SSD1306_DEFS_H

/**
 * 10.1.1 Set Lower Column Start Address for Page Addressing Mode (00h~0Fh)
 * This command specifies the lower nibble of the 8-bit column start address for the display data RAM under
 * Page Addressing Mode. The column address will be incremented by each data access. Please refer to Section
 * Table 9-1 and Section 10.1.3 for details.
 */
#define OP_SET_LOWER_START_ADDRESS(addr) (0x00 + addr)
/**
 * 10.1.2 Set Higher Column Start Address for Page Addressing Mode (10h~1Fh)
 * This command specifies the higher nibble of the 8-bit column start address for the display data RAM under
 * Page Addressing Mode. The column address will be incremented by each data access. Please refer to Section
 * Table 9-1 and Section 10.1.3 for details.
 */
#define OP_SET_HIGHER_START_ADDRESS(addr) (0x10 + addr)
/**
 * 10.1.3 Set Memory Addressing Mode (20h)
 * There are 3 different memory addressing mode in SSD1306: page addressing mode, horizontal addressing
 * mode and vertical addressing mode. This command sets the way of memory addressing into one of the above
 * three modes. In there, “COL” means the graphic display data RAM column.
 */
#define OP_SET_MEMORY_ADDRESSING_MODE 0x20
/**
 * Page addressing mode (A[1:0]=10xb)
 * In page addressing mode, after the display RAM is read/written, the column address pointer is increased
 * automatically by 1. If the column address pointer reaches column end address, the column address pointer is
 * reset to column start address and page address pointer is not changed. Users have to set the new page and
 * column addresses in order to access the next page RAM content. The sequence of movement of the PAGE
 * and column address point for page addressing mode is shown in Figure 10-1.
 * In normal display data RAM read or write and page addressing mode, the following steps are required to
 * define the starting RAM access pointer location:
 * • Set the page start address of the target display location by command B0h to B7h.
 * • Set the lower start column address of pointer by command 00h~0Fh.
 * • Set the upper start column address of pointer by command 10h~1Fh.
 * For example, if the page address is set to B2h, lower column address is 03h and upper column address is 10h,
 * then that means the starting column is SEG3 of PAGE2. The RAM access pointer is located as shown in
 * Figure 10-2. The input data byte will be written into RAM position of column 3.
 */
#define OP_SET_MEMORY_ADDRESSING_MODE_PAGE 0x10
/**
 * Horizontal addressing mode (A[1:0]=00b)
 * In horizontal addressing mode, after the display RAM is read/written, the column address pointer is increased
 * automatically by 1. If the column address pointer reaches column end address, the column address pointer is
 * reset to column start address and page address pointer is increased by 1. The sequence of movement of the
 * page and column address point for horizontal addressing mode is shown in Figure 10-3. When both column
 * and page address pointers reach the end address, the pointers are reset to column start address and page start
 * address (Dotted line in Figure 10-3.)
 */
#define OP_SET_MEMORY_ADDRESSING_MODE_HORIZONTAL 0x00
/**
 * Vertical addressing mode: (A[1:0]=01b)
 * In vertical addressing mode, after the display RAM is read/written, the page address pointer is increased
 * automatically by 1. If the page address pointer reaches the page end address, the page address pointer is reset
 * to page start address and column address pointer is increased by 1. The sequence of movement of the page
 * and column address point for vertical addressing mode is shown in Figure 10-4. When both column and page
 * address pointers reach the end address, the pointers are reset to column start address and page start address
 * (Dotted line in Figure 10-4.)
 * In normal display data RAM read or write and horizontal / vertical addressing mode, the following steps are
 * required to define the RAM access pointer location:
 * • Set the column start and end address of the target display location by command 21h.
 * • Set the page start and end address of the target display location by command 22h.
 * Example is shown in Figure 10-5.
 */
#define OP_SET_MEMORY_ADDRESSING_MODE_VERTICAL 0x01
/**
 * 10.1.4 Set Column Address (21h)
 * This triple byte command specifies column start address and end address of the display data RAM. This
 * command also sets the column address pointer to column start address. This pointer is used to define the
 * current read/write column address in graphic display data RAM. If horizontal address increment mode is
 * enabled by command 20h, after finishing read/write one column data, it is incremented automatically to the
 * next column address. Whenever the column address pointer finishes accessing the end column address, it is
 * reset back to start column address and the row address is incremented to the next row.
 */
#define OP_SET_COLUMN_ADDRESS 0x21
/**
 * 10.1.5 Set Page Address (22h)
 * This triple byte command specifies page start address and end address of the display data RAM. This
 * command also sets the page address pointer to page start address. This pointer is used to define the current
 * read/write page address in graphic display data RAM. If vertical address increment mode is enabled by
 * command 20h, after finishing read/write one page data, it is incremented automatically to the next page
 * address. Whenever the page address pointer finishes accessing the end page address, it is reset back to start
 * page address.
 * The figure below shows the way of column and page address pointer movement through the example: column
 * start address is set to 2 and column end address is set to 125, page start address is set to 1 and page end
 * address is set to 6; Horizontal address increment mode is enabled by command 20h. In this case, the graphic
 * display data RAM column accessible range is from column 2 to column 125 and from page 1 to page 6 only.
 * In addition, the column address pointer is set to 2 and page address pointer is set to 1. After finishing
 * read/write one pixel of data, the column address is increased automatically by 1 to access the next RAM
 * location for next read/write operation (solid line in Figure 10-5). Whenever the column address pointer
 * finishes accessing the end column 125, it is reset back to column 2 and page address is automatically
 * increased by 1 (solid line in Figure 10-5). While the end page 6 and end column 125 RAM location is
 * accessed, the page address is reset back to 1 and the column address is reset back to 2 (dotted line in Figure
 * 10-5).
 */
#define OP_SET_PAGE_ADDRESS 0x22
/**
 * 10.1.6 Set Display Start Line (40h~7Fh)
 * This command sets the Display Start Line register to determine starting address of display RAM, by selecting
 * a value from 0 to 63. With value equal to 0, RAM row 0 is mapped to COM0. With value equal to 1, RAM
 * row 1 is mapped to COM0 and so on.
 * Refer to Table 10-1 for more illustrations.
 */
#define OP_SET_DISPLAY_START_LINE(value) (0x40 + value)
/**
 * 10.1.7 Set Contrast Control for BANK0 (81h)
 * This command sets the Contrast Setting of the display. The chip has 256 contrast steps from 00h to FFh. The
 * segment output current increases as the contrast step value increases.
 */
#define OP_SET_CONTRAST_CONTROL 0x81
/**
 * 10.1.8 Set Segment Re-map (A0h/A1h)
 * This command changes the mapping between the display data column address and the segment driver. It
 * allows flexibility in OLED module design. Please refer to Table 9-1.
 * This command only affects subsequent data input. Data already stored in GDDRAM will have no changes.
 */
#define OP_SET_SEGMENT_REMAP(mode) (0xA0 + mode)
/**
 * 10.1.9 Entire Display ON (A4h/A5h)
 * A4h command enable display outputs according to the GDDRAM contents.
 * If A5h command is issued, then by using A4h command, the display will resume to the GDDRAM contents.
 * In other words, A4h command resumes the display from entire display “ON” stage.
 * A5h command forces the entire display to be “ON”, regardless of the contents of the display data RAM.
 */
#define OP_SET_ENTIRE_DISPLAY_ON(force) (0xA4 + force)
/**
 * 10.1.10 Set Normal/Inverse Display (A6h/A7h)
 * This command sets the display to be either normal or inverse. In normal display a RAM data of 1 indicates an
 * “ON” pixel while in inverse display a RAM data of 0 indicates an “ON” pixel.
 */
#define OP_SET_NORMAL_INVERSE_DISPLAY(inverse) (0xA6 + inverse)
/**
 * 10.1.11 Set Multiplex Ratio (A8h)
 * This command switches the default 63 multiplex mode to any multiplex ratio, ranging from 16 to 63. The
 * output pads COM0~COM63 will be switched to the corresponding COM signal.
 */
#define OP_SET_MULTIPLEX_RATIO 0xA8
/**
 * 10.1.12 Set Display ON/OFF (AEh/AFh)
 * These single byte commands are used to turn the OLED panel display ON or OFF.
 * When the display is ON, the selected circuits by Set Master Configuration command will be turned ON.
 * When the display is OFF, those circuits will be turned OFF and the segment and common output are in VSS
 * state and high impedance state, respectively. These commands set the display to one of the two states:
 * o AEh : Display OFF
 * o AFh : Display ON
 */
#define OP_SET_DISPLAY_ON_OFF(on) (0xAE + on)
/**
 * 10.1.13 Set Page Start Address for Page Addressing Mode (B0h~B7h)
 * This command positions the page start address from 0 to 7 in GDDRAM under Page Addressing Mode.
 * Please refer to Table 9-1 and Section 10.1.3 for details.
 */
#define OP_SET_PAGE_START_ADDRESS(addr) (0xB0 + addr)
/**
 * 10.1.14 Set COM Output Scan Direction (C0h/C8h)
 * This command sets the scan direction of the COM output, allowing layout flexibility in the OLED module
 * design. Additionally, the display will show once this command is issued. For example, if this command is
 * sent during normal display then the graphic display will be vertically flipped immediately. Please refer to
 * Table 10-3 for details.
 */
#define OP_SET_COM_OUTPUT_SCAN_DIRECTION(dir) (0xC0 + dir)
/**
 * 10.1.15 Set Display Offset (D3h)
 * This is a double byte command. The second command specifies the mapping of the display start line to one of
 * COM0~COM63 (assuming that COM0 is the display start line then the display start line register is equal to 0).
 * For example, to move the COM16 towards the COM0 direction by 16 lines the 6-bit data in the second byte
 * should be given as 010000b. To move in the opposite direction by 16 lines the 6-bit data should be given by
 * 64 – 16, so the second byte would be 100000b. The following two tables (Table 10-1, Table 10-2) show the
 * example of setting the command C0h/C8h and D3h.
 */
#define OP_SET_DISPLAY_OFFSET 0xD3
/**
 * 10.1.16 Set Display Clock Divide Ratio/ Oscillator Frequency (D5h)
 * This command consists of two functions:
 * • Display Clock Divide Ratio (D)(A[3:0])
 * Set the divide ratio to generate DCLK (Display Clock) from CLK. The divide ratio is from 1 to 16,
 * with reset value = 1. Please refer to section 8.3 for the details relationship of DCLK and CLK.
 * • Oscillator Frequency (A[7:4])
 * Program the oscillator frequency Fosc that is the source of CLK if CLS pin is pulled high. The 4-bit
 * value results in 16 different frequency settings available as shown below. The default setting is
 * 1000b.
 */
#define OP_SET_DISPLAY_CLOCK_DIVIDE_RATIO_OSCILLATOR_FREQUENCY 0xD5
/**
 * 10.1.17 Set Pre-charge Period (D9h)
 * This command is used to set the duration of the pre-charge period. The interval is counted in number of
 * DCLK, where RESET equals 2 DCLKs.
 */
#define OP_SET_PRECHARGE_PERIOD 0xD9
/**
 * 10.1.18 Set COM Pins Hardware Configuration (DAh)
 * This command sets the COM signals pin configuration to match the OLED panel hardware layout. The table
 * below shows the COM pin configuration under different conditions (for MUX ratio =64):
 */
#define OP_SET_COM_PINS_HARDWARE_CONFIGURATION 0xDA
/**
 * 10.1.19 Set VCOMH Deselect Level (DBh)
 * This command adjusts the VCOMH regulator output.
 */
#define OP_SET_VCOMH_DESELECT_LEVEL 0xDB
/**
 * 10.1.20 NOP (E3h)
 * No Operation Command.
 */
#define OP_NOP 0xE3
/* Undocumented. */
#define OP_SET_DC_DC_ENABLE 0x8D

#endif