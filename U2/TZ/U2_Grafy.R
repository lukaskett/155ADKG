#Load package
library(plotly)
library(dplyr)

#Read data - check directory path
JarvisScanDATA <- read.csv("JarvisScan.txt")
QuickHullDATA <- read.csv("QuickHull.txt")
SweepLineDATA <- read.csv("SweepLine.txt")
GrahamScanDATA <- read.csv("GrahamScan.txt")

#Calculate average elapsed time for 10 repetition
average_j <- c()
average_q <- c()
average_s <- c()
average_g <- c()
rozptyl_j <- c()
rozptyl_q <- c()
rozptyl_s <- c()
rozptyl_g <- c()

for(i in 1:33)
{
  sum_j = 0
  sum_q = 0
  sum_s = 0
  sum_g = 0
  min_j = 1000000
  max_j = 0
  min_q = 1000000
  max_q = 0
  min_s = 1000000
  max_s = 0
  min_g = 1000000
  max_g = 0
  
  for(n in ((i*10) + seq(1, 10, 1) - 10))
  {
    sum_j <- sum_j + JarvisScanDATA[,c("elapsed.time.miliseconds.")][n]
    sum_q <- sum_q + QuickHullDATA[,c("elapsed.time.miliseconds.")][n]
    sum_s <- sum_s + SweepLineDATA[,c("elapsed.time.miliseconds.")][n]
    sum_g <- sum_g + GrahamScanDATA[,c("elapsed.time.miliseconds.")][n]
 
    #Maximum   
    if(JarvisScanDATA[,c("elapsed.time.miliseconds.")][n] > max_j)
    {max_j = JarvisScanDATA[,c("elapsed.time.miliseconds.")][n]}
    
    if(QuickHullDATA[,c("elapsed.time.miliseconds.")][n] > max_q)
    {max_q = QuickHullDATA[,c("elapsed.time.miliseconds.")][n]}
    
    if(SweepLineDATA[,c("elapsed.time.miliseconds.")][n] > max_s)
    {max_s = SweepLineDATA[,c("elapsed.time.miliseconds.")][n]}
     
    if(GrahamScanDATA[,c("elapsed.time.miliseconds.")][n] > max_g)
    {max_g = GrahamScanDATA[,c("elapsed.time.miliseconds.")][n]}
    
    #Minimum
    if(JarvisScanDATA[,c("elapsed.time.miliseconds.")][n] < min_j)
    {min_j = JarvisScanDATA[,c("elapsed.time.miliseconds.")][n]}
    
    if(QuickHullDATA[,c("elapsed.time.miliseconds.")][n] < min_q)
    {min_q = QuickHullDATA[,c("elapsed.time.miliseconds.")][n]}

    if(SweepLineDATA[,c("elapsed.time.miliseconds.")][n] < min_s)
    {min_s = SweepLineDATA[,c("elapsed.time.miliseconds.")][n]}
    
    if(GrahamScanDATA[,c("elapsed.time.miliseconds.")][n] < min_g)
    {min_g = GrahamScanDATA[,c("elapsed.time.miliseconds.")][n]}
  }
  #Average
  average_j <-c(average_j, sum_j/10)
  average_q <-c(average_q, sum_q/10)
  average_s <-c(average_s, sum_s/10)
  average_g <-c(average_g, sum_g/10)
  
  #Rozptyl
  rozptyl_j <- c(rozptyl_j, abs(min_j - max_j))
  rozptyl_q <- c(rozptyl_q, abs(min_q - max_q))
  rozptyl_s <- c(rozptyl_s, abs(min_s - max_s))
  rozptyl_g <- c(rozptyl_g, abs(min_g - max_g))

}

#Get values fot each 10th row
method <- c()
points <- c()

for(i in seq(1, length(JarvisScanDATA$Count.points), 10))
{
  method <- c(method,JarvisScanDATA$generating.method[i])
  points <- c(points,JarvisScanDATA$Count.points[i])
}

