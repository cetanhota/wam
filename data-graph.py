import sys
import matplotlib.pyplot as plt
import mysql.connector
import pandas as pd
import numpy as np
from datetime import date

today=date.today()
d1 = today.strftime("%d/%m/%Y")

mydb = mysql.connector.connect(
  host="35.223.68.152",
  user="weatherapp",
  password="Thunderstorm!!",
  database="weather"
)
mycursor = mydb.cursor()
sql = "select tmp,hum,dew,hif,ts from cloud order by ts desc limit 96"
mycursor.execute(sql)
myresults = mycursor.fetchall()

df = pd.DataFrame(myresults, columns=['tmp', 'hum', 'dew', 'hif', 'ts']).set_index('ts')
df.columns=['Temperture','Humidity','Dew Point','Heat Index']
df.plot(kind='line')

plt.title('\nLast 24 Hours of Data.\n')
plt.xlabel(d1)
plt.ylabel('Temperture')
plt.xticks([])
plt.grid(True)
plt.legend(loc='upper center', bbox_to_anchor=(0.5, 1.08),
          ncol=4, fancybox=True, shadow=True)
plt.savefig('weather.png')
plt.show()

mycursor.close()
mydb.close()
