#ifndef W25Q_DEFS_H
#define W25Q_DEFS_H

/**
 * 8.2.1 Write Enable (06h)
 * The Write Enable instruction (Figure 5) sets the Write Enable Latch (WEL) bit in the Status Register to a
 * 1. The WEL bit must be set prior to every Page Program, Quad Page Program, Sector Erase, Block
 * Erase, Chip Erase, Write Status Register and Erase/Program Security Registers instruction. The Write
 * Enable instruction is entered by driving /CS low, shifting the instruction code “06h” into the Data Input (DI)
 * pin on the rising edge of CLK, and then driving /CS high.
 */
#define OP_WRITE_ENABLE 0x06
/**
 * 8.2.2 Write Enable for Volatile Status Register (50h)
 * The non-volatile Status Register bits described in section 7.1 can also be written to as volatile bits. This
 * gives more flexibility to change the system configuration and memory protection schemes quickly without
 * waiting for the typical non-volatile bit write cycles or affecting the endurance of the Status Register
 * nonvolatile bits. To write the volatile values into the Status Register bits, the Write Enable for Volatile Status
 * Register (50h) instruction must be issued prior to a Write Status Register (01h) instruction. Write Enable
 * for Volatile Status Register instruction (Figure 6) will not set the Write Enable Latch (WEL) bit, it is only
 * valid for the Write Status Register instruction to change the volatile Status Register bit values.
 */
#define OP_WRITE_ENABLE_FOR_VOLATILE_STATUS_REGISTER 0x50
/**
 * 8.2.3 Write Disable (04h)
 * The Write Disable instruction (Figure 7) resets the Write Enable Latch (WEL) bit in the Status Register to
 * a 0. The Write Disable instruction is entered by driving /CS low, shifting the instruction code “04h” into the
 * DI pin and then driving /CS high. Note that the WEL bit is automatically reset after Power-up and upon
 * completion of the Write Status Register, Erase/Program Security Registers, Page Program, Quad Page
 * Program, Sector Erase, Block Erase, Chip Erase and Reset instructions.
 */
#define OP_WRITE_DISABLE 0x04
/**
 * 8.2.4 Read Status Register-1 (05h), Status Register-2 (35h) & Status Register-3 (15h)
 * The Read Status Register instructions allow the 8-bit Status Registers to be read. The instruction is
 * entered by driving /CS low and shifting the instruction code “05h” for Status Register-1, “35h” for Status
 * Register-2 or “15h” for Status Register-3 into the DI pin on the rising edge of CLK. The status register bits
 * are then shifted out on the DO pin at the falling edge of CLK with most significant bit (MSB) first as shown
 * in Figure 8. Refer to section 7.1 for Status Register descriptions.
 * The Read Status Register instruction may be used at any time, even while a Program, Erase or Write
 * Status Register cycle is in progress. This allows the BUSY status bit to be checked to determine when
 * the cycle is complete and if the device can accept another instruction. The Status Register can be read
 * continuously, as shown in Figure 8. The instruction is completed by driving /CS high.
 */
#define OP_READ_STATUS_REGISTER(reg) (reg == 1 ? 0x05 : reg == 2 ? 0x35 \
                                                 : reg == 3   ? 0x15 \
                                                              : "Invalid register")
/**
 * 8.2.5 Write Status Register-1 (01h), Status Register-2 (31h) & Status Register-3 (11h)
 * The Write Status Register instruction allows the Status Registers to be written. The writable Status
 * Register bits include: SRP0, SEC, TB, BP[2:0] in Status Register-1; CMP, LB[3:1], QE, SRP1 in Status
 * Register-2; HOLD/RST, DRV1, DRV0, WPS & ADP in Status Register-3. All other Status Register bit
 * locations are read-only and will not be affected by the Write Status Register instruction. LB[3:1] are
 * nonvolatile OTP bits, once it is set to 1, it cannot be cleared to 0.
 * To write non-volatile Status Register bits, a standard Write Enable (06h) instruction must previously have
 * been executed for the device to accept the Write Status Register instruction (Status Register bit WEL
 * must equal 1). Once write enabled, the instruction is entered by driving /CS low, sending the instruction
 * code “01h/31h/11h”, and then writing the status register data byte as illustrated in Figure 9a & 9b.
 * To write volatile Status Register bits, a Write Enable for Volatile Status Register (50h) instruction must
 * have been executed prior to the Write Status Register instruction (Status Register bit WEL remains 0).
 * However, SRP1 and LB[3:1] cannot be changed from “1” to “0” because of the OTP protection for these
 * bits. Upon power off or the execution of a Software/Hardware Reset, the volatile Status Register bit
 * values will be lost, and the non-volatile Status Register bit values will be restored.
 * During non-volatile Status Register write operation (06h combined with 01h/31h/11h), after /CS is driven
 * high, the self-timed Write Status Register cycle will commence for a time duration of tW (See AC
 * Characteristics). While the Write Status Register cycle is in progress, the Read Status Register
 * instruction may still be accessed to check the status of the BUSY bit. The BUSY bit is a 1 during the Write
 * Status Register cycle and a 0 when the cycle is finished and ready to accept other instructions again.
 * After the Write Status Register cycle has finished, the Write Enable Latch (WEL) bit in the Status Register
 * will be cleared to 0.
 * During volatile Status Register write operation (50h combined with 01h/31h/11h), after /CS is driven high,
 * the Status Register bits will be refreshed to the new values within the time period of tSHSL2 (See AC
 * Characteristics). BUSY bit will remain 0 during the Status Register bit refresh period.
 * The Write Status Register instruction can be used in both SPI mode and QPI mode. However, the QE bit
 * cannot be written to when the device is in the QPI mode, because QE=1 is required for the device to
 * enter and operate in the QPI mode.
 * Refer to section 7.1 for Status Register descriptions.
 * The W25Q128FV is also backward compatible to Winbond‟s previous generations of serial flash
 * memories, in which the Status Register-1&2 can be written using a single “Write Status Register-1 (01h)”
 * command. To complete the Write Status Register-1&2 instruction, the /CS pin must be driven high after
 * the sixteenth bit of data that is clocked in as shown in Figure 9c & 9d. If /CS is driven high after the eighth
 * clock, the Write Status Register-1 (01h) instruction will only program the Status Register-1, the Status
 * Register-2 will not be affected (Previous generations will clear CMP and QE bits).
 */
