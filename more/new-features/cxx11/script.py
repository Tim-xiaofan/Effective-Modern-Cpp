#!/usr/bin/python3
'''
读取csv文件并绘制图像
'''
import argparse
import csv
import matplotlib.pyplot as plt
import os

def check_csv_structure(csv_files):
    first_file = csv_files[0]  # Select the first CSV file as the reference
    with open(first_file, 'r') as first_csvfile:
        first_csv_reader = csv.reader(first_csvfile)
        first_header = next(first_csv_reader)  # Get the header row of the first file

    for csv_file in csv_files[1:]:
        with open(csv_file, 'r') as csvfile:
            csv_reader = csv.reader(csvfile)
            header = next(csv_reader)  # Get the header row of the current file

            if header != first_header:
                print(f"Error: The structure of {csv_file} is different from the reference file.")
                return False

    return True

def plot_data_from_csv(csv_files, figure_name):
    if not check_csv_structure(csv_files):
        return

    csv_files = set(csv_files)
    
    for csv_file in csv_files:
        # Lists to store the data
        x_values = []
        y_values = []

        # Read data from the CSV file
        with open(csv_file, 'r') as csvfile:
            csv_reader = csv.reader(csvfile)
            header = next(csv_reader)  # Skip the header row
            for row in csv_reader:
                x_values.append(float(row[0]))  # Assuming x is numeric
                y_values.append(float(row[1]))  # Assuming y is numeric
        plt.plot(x_values, y_values, marker='o', linestyle='-', label=os.path.splitext(os.path.basename(csv_file))[0])

    # Create the plot
    # plt.figure(figsize=(10, 6))
    plt.title('Data Plot')
    plt.xlabel(header[0])
    plt.ylabel(header[1])
    plt.grid(True)
    plt.legend()
    
    plt.savefig(figure_name, dpi=300, bbox_inches='tight')
	
    # Show the plot
    plt.show()

def main():
    parser = argparse.ArgumentParser(description='Plot data from CSV files on the same figure.')
    parser.add_argument('-f', '--files', metavar='csv files', required=True, \
                dest='files', nargs='+', help='Paths to the CSV files containing data')
    parser.add_argument('-o', '--figure', metavar='figure name', required=True, dest='figure', \
                help='save figure file name')
    args = parser.parse_args()
    plot_data_from_csv(args.files, args.figure)

if __name__ == "__main__":
    main()

