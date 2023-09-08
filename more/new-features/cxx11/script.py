#!/usr/bin/python3
'''
读取csv文件并绘制图像
'''
import argparse
import csv
import matplotlib.pyplot as plt
import os

def plot_data_from_csv(csv_file):
    # Lists to store the data
    x_values = []
    y_values = []

    # Read data from the CSV file
    with open(csv_file, 'r') as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)  # Skip the header row
        for row in csv_reader:
            x_values.append(float(row[0]))  # Assuming x is numeric
            y_values.append(float(row[1]))  # Assuming y is numeric

    # Create the plot
    plt.figure(figsize=(10, 6))
    plt.plot(x_values, y_values, marker='o', linestyle='-')
    plt.title('Data Plot')
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.grid(True)
    
    d, file_name_with_ext = os.path.split(csv_file)
    file_name, _ = os.path.splitext(file_name_with_ext)
    plt.savefig(os.path.join(d, file_name), dpi=300, bbox_inches='tight')
	
    # Show the plot
    plt.show()

def main():
    parser = argparse.ArgumentParser(description='Plot data from a CSV file.')
    parser.add_argument('-f', '--file', metavar='csv file', required=True, dest='file', action='store', help='Path to the CSV file containing data')
    args = parser.parse_args()
    plot_data_from_csv(args.file)

if __name__ == "__main__":
    main()