#define OP_WRITE_STATUS_REGISTER(reg) (reg == 1 ? 0x01 : reg == 2 ? 0x31 \
                                                  : reg == 3   ? 0x11 \
                                                               : "Invalid register")
/**
 * 8.2.6 Read Data (03h)
 * The Read Data instruction allows one or more data bytes to be sequentially read from the memory. The
 * instruction is initiated by driving the /CS pin low and then shifting the instruction code “03h” followed by a
 * 24-bit address (A23-A0) into the DI pin. The code and address bits are latched on the rising edge of the
 * CLK pin. After the address is received, the data byte of the addressed memory location will be shifted out
 * on the DO pin at the falling edge of CLK with most significant bit (MSB) first. The address is automatically
 * incremented to the next higher address after each byte of data is shifted out allowing for a continuous
 * stream of data. This means that the entire memory can be accessed with a single instruction as long as
 * the clock continues. The instruction is completed by driving /CS high.
 * The Read Data instruction sequence is shown in Figure 14. If a Read Data instruction is issued while an
 * Erase, Program or Write cycle is in process (BUSY=1) the instruction is ignored and will not have any
 * effects on the current cycle. The Read Data instruction allows clock rates from D.C. to a maximum of fR
 * (see AC Electrical Characteristics).
 * The Read Data (03h) instruction is only supported in Standard SPI mode.
 */
#define OP_READ_DATA 0x03
/**
 * 8.2.7 Fast Read (0Bh)
 * The Fast Read instruction is similar to the Read Data instruction except that it can operate at the highest
 * possible frequency of FR (see AC Electrical Characteristics). This is accomplished by adding eight
 * “dummy” clocks after the 24-bit address as shown in Figure 16. The dummy clocks allow the devices
 * internal circuits additional time for setting up the initial address. During the dummy clocks the data value
 * on the DO pin is a “don‟t care”.
 */
#define OP_FAST_READ 0x08
/**
 * 8.2.8 Fast Read Dual Output (3Bh)
 * The Fast Read Dual Output (3Bh) instruction is similar to the standard Fast Read (0Bh) instruction except
 * that data is output on two pins; IO0 and IO1. This allows data to be transferred at twice the rate of
 * standard SPI devices. The Fast Read Dual Output instruction is ideal for quickly downloading code from
 * Flash to RAM upon power-up or for applications that cache code-segments to RAM for execution.
 * Similar to the Fast Read instruction, the Fast Read Dual Output instruction can operate at the highest
 * possible frequency of FR (see AC Electrical Characteristics). This is accomplished by adding eight
 * “dummy” clocks after the 24-bit address as shown in Figure 18. The dummy clocks allow the device's
 * internal circuits additional time for setting up the initial address. The input data during the dummy clocks
 * is “don‟t care”. However, the IO0 pin should be high-impedance prior to the falling edge of the first data
 * out clock.
 */
#define OP_FAST_READ_DUAL_OUTPUT 0x3B
/**
 * 8.2.9 Fast Read Quad Output (6Bh)
 * The Fast Read Quad Output (6Bh) instruction is similar to the Fast Read Dual Output (3Bh) instruction
 * except that data is output on four pins, IO0, IO1, IO2, and IO3. The Quad Enable (QE) bit in Status
 * Register-2 must be set to 1 before the device will accept the Fast Read Quad Output Instruction. The
 * Fast Read Quad Output Instruction allows data to be transferred at four times the rate of standard SPI
 * devices.
 * The Fast Read Quad Output instruction can operate at the highest possible frequency of FR (see AC
 * Electrical Characteristics). This is accomplished by adding eight “dummy” clocks after the 24-bit address
 * as shown in Figure 20. The dummy clocks allow the device's internal circuits additional time for setting up
 * the initial address. The input data during the dummy clocks is “don‟t care”. However, the IO pins should
 * be high-impedance prior to the falling edge of the first data out clock.
 */
#define OP_FAST_READ_QUAD_OUTPUT 0x6B
/**
 * 8.2.10 Fast Read Dual I/O (BBh)
 * The Fast Read Dual I/O (BBh) instruction allows for improved random access while maintaining two IO
 * pins, IO0 and IO1. It is similar to the Fast Read Dual Output (3Bh) instruction but with the capability to
 * input the Address bits (A23-0) two bits per clock. This reduced instruction overhead may allow for code
 * execution (XIP) directly from the Dual SPI in some applications.
 */
#define OP_FAST_READ_DUAL_IO 0xBB
/**
 * 8.2.11 Fast Read Quad I/O (EBh)
 * The Fast Read Quad I/O (EBh) instruction is similar to the Fast Read Dual I/O (BBh) instruction except
 * that address and data bits are input and output through four pins IO0, IO1, IO2 and IO3 and four Dummy
 * clocks are required in SPI mode prior to the data output. The Quad I/O dramatically reduces instruction
 * overhead allowing faster random access for code execution (XIP) directly from the Quad SPI. The Quad
 * Enable bit (QE) of Status Register-2 must be set to enable the Fast Read Quad I/O Instruction.
 */
#define OP_FAST_READ_QUAD_IO 0xEB
/**
 * 8.2.12 Word Read Quad I/O (E7h)
 * The Word Read Quad I/O (E7h) instruction is similar to the Fast Read Quad I/O (EBh) instruction except
 * that the lowest Address bit (A0) must equal 0 and only two Dummy clocks are required prior to the data
 * output. The Quad I/O dramatically reduces instruction overhead allowing faster random access for code
 * execution (XIP) directly from the Quad SPI. The Quad Enable bit (QE) of Status Register-2 must be set to
 * enable the Word Read Quad I/O Instruction.
 */
#define OP_WORD_READ_QUAD_IO 0xE7
/**
 * 8.2.13 Octal Word Read Quad I/O (E3h)
 * The Octal Word Read Quad I/O (E3h) instruction is similar to the Fast Read Quad I/O (EBh) instruction
 * except that the lower four Address bits (A0, A1, A2, A3) must equal 0. As a result, the dummy clocks are
 * not required, which further reduces the instruction overhead allowing even faster random access for code
 * execution (XIP). The Quad Enable bit (QE) of Status Register-2 must be set to enable the Octal Word
 * Read Quad I/O Instruction.
 */
