import serial
import re
import csv
import time
from datetime import datetime
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# === CONFIG ===
SERIAL_PORT = '/dev/tty.usbmodem1103' 
BAUD_RATE = 115200
MAX_POINTS = 200

# === CSV LOG SETUP ===
csv_filename = f"log_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
csvfile = open(csv_filename, 'w', newline='')
writer = csv.writer(csvfile)
writer.writerow(['Time (s)', 'Raw', 'Scaled', 'PWM (%)'])

# === PATTERNS ===
photo_pattern = re.compile(r'Photocell read: raw=(\d+), scaled=true, value=(\d+)')
pwm_pattern = re.compile(r'LED PWM duty cycle set to (\d+)%')

# === BUFFERS ===
timestamps, raw_vals, scaled_vals, pwm_vals = [], [], [], []
current_pwm = 0
start_time = time.time()

# === FLAGS FOR TOGGLING ===
show_raw = True
show_scaled = True
show_pwm = True

# === SERIAL ===
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
print(f"Logging from {SERIAL_PORT} at {BAUD_RATE}")
print(f"Saving to {csv_filename}")

# === PLOT SETUP ===
fig, ax = plt.subplots()
line_raw, = ax.plot([], [], label='Raw', color='red')
line_scaled, = ax.plot([], [], label='Scaled', color='green')
line_pwm, = ax.plot([], [], label='PWM (%)', color='blue')
ax.set_xlabel("Time (s)")
ax.set_ylabel("Value")
ax.grid(True)
ax.legend(loc='upper right')
plt.tight_layout()

# === PLOT UPDATE FUNCTION ===
def update(frame):
    global current_pwm

    while ser.in_waiting:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        elapsed = time.time() - start_time

        photo_match = photo_pattern.search(line)
        pwm_match = pwm_pattern.search(line)

        if pwm_match:
            current_pwm = int(pwm_match.group(1))

        elif photo_match:
            raw = int(photo_match.group(1))
            scaled = int(photo_match.group(2))

            timestamps.append(elapsed)
            raw_vals.append(raw)
            scaled_vals.append(scaled)
            pwm_vals.append(current_pwm)

            if len(timestamps) > MAX_POINTS:
                timestamps.pop(0)
                raw_vals.pop(0)
                scaled_vals.pop(0)
                pwm_vals.pop(0)

            writer.writerow([f"{elapsed:.2f}", raw, scaled, current_pwm])
            csvfile.flush()

    # Update plot lines
    line_raw.set_data(timestamps, raw_vals if show_raw else [None]*len(timestamps))
    line_scaled.set_data(timestamps, scaled_vals if show_scaled else [None]*len(timestamps))
    line_pwm.set_data(timestamps, pwm_vals if show_pwm else [None]*len(timestamps))

    # Auto Y-axis scaling based only on visible data
    y_vals = []
    if show_raw: y_vals += raw_vals
    if show_scaled: y_vals += scaled_vals
    if show_pwm: y_vals += pwm_vals
    if y_vals:
        ax.set_xlim(timestamps[0], timestamps[-1])
        ax.set_ylim(min(y_vals) - 10, max(y_vals) + 10)

    return line_raw, line_scaled, line_pwm

# === KEY PRESS TOGGLE HANDLER ===
def on_key(event):
    global show_raw, show_scaled, show_pwm
    if event.key == 'r':
        show_raw = not show_raw
        print(f"Raw display {'ON' if show_raw else 'OFF'}")
    elif event.key == 's':
        show_scaled = not show_scaled
        print(f"Scaled display {'ON' if show_scaled else 'OFF'}")
    elif event.key == 'p':
        show_pwm = not show_pwm
        print(f"PWM display {'ON' if show_pwm else 'OFF'}")

fig.canvas.mpl_connect('key_press_event', on_key)
ani = FuncAnimation(fig, update, interval=100)

try:
    plt.show()
except KeyboardInterrupt:
    print("Stopped by user.")

csvfile.close()
ser.close()
