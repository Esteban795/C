filepath = input("Filepath : ")
f = open(filepath,"r")
tab = []
for i in f:
    if "SAT" in i:
        continue
    ligne = [int(j) for j in i.split(" ") if (int(j)) > 0]
    tab.extend(ligne)

f.close()
c = 0
mat = [[0 for i in range(9)] for j in range(9)]
for i in range(9):
    for j in range(9):
        mat[i][j] = (tab[c]%81) % 9
        c+= 1

output = open("td49/sortie.out","w")
for i in mat:
    output.write(" ".join([str(j) for j in i]) + "\n")
output.close()

