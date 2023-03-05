# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:04:59 2023

@author: lukas
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

""" SETUP """
ID = 3


plt.style.use("default")

data = pd.read_csv("TEST1_{}.csv".format(ID), sep=',')

# Extracting the data to lists
t = data["Time [ms]"].to_list()
p = data["Motor Power [8bit int]"].to_list()
rpm = data["Motor RPM [1/min]"].to_list()

'''
# Plot power  curve
fig, ax = plt.subplots(figsize=(16,8))
ax.plot(t, p, color="k", label="Motor Power [8bit int]")
plt.title("Motor Power vs Time")
plt.legend()
plt.savefig("p_{}.png".format(ID), dpi=300)

# Plot RPM  curve
fig, ax = plt.subplots(figsize=(16,8))
ax.plot(t, rpm, color="b", label="Motor RPM [1/min]")
plt.title("Motor RPM vs Time")
plt.legend()
plt.savefig("rpm_{}.png".format(ID), dpi=300)
'''

# Merged plot
fig, ax1 = plt.subplots(figsize=(16,8))
ax1color = "b"
ax1.set_xlabel('Time [ms]')
ax1.set_ylabel('Motor Power [8bit int]', color=ax1color)
ax1.plot(t, p, color=ax1color)
ax1.tick_params(axis='y', labelcolor=ax1color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2color = "coral"
ax2.set_ylabel('Motor RPM [1/min]', color=ax2color)
ax2.plot(t, rpm, color=ax2color)
ax2.tick_params(axis='y', labelcolor=ax2color)
plt.savefig("merge_{}.png".format(ID), dpi=300)