#define OP_WORD_READ_QUAD_IO_OCTAL 0xE3
/**
 * 8.2.14 Set Burst with Wrap (77h)
 * In Standard SPI mode, the Set Burst with Wrap (77h) instruction is used in conjunction with “Fast Read
 * Quad I/O” and “Word Read Quad I/O” instructions to access a fixed length of 8/16/32/64-byte section
 * within a 256-byte page. Certain applications can benefit from this feature and improve the overall system
 * code execution performance.
 * Similar to a Quad I/O instruction, the Set Burst with Wrap instruction is initiated by driving the /CS pin low
 * and then shifting the instruction code “77h” followed by 24 dummy bits and 8 “Wrap Bits”, W7-0. The
 * instruction sequence is shown in Figure 28. Wrap bit W7 and the lower nibble W3-0 are not used.
 * Once W6-4 is set by a Set Burst with Wrap instruction, all the following “Fast Read Quad I/O” and “Word
 * Read Quad I/O” instructions will use the W6-4 setting to access the 8/16/32/64-byte section within any
 * page. To exit the “Wrap Around” function and return to normal read operation, another Set Burst with
 * Wrap instruction should be issued to set W4 = 1. The default value of W4 upon power on or after a
 * software/hardware reset is 1.
 * In QPI mode, the “Burst Read with Wrap (0Ch)” instruction should be used to perform the Read operation
 * with “Wrap Around” feature. The Wrap Length set by W5-4 in Standard SPI mode is still valid in QPI
 * mode and can also be re-configured by “Set Read Parameters (C0h)” instruction. Refer to 8.2.44 and
 * 8.2.45 for details.
 */
#define OP_SET_BURST_WITH_WRAP 0x77
/**
 * 8.2.15 Page Program (02h)
 * The Page Program instruction allows from one byte to 256 bytes (a page) of data to be programmed at
 * previously erased (FFh) memory locations. A Write Enable instruction must be executed before the
 * device will accept the Page Program Instruction (Status Register bit WEL= 1). The instruction is initiated
 * by driving the /CS pin low then shifting the instruction code “02h” followed by a 24-bit address (A23-A0)
 * and at least one data byte, into the DI pin. The /CS pin must be held low for the entire length of the
 * instruction while data is being sent to the device. The Page Program instruction sequence is shown in
 * Figure 29.
 * If an entire 256 byte page is to be programmed, the last address byte (the 8 least significant address bits)
 * should be set to 0. If the last address byte is not zero, and the number of clocks exceeds the remaining
 * page length, the addressing will wrap to the beginning of the page. In some cases, less than 256 bytes (a
 * partial page) can be programmed without having any effect on other bytes within the same page. One
 * condition to perform a partial page program is that the number of clocks cannot exceed the remaining
 * page length. If more than 256 bytes are sent to the device the addressing will wrap to the beginning of the
 * page and overwrite previously sent data.
 * As with the write and erase instructions, the /CS pin must be driven high after the eighth bit of the last
 * byte has been latched. If this is not done the Page Program instruction will not be executed. After /CS is
 * driven high, the self-timed Page Program instruction will commence for a time duration of tpp (See AC
 * Characteristics). While the Page Program cycle is in progress, the Read Status Register instruction may
 * still be accessed for checking the status of the BUSY bit. The BUSY bit is a 1 during the Page Program
 * cycle and becomes a 0 when the cycle is finished and the device is ready to accept other instructions
 * again. After the Page Program cycle has finished the Write Enable Latch (WEL) bit in the Status Register
 * is cleared to 0. The Page Program instruction will not be executed if the addressed page is protected by
 * the Block Protect (CMP, SEC, TB, BP2, BP1, and BP0) bits or the Individual Block/Sector Locks.
 */
#define OP_PAGE_PROGRAM 0x02
/**
 * 8.2.16 Quad Input Page Program (32h)
 * The Quad Page Program instruction allows up to 256 bytes of data to be programmed at previously
 * erased (FFh) memory locations using four pins: IO0, IO1, IO2, and IO3. The Quad Page Program can
 * improve performance for PROM Programmer and applications that have slow clock speeds <5MHz.
 * Systems with faster clock speed will not realize much benefit for the Quad Page Program instruction
 * since the inherent page program time is much greater than the time it take to clock-in the data.
 * To use Quad Page Program the Quad Enable (QE) bit in Status Register-2 must be set to 1. A Write
 * Enable instruction must be executed before the device will accept the Quad Page Program instruction
 * (Status Register-1, WEL=1). The instruction is initiated by driving the /CS pin low then shifting the
 * instruction code “32h” followed by a 24-bit address (A23-A0) and at least one data byte, into the IO pins.
 * The /CS pin must be held low for the entire length of the instruction while data is being sent to the device.
 * All other functions of Quad Page Program are identical to standard Page Program. The Quad Page
 * Program instruction sequence is shown in Figure 30.8.2.16 Quad Input Page Program (32h)
 * The Quad Page Program instruction allows up to 256 bytes of data to be programmed at previously
 * erased (FFh) memory locations using four pins: IO0, IO1, IO2, and IO3. The Quad Page Program can
 * improve performance for PROM Programmer and applications that have slow clock speeds <5MHz.
 * Systems with faster clock speed will not realize much benefit for the Quad Page Program instruction
 * since the inherent page program time is much greater than the time it take to clock-in the data.
 * To use Quad Page Program the Quad Enable (QE) bit in Status Register-2 must be set to 1. A Write
 * Enable instruction must be executed before the device will accept the Quad Page Program instruction
 * (Status Register-1, WEL=1). The instruction is initiated by driving the /CS pin low then shifting the
 * instruction code “32h” followed by a 24-bit address (A23-A0) and at least one data byte, into the IO pins.
 * The /CS pin must be held low for the entire length of the instruction while data is being sent to the device.
 * All other functions of Quad Page Program are identical to standard Page Program. The Quad Page
 * Program instruction sequence is shown in Figure 30.
 */
#define OP_PAGE_PROGRAM_QUAD_INPUT 0x32
/**
 * 8.2.17 Sector Erase (20h)
 * The Sector Erase instruction sets all memory within a specified sector (4K-bytes) to the erased state of all
 * 1s (FFh). A Write Enable instruction must be executed before the device will accept the Sector Erase
 * Instruction (Status Register bit WEL must equal 1). The instruction is initiated by driving the /CS pin low
 * and shifting the instruction code “20h” followed a 24-bit sector address (A23-A0). The Sector Erase
 * instruction sequence is shown in Figure 31a & 31b.
 * The /CS pin must be driven high after the eighth bit of the last byte has been latched. If this is not done
 * the Sector Erase instruction will not be executed. After /CS is driven high, the self-timed Sector Erase
 * instruction will commence for a time duration of tSE (See AC Characteristics). While the Sector Erase
 * cycle is in progress, the Read Status Register instruction may still be accessed for checking the status of
 * the BUSY bit. The BUSY bit is a 1 during the Sector Erase cycle and becomes a 0 when the cycle is
 * finished and the device is ready to accept other instructions again. After the Sector Erase cycle has
 * finished the Write Enable Latch (WEL) bit in the Status Register is cleared to 0. The Sector Erase
 * instruction will not be executed if the addressed page is protected by the Block Protect (CMP, SEC, TB,
 * BP2, BP1, and BP0) bits or the Individual Block/Sector Locks.
 */
