import sys
import matplotlib.pyplot as plt
import mysql.connector
import pandas as pd
import numpy as np
from datetime import date
from matplotlib.animation import FuncAnimation
from itertools import count

today=date.today()
d1 = today.strftime("%d/%m/%Y")

mydb = mysql.connector.connect(
  host="35.223.68.152",
  user="weatherapp",
  password="Thunderstorm!!",
  database="weather"
)
mycursor = mydb.cursor()
sql = "select tmp,hum,ts from cloud order by ts"
mycursor.execute(sql)
myresults = mycursor.fetchall()

plt.style.use('fivethirtyeight')

x_vals = []
y_vals = []

index = count()

def animate(i):
    df = pd.DataFrame(myresults, columns=['tmp', 'hum', 'ts']).set_index('ts')
    df.columns=['Temperture','Humidity']
    x = (['ts'])
    y1 = (['tmp'])
    y2 = (['hum'])

    plt.cla()

    plt.plot(x, y1, label='Tmp')
    plt.plot(x, y2, label='Hum')

    plt.legend(loc='upper left')
    plt.tight_layout()

ani = FuncAnimation(plt.gcf(), animate, interval=10)
print (['ts'])
plt.tight_layout()
plt.show()
