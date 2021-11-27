import numpy as np
#file = open("graphRandom.dot","w")
print("Graph{\n")

#print("nhap so dinh:")
numberPeak = int(input())

code = list(np.random.randint(numberPeak, size = numberPeak - 2))
code.append(0)
decode =[[x] for x in code]
check = [0] * numberPeak
for x in code:
    	check[x] += 1
# print(check)
checkZero = []
for i in range(numberPeak):
		if(check[i]==0):
				checkZero.append(i)
checkZero.append(numberPeak+1)

count = 0
for n in range(numberPeak-1):
	vertex = checkZero[count]
	check[vertex] = 1
	check[code[n]] -= 1
	if(check[code[n]] == 0):
		if(code[n] < checkZero[count + 1]):
			checkZero[count]=code[n]
	else : count += 1
	code[n] = vertex
	decode[n].append(vertex)
	print(str(decode[n][0])+ " -- " + str(decode[n][1])+"\n")


print("}")
# print(decode)
print("done")