#define OP_ERASE_SECTOR 0x20
/**
 * 8.2.18 32KB Block Erase (52h)
 * The Block Erase instruction sets all memory within a specified block (32K-bytes) to the erased state of all
 * 1s (FFh). A Write Enable instruction must be executed before the device will accept the Block Erase
 * Instruction (Status Register bit WEL must equal 1). The instruction is initiated by driving the /CS pin low
 * and shifting the instruction code “52h” followed a 24-bit block address (A23-A0). The Block Erase
 * instruction sequence is shown in Figure 32a & 32b.
 * The /CS pin must be driven high after the eighth bit of the last byte has been latched. If this is not done
 * the Block Erase instruction will not be executed. After /CS is driven high, the self-timed Block Erase
 * instruction will commence for a time duration of tBE1 (See AC Characteristics). While the Block Erase
 * cycle is in progress, the Read Status Register instruction may still be accessed for checking the status of
 * the BUSY bit. The BUSY bit is a 1 during the Block Erase cycle and becomes a 0 when the cycle is
 * finished and the device is ready to accept other instructions again. After the Block Erase cycle has
 * finished the Write Enable Latch (WEL) bit in the Status Register is cleared to 0. The Block Erase
 * instruction will not be executed if the addressed page is protected by the Block Protect (CMP, SEC, TB,
 * BP2, BP1, and BP0) bits or the Individual Block/Sector Locks.
 */
#define OP_ERASE_BLOCK_32KB 0x52
/**
 * 8.2.19 64KB Block Erase (D8h)
 * The Block Erase instruction sets all memory within a specified block (64K-bytes) to the erased state of all
 * 1s (FFh). A Write Enable instruction must be executed before the device will accept the Block Erase
 * Instruction (Status Register bit WEL must equal 1). The instruction is initiated by driving the /CS pin low
 * and shifting the instruction code “D8h” followed a 24-bit block address (A23-A0). The Block Erase
 * instruction sequence is shown in Figure 33a & 33b.
 * The /CS pin must be driven high after the eighth bit of the last byte has been latched. If this is not done
 * the Block Erase instruction will not be executed. After /CS is driven high, the self-timed Block Erase
 * instruction will commence for a time duration of tBE (See AC Characteristics). While the Block Erase cycle
 * is in progress, the Read Status Register instruction may still be accessed for checking the status of the
 * BUSY bit. The BUSY bit is a 1 during the Block Erase cycle and becomes a 0 when the cycle is finished
 * and the device is ready to accept other instructions again. After the Block Erase cycle has finished the
 * Write Enable Latch (WEL) bit in the Status Register is cleared to 0. The Block Erase instruction will not be
 * executed if the addressed page is protected by the Block Protect (CMP, SEC, TB, BP2, BP1, and BP0)
 * bits or the Individual Block/Sector Locks.
 */
#define OP_ERASE_BLOCK_64KB 0xD8
/**
 * 8.2.20 Chip Erase (C7h / 60h)
 * The Chip Erase instruction sets all memory within the device to the erased state of all 1s (FFh). A Write
 * Enable instruction must be executed before the device will accept the Chip Erase Instruction (Status
 * Register bit WEL must equal 1). The instruction is initiated by driving the /CS pin low and shifting the
 * instruction code “C7h” or “60h”. The Chip Erase instruction sequence is shown in Figure 34.
 * The /CS pin must be driven high after the eighth bit has been latched. If this is not done the Chip Erase
 * instruction will not be executed. After /CS is driven high, the self-timed Chip Erase instruction will
 * commence for a time duration of tCE (See AC Characteristics). While the Chip Erase cycle is in progress,
 * the Read Status Register instruction may still be accessed to check the status of the BUSY bit. The
 * BUSY bit is a 1 during the Chip Erase cycle and becomes a 0 when finished and the device is ready to
 * accept other instructions again. After the Chip Erase cycle has finished the Write Enable Latch (WEL) bit
 * in the Status Register is cleared to 0. The Chip Erase instruction will not be executed if any memory
 * region is protected by the Block Protect (CMP, SEC, TB, BP2, BP1, and BP0) bits or the Individual
 * Block/Sector Locks.
 */
#define OP_ERASE_CHIP 0xC7
/**
 * 8.2.21 Erase / Program Suspend (75h)
 * The Erase/Program Suspend instruction “75h”, allows the system to interrupt a Sector or Block Erase
 * operation or a Page Program operation and then read from or program/erase data to, any other sectors
 * or blocks. The Erase/Program Suspend instruction sequence is shown in Figure 35a & 35b.
 * The Write Status Register instruction (01h) and Erase instructions (20h, 52h, D8h, C7h, 60h, 44h) are not
 * allowed during Erase Suspend. Erase Suspend is valid only during the Sector or Block erase operation. If
 * written during the Chip Erase operation, the Erase Suspend instruction is ignored. The Write Status
 * Register instructions (01h, 31h, 11h) and Program instructions (02h, 32h, 42h) are not allowed during
 * Program Suspend. Program Suspend is valid only during the Page Program or Quad Page Program
 * operation.
 * The Erase/Program Suspend instruction “75h” will be accepted by the device only if the SUS bit in the
 * Status Register equals to 0 and the BUSY bit equals to 1 while a Sector or Block Erase or a Page
 * Program operation is on-going. If the SUS bit equals to 1 or the BUSY bit equals to 0, the Suspend
 * instruction will be ignored by the device. A maximum of time of “tSUS” (See AC Characteristics) is required
 * to suspend the erase or program operation. The BUSY bit in the Status Register will be cleared from 1 to
 * 0 within “tSUS” and the SUS bit in the Status Register will be set from 0 to 1 immediately after
 * Erase/Program Suspend. For a previously resumed Erase/Program operation, it is also required that the
 * Suspend instruction “75h” is not issued earlier than a minimum of time of “tSUS” following the preceding
 * Resume instruction “7Ah”.
 * Unexpected power off during the Erase/Program suspend state will reset the device and release the
 * suspend state. SUS bit in the Status Register will also reset to 0. The data within the page, sector or
 * block that was being suspended may become corrupted. It is recommended for the user to implement
 * system design techniques against the accidental power interruption and preserve data integrity during
 * erase/program suspend state.
 */
