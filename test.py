import unittest
import numpy as np
from matrix_opencl import add_matrix

class TestMatrixAddition(unittest.TestCase):
    def setUp(self):
        """Set up some common test matrices"""
        self.matrix1_2x3 = np.array([[1.0, 2.0, 3.0], 
                                   [4.0, 5.0, 6.0]], dtype=np.float32)
        self.matrix2_2x3 = np.array([[7.0, 8.0, 9.0], 
                                   [10.0, 11.0, 12.0]], dtype=np.float32)
        
    def test_basic_addition(self):
        """Test basic matrix addition with known result"""
        expected = np.array([[8.0, 10.0, 12.0], 
                           [14.0, 16.0, 18.0]], dtype=np.float32)
        result = add_matrix(self.matrix1_2x3, self.matrix2_2x3)
        np.testing.assert_array_almost_equal(result, expected)

    def test_identity_addition(self):
        """Test addition with identity matrix"""
        matrix = np.array([[1.0, 2.0], 
                          [3.0, 4.0]], dtype=np.float32)
        identity = np.eye(2, dtype=np.float32)
        expected = np.array([[2.0, 2.0], 
                           [3.0, 5.0]], dtype=np.float32)
        result = add_matrix(matrix, identity)
        np.testing.assert_array_almost_equal(result, expected)

    def test_zero_matrix_addition(self):
        """Test addition with zero matrix"""
        matrix = np.array([[1.0, 2.0], 
                          [3.0, 4.0]], dtype=np.float32)
        zeros = np.zeros((2, 2), dtype=np.float32)
        result = add_matrix(matrix, zeros)
        np.testing.assert_array_almost_equal(result, matrix)

    def test_large_matrix(self):
        """Test addition of larger matrices"""
        size = 1000
        large_matrix1 = np.random.rand(size, size).astype(np.float32)
        large_matrix2 = np.random.rand(size, size).astype(np.float32)
        expected = large_matrix1 + large_matrix2
        result = add_matrix(large_matrix1, large_matrix2)
        np.testing.assert_array_almost_equal(result, expected)

    def test_different_size_matrices(self):
        """Test that adding matrices of different sizes raises an error"""
        matrix1 = np.array([[1.0, 2.0]], dtype=np.float32)
        matrix2 = np.array([[1.0], [2.0]], dtype=np.float32)
        with self.assertRaises(ValueError):
            add_matrix(matrix1, matrix2)

    def test_non_numeric_matrices(self):
        """Test that adding matrices with non-numeric types raises an error"""
        matrix1 = np.array([['a', 'b'], ['c', 'd']])
        matrix2 = np.array([['e', 'f'], ['g', 'h']])
        with self.assertRaises(TypeError):
            add_matrix(matrix1, matrix2)

    def test_precision(self):
        """Test precision of floating point addition"""
        matrix1 = np.array([[1.23456789, 2.34567890], 
                           [3.45678901, 4.56789012]], dtype=np.float32)
        matrix2 = np.array([[5.67890123, 6.78901234], 
                           [7.89012345, 8.90123456]], dtype=np.float32)
        expected = matrix1 + matrix2
        result = add_matrix(matrix1, matrix2)
        np.testing.assert_array_almost_equal(result, expected, decimal=8)

    def test_negative_numbers(self):
        """Test addition with negative numbers"""
        matrix1 = np.array([[-1.0, -2.0], 
                           [-3.0, -4.0]], dtype=np.float32)
        matrix2 = np.array([[5.0, 6.0], 
                           [7.0, 8.0]], dtype=np.float32)
        expected = np.array([[4.0, 4.0], 
                           [4.0, 4.0]], dtype=np.float32)
        result = add_matrix(matrix1, matrix2)
        np.testing.assert_array_almost_equal(result, expected)

    def test_single_element_matrices(self):
        """Test addition of 1x1 matrices"""
        matrix1 = np.array([[5.0]], dtype=np.float32)
        matrix2 = np.array([[3.0]], dtype=np.float32)
        expected = np.array([[8.0]], dtype=np.float32)
        result = add_matrix(matrix1, matrix2)
        np.testing.assert_array_almost_equal(result, expected)

    def test_performance(self):
        """Test performance with timeout"""
        import time
        size = 2000
        matrix1 = np.random.rand(size, size).astype(np.float32)
        matrix2 = np.random.rand(size, size).astype(np.float32)
        
        start_time = time.time()
        result = add_matrix(matrix1, matrix2)
        end_time = time.time()
        
        # Assert that the computation takes less than 5 seconds
        self.assertLess(end_time - start_time, 5.0)

if __name__ == '__main__':
    unittest.main()