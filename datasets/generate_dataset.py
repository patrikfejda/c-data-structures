import random, string

afile = open("dataset", "w" )
nums = []
length = 10
number_of_records = 10000

for i in range(number_of_records):
    line = str(random.randint(1, 100000))
    nums.append(int(line))
    afile.write(line+";")
    name = ''.join((random.choice(string.ascii_lowercase) for x in range(length)))
    afile.write(name)
    afile.write(line+";\n")

afile.close()




random.shuffle(nums)
rand_nums = nums

textfile = open("dataset_rand", "w")
for element in rand_nums:
    textfile.write(str(element) + "\n")
textfile.close()
