
reportFile = 'report2.txt'
filename1 = 'logFixedDelay.csv'

def minPositive(list1):
    return min(filter(lambda x:x>0, list1))

chefItem = []
chefTime = []
waiterItem = []
waiterTime = []
maachC = []
dalC = []
bhaatC = []
maachW = []
dalW = []
bhaatW = []

with open(filename1, 'r') as file1:
    for line in file1:
        data = line.split(',')

        if( data[1] == 'Chef'):
            chefItem.append(data[3])
            chefTime.append(int(data[4]))
            if (data[3] == 'Maach'):
                maachC.append(int(data[4]))
            elif (data[3] == 'Bhaat'):
                bhaatC.append(int(data[4]))
            elif (data[3] == 'Dal'):
                dalC.append(int(data[4]))

        elif( data[1] == 'Waiter'):
            waiterItem.append(data[3])
            waiterTime.append(int(data[4]))
            if (data[3] == 'Maach'):
                maachW.append(int(data[4]))
            elif (data[3] == 'Bhaat'):
                bhaatW.append(int(data[4]))
            elif (data[3] == 'Dal'):
                dalW.append(int(data[4]))


file1.close()

diffMaachC = [j-i for i, j in zip(maachC[:-1], maachC[1:])];
diffDalC = [j-i for i, j in zip(dalC[:-1], dalC[1:])];
diffBhaatC = [j-i for i, j in zip(bhaatC[:-1], bhaatC[1:])];
diffMaachW = [j-i for i, j in zip(maachW[:-1], maachW[1:])];
diffDalW = [j-i for i, j in zip(dalW[:-1], dalW[1:])];
diffBhaatW = [j-i for i, j in zip(bhaatW[:-1], bhaatW[1:])];


with open(reportFile,'w') as repFl:
    repFl.write('1. Number of items generated : '+str(len(chefItem))+'\n' )
    repFl.write('2. Number of iterms received by waiter : '+ str(len(waiterItem) )+ '\n')
    repFl.write('3. Number of items wasted : '+ str(len(chefItem) - len(waiterItem)) + '\n')
    repFl.write('4. Number of Maach generated,recevied and wasted : ' + str(len(maachC)) + ' ,'+ str(len(maachW)) + ' ,'+ str(len(maachC) - len(maachW)) + '\n')
    repFl.write('5. Number of Dal generated,recevied and wasted : ' + str(len(dalC)) + ' ,'+ str(len(dalW)) + ' ,'+ str(len(dalC) - len(dalW)) + '\n')
    repFl.write('6. Number of Bhaat generated,recevied and wasted : ' + str(len(bhaatC)) + ' ,'+ str(len(bhaatW)) + ' ,'+ str(len(bhaatC) - len(bhaatW)) + '\n')
    repFl.write('7. Maximum delay in generating any two sucessive items: '+ str( max( [max(diffMaachC) , max(diffDalC), max(diffBhaatC)] )) + ' micorseconds' +'\n')
    repFl.write('8. Maximum delay in receiving any two successive items : '+ str( max( [max(diffMaachW) , max(diffDalW), max(diffBhaatW)] )) + ' micorseconds' +'\n')
    repFl.write('9. Maximum delay generating two successive maach, dal, bhaat respectively :'+ str(max(diffMaachC)) + ' micorseconds,'+ str(max(diffDalC))+ ' micorseconds,'+ str(max(diffBhaatC) ) + ' micorseconds' +'\n')
    repFl.write('10. Maximum delay receiving two successive maach, dal, bhaat respectively :'+ str(max(diffMaachW)) + ' micorseconds,'+ str(max(diffDalW))+ ' micorseconds,'+ str(max(diffBhaatW) ) + ' micorseconds' +'\n')
    repFl.write('11. Minimum delay in generating any two sucessive items: '+ str( minPositive( [max(diffMaachC) , minPositive(diffDalC), minPositive(diffBhaatC)] )) + ' micorseconds' +'\n')
    repFl.write('12. Minimum delay in receiving any two successive items : '+ str( minPositive( [max(diffMaachW) , minPositive(diffDalW), minPositive(diffBhaatW)] )) + ' micorseconds' +'\n')
    repFl.write('13. Minimum delay generating two successive maach, dal, bhaat respectively :'+ str(minPositive(diffMaachC)) + ' micorseconds,'+ str(minPositive(diffDalC))+ ' micorseconds,'+ str(minPositive(diffBhaatC) ) + ' micorseconds' +'\n')
    repFl.write('14. Minimum delay receiving two successive maach, dal, bhaat respectively :'+ str(minPositive(diffMaachW)) + ' micorseconds,'+ str(minPositive(diffDalW))+ ' micorseconds,'+ str(minPositive(diffBhaatW)) + ' micorseconds' +'\n')

repFl.close()
