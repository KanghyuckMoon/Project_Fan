#!/usr/bin/env python3
"""Host regression tests: python3 tests/run_rendering_tests.py (requires gcc).

Only the hardware byte transport SPI_Write is replaced. Production
rendering code is compiled against fake AVR registers and a TFT protocol decoder.
"""
import os
from pathlib import Path
import subprocess
import tempfile
import argparse


ROOT = Path(__file__).resolve().parents[1]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--preview-dir", type=Path, help="Export actual application screens as PPM")
    arguments = parser.parse_args()
    with tempfile.TemporaryDirectory(prefix="fan-rendering-") as directory:
        work = Path(directory)
        (work / "avr").mkdir()
        (work / "util").mkdir()
        (work / "avr/io.h").write_text(
            "#include <stdint.h>\n"
            "extern uint8_t PORTA, PORTB, DDRA, DDRB;\n"
            "extern volatile uint8_t SPCR, SPSR, SPDR;\n"
            "#define PB0 0\n#define PB1 1\n#define PB2 2\n"
            "#define PA0 0\n#define PA1 1\n"
            "#define SPE 6\n#define MSTR 4\n#define SPIF 7\n#define SPI2X 0\n"
        )
        (work / "avr/interrupt.h").write_text("")
        (work / "avr/pgmspace.h").write_text(
            "#define PROGMEM\n#define pgm_read_byte(p) (*(const uint8_t *)(p))\n"
            "#define pgm_read_word(p) (*(const uint16_t *)(p))\n"
        )
        (work / "util/delay.h").write_text("#define _delay_ms(ms) ((void)(ms))\n")
        source = (ROOT / "src/project/tft.c").read_text()
        start = source.index("void SPI_Write(")
        end = source.index("void TFT_Command(", start)
        (work / "spi.c").write_text('#include "tft.h"\n' + source[start:end])
        (work / "tft.c").write_text(source[:start] + source[end:])
        command = ["gcc", "-std=c11", "-g", "-Wall", "-Wextra",
                   "-Wno-unused-function", "-fsanitize=address,undefined",
                   "-fno-omit-frame-pointer", "-I" + str(work)]
        for path in ("src/common", "src/project", "src/project/GUI", "src/driver", "src/ap"):
            command.append("-I" + str(ROOT / path))
        command += [str(work / "tft.c")]
        for path in ("src/project/GUI/GUI.c", "src/project/GUI/GUIButton.c",
                     "src/project/GUI/GUIBounds.c", "src/project/GUI/FontAndColor.c",
                     "src/ap/ap.c", "tests/rendering_test.c"):
            command.append(str(ROOT / path))
        command += ["-o", str(work / "rendering_test")]
        subprocess.run(command, check=True)
        environment = os.environ.copy()
        # LeakSanitizer cannot run under the desktop executor's ptrace wrapper.
        environment["ASAN_OPTIONS"] = "detect_leaks=0"
        if arguments.preview_dir:
            arguments.preview_dir.mkdir(parents=True, exist_ok=True)
            environment["RENDER_PREVIEW_DIR"] = str(arguments.preview_dir.resolve())
        subprocess.run([str(work / "rendering_test")], check=True, env=environment)
        # Exercise the actual production SPDR/SPIF byte function separately.
        spi_command = command[:command.index(str(work / "tft.c"))]
        spi_command += [str(work / "spi.c"), str(ROOT / "tests/spi_test.c"),
                        "-o", str(work / "spi_test")]
        subprocess.run(spi_command, check=True)
        subprocess.run([str(work / "spi_test")], check=True, env=environment)


if __name__ == "__main__":
    main()
