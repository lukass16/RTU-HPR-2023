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

data = pd.read_csv("TEST3_{}.csv".format(ID), sep=',')
datarpm = pd.read_csv("TEST3_{}RPM.csv".format(ID), sep=',')

# Extracting test data to lists
t = data["Time [ms]"].to_list()
p = data["Motor Power [8bit int]"].to_list()
platrpm = data["Platform RPM [1/min]"].to_list()
rpm = data["Motor RPM [1/min]"].to_list()

tsep = datarpm["Time [ms]"].to_list()
rpmsep = datarpm["Motor RPM [1/min]"].to_list()

# calculate data
negplatrpm = [-x for x in platrpm]


# Platform RPM and Motor RPM vs Time
fig, ax1 = plt.subplots(figsize=(16,8))
ax1color = "k"
ax1.set_xlabel('Time [ms]')
ax1.set_ylabel('Motor RPM [1/min]', color=ax1color)
ax1.plot(t, rpm, color=ax1color)
ax1.tick_params(axis='y', labelcolor=ax1color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2color = "y"
ax2.set_ylabel('Negative of Platform RPM [1/min]', color=ax2color)
ax2.plot(t, negplatrpm, color=ax2color)
ax2.tick_params(axis='y', labelcolor=ax2color)
#plt.savefig("rpm_and_platrpm_{}.png".format(ID), dpi=300)



# Platform RPM and Separate Motor RPM vs Time
fig, ax1 = plt.subplots(figsize=(16,8))
ax1color = "b"
ax1.set_xlabel('Time [ms]')
ax1.set_ylabel('Motor RPM [1/min]', color=ax1color)
ax1.plot(tsep, rpmsep, color=ax1color)
ax1.tick_params(axis='y', labelcolor=ax1color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2color = "y"
ax2.set_ylabel('Negative of Platform RPM [1/min]', color=ax2color)
ax2.plot(t, negplatrpm, color=ax2color)
ax2.tick_params(axis='y', labelcolor=ax2color)
#plt.savefig("rpm_and_platrpm_{}.png".format(ID), dpi=300)



# Platform RPM and Combined Motor RPM vs Time
fig, ax1 = plt.subplots(figsize=(16,8))
ax1color = "b"
ax1.set_xlabel('Time [ms]')
ax1.set_ylabel('Motor RPM [1/min]', color=ax1color)
ax1.plot(tsep, rpmsep, color="b", label="Motor RPM from separate test")
ax1.plot(t, rpm, color="k", label="Motor RPM during test")
ax1.legend()
ax1.tick_params(axis='y', labelcolor=ax1color)

ax2 = ax1.twinx()  # instantiate a second axes that shares the same x-axis
ax2color = "y"
ax2.set_ylabel('Negative of Platform RPM [1/min]', color=ax2color)
ax2.plot(t, negplatrpm, color=ax2color)
ax2.tick_params(axis='y', labelcolor=ax2color)
plt.title("Platform RPM and Combined Motor RPM vs Time {} Wheel".format(ID))
plt.savefig("combined_{}.png".format(ID), dpi=300)