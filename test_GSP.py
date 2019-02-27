import pygsp as gsp
import numpy as np
import matplotlib.pyplot as plt


def main():
	gsp.plotting.BACKEND = 'matplotlib'
	plt.rcParams['figure.figsize'] = (10,5)

	# Generate graph

	# Reproducibility
	rs = np.random.RandomState(42)
	# Generate adjacency matrix from random values sampled from uniform distribution in [0,1]
	W = rs.uniform(size=(30,30))
	# Make adjacency matrix symmetric
	W = W + W.T	
	np.fill_diagonal(W. 0)
	# Create graph from adjacency matrix
	G = gsp.graphs.Graph(W)

	# Print number of nodes and number of edges
	print('The graph G has {} nodes and {} edges'.format(G.N, G.Ne))

	# Check some properties of the graph
	connected = G.is_connected()
	if connected:
		print('The graph is connected')
	else:
		print('The graph is not connected')

	directed = G.is_directed()
	if directed:
		print('The graph is directed')
	else:
		print('The graph is undirected')

	# Weight matrix in scipy.sparse.lil.lil_matrix format
	W = G.W

	# Weight matrix as ndarray (use it carefully for very large graphs)
	W_np = G.W.toarray()

	# Laplacian (combinatorial) matrix in scipy.sparse.csc.csc_matrix format
	L = G.L

	# Laplacian (combinatorial) matrix as ndarray
	L_np = G.L.toarray()

	# Compute graph Fourier basis, i.e. the matrix U whose columns are the eigenvectors of L
	G.compute_fourier_basis()

	# Get U matrix. G.U returns the U matrix directly in ndarray
	U = G.U

	# Plot graph

	# First set coordinates for the nodes of the graph, i.e. put the nodes of the graph to the 2D or 3D space
	G.set_coordinates('ring2D')
	# then plot
	G.plot()

	# Generate random graph signal x
	x = np.random.rand(G.N)

	# Get the GFT of x
	x_hat = U.T.dot(x)

	# Get the IGFT of x_hat (i.e. the initial graph signal x)
	x_init = U.dot(x_hat)

	# Compute the norm between x and x_init (it should be very small, i.e. k*e-15)
	print(np.linalg.norm(x - x_init))

	# Plot second eigenvector (the one that corresponds to the second smallest eigenvalue of the Laplacian)
	G.plot_signal(U[:, 1])

	# Plot the graph signal x
	G.plot_signal(x)


	return


if __name__ == '__main__':
	main()