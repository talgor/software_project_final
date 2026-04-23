import sys, math, numpy as np, c_symnmf
np.random.seed(1234)


def parse_file(file_name):
    datapoints = []
    with open(file_name, "r") as file:
        for line in file:
            coaordinates = line.strip().split(",")
            coaordinates = [float(co) for co in coaordinates]
            datapoints.append(coaordinates)
    return datapoints

def initialize_H(W, k, n):
    line_averages = [sum(line) / len(line) for line in W]
    W_average = sum(line_averages) / len(line_averages)
    initial_H = np.random.uniform(0, 2*math.sqrt(W_average/k), (k, n))
    #is that what i want?
    return initial_H

def print_matrix(matrix):
    for line in matrix:
        formatted = (f"{c:.4f}" for c in line)
        print(*formatted, sep=",")
    return

def compute_H(filename, k):
    datapoints= parse_file(filename)
    W = c_symnmf.norm(datapoints)
    initial_H = initialize_H(W, k, len(datapoints))
    return c_symnmf.symnmf(W, initial_H)


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
        output_matrix = c_symnmf.symnmf(W, initial_H)

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
        print("An Error Has Occured")
        sys.exit(1)