#define OP_ERASE_PROGRAM_SUSPEND 0x75
/**
 * 8.2.22 Erase / Program Resume (7Ah)
 * The Erase/Program Resume instruction “7Ah” must be written to resume the Sector or Block Erase
 * operation or the Page Program operation after an Erase/Program Suspend. The Resume instruction
 * “7Ah” will be accepted by the device only if the SUS bit in the Status Register equals to 1 and the BUSY
 * bit equals to 0. After issued the SUS bit will be cleared from 1 to 0 immediately, the BUSY bit will be set
 * from 0 to 1 within 200ns and the Sector or Block will complete the erase operation or the page will
 * complete the program operation. If the SUS bit equals to 0 or the BUSY bit equals to 1, the Resume
 * instruction “7Ah” will be ignored by the device. The Erase/Program Resume instruction sequence is
 * shown in Figure 36a & 36b.
 * Resume instruction is ignored if the previous Erase/Program Suspend operation was interrupted by
 * unexpected power off. It is also required that a subsequent Erase/Program Suspend instruction not to be
 * issued within a minimum of time of “tSUS” following a previous Resume instruction.8.2.22 Erase / Program Resume (7Ah)
 * The Erase/Program Resume instruction “7Ah” must be written to resume the Sector or Block Erase
 * operation or the Page Program operation after an Erase/Program Suspend. The Resume instruction
 * “7Ah” will be accepted by the device only if the SUS bit in the Status Register equals to 1 and the BUSY
 * bit equals to 0. After issued the SUS bit will be cleared from 1 to 0 immediately, the BUSY bit will be set
 * from 0 to 1 within 200ns and the Sector or Block will complete the erase operation or the page will
 * complete the program operation. If the SUS bit equals to 0 or the BUSY bit equals to 1, the Resume
 * instruction “7Ah” will be ignored by the device. The Erase/Program Resume instruction sequence is
 * shown in Figure 36a & 36b.
 * Resume instruction is ignored if the previous Erase/Program Suspend operation was interrupted by
 * unexpected power off. It is also required that a subsequent Erase/Program Suspend instruction not to be
 * issued within a minimum of time of “tSUS” following a previous Resume instruction.
 */
#define OP_ERASE_PROGRAM_RESUME 0x7A
/** 
 * 8.2.23 Power-down (B9h)
 * Although the standby current during normal operation is relatively low, standby current can be further
 * reduced with the Power-down instruction. The lower power consumption makes the Power-down
 * instruction especially useful for battery powered applications (See ICC1 and ICC2 in AC Characteristics).
 * The instruction is initiated by driving the /CS pin low and shifting the instruction code “B9h” as shown in
 * Figure 37a & 37b.
 * The /CS pin must be driven high after the eighth bit has been latched. If this is not done the Power-down
 * instruction will not be executed. After /CS is driven high, the power-down state will entered within the time
 * duration of tDP (See AC Characteristics). While in the power-down state only the Release Power-down /
 * Device ID (ABh) instruction, which restores the device to normal operation, will be recognized. All other
 * instructions are ignored. This includes the Read Status Register instruction, which is always available
 * during normal operation. Ignoring all but one instruction makes the Power Down state a useful condition
 * for securing maximum write protection. The device always powers-up in the normal operation with the
 * standby current of ICC1.
 */
#define OP_POWER_DOWN 0xB9
/**
 * 8.2.24 Release Power-down / Device ID (ABh)
 * The Release from Power-down / Device ID instruction is a multi-purpose instruction. It can be used to
 * release the device from the power-down state, or obtain the devices electronic identification (ID) number.
 * To release the device from the power-down state, the instruction is issued by driving the /CS pin low,
 * shifting the instruction code “ABh” and driving /CS high as shown in Figure 38a & 38b. Release from
 * power-down will take the time duration of tRES1 (See AC Characteristics) before the device will resume
 * normal operation and other instructions are accepted. The /CS pin must remain high during the tRES1 time
 * duration.
 * When used only to obtain the Device ID while not in the power-down state, the instruction is initiated by
 * driving the /CS pin low and shifting the instruction code “ABh” followed by 3-dummy bytes. The Device ID
 * bits are then shifted out on the falling edge of CLK with most significant bit (MSB) first. The Device ID
 * value for the W25Q128FV is listed in Manufacturer and Device Identification table. The Device ID can be
 * read continuously. The instruction is completed by driving /CS high.
 * When used to release the device from the power-down state and obtain the Device ID, the instruction is
 * the same as previously described, and shown in Figure 38c & 38d, except that after /CS is driven high it
 * must remain high for a time duration of tRES2 (See AC Characteristics). After this time duration the device
 * will resume normal operation and other instructions will be accepted. If the Release from Power-down /
 * Device ID instruction is issued while an Erase, Program or Write cycle is in process (when BUSY equals
 * 1) the instruction is ignored and will not have any effects on the current cycle.
 */
#define OP_RELEASE_POWER_DOWN_DEVICE_ID 0xAB
/**
 * 8.2.25 Read Manufacturer / Device ID (90h)
 * The Read Manufacturer/Device ID instruction is an alternative to the Release from Power-down / Device
 * ID instruction that provides both the JEDEC assigned manufacturer ID and the specific device ID.
 * The Read Manufacturer/Device ID instruction is very similar to the Release from Power-down / Device ID
 * instruction. The instruction is initiated by driving the /CS pin low and shifting the instruction code “90h”
 * followed by a 24-bit address (A23-A0) of 000000h. After which, the Manufacturer ID for Winbond (EFh)
 * and the Device ID are shifted out on the falling edge of CLK with most significant bit (MSB) first as shown
 * in Figure 39. The Device ID values for the W25Q128FV are listed in Manufacturer and Device
 * Identification table. The instruction is completed by driving /CS high.
 */
