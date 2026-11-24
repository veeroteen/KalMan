import numpy as np
import matplotlib.pyplot as plt

# Загрузка данных (укажите ваши точные имена файлов)
est = np.loadtxt("est.csv")    # 6 столбцов: x, y, z, vx, vy, vz
meas = np.loadtxt("meas.csv")  # 3 столбца: zx, zy, zz
true = np.loadtxt("true.csv")  # 3 столбца: x, y, z

t = np.arange(len(true))

fig = plt.figure(figsize=(14, 8))

# 1. Общий 3D-график траектории
ax3d = fig.add_subplot(2, 2, 1, projection="3d")
ax3d.plot(true[:, 0], true[:, 1], true[:, 2], "g-", linewidth=2, label="Истина (true)")
ax3d.scatter(meas[:, 0], meas[:, 1], meas[:, 2], color="red", alpha=0.6, label="Датчик (z)")
ax3d.plot(est[:, 0], est[:, 1], est[:, 2], "b-o", markersize=4, label="Фильтр Калмана")
ax3d.set_title("3D Траектория")
ax3d.set_xlabel("X")
ax3d.set_ylabel("Y")
ax3d.set_zlabel("Z")
ax3d.legend()

# 2. Графики по каждой координате от времени (X, Y, Z)
axes_names = ["X", "Y", "Z"]
for i in range(3):
    ax = fig.add_subplot(2, 2, i + 2)
    ax.plot(t, true[:, i], "g-", linewidth=2, label="Истина")
    ax.plot(t, meas[:, i], "r--x", alpha=0.6, label="Датчик (z)")
    ax.plot(t, est[:, i], "b-o", markersize=4, label="Фильтр Калмана")
    ax.set_title(f"Координата {axes_names[i]}(t)")
    ax.set_xlabel("Шаг t")
    ax.set_ylabel(axes_names[i])
    ax.grid(True)
    ax.legend()

plt.tight_layout()
plt.show()