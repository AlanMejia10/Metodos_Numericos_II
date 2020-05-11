#include <iostream>
#include <cmath>
#include "Matrix.h"

Matrix::Matrix(int m, int n) : m_rows(m), m_cols(n) {
	std::cout << "ctr called" << std::endl;
	CreateMatrix();
}

Matrix::Matrix(const Matrix& other) : m_rows(other.m_rows), m_cols(other.m_cols) {
    std::cout<<"copy ctr called"<<std::endl;
    std::cout<<"mrows : " << m_rows << "m_cols: "<<m_cols<<std::endl;

    m_matrix = new float*[m_rows];

    for(int i = 0; i < m_rows; i++)
        m_matrix[i] = new float[m_cols];

    /* Coping the values */
    for(int i=0; i<m_rows; i++)
        for(int j=0; j<m_cols; j++)
            m_matrix[i][j] = other.m_matrix[i][j];

    std::cout<<"end cpy ctr"<<std::endl;
}

Matrix::~Matrix(){
    std::cout << "dtr called" <<std::endl;

    for(int i = 0; i < m_rows; ++i)
        delete[] m_matrix[i];

    delete[] m_matrix;
}

float** Matrix::GetMatrixPtr()const { return m_matrix; }

int Matrix::GetRows()const { return m_rows; }

int Matrix::GetColumns()const{ return m_cols; }

void Matrix::Init(){
    FillMatrix();
}

std::ostream& operator<<(std::ostream& out, const Matrix& mat) {
	for(int i = 0; i < mat.m_rows; ++i) {
		for(int j = 0; j < mat.m_cols; ++j)
			out<<mat.m_matrix[i][j]<<"\t  ";

		out<<"\n";
	}
	return out;
}

std::istream& operator>>(std::istream& in, Matrix& mat){
    for(int i = 0; i < mat.m_rows; ++i)
        for(int j = 0; j < mat.m_cols; ++j){
            std::cout<<"matrix["<<i+1<<"]["<<j+1<<"]: ";
            in>>mat.m_matrix[i][j];
    }

    return in;
}

const Matrix& Matrix::operator=(const Matrix& other){
    std::cout<<"assignment operator called"<<std::endl;
    if(&other != this){
        if(m_rows == other.m_rows && m_cols == other.m_cols){
            std::cout<<"same size"<<std::endl;
            for(int i=0; i<m_rows; i++)
                for(int j=0; j<m_cols; j++)
                    m_matrix[i][j] = other.m_matrix[i][j];
        }else{
        std::cout<<"diferent size"<<std::endl;
            /* Resizing the matrix */
            for(int i = 0; i < m_rows; ++i)
                delete[] m_matrix[i];

            delete[] m_matrix;

            m_rows = other.m_rows;
            m_cols = other.m_cols;
            CreateMatrix();

            for(int i=0; i<m_rows; i++)
                for(int j=0; j<m_cols; j++)
                    m_matrix[i][j] = other.m_matrix[i][j];
        }
    }
    return *this;
}

void Matrix::CreateMatrix(){
    m_matrix = new float*[m_rows];

    for(int i = 0; i < m_rows; i++)
        m_matrix[i] = new float[m_cols];

    /* initializing the matrix to 0 */
    for(int i=0; i<m_rows; i++)
        for(int j=0; j<m_cols; j++)
            m_matrix[i][j] = 0;
}

void Matrix::FillMatrix(){
    for(int i = 0; i < m_rows; ++i)
        for(int j = 0; j < m_cols; ++j){
            std::cout<<"matrix["<<i+1<<"]["<<j+1<<"]: ";
            std::cin>>m_matrix[i][j];
    }
}

void Matrix::PrintMatrix(){
    for(int i = 0; i < m_rows; ++i){
        for(int j = 0; j < m_cols; ++j)
            std::cout<<m_matrix[i][j]<<"\t  ";

        std::cout<<std::endl;
    }
}

