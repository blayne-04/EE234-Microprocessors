| Parameter | Value / Formula | Context |
| :--- | :--- | :--- |
| **Clock Frequency** | 111.111 MHz | Base Zynq Clock |
| **TTC Prescaler** | 64 ($N=5$) | `TTC0_CLK_CTRL` setting |
| **PWM Period (20ms)** | 34,722 ticks | 50Hz Frequency |
| **Min Pulse (1.0ms)** | 1,736 ticks | Full Clockwise (CW) |
| **Center Pulse (1.5ms)** | 2,604 ticks | Neutral/Center |
| **Max Pulse (2.0ms)** | 3,472 ticks | Full Counter-Clockwise (CCW) |
| **Sweep Step (5s)** | ~7 ticks | Increment per 20ms interrupt |
| **Absolute Position** | `1736 + ((Pos * 1736) / 100)` | Maps 0-100% to pulse range |
| **ADC Mapping** | `1736 + ((ADC * 1736) / 4095)` | Maps 12-bit ADC to pulse range |

### Register Configuration Summary
* **Interval Mode**: Bit 1 in `TTC0_CNT_CTRL(1)`.
* **Match Mode**: Bit 3 in `TTC0_CNT_CTRL(1)`.
* **Output Enable**: Bit 4 in `TTC0_CNT_CTRL(1)`.
* **Waveform Polarity**: Bit 5 in `TTC0_CNT_CTRL(1)` set to 1 for Active-High.
* **Interrupt ID**: 42 (TTC0 Counter 1).

### EXPLANATION
TTC0 comes with 111.111 MHz clk, after dividing by 64, we have a base clk of 1.736mil ticks/s, multiplying by some time, 
for example 20ms, .020s * 1.736mil ticks/s is approx 34,722 ticks for 20ms.