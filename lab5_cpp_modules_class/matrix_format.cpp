#include "matrix_format.hpp"
//#define SScheckforPos_v2

StatusCode Matrix::LoadMatrix(const std::string &FileAdress, ErrInfo *err = nullptr)
{
    this->_column_count = 0, this->_str_count = 0;
    std::ifstream file(FileAdress);
    if (file.is_open())
    {
        StatusCode status_code;
#ifdef checkMatr_v1
        if (!(status_code = CheckMatr(file, matr_str, matr_column, err)))
#else
        if (!(status_code = SSCheckMatr(file, matr_str, matr_column, err)))
#endif
        {
            if (!fAdditionalCheck || !(status_code = fAdditionalCheck(nullptr, matr_str, matr_column)))
            {
                if (!(status_code = CreateMatr(matr, matr_str, matr_column, err)))
                {
                    file.clear();
                    file.seekg(0);
                    FReadMatr(file, matr, matr_str, matr_column);
                    file.close();
                    if (!fAdditionalCheck || !(status_code = fAdditionalCheck(matr, matr_str, matr_column)))
                        return status_code;
                }
                DeleteMatr(matr, matr_str);
            }
        }
        file.close();
        matr_str = 0, matr_column = 0;
        return status_code;
    }
    return FileOpenErr;
}