import os
import sys

if __name__ == "__main__":
	mydict = dict()
	resultdict = dict()

	if len(sys.argv) < 2:
		print("need specify path name")
		sys.exit(1)

	with open(sys.argv[1], "r") as f:
		for i in f:
			line = i.replace("\n", "")
			mydict[line] = mydict.get(line, 0) + 1

	for key, value in mydict.items():
		resultdict[value] = resultdict.get(value, 0) + 1

	mydict = dict(sorted(mydict.items(), key=lambda item: item[1]))
	resultdict = dict(sorted(resultdict.items(), key=lambda item: item[1]))

	for key, value in mydict.items():
		print("{}: {}".format(key, value))
	print()
	for key, value in resultdict.items():
		print("{}: {}".format(key, value))
