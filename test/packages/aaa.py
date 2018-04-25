import os

result = open('prueba.txt', 'w')

dirs = os.listdir(".")

for i in dirs:
  if os.path.isdir(i):
    #print i
    result.write(i + ":\n")
    result.write("  path: "+ i + "\n")
    result.write("  run: True\n\n")