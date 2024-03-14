import subprocess
import threading

class FpgaAccelerometerReader:
    def __init__(self, nios2_terminal_path):
        self._nios2_terminal_path = nios2_terminal_path
        self._process = None

    def start(self):
        self._process = subprocess.Popen(
            [self._nios2_terminal_path],
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            stdin=subprocess.PIPE,
            bufsize=1,
            universal_newlines=True,
            )

        threading.Thread(target=self._read_output, daemon=True).start()

    def _read_output(self):
        """Reads output from nios2-terminal and prints to Windows terminal."""
        while True:
            output_line = self._process.stdout.readline()
            if not output_line:
                break
            
            print(output_line, end='')

    def stop(self):
        if self._process:
            self._process.terminate()
            self._process = None

if __name__ == "__main__":
    nios2_terminal_path = 'C:/intelFPGA_lite/18.1/quartus/bin64/nios2-terminal.exe'
    reader = FpgaAccelerometerReader(nios2_terminal_path)
    try:
        reader.start()
        input("Press Enter to stop...\n")
    finally:
        reader.stop()