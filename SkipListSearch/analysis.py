import sys
items = ['dal','daal','mach','maach','bhat','bhaat','bat','baat']
f = open(sys.argv[1]).read().split('\n')[1:]


if len(f[-1]) <= 1:
	f = f[:-1]
	print f[-1]
else:
	print 'no issues'


chefCount = 0
waiterCount = 0
machCount = 0
dalCount = 0
bhatCount = 0
machWaiter = 0
dalWaiter = 0
bhatWaiter = 0
for line in f:
	stuff = line.split(',')
	if 'chef' in stuff[0].lower():
		chefCount += 1		
		if 'maach' in stuff[2].lower() or 'mach' in stuff[2].lower():
			machCount += 1
		if 'dal' in stuff[2].lower() or 'daal' in stuff[2].lower():
			dalCount += 1
		if 'bhaat' in stuff[2].lower() or 'baat' in stuff[2].lower() or 'bat' in stuff[2].lower():
			bhatCount += 1
	if 'waiter' in stuff[0].lower():
		waiterCount += 1		
		if 'maach' in stuff[2].lower() or 'mach' in stuff[2].lower():
			machWaiter += 1
		if 'dal' in stuff[2].lower() or 'daal' in stuff[2].lower():
			dalWaiter += 1
		if 'bhaat' in stuff[2].lower() or 'baat' in stuff[2].lower() or 'bat' in stuff[2].lower():
			bhatWaiter += 1



print "items generated, received, wasted",chefCount,waiterCount, chefCount - waiterCount
print "maach generated, received, wasted",machCount,machWaiter, machCount - machWaiter
print "bhaat generated, received, wasted",bhatCount,bhatWaiter, bhatCount - bhatWaiter
print "dal generated, received, wasted",dalCount,dalWaiter, dalCount - dalWaiter


