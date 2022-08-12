import csv
#import pandas as pd
#import xlrd as xl 
#from pandas import ExcelWriter
#from pandas import ExcelFile
import math
import numpy as np
import matplotlib.pyplot as plt
import datetime
import matplotlib.dates as mdates
import matplotlib.ticker as mtick
from matplotlib.ticker import MultipleLocator, FormatStrFormatter
from dateutil.parser import parse
import operator
import sys

#Argon Density (when liquid (at b.p. ) ########
rho_lar=1.3954	#g/cm^3
#1.66201E-03
###############################################

#Proton Mass ###########################
m_proton=0.938272046 #GeV/c2 (from PDG)
########################################


'''
*Appendix: Significance of Calculated Quantities
*Collision stopping power: average rate of energy loss per unit path length, due to Coulomb collisions that result in the ionization and excitation of atoms. For heavy charged particles, the collision stopping power is often called electronic stopping power.
*Density-effect correction: enters into the formula for the collision stopping power and takes into account the reduction of the collision stopping power due to the polarization of the medium by the incident electron.
*Radiative stopping power: average rate of energy loss per unit path length due to collisions with atoms and atomic electrons in which bremsstrahlung quanta are emitted. Important only for electrons.
*Nuclear stopping power: average rate of energy loss per unit path length due to the transfer of energy to recoiling atoms in elastic collisions. Important only for heavy charged particles.
*Total stopping power: for electrons, the sum of the collision and radiative stopping powers; for protons and helium ions, the sum of collision and nuclear stopping powers.
*CSDA range: a very close approximation to the average path length traveled by a charged particle as it slows down to rest, calculated in the continuous-slowing-down approximation. 
 In this approximation, the rate of energy loss at every point along the track is assumed to be equal to the total stopping power. 
 Energy-loss fluctuations are neglected. 
 The CSDA range is obtained by integrating the reciprocal of the total stopping power with respect to energy.
*Projected range: average value of the depth to which a charged particle will penetrate in the course of slowing down to rest. 
This depth is measured along the initial direction of the particle.
*Detour factor: ratio of the projected range to the CSDA range. 
 As the result of multiple scattering, the trajectory of the particle is wiggly rather than straight, and the detour factor is always smaller than unity.
*Radiation yield: average fraction of the initial kinetic energy of an electron that is converted to bremsstrahlung energy as a particle slows down to rest, calculated in the continuous-slowing-down approximation. Important only for electrons.
'''

mom_gev = []
ke_gev = []
ke_MeV = []
csdarange = []
dedx_tot = []

filename="range_of_protons_in_LAr.txt"

nline=0
with open(filename, 'r') as f:
  reader = csv.reader(f, delimiter=' ')
  for row in reader:
    nline += 1
    if (nline>8):
      #print(row[0],row[5])
      #print('ke:{0}; range:{1}'.format(row[0], row[5]))
      print('ke:{0}; csda range:{1}; proj. range:{2}'.format(float(row[0]), float(row[4]), float(row[5])))
      mom=math.sqrt( (float(row[0])/1000.)*(2.*m_proton + (float(row[0])/1000.)) )
      mom_gev.append(float(mom))

      ke=float(float(row[0])/1000.)
      ke_gev.append(float(ke))
      ke_MeV.append(float(ke*1000.))

      csdarange.append(float(row[4])/rho_lar)

      #dedx_electron=row[1]	
      #dedx_nuclear=row[2]
      #dedex_tot=row[3]		
      dedx_tot.append(float(row[3])*rho_lar)

#Dump the data to a file
print('\n')
#for i in range(mom_gev.size()):
#  print('{0} {1}',mom_gev[i],range[i])
with open("proton_csda_mom.dat","w") as fout:
  writer = csv.writer(fout, delimiter=' ')
  writer.writerows(zip(mom_gev,csdarange))


with open("proton_csda_ke.dat","w") as fout1:
  writer = csv.writer(fout1, delimiter=' ')
  writer.writerows(zip(ke_gev,csdarange))


with open("proton_dedx_ke.dat","w") as fout2:
  writer = csv.writer(fout2, delimiter=' ')
  writer.writerows(zip(ke_gev,dedx_tot))



#plt.figure(figsize=(20,10))
plt.rcParams['axes.grid'] = True
#ax = plt.subplot()
#ax = plt.subplot(1,1,1)
#plt.plot(mom_gev, csdarange, 'o')
#plt.xlabel("Proton Momentum [GeV/c]")

plt.plot(ke_gev, csdarange, 'o')
#plt.xlabel("Proton Momentum [GeV/c]")
plt.xlabel("Proton Kinetic Energy [GeV]")


#plt.ylabel("CSDA Range [g/cm$^{2}$]")
plt.ylabel("CSDA Range [cm]")

plt.show()


#Dump the data to a file
#for i in range(mom_gev.size()):
#  print('{0} {1}',mom_gev,range)

#fout = open(“proton_csda_mom.dat”,”w”) 

#fout.write(mom_gev, ) 


