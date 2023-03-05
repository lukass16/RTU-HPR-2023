# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:04:59 2023

@author: lukas
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

""" SETUP """

start = 0
end = 3825

plt.style.use("default")

data = pd.read_csv("P900_I5000_D5.csv", sep=',')

# Extracting the data to lists
t = data["Time [ms]"].to_list()
pl_v = data["Wheel Speed [%]"].to_list()
w_s = data["Plat Ang Vel [rad/s]"].to_list()

Kp = data["Kp"].to_list()
Ki = data["Ki"].to_list()
Kd = data["Kd"].to_list()

P = data["P"].to_list()
I = data["I"].to_list()
D = data["D"].to_list()

# Generate setpoint
setPoint = list()
for el in t:
    setPoint.append(0)

# Printing info
print("Data successfully extracted from data.CSV, amount:", len(t))
print(max(pl_v))


# Plot platform velocity data
fig, ax = plt.subplots(figsize=(16,8))
ax.plot(t[start:end], pl_v[start:end], color="coral", label="Plat Ang Vel [rad/s]")
ax.plot(t[start:end], setPoint[start:end], color="b", label="Setpoint")
plt.title("Platform Angular Velocity vs Time")
plt.legend()
plt.savefig("platform.png", dpi=300)


# Plot component development data
fig, ax = plt.subplots(figsize=(20,15))
ax.plot(t[start:end], pl_v[start:end], color="coral", label="Plat Ang Vel [rad/s]")
ax.plot(t[start:end], w_s[start:end], color="k", label="Wheel Speed [%]")
ax.plot(t[start:end], setPoint[start:end], color="b", label="Setpoint")
ax.plot(t[start:end], P[start:end], color="g", label="Proportional component")
ax.plot(t[start:end], I[start:end], color="y", label="Integral component")
ax.plot(t[start:end], D[start:end], color="m", label="Derivative component")
plt.title("PID controler component development vs Time")
plt.grid()
plt.legend()
plt.savefig("components.png", dpi=500)


# Plot all data
fig, ax = plt.subplots(figsize=(12,8))
ax.plot(t[start:end], pl_v[start:end], color="coral", label="Plat Ang Vel [rad/s]")
ax.plot(t[start:end], setPoint[start:end], color="b", label="Setpoint")
ax.plot(t[start:end], Kp[start:end], color="k", label="Proportional gain")
ax.plot(t[start:end], Ki[start:end], color="y", label="Integral gain")
ax.plot(t[start:end], Kd[start:end], color="w", label="Derivative gain")
ax.plot(t[start:end], P[start:end], color="r", label="Proportional component")
ax.plot(t[start:end], I[start:end], color="g", label="Integral component")
ax.plot(t[start:end], D[start:end], color="m", label="Derivative component")
plt.title("All data vs Time")
plt.legend()
plt.savefig("all.png", dpi=300)
