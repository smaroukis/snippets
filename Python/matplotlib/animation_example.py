# Classified matplotlib.animation.FuncAnimation into a Plotter class
# FuncAnimation is called during the Plotter construction
# FuncAnimation calls the Plotter.update function every interval
# Plotter.update updates the data and the plot
# Data can be updated in the Plotter.update 
# Plotter.store_datapoint is used to separate data ingestion from plotting


import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

class Plotter:

    def __init__(self):
        self.x = []
        self.y = []
        self.interval = 100

        self.fig, self.ax = plt.subplots()
        self.line, = self.ax.plot([], [], lw=2)
        self.ax.set_xlim(0, 2*np.pi) 
        self.ax.set_ylim(-1, 1)
        self.ani = FuncAnimation(self.fig, self.update, interval=self.interval) 

    def __del__(self):
        pass
        # if not self.fig.closed:
        #     print("closing plot")
        #     self.fig.close()

    def store_datapoint(self, x, y):
        """ Store data in a list
        Separates the ingestion of data from the plotting of data"""
        self.x.append(x)
        self.y.append(y)

    def update(self, i):
        """ Update/Animate function called by FuncAnimation
        Interval is defined by FuncAnimation and self.interval
        FuncAnimation increments and passes frame number i and calls update(i)"""
        # Update Data
        x_i = 2*np.pi*i/100
        y_i = np.sin(x_i)
        self.store_datapoint(x_i, y_i)
        
        # Update Plot
        if i > 1: # fix error for numpy of one element
            self.ax.set_xlim([min(self.x), max(self.x)])
            self.ax.set_ylim([min(self.y), max(self.y)])

        self.line.set_data(self.x, self.y)
        return self.line, 

plotter = Plotter()
plt.show()