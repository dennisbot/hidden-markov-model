from collections import defaultdict

def prueba():
	a = ['a', 'b', 'a']
	b = [2, 3, 4]

	
	map = defaultdict(int)
	tupleA = ('1', 2, 3, 4)
	map[tupleA] += 2
	map[('2', 4)] += 3
	print tupleA + (1,)

	print map[(tupleA, 1)]
	print map[('3', 4)]
	print map[('2', 4)]

	res = dict(zip(a, b))
	print res

prueba()