float Matrix::Determinant(const Matrix& mat){
    float det=0;
     for(int j = 0; j < mat.m_cols; ++j)
     det += ((0+j) % 2 == 0 ? 1 : -1) * mat.m_matrix[0][j] * Cofactor(mat, 0, j);

    return det;
}

float Matrix::Cofactor(const Matrix& mat, int m, int n){
    int k = 0, l = 0;
    Matrix cofMat(mat.m_rows-1, mat.m_cols-1);
    float** subMat = cofMat.GetMatrixPtr();

    for(int i = 0; i < mat.m_rows; ++i){
        for(int j = 0; j < mat.m_cols; ++j){
            if(i != m && j != n){
                if(l < cofMat.m_cols){
                    subMat[k][l] = mat.m_matrix[i][j];
                    ++l;
                }else{
                    ++k;
                    l=0;
                    subMat[k][l] = mat.m_matrix[i][j];
                    ++l;
                }
            }
        }
    }

    if(cofMat.m_rows == 1)
        return subMat[0][0];
    else
        Determinant(cofMat);
}

Matrix Matrix::Inverse(const Matrix& mat){
    Matrix inv(mat.m_rows, mat.m_cols);
    float** invPtr = inv.GetMatrixPtr();
    for(int i=0; i<mat.m_rows; ++i){
        for(int j=0; j < mat.m_cols; ++j){
            invPtr[i][j] = ((i+j) % 2 == 0 ? 1 : -1) * Cofactor(mat, i, j);
        }
    }

    Matrix matInvTrn = Matrix::Transpose(inv);
    float** matInvTrnPtr = matInvTrn.GetMatrixPtr();

   Matrix invComplete(mat.m_rows, mat.m_cols);
    float** invCompletePtr = invComplete.GetMatrixPtr();

    float detMat= 1/Matrix::Determinant(mat);

    for(int i=0; i<mat.m_rows; ++i)
        for(int j =0; j<mat.m_cols; ++j)
            invCompletePtr[i][j] = detMat * matInvTrnPtr[i][j];

    return invComplete;

}

Matrix Matrix::Transpose(const Matrix& mat){
    Matrix trnMat(mat.m_rows, mat.m_cols);
    float** trnPtr = trnMat.GetMatrixPtr();

    for(int i=0; i<mat.m_rows; ++i)
        for(int j=0; j<mat.m_cols; j++)
            trnPtr[i][j] = mat.m_matrix[j][i];

    return trnMat;

}

Matrix Matrix::Mult(const Matrix& matA, const Matrix& matB){

    Matrix multMat(matA.m_rows, matB.m_cols);
    float** multPtr = multMat.GetMatrixPtr();

    for(int i=0; i<matA.m_rows; ++i){
        for(int j=0; j<matB.m_cols; ++j){
            multPtr[i][j]=0;
            for(int k=0;k < matA.m_cols; ++k){
				multPtr[i][j]=multPtr[i][j] + matA.m_matrix[i][k]*matB.m_matrix[k][j];
			}
        }
    }

    return multMat;
}

Matrix Matrix::Add(const Matrix& matA, const Matrix& matB){
    Matrix addMat(matA.m_rows, matB.m_cols);
    float** addPtr = addMat.GetMatrixPtr();

    for(int i=0; i<matA.m_rows; ++i)
        for(int j=0; j<matA.m_cols; ++j)
            addPtr[i][j]= matA.m_matrix[i][j] + matB.m_matrix[i][j];

    return addMat;
}

Matrix Matrix::Subtract(const Matrix& matA, const Matrix& matB){
    Matrix subMat(matA.m_rows, matB.m_cols);
    float** subPtr = subMat.GetMatrixPtr();

    for(int i=0; i<matA.m_rows; ++i)
        for(int j=0; j<matA.m_cols; ++j)
            subPtr[i][j]= matA.m_matrix[i][j] - matB.m_matrix[i][j];

    return subMat;
}
