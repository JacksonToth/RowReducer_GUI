#include <iostream>
#include <cmath>
#include <cassert>
#include <iomanip>
#include <valarray>
#include <windows.h>

// Class declarations
class Entry
{
public:
    // Variables
    double entry_value;
    int entry_x_position;
    int entry_y_position;

    Entry *p_next; // LL pointer

    // Constructor
    Entry(int x, int y, double value); // int id, int dimension

    // Destructor
    ~Entry();

    // Functions
    Entry *get_next();

    // Entry *get_head();

    void set_next(Entry *p_new_next);

    // void set_head( Entry *p_new_head );

    int get_x_pos();

    int get_y_pos();

    // int get_Entry_id();

    double get_value();

    void internal_set_value(double value);
};

class Matrix
{
public:
    // Variables
    std::string matrix_id;
    int associated_dimension;   // XDIM
    int associated_y_dimension; // YDIM
    int aug_xdim;
    int aug_ydim;
    bool is_augmented;
    bool print_steps;

    Matrix *next_matrix; // LL pointer for the MatrixList class

    Entry *p_head; // LL head

    Entry *aug_p_head; // Augmented matrix LL head

    // Constructor
    Matrix(std::string id, int dim, int ydim);

    // Destructor
    ~Matrix();

    // Functions
    Matrix *get_next_matrix(); // Used in MatrixList class

    void set_next_matrix(Matrix *p_new_matrix); // Used in MatrixList class

    Entry *get_head();

    Entry *get_aug_head();

    void set_head(Entry *p_new_head);

    std::string get_id();

    void set_id(std::string new_id);

    int get_x_dimension();

    int get_y_dimension();

    int get_aug_x();

    int get_aug_y();

    double retrieve_value_from_entry(int x, int y);

    void edit_value(double value, int x, int y);

    double aug_retrieve_value_from_entry(int x, int y);

    void aug_edit_value(double value, int x, int y);

    void print_Matrix();

    void G_print_Matrix(HWND hWnd);

    void print_entry(int x, int y);

    void insert_into_LL(Entry *p_new);

    void create_matrix();

    void create_augmented_matrix();

    void create_identity_augmented_matrix();

    void set_augmented_xdim(int a_xdim);

    void set_augmented_ydim(int a_ydim);

    void augmented_insert_into_LL(Entry *p_new);

    void row_reduce();

    void row_reduce2();

    void rr_row_swap(int row_1_y_coord, int row_2_y_coord);

    void rr_row_scale(int y_coord, double scalar);

    void rr_row_addition(int row_1_y_coord, int row_2_y_coord);

    void rr_scale_then_add(int row_1_y_coord, int row_2_y_coord, double scalar);

    int inconsistency_check();

    void convert_to_fraction(double number); // Taken from the web

    std::string winapi_convert_to_fraction(double number);

    std::string return_fraction_conversion(double number);

    int gcd(int a, int b);

    std::string removeTrailingZeros(const std::string &str);

    int largest_row();

    int aug_largest_row();

    void print_steps_true();

    void print_steps_false();

    int size_of_row(int y);

    int aug_size_of_row(int y);

    void remove_garbage();

    void aug_true();

    void set_aug_head(Entry *p_new_head);
};

class MatrixList
{

public:
    // Variables
    Matrix *m_list_head;
    int number_of_matrices;

    // Constructor
    MatrixList();

    // Destructor
    ~MatrixList();

    // Functions
    Matrix *get_list_head();

    void set_list_head(Matrix *p_new_list_head);

    Matrix *find_matrix(std::string id);

    void insert_matrix_into_list(Matrix *new_matrix);

    Matrix *winapi_new_matrix(std::string new_id, int new_xdim, int new_ydim);

    void new_matrix(std::string new_id, int new_xdim, int new_ydim);

    int get_number_of_matrices();

    void print_list();
};
