# Installing from GitHub 

## macOS/Linux

```bash
git clone https://github.com/kpf5297/pid-learning-lab.git
cd uart_plotter
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python uart_plot.py --serial-port /dev/ttyUSB0
```

---

## Windows

```bash
git clone https://github.com/kpf5297/pid-learning-lab.git
cd uart_plotter
python -m venv venv
venv\Scripts\activate   # or .\venv\Scripts\Activate.ps1 in PowerShell
pip install -r requirements.txt
python uart_plot.py --serial-port COM5
```

---

## Key Controls

- Press `r` to toggle raw ADC data.
- Press `s` to toggle scaled values.
- Press `p` to toggle PWM duty cycle.

Example session:

```text
$ python uart_plot.py --serial-port /dev/ttyUSB0
Logging from /dev/ttyUSB0 at 115200
Saving to log_20250713_123456.csv
Raw display ON
Scaled display ON
PWM display ON
```
