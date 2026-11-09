#include <gtest/gtest.h>
#include "csr_matrix.h"


CSR_matrix<double> make(size_t r, size_t c, const std::vector<size_t>& cols,
	const std::vector<size_t>& rows, const std::vector<double>& vals)
{
	CSR_matrix<double> m(r, c);
	m.set_from_vectors(cols, rows, vals);
	return m;
}

CSR_matrix<double> make_A()
{
	return make(3, 3, { 0, 2, 2, 0, 1 }, { 0, 2, 3, 5 }, { 1, 2, 3, 4, 5 });
}

TEST(TestMatrix, can_create_matrix)
{
	ASSERT_NO_THROW(CSR_matrix<double> m(10, 10, 20));
}

TEST(TestMatrix, can_generate_matrix)
{
	CSR_matrix<double> m(10, 10);
	ASSERT_NO_THROW(m.generate_matrix(3, 0.5, 2.5));
}

TEST(TestMatrix, can_set_from_vectors)
{
	CSR_matrix<double> m(5, 5);
	std::vector<size_t> cols = { 0, 2, 4, 1, 3 };
	std::vector<size_t> rows = { 0, 2, 2, 4, 5, 5 };
	std::vector<double> vals = { 1.0, 2.0, 3.0, 4.0, 5.0 };
	ASSERT_NO_THROW(m.set_from_vectors(cols, rows, vals));
}

TEST(TestMultiply, gives_correct_result)
{
	std::vector<double> y = make_A() * std::vector<double>{ 1, 2, 3 };
	ASSERT_EQ(3u, y.size());
	EXPECT_DOUBLE_EQ(7.0, y[0]);   // 1*1 + 2*3
	EXPECT_DOUBLE_EQ(9.0, y[1]);   // 3*3
	EXPECT_DOUBLE_EQ(14.0, y[2]);  // 4*1 + 5*2
}

TEST(TestMultiply, throws_when_vector_size_is_wrong)
{
	ASSERT_THROW(make_A() * std::vector<double>(2, 1.0), std::invalid_argument);
}

TEST(TestAddition, gives_correct_result)
{
	CSR_matrix<double> B = make(3, 3, { 0, 1, 2 }, { 0, 2, 2, 3 }, { -1, 7, 1 });

	CSR_matrix<double> expected = make(3, 3, { 0, 1, 2, 2, 0, 1, 2 }, { 0, 3, 4, 7 }, { 0, 7, 2, 3, 4, 5, 1 });

	EXPECT_TRUE(expected == make_A() + B);
}

TEST(TestAddition, throws_when_sizes_are_different)
{
	CSR_matrix<double> A(3, 3), B(3, 4);
	ASSERT_THROW(A + B, std::invalid_argument);
}

TEST(TestTranspose, gives_correct_result)
{
	CSR_matrix<double> expected = make(3, 3, { 0, 2, 2, 0, 1 }, { 0, 2, 3, 5 }, { 1, 4, 5, 2, 3 });
	EXPECT_TRUE(expected == make_A().transpose());
}

TEST(TestTranspose, changes_size_of_non_square_matrix)
{
	CSR_matrix<double> M = make(2, 3, { 2, 0 }, { 0, 1, 2 }, { 5, 6 });
	CSR_matrix<double> T = M.transpose();
	EXPECT_EQ(3u, T.n_rows);
	EXPECT_EQ(2u, T.n_cols);
}

TEST(TestTranspose, double_transpose_gives_original_matrix)
{
	CSR_matrix<double> A = make_A();
	EXPECT_TRUE(A == A.transpose().transpose());
}

TEST(TestEquality, matrix_is_equal_to_its_copy)
{
	CSR_matrix<double> A = make_A();
	CSR_matrix<double> copy(A);
	EXPECT_TRUE(A == copy);
}

TEST(TestEquality, matrices_with_different_values_are_not_equal)
{
	CSR_matrix<double> C = make(3, 3, { 0, 2, 2, 0, 1 }, { 0, 2, 3, 5 }, { 1, 2, 3, 4, 6 });
	EXPECT_FALSE(make_A() == C);
}

TEST(TestEquality, matrices_with_different_sizes_are_not_equal)
{
	CSR_matrix<double> A(3, 3), B(3, 4);
	EXPECT_FALSE(A == B);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}