# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:04:59 2023

@author: lukas
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

""" SETUP """

plt.style.use("default")

data = pd.read_csv("piddata.csv", sep=',')

# Extracting the data to lists
t = data["Time [ms]"].to_list()
pl_v = data["Plat Ang Vel [rad/s]"].to_list()
w_s = data["Wheel Speed [%]"].to_list()

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

# Plot platform velocity data
fig, ax = plt.subplots(figsize=(12,8))
ax.plot(t, pl_v, color="o", label="Plat Ang Vel [rad/s]")
ax.plot(t, setPoint, color="b", label="Setpoint")
plt.title("Platform Angular Velocity vs Time")
plt.savefig("platvel.png", dpi=300)

# Plot component development data
fig, ax = plt.subplots(figsize=(12,8))
ax.plot(t, pl_v, color="o", label="Plat Ang Vel [rad/s]")
ax.plot(t, setPoint, color="b", label="Setpoint")
ax.plot(t, P, color="g", label="Proportional component")
ax.plot(t, I, color="y", label="Integral component")
ax.plot(t, D, color="m", label="Derivative component")
plt.title("PID controler component development vs Time")
plt.savefig("components.png", dpi=300)

# Plot all data
fig, ax = plt.subplots(figsize=(12,8))
ax.plot(t, pl_v, color="o", label="Plat Ang Vel [rad/s]")
ax.plot(t, setPoint, color="b", label="Setpoint")
ax.plot(t, Kp, color="k", label="Proportional gain")
ax.plot(t, Ki, color="y", label="Integral gain")
ax.plot(t, Kd, color="w", label="Derivative gain")
ax.plot(t, P, color="r", label="Proportional component")
ax.plot(t, I, color="g", label="Integral component")
ax.plot(t, D, color="m", label="Derivative component")
plt.title("All data vs Time")
plt.savefig("all.png", dpi=300)