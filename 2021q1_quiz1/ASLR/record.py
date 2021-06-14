import os
import sys
import subprocess

if __name__ == "__main__":
	final = 1000000
	with open("aslr.txt", "w") as f:
		for i in range(0, final):
			p = subprocess.Popen("./main", stdout=subprocess.PIPE, stderr=subprocess.PIPE)
			out, err = p.communicate()

			out = out.decode("utf-8").replace("\n", "");
			err = err.decode("utf-8")

			if err == "":
				print("{}: {}".format(i, out))
				f.write("{}\n".format(out))
			else: i -= 1
