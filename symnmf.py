import sys, math, numpy as np, mysymnmf as c_symnmf
np.random.seed(1234)

#parses the file and outputs a list of lists containing the datapoints
def parse_file(file_name):
    datapoints = []
    with open(file_name, "r") as file:
        for line in file:
            coordinates = line.strip().split(",")
            coordinates = [float(co) for co in coordinates]
            datapoints.append(coordinates)
    return datapoints

#initializes a randomized H matrix of the right dimentions
def initialize_H(W, k, n):
    W_average = np.mean(W)
    initial_H = np.random.uniform(0, 2*math.sqrt(W_average/k), (n, k)).tolist()
    return initial_H

#pretty prints a matrix
def print_matrix(matrix):
    for line in matrix:
        formatted = (f"{c:.4f}" for c in line)
        print(*formatted, sep=",")
    return

def compute_H(filename, k):
    #a function for the anallysis file to use
    datapoints= parse_file(filename)
    W = c_symnmf.norm(datapoints)
    initial_H = initialize_H(W, k, len(datapoints))
    return c_symnmf.symnmf(W, initial_H, len(datapoints), k)


def main():
    if(len(sys.argv) != 4):
        print("An Error Has Occurred")
        sys.exit(1)
    k = int(sys.argv[1])
    goal = sys.argv[2]
    file_name = sys.argv[3]

    datapoints= parse_file(file_name)
    if(k >= len(datapoints)):
        print("An Error Has Occurred")
        sys.exit(1)

    output_matrix = []

    if goal == "symnmf":
        W = c_symnmf.norm(datapoints)
        initial_H = initialize_H(W, k, len(datapoints))
        output_matrix = c_symnmf.symnmf(W, initial_H, len(datapoints), k)

    elif goal == "sym":
        output_matrix = c_symnmf.sym(datapoints)

    elif goal == "ddg":
        output_matrix = c_symnmf.ddg(datapoints)

    elif goal == "norm":
        output_matrix = c_symnmf.norm(datapoints)
    
    else:
        print("An Error Has Occurred")
        sys.exit(1) 

    print_matrix(output_matrix)      




if __name__ == "__main__":
    try:
        main()
    except Exception:
        print("An Error Has Occurred")
        sys.exit(1)



