algo = []
with open("Algo.csv", "r") as file1:
	for line in file1:
		algo.append(int((line.split(','))[0]))
file1.close()

ir = []
with open("IR.csv", "r") as file2:
	for line in file2:
		ir.append(int((line.split(','))[0]))
file2.close()



a = set(algo)
b = set(ir)

print 'algo: ',len(a)
print 'ir: ',len(b)
print 'intersection: ',len( a.intersection(b))
print 'union: ',len( a.union(b))