#Data frame with average values
Jarvis_avg <- data.frame(points, method, average_j, rozptyl_j)
Quick_avg <- data.frame(points, method, average_q, rozptyl_q)
Sweep_avg <- data.frame(points, method, average_s, rozptyl_s)
Graham_avg <- data.frame(points, method, average_g, rozptyl_g)

#Separate methods - raster, random, circle
#1)Jarvis Scan
Jarvis_raster <- select(filter(Jarvis_avg, method == 3), c(points, method, average_j, rozptyl_j))
Jarvis_random <- select(filter(Jarvis_avg, method == 2), c(points, method, average_j, rozptyl_j))
Jarvis_circle <- select(filter(Jarvis_avg, method == 1), c(points, method, average_j, rozptyl_j))

#2)Quick Hull
Quick_raster <- select(filter(Quick_avg, method == 3), c(points, method, average_q, rozptyl_q))
Quick_random <- select(filter(Quick_avg, method == 2), c(points, method, average_q, rozptyl_q))
Quick_circle <- select(filter(Quick_avg, method == 1), c(points, method, average_q, rozptyl_q))

#3)Sweep Line
Sweep_raster <- select(filter(Sweep_avg, method == 3), c(points, method, average_s, rozptyl_s))
Sweep_random <- select(filter(Sweep_avg, method == 2), c(points, method, average_s, rozptyl_s))
Sweep_circle <- select(filter(Sweep_avg, method == 1), c(points, method, average_s, rozptyl_s))

#4)Graham Scan
Graham_raster <- select(filter(Graham_avg, method == 3), c(points, method, average_g, rozptyl_g))
Graham_random <- select(filter(Graham_avg, method == 2), c(points, method, average_g, rozptyl_g))
Graham_circle <- select(filter(Graham_avg, method == 1), c(points, method, average_g, rozptyl_g))

#Data to plot
plot_jarvis <- data.frame(Jarvis_circle$points, Jarvis_raster$average, Jarvis_random$average, Jarvis_circle$average)
plot_quick <- data.frame(Quick_circle$points, Quick_raster$average, Quick_random$average, Quick_circle$average)
plot_sweep <- data.frame(Sweep_circle$points, Sweep_raster$average, Sweep_random$average, Sweep_circle$average)
plot_graham <- data.frame(Graham_circle$points, Graham_raster$average, Graham_random$average, Graham_circle$average)

#####Plot whatever you want

