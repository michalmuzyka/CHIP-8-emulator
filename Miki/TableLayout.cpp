#include "TableLayout.hpp"

TableLayout::TableLayout(int cols, int rows, int cell_width, int cell_height)
    :cols_count{ cols }, rows_count{ rows }, cell_width{ cell_width }, cell_height{ cell_height } {}

int TableLayout::get_table_width() const {
    return  cols_count * cell_width;
}

int TableLayout::get_table_height() const {
    return  rows_count * cell_height;
}

int TableLayout::row(float row_number, float percentage) const {
    if (row_number < 0)
        return cell_height * (rows_count + row_number) + get_table_height() * (percentage / 100.0);
    return cell_height * row_number + get_table_height() * (percentage / 100.0);
}

int TableLayout::col(float col_number, float percentage) const {
    if (col_number < 0)
        return cell_width * (cols_count + col_number) + get_table_width() * (percentage / 100.0);
    return cell_width * col_number + get_table_width() * (percentage / 100.0);
}

int TableLayout::width(float count, float percentage) const {
    return count * cell_width + get_table_width() * (percentage / 100.0);
}

int TableLayout::height(float count, float percentage) const {
    return count * cell_height + get_table_height() * (percentage / 100.0);
}