#define OP_READ_MANUFACTURER_DEVICE_ID 0x90
/**
 * 8.2.26 Read Manufacturer / Device ID Dual I/O (92h)
 * The Read Manufacturer / Device ID Dual I/O instruction is an alternative to the Read Manufacturer /
 * Device ID instruction that provides both the JEDEC assigned manufacturer ID and the specific device ID
 * at 2x speed.
 * The Read Manufacturer / Device ID Dual I/O instruction is similar to the Fast Read Dual I/O instruction.
 * The instruction is initiated by driving the /CS pin low and shifting the instruction code “92h” followed by a
 * 24-bit address (A23-A0) of 000000h, but with the capability to input the Address bits two bits per clock.
 * After which, the Manufacturer ID for Winbond (EFh) and the Device ID are shifted out 2 bits per clock on
 * the falling edge of CLK with most significant bits (MSB) first as shown in Figure 40. The Device ID values
 * for the W25Q128FV are listed in Manufacturer and Device Identification table. If the 24-bit address is
 * initially set to 000001h the Device ID will be read first and then followed by the Manufacturer ID. The
 * Manufacturer and Device IDs can be read continuously, alternating from one to the other. The instruction
 * is completed by driving /CS high.
 */
#define OP_READ_MANUFACTURER_DEVICE_DUAL_IO 0x92
/**
 * 8.2.27 Read Manufacturer / Device ID Quad I/O (94h)
 * The Read Manufacturer / Device ID Quad I/O instruction is an alternative to the Read Manufacturer /
 * Device ID instruction that provides both the JEDEC assigned manufacturer ID and the specific device ID
 * at 4x speed.
 * The Read Manufacturer / Device ID Quad I/O instruction is similar to the Fast Read Quad I/O instruction.
 * The instruction is initiated by driving the /CS pin low and shifting the instruction code “94h” followed by a
 * four clock dummy cycles and then a 24-bit address (A23-A0) of 000000h, but with the capability to input
 * the Address bits four bits per clock. After which, the Manufacturer ID for Winbond (EFh) and the Device
 * ID are shifted out four bits per clock on the falling edge of CLK with most significant bit (MSB) first as
 * shown in Figure 41. The Device ID values for the W25Q128FV are listed in Manufacturer and Device
 * Identification table. If the 24-bit address is initially set to 000001h the Device ID will be read first and then
 * followed by the Manufacturer ID. The Manufacturer and Device IDs can be read continuously, alternating
 * from one to the other. The instruction is completed by driving /CS high.
 */
#define OP_READ_MANUFACTURER_DEVICE_ID_QUAD_IO 0x94
/**
 * 8.2.28 Read Unique ID Number (4Bh)
 * The Read Unique ID Number instruction accesses a factory-set read-only 64-bit number that is unique to
 * each W25Q128FV device. The ID number can be used in conjunction with user software methods to help
 * prevent copying or cloning of a system. The Read Unique ID instruction is initiated by driving the /CS pin
 * low and shifting the instruction code “4Bh” followed by a four bytes of dummy clocks. After which, the 64-
 * bit ID is shifted out on the falling edge of CLK as shown in Figure 42.8.2.28 Read Unique ID Number (4Bh)
 * The Read Unique ID Number instruction accesses a factory-set read-only 64-bit number that is unique to
 * each W25Q128FV device. The ID number can be used in conjunction with user software methods to help
 * prevent copying or cloning of a system. The Read Unique ID instruction is initiated by driving the /CS pin
 * low and shifting the instruction code “4Bh” followed by a four bytes of dummy clocks. After which, the 64-
 * bit ID is shifted out on the falling edge of CLK as shown in Figure 42.
 */
#define OP_READ_UNIQUE_ID_NUMBER 0x4B
/**
 * 8.2.29 Read JEDEC ID (9Fh)
 * For compatibility reasons, the W25Q128FV provides several instructions to electronically determine the
 * identity of the device. The Read JEDEC ID instruction is compatible with the JEDEC standard for SPI
 * compatible serial memories that was adopted in 2003. The instruction is initiated by driving the /CS pin
 * low and shifting the instruction code “9Fh”. The JEDEC assigned Manufacturer ID byte for Winbond (EFh)
 * and two Device ID bytes, Memory Type (ID15-ID8) and Capacity (ID7-ID0) are then shifted out on the
 * falling edge of CLK with most significant bit (MSB) first as shown in Figure 43a & 43b. For memory type
 * and capacity values refer to Manufacturer and Device Identification table.
 */
#define OP_READ_JEDEC_ID 0x9F
/**
 * 8.2.30 Read SFDP Register (5Ah)
 * The W25Q128BV features a 256-Byte Serial Flash Discoverable Parameter (SFDP) register that contains
 * information about device configurations, available instructions and other features. The SFDP parameters
 * are stored in one or more Parameter Identification (PID) tables. Currently only one PID table is specified,
 * but more may be added in the future. The Read SFDP Register instruction is compatible with the SFDP
 * standard initially established in 2010 for PC and other applications, as well as the JEDEC standard
 * JESD216 that is published in 2011. Most Winbond SpiFlash Memories shipped after June 2011 (date
 * code 1124 and beyond) support the SFDP feature as specified in the applicable datasheet.
 * The Read SFDP instruction is initiated by driving the /CS pin low and shifting the instruction code “5Ah”
 * followed by a 24-bit address (A23-A0)(1) into the DI pin. Eight “dummy” clocks are also required before the
 * SFDP register contents are shifted out on the falling edge of the 40th CLK with most significant bit (MSB)
 * first as shown in Figure 34. For SFDP register values and descriptions, please refer to the Winbond
 * Application Note for SFDP Definition Table.
 */
#define OP_READ_SFDP_REGISTER 0x5A
/**
 * 8.2.31 Erase Security Registers (44h)
 * The W25Q128FV offers three 256-byte Security Registers which can be erased and programmed
 * individually. These registers may be used by the system manufacturers to store security and other
 * important information separately from the main memory array.
 * The Erase Security Register instruction is similar to the Sector Erase instruction. A Write Enable
 * instruction must be executed before the device will accept the Erase Security Register Instruction (Status
 * Register bit WEL must equal 1). The instruction is initiated by driving the /CS pin low and shifting the
 * instruction code “44h” followed by a 24-bit address (A23-A0) to erase one of the three security registers.
 * The Erase Security Register instruction sequence is shown in Figure 45. The /CS pin must be driven high
 * after the eighth bit of the last byte has been latched. If this is not done the instruction will not be executed.
 * After /CS is driven high, the self-timed Erase Security Register operation will commence for a time
 * duration of tSE (See AC Characteristics). While the Erase Security Register cycle is in progress, the Read
 * Status Register instruction may still be accessed for checking the status of the BUSY bit. The BUSY bit is
 * a 1 during the erase cycle and becomes a 0 when the cycle is finished and the device is ready to accept
 * other instructions again. After the Erase Security Register cycle has finished the Write Enable Latch
 * (WEL) bit in the Status Register is cleared to 0. The Security Register Lock Bits (LB3-1) in the Status
 * Register-2 can be used to OTP protect the security registers. Once a lock bit is set to 1, the
 * corresponding security register will be permanently locked, Erase Security Register instruction to that
 * register will be ignored (Refer to section 7.1.8 for detail descriptions).
 */
