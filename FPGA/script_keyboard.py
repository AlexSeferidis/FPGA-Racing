import subprocess
import threading
import time
import keyboard

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
        #"""Reads output from nios2-terminal, interprets it, and simulates keyboard actions."""
        x = 0
        while True:
            output_line = self._process.stdout.readline().split("/")
            if not output_line:
                break
            if x % 5 == 0: # adjusts keyboard input rate bigger = slower
                self._process_accelerometer_data(output_line)
                time.sleep(0.01)
            x = x+1

    def _process_accelerometer_data(self, data):
        try:
            ButtonA, ButtonB, Steering, Drive = data

            #print(ButtonA, ButtonB, Steering, Drive)
            
            # steering: + left - right
            # drive: + accelerate - decelerate
            if ButtonA == '1':
                keyboard.press('q')
            elif ButtonA == '0':  
                keyboard.release('q')
            #time.sleep(0.001)

            if ButtonB == '1':
                keyboard.press('e')
            elif ButtonB == '0':  
                keyboard.release('e')
            #time.sleep(0.001)
            if Steering == '1' and Drive == '1\n':
                keyboard.press('a + w')
            elif Steering == '1' and Drive == '0\n': 
                keyboard.press('a')
                keyboard.release('w') 
                keyboard.release('s')
            elif Steering == '1'and Drive == '-1\n':  
                keyboard.press('a + s')
            elif Steering == '0'and Drive == '1\n':  
                keyboard.press('w')
                keyboard.release('a')
                keyboard.release('d')
            elif Steering == '0'and Drive == '0\n':  
                keyboard.release('w')
                keyboard.release('a')
                keyboard.release('s')
                keyboard.release('d')
            elif Steering == '0'and Drive == '-1\n':  
                keyboard.press('s')
                keyboard.release('a')
                keyboard.release('d')
            elif Steering == '-1'and Drive == '1\n':  
                keyboard.press('d + w')
            elif Steering == '-1'and Drive == '0\n':  
                keyboard.press('d')
                keyboard.release('w')
                keyboard.release('s')
            elif Steering == '-1'and Drive == '-1\n':  
                keyboard.press('d + s')

            #time.sleep(0.001)
                
        
        except ValueError:
            # Handle cases where data is not in the expected format
            # print(f"Received unexpected data format: {data}")
            pass

    def stop(self):
        if self._process:
            self._process.terminate()
            self._process = None

if __name__ == "__main__":
    nios2_terminal_path = 'C:/intelFPGA_lite/18.1/quartus/bin64/nios2-terminal.exe'
    reader = FpgaAccelerometerReader(nios2_terminal_path)
    try:
        reader.start()
        input("Hit Enter to stop...\n")
    finally:
        reader.stop()