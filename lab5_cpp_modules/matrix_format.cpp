//  SETTINGS:
#define allocmemory_try_catch_method
//#define checkMatr_v1
//#define SScheckforPos_v2

#include "matrix_format.hpp"

#define PreparatoryStep -1

std::streamoff GetOffSet()
{
    std::ofstream tmp("tafsfgqts;№№443fd4$dfg42tr3214sdsdff32423", std::ios::out);
    std::streamoff PlatformOffSet = (tmp << '\n').tellp();
    tmp.close();
    remove("tafsfgqts;№№443fd4$dfg42tr3214sdsdff32423");
    return PlatformOffSet;
}

template <typename input>
size_t TextVievSize(input a, std::ios::fmtflags flags = (std::ios::fmtflags)0, std::streamsize prec = 6)
{
    std::ios::fmtflags resetflags = (std::ios::fmtflags)0;
    if (flags)
    {
        if (flags & std::ios::basefield)
            resetflags = resetflags | std::ios::basefield;
        if (flags & std::ios::floatfield)
            resetflags = resetflags | std::ios::floatfield;
    }
    return ((std::ostringstream &)(std::ostringstream() << std::resetiosflags(resetflags) << std::setiosflags(flags) << std::setprecision(prec) << a)).str().size();
}

bool IstreamElementCorCheck(std::istream &input, ErrInfo *err = nullptr)
{
    __matrix_elem_t tmp;
    std::streamoff pos = input.tellg();
    char StreamCurElem;
    if (!(input >> tmp) || ((StreamCurElem = input.peek()) != ' ' && StreamCurElem != '\n' && StreamCurElem != '\t' && StreamCurElem != EOF))
    {
        if (err)
#if defined(checkMatr_v1) || defined(SScheckforPos_v2)
            err->fbit.pos = pos;
#else
        {
            input.clear();
            input.seekg(0, std::ios::end);
            err->fbit.pos = input.tellg() - pos;
        }
#endif
        return false;
    }
    return true;
}