#define OP_ERASE_SECURITY_REGISTERS 0x44
/**
 * 8.2.32 Program Security Registers (42h)
 * The Program Security Register instruction is similar to the Page Program instruction. It allows from one
 * byte to 256 bytes of security register data to be programmed at previously erased (FFh) memory
 * locations. A Write Enable instruction must be executed before the device will accept the Program Security
 * Register Instruction (Status Register bit WEL= 1). The instruction is initiated by driving the /CS pin low
 * then shifting the instruction code “42h” followed by a 24-bit address (A23-A0) and at least one data byte,
 * into the DI pin. The /CS pin must be held low for the entire length of the instruction while data is being
 * sent to the device.
 * The Program Security Register instruction sequence is shown in Figure 46. The Security Register Lock
 * Bits (LB3-1) in the Status Register-2 can be used to OTP protect the security registers. Once a lock bit is
 * set to 1, the corresponding security register will be permanently locked, Program Security Register
 * instruction to that register will be ignored (See 7.1.8, 8.2.25 for detail descriptions).
 */
#define OP_PROGRAM_SECURITY_REGISTERS 0x42
/**
 * 8.2.33 Read Security Registers (48h)
 * The Read Security Register instruction is similar to the Fast Read instruction and allows one or more data
 * bytes to be sequentially read from one of the four security registers. The instruction is initiated by driving
 * the /CS pin low and then shifting the instruction code “48h” followed by a 24-bit address (A23-A0) and
 * eight “dummy” clocks into the DI pin. The code and address bits are latched on the rising edge of the CLK
 * pin. After the address is received, the data byte of the addressed memory location will be shifted out on
 * the DO pin at the falling edge of CLK with most significant bit (MSB) first. The byte address is
 * automatically incremented to the next byte address after each byte of data is shifted out. Once the byte
 * address reaches the last byte of the register (byte address FFh), it will reset to address 00h, the first byte
 * of the register, and continue to increment. The instruction is completed by driving /CS high. The Read
 * Security Register instruction sequence is shown in Figure 47. If a Read Security Register instruction is
 * issued while an Erase, Program or Write cycle is in process (BUSY=1) the instruction is ignored and will
 * not have any effects on the current cycle. The Read Security Register instruction allows clock rates from
 * D.C. to a maximum of FR (see AC Electrical Characteristics).
 */
#define OP_READ_SECURITY_REGISTERS 0x48
/**
 * 8.2.34 Set Read Parameters (C0h)
 * In QPI mode, to accommodate a wide range of applications with different needs for either maximum read
 * frequency or minimum data access latency, “Set Read Parameters (C0h)” instruction can be used to
 * configure the number of dummy clocks for “Fast Read (0Bh)”, “Fast Read Quad I/O (EBh)” & “Burst Read
 * with Wrap (0Ch)” instructions, and to configure the number of bytes of “Wrap Length” for the “Burst Read
 * with Wrap (0Ch)” instruction.
 * In Standard SPI mode, the “Set Read Parameters (C0h)” instruction is not accepted. The dummy clocks
 * for various Fast Read instructions in Standard/Dual/Quad SPI mode are fixed, please refer to the
 * Instruction Table 1-2 for details. The “Wrap Length” is set by W5-4 bit in the “Set Burst with Wrap (77h)”
 * instruction. This setting will remain unchanged when the device is switched from Standard SPI mode to
 * QPI mode.
 * The default “Wrap Length” after a power up or a Reset instruction is 8 bytes, the default number of
 * dummy clocks is 2. The number of dummy clocks is only programmable for “Fast Read (0Bh)”, “Fast
 * Read Quad I/O (EBh)” & “Burst Read with Wrap (0Ch)” instructions in the QPI mode. Whenever the
 * device is switched from SPI mode to QPI mode, the number of dummy clocks should be set again, prior
 * to any 0Bh, EBh or 0Ch instructions.
 */
#define OP_SET_READ_PARAMETERS 0xC0
/**
 * 8.2.35 Burst Read with Wrap (0Ch)
 * The “Burst Read with Wrap (0Ch)” instruction provides an alternative way to perform the read operation
 * with “Wrap Around” in QPI mode. The instruction is similar to the “Fast Read (0Bh)” instruction in QPI
 * mode, except the addressing of the read operation will “Wrap Around” to the beginning boundary of the
 * “Wrap Length” once the ending boundary is reached.
 * The “Wrap Length” and the number of dummy clocks can be configured by the “Set Read Parameters
 * (C0h)” instruction.
 */
#define OP_BURSE_READ_WITH_WRAP 0x0C
/**
 * 8.2.36 Enter QPI Mode (38h)
 * The W25Q128FV support both Standard/Dual/Quad Serial Peripheral Interface (SPI) and Quad
 * Peripheral Interface (QPI). However, SPI mode and QPI mode cannot be used at the same time. “Enter
 * QPI (38h)” instruction is the only way to switch the device from SPI mode to QPI mode.
 * Upon power-up, the default state of the device upon is Standard/Dual/Quad SPI mode. This provides full
 * backward compatibility with earlier generations of Winbond serial flash memories. See Instruction Set
 * Table 1-3 for all supported SPI commands. In order to switch the device to QPI mode, the Quad Enable
 * (QE) bit in Status Register-2 must be set to 1 first, and an “Enter QPI (38h)” instruction must be issued. If
 * the Quad Enable (QE) bit is 0, the “Enter QPI (38h)” instruction will be ignored and the device will remain
 * in SPI mode.
 * See Instruction Set Table 3 for all the commands supported in QPI mode.
 * When the device is switched from SPI mode to QPI mode, the existing Write Enable and Program/Erase
 * Suspend status, and the Wrap Length setting will remain unchanged.
 */
