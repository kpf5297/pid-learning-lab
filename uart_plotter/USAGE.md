# Installing from GitHub 

## macOS/Linux

```bash
git clone https://github.com/kpf5297/pid-learning-lab.git
cd uart_plotter
python3 -m venv venv
source venv/bin/activate
pip install -r requirements.txt
python uart_plot.py
```

---

## Windows

```bash
git clone https://github.com/kpf5297/pid-learning-lab.git
cd uart_plotter
python -m venv venv
venv\Scripts\activate   # or .\venv\Scripts\Activate.ps1 in PowerShell
pip install -r requirements.txt
python uart_plot.py
```

---