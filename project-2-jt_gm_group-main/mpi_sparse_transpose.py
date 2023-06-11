import os # Needed to open text files
import sys # Needed to run command lines
from mpi4py import MPI # Needed to run MPI commands

def main(argc, argv):
	mpi_comm = MPI.COMM_WORLD
	rank = mpi_comm.Get_rank()
	size = mpi_comm.Get_size()
	n, m = 0, 0 # Total number of rows and columns
	
	file_list = [] # Get all of the values from the text file.
	list_row = [] # List of rows for each value
	list_col = [] # List of columns for each value
	list_val = [] # List of values for each row and column
	r, c, v = 0, 0, 0 # Current row, column, and value

	# There needs to be at least 2 processes in order to run the program.
	# Abort if the size is less than 2. Also be sure the number of
	# arguments provided is valid.
	if size < 2:
		print("There needs to be at least 2 processes.")
		exit(-1)
		
	if argc != 2:
		print("Invalid number of arguments.")
		exit(-1)

	# Rank 0 will be reading the text file containing the tuples
	# of row number, column number, and the value of the matrix.
	# Then each tuple element will have their values extracted
	# and stored in the three separate lists:
	# rows, columns, and matrix values.
	# Once those lists are populated, rank 0 will begin reconstructing
	# the sparse matrix by finding the number of rows and columns
	# based on the maximum values of the list of values followed by
	# populating the entire matrix with zeroes. Then, each nonzero
	# value will be inserted according to row and column number.
	# All nonzero ranks will receive the original matrix from
	# rank 0 and transpose it by swapping the rows with columns,
	# followed by outputting the result matrix.
	print("I am rank {} out of {}".format(rank, size))
	if rank == 0:
		# Try to open the file if possible.
		input_file = open(argv[1], "r")
		for lines in input_file:
			# Print out the original matrix first.
			curr_line = lines.strip().split()
			file_list.append(curr_line)
		input_file.close()
		
		# For each tuple of nonzero values, obtain the row, column, and value of the sparse matrix
		# and then store them in three separate lists.
		for i in range(len(file_list)):
			r = int(file_list[i][0])
			list_row.append(r)
			c = int(file_list[i][1])
			list_col.append(c)
			v = int(file_list[i][2])
			list_val.append(v)
		print("Rows: ", list_row)
		print("Columns: ", list_col)
		print("Values: ", list_val)
		
		# Get the number of rows and columns by obtaining the maximum values.
		n = max(list_row)+1
		m = max(list_col)+1
		
		print("Rows={} and Columns={}".format(n, m))
		
		matrix = [[0 for col in range(m)] for row in range(n)] # Initialize all of the values of the original matrix to zero.
		
		# Now begin obtaining the original matrix based on the row and column location.
		# Then, print out the completed matrix.
		for i in range(len(list_val)):
			r = list_row[i]
			c = list_col[i]
			matrix[r][c] = list_val[i]
		for i in range(0, n):
			print(matrix[i])
			
		# Send over the number of rows and columns for the restored sparse matrix,
		# along with the matrix itself over to any nonzero process for transposing.
		req_row = mpi_comm.isend(n, dest=1, tag=10)
		req_col = mpi_comm.isend(m, dest=1, tag=11)
		req_row.wait()
		req_col.wait()
		
		req_mat = mpi_comm.isend(matrix, dest=1, tag=12)
		req_mat.wait()
		
	elif rank == 1:
		req_row = mpi_comm.irecv(source=0, tag=10)
		req_col = mpi_comm.irecv(source=0, tag=11)
		n_req = req_row.wait() # Get the number of rows received.
		m_req = req_col.wait() # Get the number of columns received.
		print("Number of rows received from process 0:", n_req)
		print("Number of columns received from process 0:", m_req)
		
		req_mat = mpi_comm.irecv(source=0, tag=12) # Get the whole matrix from process 0.
		mat_req = req_mat.wait()
			
		for j in range(0, n_req):
			print(mat_req[j])
		print("Now transposing the matrix...")
		transposed = [[mat_req[j][i] for j in range(len(mat_req))] for i in range(len(mat_req[0]))] # Transpose matrix by swapping the rows and columns.
		print("Transpose complete. Here's the result:")
		# Print the completed transposed sparse matrix.
		for j in range(0, m_req):
			print(transposed[j])
	else:
		pass

if __name__ == "__main__":
	main(len(sys.argv), sys.argv)
