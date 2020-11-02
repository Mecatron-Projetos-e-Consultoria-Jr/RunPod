import matplotlib.pyplot as plt
import matplotlib.animation as animation
import subprocess
import sys 

fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

def animate(i):

    # read the file passed in the argument
    lines = subprocess.check_output(['tail', '-n', '1000', sys.argv[1]]).decode()
    csv_data = lines.split('\n')
    x_data = []
    y_data = []

    for line in csv_data:
        if line == '':
            continue
        x,y = line.split(',')
        print(x,y)
        x_data.append(float(x))
        y_data.append(float(y))

    ax1.clear()
    ax1.plot(x_data[-250:],y_data[-250:])


ani = animation.FuncAnimation(fig,animate,interval=1)
plt.show()
