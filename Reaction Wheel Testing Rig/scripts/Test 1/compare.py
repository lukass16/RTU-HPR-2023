# -*- coding: utf-8 -*-
"""
Created on Wed Feb 22 10:04:59 2023

@author: lukas
"""

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

""" SETUP """
ID = 4


plt.style.use("default")

data0 = pd.read_csv("TEST1_01.csv".format(ID), sep=',')
data1 = pd.read_csv("TEST1_11.csv".format(ID), sep=',')
data2 = pd.read_csv("TEST1_21.csv".format(ID), sep=',')
data3 = pd.read_csv("TEST1_31.csv".format(ID), sep=',')
data4 = pd.read_csv("TEST1_41.csv".format(ID), sep=',')

# Extracting the data to lists
t0 = data0["Time [ms]"].to_list()
t1 = data1["Time [ms]"].to_list()
t2 = data2["Time [ms]"].to_list()
t3 = data3["Time [ms]"].to_list()
t4 = data4["Time [ms]"].to_list()
p4 = data4["Motor Power [8bit int]"].to_list()

rpm0 = data0["Motor RPM [1/min]"].to_list()
rpm1 = data1["Motor RPM [1/min]"].to_list()
rpm2 = data2["Motor RPM [1/min]"].to_list()
rpm3 = data3["Motor RPM [1/min]"].to_list()
rpm4 = data4["Motor RPM [1/min]"].to_list()


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
ax1.plot(t4, p4, color=ax1color)
ax1.tick_params(axis='y', labelcolor=ax1color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2color = "coral"
ax2.set_ylabel('Motor RPM [1/min]', color=ax2color)
ax2.plot(t0, rpm0, color="k", label="no wheel")
ax2.plot(t1, rpm1, color="m", label="1 wheel")
ax2.plot(t2, rpm2, color="r", label="2 wheels")
ax2.plot(t3, rpm3, color="g", label="3 wheels")
ax2.plot(t4, rpm4, color="coral", label="4 wheels")
ax2.legend()
ax2.tick_params(axis='y', labelcolor=ax2color)
plt.savefig("compare.png".format(ID), dpi=300)