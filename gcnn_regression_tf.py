import tensorflow as tf
import numpy as np


# Application of the Chebyshev polynomials of order n on square matrics NxN (recursive implementation)
def chebyshev_T(n, X):
	(rows, cols) = X.shape
	if rows == cols:
		if n == 0:
			return np.ones(X.shape)
		elif n == 1:
			return X
		else:
			return (2 * X).dot(chebyshev_T(n - 1, X)) - chebyshev_T(n - 2, X)
	else:
		print('The input matrix has to be square.')
		return


# Implementation of the Defferrard's et al. graph convolutional layer
class GraphConvLayer(tf.keras.layers.Layer):
	def __init__(self, num_outputs, K, num_nodes, L):
		super(GraphConvLayer, self).__init__()
		self.num_outputs = num_outputs
		self.K = K
		self.num_nodes = num_nodes
		self.L = L
		self.kernel = self.add_variable("kernel", shape=[self.num_outputs, self.K])

	# Forward pass, X: (num_vectors x num_nodes)
	def call(self, X):
		num_vectors = X.shape[0]

		# Weight matrix: (num_outputs x K)
		w = self.get_weights()[0]

		# Output matrix of the layer: (num_outputs x num_nodes)
		Y = np.zeros((self.num_outputs, self.num_nodes))

		# For each filter in the layer
		for i in range(self.num_outputs):

			# Compute graph convolutional filter g
			g = np.zeros((self.num_nodes, self.num_nodes))
			for k in range(self.K):
				g += (w[i, k] * chebyshev_T(k, self.L))

			# Passing graph signal x through filter g
			y = np.zeros((1, self.num_nodes))
			for j in range(num_vectors):
				x = X[j, :]
				x = x.reshape((x.shape[0], 1))
				y_temp = g.dot(x)
				y_temp = y_temp.reshape((y_temp.shape[0], ))
				y += y_temp
			y /= num_vectors
			Y[i, :] = y
		return Y


def main():
	num_nodes = 5
	num_filters = 3
	K = 2
	#L = np.random.randint(5, size=(num_nodes, num_nodes))
	L = np.random.rand(num_nodes, num_nodes)
	#L = (L + L.T) / 2
	layer = GraphConvLayer(num_filters, K, num_nodes, L)
	X = np.random.rand(10, 5)
	Y = layer(X)
	print(X)
	print('\n')
	print(Y)
	return


if __name__ == '__main__':
	main()