#ifdef checkMatr_v1
StatusCode CheckMatrix(std::ifstream &file, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
{
    int size_for_check = 0;
    file >> std::ws;
    while (!file.eof())
    {
        if (!IstreamElementCorCheck(file, err))
        {
            if (err)
            {
                err->fbit.matr_str = matr_str;
                err->fbit.matr_column = matr_column;
            }
            return FailbitErr;
        }
        matr_column++;
        char tmp(0);
        while (tmp != EOF)
        {
            tmp = file.get();
            switch (tmp)
            {
            case '\t':
            case ' ':
                break;
            case '\n':
                file >> std::ws;
                tmp = file.peek();
            case EOF:
                if (size_for_check != matr_str * matr_column)
                {
                    if (err)
                        err->NotRectangular_str_in_Matrix = matr_str;
                    return NotRectangularMatrixErr;
                }
                matr_str++;

                if (tmp != EOF)
                {
                    size_for_check += matr_column;
                    matr_column = 0;
                    tmp = EOF;
                }
                break;
            default:
                file.unget();
                tmp = EOF;
            }
        }
    }
    if (!matr_str)
    {
        return EmptyFileErr;
    }
    return Good;
}

#else
StatusCode SSCheckMatrix(std::ifstream &file, int &matr_str, int &matr_column, ErrInfo *err = nullptr)
{
    int size_for_check = 0;
    while (!(file >> std::ws).eof())
    {
        std::string tmp;
        matr_column = 0;
        getline(file, tmp);
        std::istringstream istr(tmp);
        while (!istr.eof())
        {
            if (!(IstreamElementCorCheck(istr, err)))
            {
                if (err)
                {
#ifdef SScheckforPos_v2
                    istr.clear();
                    /*istr.seekg(err->fbit.pos, ios::beg);
                    istr >> tmp;
                    streamoff SS_pos;
                    if ((SS_pos = istr.tellg()) == EOF)
                    {
                        istr.clear();
                        istr.seekg(0, ios::end);
                        SS_pos = istr.tellg();
                    }*/
                    //err->fbit.pos = file.tellg() - ((streamoff)istr.str().size() - (istr.tellg() - (streamoff)tmp.size())) - (streamoff)1; ///(istr.tellg() - (streamoff)tmp.size()))->ERR.POS
                    err->fbit.pos = file.tellg() - ((streamoff)istr.str().size() - err->fbit.pos) - GetOffSet();
#else
                    err->fbit.pos = file.tellg() - err->fbit.pos - GetOffSet();
#endif
                    err->fbit.matr_str = matr_str;
                    err->fbit.matr_column = matr_column;
                }
                return FailbitErr;
            }
            matr_column++;
            istr >> std::ws;
        }
        matr_str++;
        size_for_check += matr_column;
        if (size_for_check != matr_str * matr_column)
        {
            if (err)
                err->NotRectangular_str_in_Matrix = matr_str;
            return NotRectangularMatrixErr;
        }
    }
    if (!matr_str)
        return EmptyFileErr;
    return Good;
}
#endif

void FReadMatrix(std::ifstream &file, __matrix_elem_t **matr, int matr_str, int matr_column)
{
    for (int i = 0; i < matr_str; i++)
        for (int j = 0; j < matr_column; j++)
            file >> *(*(matr + i) + j);
}

void DeleteMatrix(__matrix_t &matrix, int matrix_str)
{
    if (matrix)
    {
        for (int i = 0; i < matrix_str; i++)
            delete[] * (matrix + i);
        delete[] matrix;
        matrix = nullptr;
    }
}

#ifndef allocmemory_try_catch_method
StatusCode CreateMatr(__matrix_elem_t **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
{
    matr = new (nothrow) Xtype *[matr_str];
    if (matr == nullptr)
    {
        if (err)
            err->MemAllocStepID = -1;
        return FailAllocMemmoryErr;
    }
    for (int i = 0; i < matr_str; i++)
    {
        *(matr + i) = new (nothrow) Xtype[matr_column];
        if (*(matr + i) == nullptr)
        {
            DeleteMatr(matr, i);
            if (err)
                err->MemAllocStepID = i;
            return FailAllocMemmoryErr;
        }
    }
    return Good;
}

#else
StatusCode CreateMatrix(__matrix_elem_t **&matr, int matr_str, int matr_column, ErrInfo *err = nullptr)
{
    int i = PreparatoryStep;
    matr = nullptr;
    try
    {
        matr = new __matrix_elem_t *[matr_str];
        for (i = 0; i < matr_str; i++)
            *(matr + i) = new __matrix_elem_t[matr_column];
    }
    catch (std::bad_alloc)
    {
        if (i > 0)
            DeleteMatrix(matr, i);
        if (err)
            err->MemAllocStepID = i;
        return FailAllocMemmoryErr;
    }
    return Good;
}
#endif

StatusCode LoadMatrix(__matrix_t &matrix, int &matrix_str, int &matrix_column, const std::string &FileAdress,
                      std::function<StatusCode(__matrix_elem_t const *const *, int, int)> fAdditionalCheck, ErrInfo *err)
{
    matrix_str = 0, matrix_column = 0;
    std::ifstream file(FileAdress);
    if (file.is_open())
    {
        StatusCode status_code;
#ifdef checkMatr_v1
        if (!(status_code = CheckMatrix(file, matrix_str, matrix_column, err)))
#else
        if (!(status_code = SSCheckMatrix(file, matrix_str, matrix_column, err)))
#endif
        {
            if (!fAdditionalCheck || !(status_code = fAdditionalCheck(nullptr, matrix_str, matrix_column)))
            {
                if (!(status_code = CreateMatrix(matrix, matrix_str, matrix_column, err)))
                {
                    file.clear();
                    file.seekg(0);
                    FReadMatrix(file, matrix, matrix_str, matrix_column);
                    file.close();
                    if (!fAdditionalCheck || !(status_code = fAdditionalCheck(matrix, matrix_str, matrix_column)))
                        return status_code;
                }
                DeleteMatrix(matrix, matrix_str);
            }
        }
        file.close();
        matrix_str = 0, matrix_column = 0;
        return status_code;
    }
    return FileOpenErr;
}

/*при вызове функции нельзя передавать аргументом параметр err, если до этого он не определялся, в другой функции,
 которую мы хотим проверить на наличие ошибок, иначе мы не получим корректную обработку*/

__matrix_t CopyMatrix(__matrix_elem_t const *const *matr, int matr_str, int matr_column, StatusCode &status_code, ErrInfo *err)
{
    __matrix_t matrix_copy;
    if (!(status_code = CreateMatrix(matrix_copy, matr_str, matr_column, err)))
        for (int i = 0; i < matr_str; i++)
            for (int j = 0; j < matr_column; j++)
                *(*(matrix_copy + i) + j) = *(*(matr + i) + j);
    return matrix_copy;
}

std::ostream &OutputMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column,
                           std::streamsize width, std::streamsize precision, std::ios::fmtflags flags)
{
    if (flags)
    {
        std::ios::fmtflags resetflags = (std::ios::fmtflags)0;
        if (flags & std::ios::basefield)
            resetflags = resetflags | std::ios::basefield;
        if (flags & std::ios::floatfield)
            resetflags = resetflags | std::ios::floatfield;
        if (flags & std::ios::adjustfield)
            resetflags = resetflags | std::ios::adjustfield;
        ostr << resetiosflags(resetflags) << setiosflags(flags);
    }
    ostr << std::setprecision(precision);
    for (int i = 0; i < matrix_str; i++)
    {
        for (int j = 0; j < matrix_column; j++)
            ostr << std::setw(width) << *(*(pp_matrix + i) + j) << ' ';
        ostr << '\n';
    }
    return ostr;
}

std::ostream &CoutPerfectMatrix(std::ostream &ostr, __matrix_elem_t const *const *pp_matrix, int matrix_str, int matrix_column)
{
    std::ostringstream numberstr("|   |", std::ostringstream::ate);
    std::ostringstream skipstr("+---+", std::ostringstream::ate);
    for (int j = 0; j < matrix_column; j++)
    {
        skipstr << std::setw(12) << std::setfill('-') << '+';
        numberstr << std::setw(6 - TextVievSize(j) / 2 - TextVievSize(j) % 2) << "" << (j + 1) << std::setw(6 - TextVievSize(j) / 2) << '|';
    }
    ostr << '\n'
         << skipstr.str() << '\n'
         << numberstr.str() << '\n'
         << skipstr.str() << '\n';
    for (int i = 0; i < matrix_str; i++)
    {
        ostr << "| " << (i + 1) << " |";
        for (int j = 0; j < matrix_column; j++)
        {
            ostr << std::setw(10) << std::right << std::scientific << std::setprecision(2) << *(*(pp_matrix + i) + j) << " |";
        }
        ostr << std::endl
             << skipstr.str() << '\n';
    }
    return ostr;
}

__matrix_t ResultFunc(__matrix_t matrix, int matrix_str, int matrix_column)
{
    for (int r = 0; r < matrix_str; r++)
        if (*(*(matrix + r) + r) < 0)
        {
            for (int k = 0; k < matrix_column; k++)
            {
                if (k == r)
                    continue;
                if (!(*(*matrix + k)) && (*(*(matrix + r) + k) > 0))
                {
                    __matrix_elem_t tmp;
                    for (int l = 0; l < matrix_column; l++)
                    {
                        tmp = *(*(matrix + r) + l);
                        *(*(matrix + r) + l) = *(*(matrix + l) + k);
                        *(*(matrix + l) + k) = tmp;
                    }
                    break;
                }
            }
        }
    return matrix;
}