#define OP_ENTER_QPI_MODE 0x38
/**
 * 8.2.37 Exit QPI Mode (FFh)
 * In order to exit the QPI mode and return to the Standard/Dual/Quad SPI mode, an “Exit QPI (FFh)”
 * instruction must be issued.
 * When the device is switched from QPI mode to SPI mode, the existing Write Enable Latch (WEL) and
 * Program/Erase Suspend status, and the Wrap Length setting will remain unchanged.
 */
#define OP_EXIT_QPI_MODE 0xFF
/**
 * 8.2.38 Individual Block/Sector Lock (36h)
 * The Individual Block/Sector Lock provides an alternative way to protect the memory array from adverse
 * Erase/Program. In order to use the Individual Block/Sector Locks, the WPS bit in Status Register-3 must
 * be set to 1. If WPS=0, the write protection will be determined by the combination of CMP, SEC, TB,
 * BP[2:0] bits in the Status Registers. The Individual Block/Sector Lock bits are volatile bits. The default
 * values after device power up or after a Reset are 1, so the entire memory array is being protected.
 * To lock a specific block or sector as illustrated in Figure 4d, an Individual Block/Sector Lock command
 * must be issued by driving /CS low, shifting the instruction code “36h” into the Data Input (DI) pin on the
 * rising edge of CLK, followed by a 24-bit address and then driving /CS high. A Write Enable instruction
 * must be executed before the device will accept the Individual Block/Sector Lock Instruction (Status
 * Register bit WEL= 1).
 */
#define OP_LOCK_INDIVIDUAL_BLOCK_SECTOR 0x36
/**
 * 8.2.39 Individual Block/Sector Unlock (39h)
 * The Individual Block/Sector Lock provides an alternative way to protect the memory array from adverse
 * Erase/Program. In order to use the Individual Block/Sector Locks, the WPS bit in Status Register-3 must
 * be set to 1. If WPS=0, the write protection will be determined by the combination of CMP, SEC, TB,
 * BP[2:0] bits in the Status Registers. The Individual Block/Sector Lock bits are volatile bits. The default
 * values after device power up or after a Reset are 1, so the entire memory array is being protected.
 * To unlock a specific block or sector as illustrated in Figure 4d, an Individual Block/Sector Unlock
 * command must be issued by driving /CS low, shifting the instruction code “39h” into the Data Input (DI)
 * pin on the rising edge of CLK, followed by a 24-bit address and then driving /CS high. A Write Enable
 * instruction must be executed before the device will accept the Individual Block/Sector Unlock Instruction
 * (Status Register bit WEL= 1).
 */
#define OP_UNLOCK_INDIVIDUAL_BLOCK_SECTOR 0x39
/**
 * 8.2.40 Read Block/Sector Lock (3Dh)
 * The Individual Block/Sector Lock provides an alternative way to protect the memory array from adverse
 * Erase/Program. In order to use the Individual Block/Sector Locks, the WPS bit in Status Register-3 must
 * be set to 1. If WPS=0, the write protection will be determined by the combination of CMP, SEC, TB,
 * BP[2:0] bits in the Status Registers. The Individual Block/Sector Lock bits are volatile bits. The default
 * values after device power up or after a Reset are 1, so the entire memory array is being protected.
 * To read out the lock bit value of a specific block or sector as illustrated in Figure 4d, a Read
 * Block/Sector Lock command must be issued by driving /CS low, shifting the instruction code “3Dh” into
 * the Data Input (DI) pin on the rising edge of CLK, followed by a 24-bit address. The Block/Sector Lock bit
 * value will be shifted out on the DO pin at the falling edge of CLK with most significant bit (MSB) first as
 * shown in Figure 54. If the least significant bit (LSB) is 1, the corresponding block/sector is locked; if
 * LSB=0, the corresponding block/sector is unlocked, Erase/Program operation can be performed.
 */
#define OP_LOCK_BLOCK_SECTOR 0x3D
/**
 * 8.2.41 Global Block/Sector Lock (7Eh)
 * All Block/Sector Lock bits can be set to 1 by the Global Block/Sector Lock instruction. The command
 * must be issued by driving /CS low, shifting the instruction code “7Eh” into the Data Input (DI) pin on the
 * rising edge of CLK, and then driving /CS high. A Write Enable instruction must be executed before the
 * device will accept the Global Block/Sector Lock Instruction (Status Register bit WEL= 1).
 */
#define OP_LOCK_GLOBAL_BLOCK_SECTOR 0x7E
/**
 * 8.2.42 Global Block/Sector Unlock (98h)
 * All Block/Sector Lock bits can be set to 0 by the Global Block/Sector Unlock instruction. The command
 * must be issued by driving /CS low, shifting the instruction code “98h” into the Data Input (DI) pin on the
 * rising edge of CLK, and then driving /CS high. A Write Enable instruction must be executed before the
 * device will accept the Global Block/Sector Unlock Instruction (Status Register bit WEL= 1).
 */
#define OP_UNLOCK_GLOBAL_BLOCK_SECTOR 0x98
/**
 * 8.2.43 Enable Reset (66h) and Reset Device (99h)
 * Because of the small package and the limitation on the number of pins, the W25Q128FV provide a
 * software Reset instruction instead of a dedicated RESET pin. Once the Reset instruction is accepted, any
 * on-going internal operations will be terminated and the device will return to its default power-on state and
 * lose all the current volatile settings, such as Volatile Status Register bits, Write Enable Latch (WEL)
 * status, Program/Erase Suspend status, Read parameter setting (P7-P0), Continuous Read Mode bit
 * setting (M7-M0) and Wrap Bit setting (W6-W4).
 * “Enable Reset (66h)” and “Reset (99h)” instructions can be issued in either SPI mode or QPI mode. To
 * avoid accidental reset, both instructions must be issued in sequence. Any other commands other than
 * “Reset (99h)” after the “Enable Reset (66h)” command will disable the “Reset Enable” state. A new
 * sequence of “Enable Reset (66h)” and “Reset (99h)” is needed to reset the device. Once the Reset
 * command is accepted by the device, the device will take approximately tRST=30us to reset. During this
 * period, no command will be accepted.
 * Data corruption may happen if there is an on-going or suspended internal Erase or Program operation
 * when Reset command sequence is accepted by the device. It is recommended to check the BUSY bit
 * and the SUS bit in Status Register before issuing the Reset command sequence.
 */
#define OP_ENABLE_RESET_AND_RESET_DEVICE(reset) (reset ? 0x99 : 0x66)

#endif
