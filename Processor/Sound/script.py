import wave
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import math
import random



types = {
    1: np.int8,
    2: np.int16,
    4: np.int32
}

wav = wave.open('click.wav', mode='r')
(nchannels, sampwidth, framerate, nframes, comptype, compname) = wav.getparams()

wav_write = wave.open('gen.wav', mode='w')
wav_write.setparams((nchannels, sampwidth, framerate, nframes, comptype, compname))
data = []
for i in range(4000000):
    data.append(4000 * ((-1) ** (i % 2)))

duration = nframes / framerate
w, h = 1000, 800
k = nframes/w/32
DPI = 100
peak = 256 ** sampwidth / 2

content = wav.readframes(nframes)
samples = np.fromstring(content, dtype=types[sampwidth])

plt.figure(1, figsize=(float(w)/DPI, float(h)/DPI), dpi=DPI)
np.set_printoptions(threshold=np.nan)

sta = 6
# 1-------------------------------------------------------------
channel = samples[0::nchannels]
channel = [int(x) for x in channel][:]
axes = plt.subplot(sta, 1, 1, axisbg='k')
axes.plot(channel, 'r')
# 2--------------------------------------------------------------
STEP = 100
freq = []
for i in range(0, len(channel) // STEP):
    fr = 0
    for j in range(STEP - 1):
        if channel[i * STEP + j] * channel[i * STEP + j + 1] < -100:
            fr += 1
    freq.append(fr)
axes = plt.subplot(sta, 1, 2, axisbg='k')
axes.plot(freq, 'blue')    
# 3----------------------------------------------------------------
STEP = 500
channel = channel[:len(channel) - len(channel) % STEP]
DArray = []
for i in range(0, len(channel) // STEP):
    su = 0
    for j in range(STEP):
        su += abs(channel[i * STEP + j])
    DArray.append(su)
axes = plt.subplot(sta, 1, 3, axisbg='k')
axes.plot(DArray, 'g')

# 4-----------------------------------------------------------
for k in range(6):
    arr = []
    for i in range(2, len(DArray) - 2):
        arr.append((DArray[i - 2] + 2 * DArray[i - 1] + 4 * DArray[i + 2] + 2 * DArray[i + 1] + DArray[i + 2]) // 10)
    DArray = arr
axes = plt.subplot(sta, 1, 4, axisbg='k')
axes.plot(DArray, 'g')
# 5 ---------------------------------------------------------------------


# ------------------------------------------------------------

plt.savefig('wave', dpi=DPI)
plt.show()

