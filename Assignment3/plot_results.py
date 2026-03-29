import matplotlib.pyplot as plt
import csv, glob, math
import numpy as np

files = sorted(glob.glob("results_h*.csv"))
n = len(files)
cols = 2
rows = math.ceil(n / cols)

fig = plt.figure(figsize=(12, 10))
fig.suptitle("Lorenz System: Trajectories for Various Step Sizes $h$", fontsize=16)

colors = plt.cm.viridis(np.linspace(0, 1, n)) if n > 0 else []
# If numpy isn't imported, let's use a simpler way to avoid adding dependencies if not needed
# But standard practice is to use a color cycle.
color_cycle = plt.rcParams['axes.prop_cycle'].by_key()['color']

handles = []
labels = []

for i, f in enumerate(files):
    ax = fig.add_subplot(rows, cols, i + 1, projection='3d')
    h_val = f.split('_h')[1].replace('.csv','')
    with open(f, 'r') as csvfile:
        reader = csv.reader(csvfile)
        next(reader)
        pts = [(float(r[1]), float(r[2]), float(r[3])) for r in reader]
        x, y, z = zip(*pts)
        line, = ax.plot(x, y, z, color=color_cycle[i % len(color_cycle)], label=f"h = {h_val}")
        ax.set_xlabel('X'); ax.set_ylabel('Y'); ax.set_zlabel('Z')
        
        if i == 0: # Collect handles for the first few to show in legend if they are distinct
            pass
    handles.append(line)
    labels.append(f"h = {h_val}")

fig.legend(handles, labels, loc='upper right', title="Step Sizes")
plt.tight_layout(rect=[0, 0.03, 1, 0.95])
plt.savefig('lorenz_subplots_3d.png')
print("Subplots saved as lorenz_subplots_3d.png")