#Jarvis Scan - methods comparison
p_jarvis <- plot_ly(plot_jarvis, x = ~Jarvis_raster$points, y = ~Jarvis_raster$average_j, name = 'Raster', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~Jarvis_random$average_j, name = 'Random', mode = 'lines') %>%
  add_trace(y = ~Jarvis_circle$average_j, name = 'Circle', mode = 'lines')%>%
  layout(title = 'Jarvis Scan',
         xaxis = list(title = 'Number of points', type = "log"),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_jarvis

#Quick Hull - methods comparison
p_quick <- plot_ly(plot_quick, x = ~Quick_raster$points, y = ~Quick_raster$average_q, name = 'Raster', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~Quick_random$average_q, name = 'Random', mode = 'lines') %>%
  add_trace(y = ~Quick_circle$average_q, name = 'Circle', mode = 'lines')%>%
  layout(title = 'Quick Hull',
         xaxis = list(title = 'Number of points', type = "log"),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_quick

#Sweep Line - methods comparison
p_sweep <- plot_ly(plot_sweep, x = ~Sweep_raster$points, y = ~Sweep_raster$average_s, name = 'Raster', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~Sweep_random$average_s, name = 'Random', mode = 'lines') %>%
  add_trace(y = ~Sweep_circle$average_s, name = 'Circle', mode = 'lines')%>%
  layout(title = 'Sweep Line',
         xaxis = list(title = 'Number of points', type = "log"),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_sweep

#Graham Scan - methods comparison
p_graham <- plot_ly(plot_graham, x = ~Graham_raster$points, y = ~Graham_raster$average_g, name = 'Raster', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~Graham_random$average_g, name = 'Random', mode = 'lines') %>%
  add_trace(y = ~Graham_circle$average_g, name = 'Circle', mode = 'lines')%>%
  layout(title = 'Graham Scan',
         xaxis = list(title = 'Number of points', type = "log"),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_graham

#Compare methods between algorithms
#1)raster
plot_raster <- data.frame(points = Graham_circle$points, avg_j = Jarvis_raster$average, avg_q = Quick_raster$average, avg_s = Sweep_raster$average, avg_g = Graham_raster$average)
p_raster <- plot_ly(plot_raster, x = ~points, y = ~avg_j, name = 'Jarvis Scan', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~avg_q, name = 'Quick Hull', mode = 'lines') %>%
  add_trace(y = ~avg_s, name = 'Sweep Line', mode = 'lines')%>%
  add_trace(y = ~avg_g, name = 'Graham Scan', mode = 'lines')%>%
  layout(title = 'Raster generating method for all algorithms',
         xaxis = list(title = 'Number of points', type = "log" ),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_raster

#2)random
plot_random <- data.frame(points = Graham_circle$points, avg_j = Jarvis_random$average, avg_q = Quick_random$average, avg_s = Sweep_random$average, avg_g = Graham_random$average)
p_random <- plot_ly(plot_random, x = ~points, y = ~avg_j, name = 'Jarvis Scan', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~avg_q, name = 'Quick Hull', mode = 'lines') %>%
  add_trace(y = ~avg_s, name = 'Sweep Line', mode = 'lines')%>%
  add_trace(y = ~avg_g, name = 'Graham Scan', mode = 'lines')%>%
  layout(title = 'Random generating method for all algorithms',
         xaxis = list(title = 'Number of points', type = "log" ),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_random

#3)circle
plot_circle <- data.frame(points = Graham_circle$points, avg_j = Jarvis_circle$average, avg_q = Quick_circle$average, avg_s = Sweep_circle$average, avg_g = Graham_circle$average)
p_circle <- plot_ly(plot_circle, x = ~points, y = ~avg_j, name = 'Jarvis Scan', type = 'scatter', mode = 'lines') %>%
  add_trace(y = ~avg_q, name = 'Quick Hull', mode = 'lines') %>%
  add_trace(y = ~avg_s, name = 'Sweep Line', mode = 'lines')%>%
  add_trace(y = ~avg_g, name = 'Graham Scan', mode = 'lines')%>%
  layout(title = 'Circle generating method for all algorithms',
         xaxis = list(title = 'Number of points', type = "log" ),
         yaxis = list(title = 'Time[miliseconds]', type = "log"))
p_circle

#Write to file
jarvisTXT <- data.frame(Jarvis_raster, Jarvis_random, Jarvis_circle)
quickTXT <- data.frame(Quick_raster, Quick_random, Quick_circle)
sweepTXT <- data.frame(Sweep_raster, Sweep_random, Sweep_circle)
grahamTXT <- data.frame(Graham_raster, Graham_random, Graham_circle)
write.csv(jarvisTXT,'D:\\CVUT\\155ADKG Algoritmy\\ulohy\\U2\\JarvisScanTXT.txt', row.names = TRUE)
write.csv(quickTXT,'D:\\CVUT\\155ADKG Algoritmy\\ulohy\\U2\\QuickHullTXT.txt', row.names = TRUE)
write.csv(sweepTXT,'D:\\CVUT\\155ADKG Algoritmy\\ulohy\\U2\\SweepLineTXT.txt', row.names = TRUE)
write.csv(grahamTXT,'D:\\CVUT\\155ADKG Algoritmy\\ulohy\\U2\\GrahamScanTXT.txt', row.names = TRUE)



