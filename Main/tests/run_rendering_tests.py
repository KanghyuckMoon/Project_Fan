#!/usr/bin/env python3
"""Host regression tests: python3 tests/run_rendering_tests.py (requires gcc).

Only the GPIO bit-banging implementation of SPI_Write is replaced. Production
rendering code is compiled against fake AVR registers and a TFT protocol decoder.
"""
import os
from pathlib import Path
import subprocess
import tempfile


ROOT = Path(__file__).resolve().parents[1]


def main():
    with tempfile.TemporaryDirectory(prefix="fan-rendering-") as directory:
        work = Path(directory)
        (work / "avr").mkdir()
        (work / "util").mkdir()
        (work / "avr/io.h").write_text(
            "#include <stdint.h>\n"
            "extern uint8_t PORTA, PORTB, DDRA, DDRB;\n"
            "#define PB0 0\n#define PB1 1\n#define PB2 2\n"
            "#define PA0 0\n#define PA1 1\n"
        )
        (work / "avr/interrupt.h").write_text("")
        (work / "util/delay.h").write_text("#define _delay_ms(ms) ((void)(ms))\n")
        source = (ROOT / "src/project/tft.c").read_text()
        start = source.index("void SPI_Write(")
        end = source.index("void TFT_Command(", start)
        (work / "tft.c").write_text(source[:start] + source[end:])
        command = ["gcc", "-std=c11", "-g", "-Wall", "-Wextra",
                   "-Wno-unused-function", "-fsanitize=address,undefined",
                   "-fno-omit-frame-pointer", "-I" + str(work)]
        for path in ("src/common", "src/project", "src/project/GUI", "src/driver"):
            command.append("-I" + str(ROOT / path))
        command += [str(work / "tft.c")]
        for path in ("src/project/GUI/GUI.c", "src/project/GUI/GUIButton.c",
                     "src/project/GUI/GUIBounds.c", "tests/rendering_test.c"):
            command.append(str(ROOT / path))
        command += ["-o", str(work / "rendering_test")]
        subprocess.run(command, check=True)
        environment = os.environ.copy()
        # LeakSanitizer cannot run under the desktop executor's ptrace wrapper.
        environment["ASAN_OPTIONS"] = "detect_leaks=0"
        subprocess.run([str(work / "rendering_test")], check=True, env=environment)


if __name__ == "__main__":
    main()
