# Installing from GitHub 

## macOS/Linux

```bash
git clone https://github.com/kpf5297/pid-learning-lab.git
cd uart_plotter
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
# replace /dev/ttyACM0 with your serial port
python uart_plot.py --port /dev/ttyACM0
```

---

## Windows

```bash
git clone https://github.com/kpf5297/pid-learning-lab.git
cd uart_plotter
python -m venv venv
venv\Scripts\activate   # or .\venv\Scripts\Activate.ps1 in PowerShell
pip install -r requirements.txt
# replace COM3 with the serial port shown in Device Manager
python uart_plot.py --port COM3
```

---
