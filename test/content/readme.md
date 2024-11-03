# Reversing Bits Cheatsheets

Welcome to the **Reversing Bits Cheatsheets** repository! This collection provides comprehensive guides on various tools essential for assembly programming, reverse engineering, and binary analysis. Each cheatsheet offers installation instructions, usage examples, and advanced tips for different operating systems.

**Website:** https://mohitmishra786.github.io/reversingBits/

## Tools Included

### Assembly & Basic Analysis
- **[NASM](src/nasm.md)**: A popular assembler for the x86 and x86-64 architectures.
- **[GAS](src/gas.md)**: GNU Assembler, part of the GNU Binutils project, used for assembling AT&T syntax assembly.
- **[objdump](src/objdump.md)**: A powerful tool for displaying information about object files.
- **[Hexdump](src/hexdump.md)**: Used to display or dump binary data in hexadecimal format.
- **[strings](src/strings.md)**: Extracts printable strings from files, useful for quick analysis.
- **[file](src/file.md)**: Determines file type by examining its contents.
- **[nm](src/nm.md)**: Lists symbols from object files.
- **[readelf](src/readelf.md)**: Displays information about ELF (Executable and Linkable Format) files.

### Debuggers & Dynamic Analysis
- **[GDB](src/gdb.md)**: The GNU Debugger for debugging programs at the source or assembly level.
- **[OllyDbg](src/ollydbg.md)**: A 32-bit assembler level debugger for Windows.
- **[WinDbg](src/windbg.md)**: Microsoft's debugger for Windows applications.
- **[QEMU](src/qemu.md)**: Emulator and virtualizer for cross-platform analysis.
- **[Valgrind](src/valgrind.md)**: Tool suite for debugging and profiling Linux programs.
- **[Unicorn](src/unicorn.md)**: Lightweight, multi-platform CPU emulator framework.

### Disassemblers & Decompilers
- **[IDA Pro](src/idapro.md)**: Industry-standard disassembler and debugger.
- **[Ghidra](src/ghidra.md)**: NSA's software reverse engineering suite.
- **[Binary Ninja](src/binaryninja.md)**: Modern reverse engineering platform.
- **[Hopper](src/hopper.md)**: Reverse engineering tool for macOS and Linux.
- **[RetDec](src/retdec.md)**: Retargetable machine-code decompiler.
- **[Radare2](src/radare2.md)**: Complete framework for reverse-engineering.
- **[Rizin](src/rizin.md)**: Fork of radare2 with enhanced features.

### Binary Analysis Frameworks
- **[Angr](src/angr.md)**: Python framework for binary analysis.
- **[BAP](src/bap.md)**: Binary Analysis Platform for reverse engineering.
- **[Capstone](src/capstone.md)**: Lightweight multi-architecture disassembly framework.
- **[Dyninst](src/dyninst.md)**: Binary instrumentation and analysis library.
- **[Frida](src/frida.md)**: Dynamic instrumentation toolkit.
- **[PIN](src/pin.md)**: Intel's dynamic binary instrumentation framework.

### Malware Analysis & Security
- **[YARA](src/yara.md)**: Pattern matching tool for malware analysis.
- **[Zynamics](src/zynamics.md)**: Binary difference analysis tools.
- **[Intel XED](src/intelXed.md)**: X86 encoder decoder library.
- **[Spike](src/spike.md)**: Network protocol fuzzer.
- **[FrEEdom](src/freedom.md)**: Binary analysis framework.
- **[Diaphora](src/diaphora.md)**: Advanced binary diffing tool for IDA Pro.

## How to Use
- **Installation**: Follow the OS-specific instructions in each cheatsheet for tool installation.
- **Usage**: Each file contains usage examples, common commands, and advanced tips.
- **Contributing**: If you have improvements or additional tools to add, please fork the repository, make your changes, and submit a pull request.

## License
This repository is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgements
- Thanks to the developers and communities behind these tools for their invaluable resources.
- Contributions are always appreciated! Check the [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines on how to contribute.