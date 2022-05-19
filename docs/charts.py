import matplotlib.pyplot as plt

data = [

    ('1000 insert', [
        ('AVL', 0.52947),
        ('RBT', 0.45551),
        ('OB', 0.17334),
        ('LP', 0.25113)]),
    ('1000 search', [
        ('AVL', 0.20357),
        ('RBT', 0.26748),
        ('OB', 0.10392),
        ('LP', 0.18993)]),
    ('1000 delete', [
        ('AVL', 0.19716),
        ('RBT', 0.23246),
        ('OB', 0.10353),
        ('LP', 0.23924)]),
    ('10 000 insert', [
        ('AVL', 7.21321),
        ('RBT', 5.36299),
        ('OB', 1.62544),
        ('LP', 2.58254)]),
    ('10 000 search', [
        ('AVL', 2.92378),
        ('RBT', 3.16818),
        ('OB', 1.04833),
        ('LP', 2.73147)]),
    ('10 000 delete', [
        ('AVL', 2.58477),
        ('RBT', 2.89742),
        ('OB', 1.02419),
        ('LP', 3.83549)]),
    ('1 000 000 insert', [
        ('AVL', 2240.168000),
        ('RBT', 1560.34700),
        ('OB', 712.64900)]),
    ('1 000 000 search', [
        ('AVL', 611.18500),
        ('RBT', 668.08200),
        ('OB', 146.58500)]),
    ('1 000 000 delete', [
        ('AVL', 768.0800),
        ('RBT', 1275.06200),
        ('OB', 361.71800)]),
]

for x in data:

    algorithm = []
    time = []

    for al, ti in x[1]:
        algorithm.append(al)
        time.append(ti)


    # algorithm = ['USA','Canada','Germany','UK','France']
    # time = [0.45000,0.42000,0.52000,0.49000,0.47000]
    colors = ['red','green','blue', 'grey']


    plt.bar(algorithm, time, color=colors)
    plt.title(f'Algorithm Vs Time {x[0]}')
    plt.xlabel('Algorithm')
    plt.ylabel('Time [ms]')
    # plt.show()
    plt.savefig(f'img/{(x[0]).replace(" ","")}.png',dpi=100)
    plt.close()








 
