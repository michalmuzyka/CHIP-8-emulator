#pragma once

//TableLayout represents logical mesh to better positioning controls inside a window

class TableLayout {
    const int cols_count;
    const int rows_count;
    const int cell_width;
    const int cell_height;
public:
    TableLayout(int cols, int rows, int cell_width, int cell_height);

    int get_table_width() const;
    int get_table_height() const;

    int row(float row_number, float percentage = 0) const;
    int col(float col_number, float percentage = 0) const;

    int width(float count, float percentage = 0) const;
    int height(float count, float percentage = 0) const;
};