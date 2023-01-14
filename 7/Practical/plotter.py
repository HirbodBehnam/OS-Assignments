import matplotlib.pyplot as plt
import sys

if len(sys.argv) <= 1:
    print("Please enter output file names of linked list app as arguments.")
    print("Pass multiple arguments to draw multiple plots.")
    exit(1)

# Read input files
plot_y = []
for input_file in sys.argv[1:]:
    data = []
    with open(input_file, 'r') as r:
        for line in r:
            if "length is" in line:
                data.append(int(line.split("length is")[1]))
    plot_y.append(data)

# Make the figure ready
plt.figure(1)
for i, data in enumerate(plot_y): # for each file draw the plot
    plt.subplot(len(plot_y), 1, i + 1)
    plt.xlabel('Time')
    plt.ylabel('Length')
    plt.title(sys.argv[i+1])
    plt.plot([(i+1) for i in range(len(data))], data)

plt.show()