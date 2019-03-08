import numpy as np


def check_symmetric(X, tol):
	return np.allclose(X, X.T, atol=tol)